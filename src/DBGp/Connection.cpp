// {{{ Copyright notice
/* Copyright (c) 2007, Adam Harvey
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *  - The names of its contributors may not be used to endorse or promote
 *  products derived from this software without specific prior written
 *  permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
// }}}

#include "DBGp/Connection.h"
#include "DBGp/Server.h"
#include "DBGp/Utility.h"
#include "DBGp/Event/ConnectionEvent.h"
#include "DBGp/Event/StatusChangeEvent.h"
#include "DBGp/Event/StderrEvent.h"
#include "DBGp/Event/StdoutEvent.h"

#include <cstring>

#include <wx/log.h>
#include <wx/mstream.h>
#include <wx/sstream.h>
#include <wx/strconv.h>

using namespace DBGp;

// {{{ Event table
//BEGIN_EVENT_TABLE(Connection, wxEvtHandler)
//	EVT_SOCKET(2, Connection::OnSocket)
//END_EVENT_TABLE()
// }}}

// {{{ Connection::Connection(wxSocketBase *socket, Server *server)
Connection::Connection(wxSocketBase *socket, Server *server) : wxEvtHandler(), handler(server->parent), server(server), socket(socket), status(STARTING), txID(0) {
	wxASSERT(socket != NULL);
	wxASSERT(server != NULL);

	/* This is pretty much a case of "you have to pick something as the
	 * default". XDebug only supports ISO-8859-1 anyway, and I'd maintain
	 * that any implementation sending characters with the high-bit set as
	 * part of the init packet has issues regardless. */
	conv = &wxConvISO8859_1;

	Connect(-1, wxEVT_SOCKET, wxSocketEventHandler(Connection::OnSocket));
	socket->SetEventHandler(*this, -1);
	socket->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	socket->Notify(true);

	SetEventHandler(handler);
}
// }}}
// {{{ Connection::~Connection()
Connection::~Connection() {
	Close();
}
// }}}

// {{{ void Connection::Close()
void Connection::Close() {
	if (socket) {
		socket->Destroy();
	}
	socket = NULL;
}
// }}}
// {{{ bool Connection::CommandSupported(const wxString &command)
bool Connection::CommandSupported(const wxString &command) {
	if (supported.count(command) > 0) {
		return supported[command];
	}
	return true;
}
// }}}
// {{{ void Connection::SetEventHandler(wxEvtHandler *handler)
void Connection::SetEventHandler(wxEvtHandler *handler) {
	this->handler = handler;
	//SetNextHandler(handler);
}
// }}}

// {{{ void Connection::Break() throw (SocketError, UnsupportedFeatureError)
void Connection::Break() throw (SocketError, UnsupportedFeatureError) {
	if (!supported[wxT("break")]) {
		throw UnsupportedFeatureError(wxT("The break command is not supported by the debugging engine."));
	}

	SendCommandImmediate(wxT("break"), MessageArguments());
}
// }}}
// {{{ void Connection::Detach() throw (SocketError, UnsupportedFeatureError)
void Connection::Detach() throw (SocketError, UnsupportedFeatureError) {
	if (!supported[wxT("detach")]) {
		throw UnsupportedFeatureError(wxT("The detach command is not supported by the debugging engine."));
	}

	SendCommandImmediate(wxT("detach"), MessageArguments());
}
// }}}
// {{{ wxString Connection::FeatureGet(const wxString &name) throw (EngineError, MalformedDocumentError, SocketError, UnsupportedFeatureError)
wxString Connection::FeatureGet(const wxString &name) throw (EngineError, MalformedDocumentError, SocketError, UnsupportedFeatureError) {
	wxXmlDocument doc(SendCommandWait(wxT("feature_get"), MessageArguments().Append(wxT("-n"), name)));
	wxXmlNode *root = doc.GetRoot();

	if (root->GetPropVal(wxT("supported"), wxT("0")) == wxT("0")) {
		throw UnsupportedFeatureError(wxT("Unsupported feature: ") + name);
	}

	return root->GetNodeContent();
}
// }}}
// {{{ bool Connection::FeatureSet(const wxString &name, const wxString &value) throw (EngineError, MalformedDocumentError, SocketError)
bool Connection::FeatureSet(const wxString &name, const wxString &value) throw (EngineError, MalformedDocumentError, SocketError) {
	wxXmlDocument doc(SendCommandWait(wxT("feature_set"), MessageArguments().Append(wxT("-n"), name).Append(wxT("-v"), value)));

	return (doc.GetRoot()->GetPropVal(wxT("success"), wxT("0")) == wxT("1"));
}
// }}}
// {{{ void Connection::Run() throw (SocketError)
void Connection::Run() throw (SocketError) {
	status = RUNNING;
	SendCommandImmediate(wxT("run"), MessageArguments());
}
// }}}
// {{{ wxString Connection::Source(const wxString &fileUri, int beginLine, int endLine) throw (EngineError, NotFoundError, SocketError)
wxString Connection::Source(const wxString &fileUri, int beginLine, int endLine) throw (EngineError, NotFoundError, SocketError) {
	MessageArguments args;

	if (fileUri != wxEmptyString) {
		args.Append(wxT("-f"), fileUri);
	}

	if (beginLine != -1) {
		args.Append(wxT("-b"), IntToString(beginLine));
	}

	if (endLine != -1) {
		args.Append(wxT("-e"), IntToString(endLine));
	}

	try {
		wxXmlDocument doc(SendCommandWait(wxT("source"), args));
		wxXmlNode *root = doc.GetRoot();

		/* The spec says that we should get a success attribute as part
		 * of the response, but XDebug doesn't actually return it.
		 * Assume success if it doesn't exist. */
		if (root->GetPropVal(wxT("success"), wxT("1")) == wxT("0")) {
			throw NotFoundError(wxString(wxT("Source of '")) + fileUri + wxT("' failed."));
		}

		return root->GetNodeContent();
	}
	catch (EngineError e) {
		if (e.GetCode() == 100) {
			throw NotFoundError(wxString(wxT("Source file not found: ")) + fileUri);
		}
		throw;
	}
	catch (...) {
		throw;
	}
}
// }}}
// {{{ Connection::EngineStatus Connection::Status(wxString *reason) throw (EngineError, MalformedDocumentError, SocketError)
Connection::EngineStatus Connection::Status(wxString *reason) throw (EngineError, MalformedDocumentError, SocketError) {
	if (reason) {
		wxXmlDocument doc(SendCommandWait(wxT("status"), MessageArguments()));
		wxXmlNode *root = doc.GetRoot();
		EngineStatus status;

		try {
			status = StringToEngineStatus(root->GetPropVal(wxT("status"), wxEmptyString));
			this->status = status;
		}
		catch (NotFoundError e) {
			throw MalformedDocumentError(wxT("Invalid status."));
		}

		*reason = root->GetPropVal(wxT("reason"), wxEmptyString);

		return status;
	}
	return status;
}
// }}}
// {{{ void Connection::StepInto() throw (SocketError)
void Connection::StepInto() throw (SocketError) {
	status = RUNNING;
	SendCommandImmediate(wxT("step_into"), MessageArguments());
}
// }}}
// {{{ void Connection::StepOut() throw (SocketError)
void Connection::StepOut() throw (SocketError) {
	status = RUNNING;
	SendCommandImmediate(wxT("step_out"), MessageArguments());
}
// }}}
// {{{ void Connection::StepOver() throw (SocketError)
void Connection::StepOver() throw (SocketError) {
	status = RUNNING;
	SendCommandImmediate(wxT("step_over"), MessageArguments());
}
// }}}
// {{{ void Connection::Stop() throw (SocketError)
void Connection::Stop() throw (SocketError) {
	SendCommandImmediate(wxT("stop"), MessageArguments());
}
// }}}
// {{{ Typemap Connection::TypemapGet() throw (EngineError, MalformedDocumentError, SocketError)
Typemap &Connection::TypemapGet() throw (EngineError, MalformedDocumentError, SocketError) {
	if (typemap.GetTypes().size() == 0) {
		wxXmlDocument doc(SendCommandWait(wxT("typemap_get"), MessageArguments()));

		for (wxXmlNode *node = doc.GetRoot()->GetChildren(); node != NULL; node = node->GetNext()) {
			if (node->GetType() == wxXML_ELEMENT_NODE && node->GetName() == wxT("map")) {
				typemap.AddType(Type(Type::StringToCommonType(node->GetPropVal(wxT("type"), wxEmptyString)), node->GetPropVal(wxT("name"), wxEmptyString), node->GetPropVal(wxT("xsi:type"), wxEmptyString)));
			}
		}
	}

	return typemap;
}
// }}}

// {{{ Breakpoint *Connection::CreateBreakpoint()
Breakpoint *Connection::CreateBreakpoint() {
	Breakpoint *breakpoint = new Breakpoint(this);
	breakpoints.push_back(breakpoint);
	return breakpoint;
}
// }}}
// {{{ Breakpoint *Connection::GetBreakpoint(const wxString &id)
Breakpoint *Connection::GetBreakpoint(const wxString &id) {
	for (BreakpointList::iterator i = breakpoints.begin(); i != breakpoints.end(); i++) {
		if ((*i)->GetID() == id) {
			return *i;
		}
	}
	return NULL;
}
// }}}
// {{{ const Connection::BreakpointList &Connection::GetBreakpoints() const
const Connection::BreakpointList &Connection::GetBreakpoints() const {
	return breakpoints;
}
// }}}
// {{{ void Connection::RemoveBreakpoint(Breakpoint *breakpoint)
void Connection::RemoveBreakpoint(Breakpoint *breakpoint) {
	wxASSERT(breakpoint != NULL);
	breakpoints.remove(breakpoint);
	delete breakpoint;
}
// }}}

// {{{ Stack Connection::StackGet() throw (EngineError, MalformedDocumentError, SocketError)
Stack Connection::StackGet() throw (EngineError, MalformedDocumentError, SocketError) {
	return Stack(this);
}
// }}}

// {{{ wxString Connection::EngineStatusToString(Connection::EngineStatus status)
wxString Connection::EngineStatusToString(Connection::EngineStatus status) {
	switch (status) {
		case STARTING:
			return wxT("starting");
		case STOPPING:
			return wxT("stopping");
		case STOPPED:
			return wxT("stopped");
		case RUNNING:
			return wxT("running");
		default:
			return wxT("break");
	}
}
// }}}
// {{{ Connection::EngineStatus Connection::StringToEngineStatus(const wxString &s) throw (NotFoundError)
Connection::EngineStatus Connection::StringToEngineStatus(const wxString &s) throw (NotFoundError) {
	wxString status(s.Lower());
	if (s == wxT("starting")) {
		return STARTING;
	}
	else if (s == wxT("stopping")) {
		return STOPPING;
	}
	else if (s == wxT("stopped")) {
		return STOPPED;
	}
	else if (s == wxT("running")) {
		return RUNNING;
	}
	else if (s == wxT("break")) {
		return BREAK;
	}
	throw NotFoundError(wxT("Unknown status '") + s + wxT("'."));
}
// }}}

// {{{ void Connection::CopyOutput() throw ()
void Connection::CopyOutput() throw () {
	try {
		MessageArguments args(1, wxT("-c"), wxT("1"));
		SendCommandImmediate(wxT("stdout"), args);
		SendCommandImmediate(wxT("stderr"), args);
	}
	catch (Error e) {
		wxLogError(wxT("Error redirecting output streams: %s."), e.GetMessage().c_str());
	}
}
// }}}
// {{{ wxXmlDocument Connection::GetMessage() throw (SocketError, SocketDestroyedError)
wxXmlDocument Connection::GetMessage() throw (MalformedDocumentError, SocketError, SocketDestroyedError) {
	char c;
	wxString lenBuf;
	unsigned long length = 0, read = 0;
	wxString message;
	char *msgBuf;
	wxMemoryInputStream *mis;
	wxXmlDocument doc;

	if (socket == NULL) {
		throw SocketDestroyedError();
	}


	wxLogDebug(wxT("Beginning receive..."));
	
	socket->Read(&c, 1);
	if (socket->Error()) {
		throw SocketError(wxT("Read error from socket."));
	}
	while (c != 0) {
		lenBuf += c;
		socket->Read(&c, 1);
		if (socket->Error()) {
			throw SocketError(wxT("Read error from socket."));
		}
	}
	lenBuf.ToULong(&length);
	wxLogDebug(wxT("Expecting %lu bytes."), length);

	if (length == 0) {
		throw SocketError(wxT("Told to read 0 bytes."));
	}

	msgBuf = new char[length + 1];
	char *writeLoc = msgBuf;
	while (read < length) {
		socket->Read(writeLoc, length - read);
		if (socket->Error()) {
			wxLogError(wxT("Error reading from socket, attempting to recover."));
		}
		else {
			unsigned long passRead = socket->LastCount();
			
			wxASSERT(passRead <= (length - read));
			read += passRead;
			writeLoc += passRead;

			wxLogDebug(wxT("Received chunk of %lu bytes; got %lu bytes of expected %lu."), passRead, read, length);
		}
	}
	wxLogDebug(wxT("Reading terminating NULL."));
	socket->Read(msgBuf + length, 1);

	message = conv->cMB2WX(msgBuf);
	wxLogDebug(wxT("RX(%lu): %s"), length, message.c_str());

	mis = new wxMemoryInputStream(msgBuf, length);
	if (!doc.Load(*mis)) {
		delete mis;
		delete[] msgBuf;
		
		throw MalformedDocumentError(wxT("Incoming XML document is malformed."));
	}
	
	delete mis;
	delete[] msgBuf;
	
	return doc;
}
// }}}
// {{{ TransactionID Connection::GetTransactionID()
TransactionID Connection::GetTransactionID() {
	TransactionID id;
	txMutex.Lock();
	id = txID++;
	txMutex.Unlock();
	return id;
}
// }}}
// {{{ void Connection::HandleEncodedMessage(wxXmlNode *node)
void Connection::HandleEncodedMessage(wxXmlNode *node) {
	bool encoded = node->GetPropVal(wxT("encoding"), wxEmptyString) == wxT("base64");

	for (wxXmlNode *child = node->GetChildren(); child != NULL; child = child->GetNext()) {
		wxXmlNodeType type = child->GetType();
		if (type == wxXML_ELEMENT_NODE) {
			HandleEncodedMessage(child);
		}
		else if (encoded && (type == wxXML_TEXT_NODE || type == wxXML_CDATA_SECTION_NODE)) {
			try {
				wxString base64(child->GetContent());
				size_t len = Base64::DataLength(base64);

				char *data = new char[len+1];
				Base64::Decode(base64, data, len);
				data[len] = 0;
				child->SetType(wxXML_CDATA_SECTION_NODE);
				/* XXX We shouldn't really re-encode the raw
				 * data, but wxXmlNode forces us to. Figure out
				 * some way of avoiding this. */
				child->SetContent(wxString(data, *conv));
				delete[] data;
			}
			catch (Base64::DecoderError e) {
				wxLogError(wxT("Error in Base64 decoding with data: %s"), child->GetContent().c_str());
			}
		}
	}
}
// }}}
// {{{ void Connection::HandleMessage(wxXmlDocument &doc) throw (EngineError, MalformedDocumentError, SocketError)
void Connection::HandleMessage(wxXmlDocument &doc) throw (EngineError, MalformedDocumentError, SocketError) {
	wxXmlNode *root = doc.GetRoot();

	HandleEncodedMessage(root);

	wxStringOutputStream os;
	doc.Save(os);
	wxLogDebug(wxT("Decoded XML: %s"), os.GetString().c_str());

	if (root->GetName() == wxT("init")) {
		NegotiateFeatures();
		CopyOutput();
		typemap = TypemapGet();

		if (handler) {
			ConnectionEvent e(this,
				root->GetPropVal(wxT("appid"), wxEmptyString),
				root->GetPropVal(wxT("idekey"), wxEmptyString),
				root->GetPropVal(wxT("session"), wxEmptyString),
				root->GetPropVal(wxT("thread"), wxEmptyString),
				root->GetPropVal(wxT("parent"), wxEmptyString),
				root->GetPropVal(wxT("language"), wxEmptyString),
				root->GetPropVal(wxT("protocol_version"), wxEmptyString),
				root->GetPropVal(wxT("fileuri"), wxEmptyString));

			handler->ProcessEvent(e);
		}
	}
	else if (root->GetName() == wxT("response")) {
		HandleResponse(root);
	}
	else if (handler && root->GetName() == wxT("stream")) {
		if (root->GetPropVal(wxT("type"), wxT("stdout")) == wxT("stdout")) {
			StdoutEvent e(this, root->GetNodeContent());
			handler->ProcessEvent(e);
		}
		else {
			StderrEvent e(this, root->GetNodeContent());
			handler->ProcessEvent(e);
		}
	}
	else {
		wxLogWarning(wxT("Unknown root element: '%s'."), root->GetName().c_str());
	}
}
// }}}
// {{{ void Connection::HandleResponse(wxXmlNode *resp) throw (EngineError)
void Connection::HandleResponse(wxXmlNode *resp) throw (EngineError) {
	/* Check for an error element, since we need to throw an
	 * exception if one exists. */
	for (wxXmlNode *node = resp->GetChildren(); node != NULL; node = node->GetNext()) {
		if (node->GetName() == wxT("error")) {
			HandleResponseError(node);
		}
	}

	/* If the status has been changed and the command wasn't "status" or
	 * "interact", then we need to update our internal status and raise an
	 * event. Because the only commands that return status attributes are
	 * "interact", "status" and the various continuation commands, we can
	 * safely process the event immediately, since our implementations of
	 * the continuation commands don't wait for a response, ergo this
	 * should be coming from the OnSocket event handler. */
	wxString command(resp->GetPropVal(wxT("command"), wxEmptyString));
	if (resp->HasProp(wxT("status")) && command != wxT("interact") && command != wxT("status")) {
		try {
			EngineStatus newStatus = StringToEngineStatus(resp->GetPropVal(wxT("status"), wxEmptyString));
			if (newStatus != status) {
				wxLogDebug(wxT("Status change: %s -> %s"), EngineStatusToString(status).c_str(), EngineStatusToString(newStatus).c_str());
				status = newStatus;

				if (handler) {
					StatusChangeEvent e(this, status);
					handler->ProcessEvent(e);
				}
			}
		}
		catch (NotFoundError e) {
			// Unknown status. Log and ignore.
			wxLogWarning(wxT("Got unknown status '%s'."), resp->GetPropVal(wxT("status"), wxEmptyString).c_str());
		}
	}
}
// }}}
// {{{ void Connection::HandleResponseError(wxXmlNode *error) throw (EngineError)
void Connection::HandleResponseError(wxXmlNode *error) throw (EngineError) {
	unsigned long code;
	wxString appErr(error->GetPropVal(wxT("apperr"), wxEmptyString)), message;
	wxXmlNode *messageNode;

	for (messageNode = error->GetChildren(); messageNode != NULL; messageNode = messageNode->GetNext()) {
		if (messageNode->GetName() == wxT("message")) {
			message = messageNode->GetNodeContent();
			break;
		}
	}

	error->GetPropVal(wxT("code"), wxEmptyString).ToULong(&code);

	wxLogDebug(wxT("Got response error: code %d; app error '%s'; message '%s'."), code, appErr.c_str(), message.c_str());
	throw EngineError(static_cast<wxUint16>(code), appErr, message);
}
// }}}
// {{{ void Connection::NegotiateFeatures() throw ()
void Connection::NegotiateFeatures() throw () {
	/* Attempt to set the encoding to UTF-8. XDebug will fail for now, but
	 * that's OK, since I suspect it'll get Unicode support when PHP 6 is
	 * closer. */
	try {
		if (FeatureSet(wxT("encoding"), wxT("UTF-8"))) {
			wxLogDebug(wxT("Encoding switched to UTF-8."));
		}
		else {
			wxLogDebug(wxT("Encoding remains ISO-8859-1."));
		}
	}
	catch (Error e) {
		wxLogDebug(wxT("Error setting encoding: %s"), e.GetMessage().c_str());
	}
	
	// Test extended commands.
	TestCommand(wxT("break"));
	TestCommand(wxT("detach"));
	TestCommand(wxT("exec"));
	TestCommand(wxT("expr"));

	/* Set max_children and max_depth to fairly large values. In the longer
	 * term, we might look at providing configuration options for these
	 * figures, but for now, let's just get everything. Not so good for
	 * people debugging over dialup... */
	try {
		FeatureSet(wxT("max_children"), wxT("1000"));
		FeatureSet(wxT("max_depth"), wxT("100"));
	}
	catch (Error e) {
		wxLogError(wxT("Error setting children/depth: %s"), e.GetMessage().c_str());
	}
}
// }}}
// {{{ void Connection::OnSocket(wxSocketEvent &event) throw ()
void Connection::OnSocket(wxSocketEvent &event) throw () {
	wxLogDebug(wxT("OnSocket called; event %d."), event.GetSocketEvent());

	// TODO: Send event.
	if (event.GetSocketEvent() == wxSOCKET_LOST) {
		//delete this;
		return;
	}

	/* If we're getting non-INPUT events here, something's awry, but OK.
	 * Let's just carry on and hope for the best. */
	if (event.GetSocketEvent() != wxSOCKET_INPUT) {
		event.Skip(true);
		return;
	}

	try {
		wxXmlDocument doc(GetMessage());

		HandleMessage(doc);
	}
	catch (EngineError e) {
		wxLogError(wxT("Caught engine error in OnSocket: %s"), e.GetMessage().c_str());
	}
	catch (MalformedDocumentError e) {
		wxLogError(wxT("Caught malformed document error in OnSocket: %s"), e.GetMessage().c_str());
	}
	catch (SocketError e) {
		wxLogError(wxT("Caught socket error in OnSocket: %s"), e.GetMessage().c_str());
	}
}
// }}}
// {{{ TransactionID Connection::SendCommand(const wxString &command, MessageArguments args, const char *data, size_t dataLength) throw (SocketError, SocketDestroyedError)
TransactionID Connection::SendCommand(const wxString &command, MessageArguments args, const char *data, size_t dataLength) throw (SocketError, SocketDestroyedError) {
	const char *buffer;
	size_t bufferLen;
	wxString message(command), txString;
	TransactionID txID = GetTransactionID();

	if (socket == NULL) {
		throw SocketDestroyedError();
	}

	txString = IntToString(txID);
	message << wxT(" ") << args.Append(wxT("-i"), txString).GetArguments();
	if (data && dataLength > 0) {
		message << wxT(" -- ") << Base64::Encode(data, dataLength);
	}
	message << wxT('\0');

#if defined(wxUSE_UNICODE) && wxUSE_UNICODE == 1
	/* In Unicode mode, cWX2MB returns a wxCharBuffer rather than a char *,
	 * which is actually pretty irritating, because it means that we can't
	 * just call it without knowing what mode we're in. We'll release the
	 * memory straight back to our keeping and delete it later, rather than
	 * having to special case further and have buffer be a different type
	 * depending on the compilation mode. */
	buffer = conv->cWX2MB(message.wc_str()).release();
#else
	/* We could call cWX2MB here as well, but it's a no-op, so we'll just
	 * call c_str() and be done with it. */
	buffer = message.c_str();
#endif

	bufferLen = std::strlen(buffer) + 1;
	wxLogDebug(wxT("TX(%u): %s"), bufferLen, message.c_str());
	socket->Write(buffer, bufferLen);

#if defined(wxUSE_UNICODE) && wxUSE_UNICODE == 1
	/* As we took ownership of the buffer's memory by calling
	 * wxCharBuffer.release() earlier, we have to delete[] it here. */
	delete[] buffer;
#endif

	if (socket->Error()) {
		throw SocketError(socket->LastError());
	}

	return txID;
}
// }}}
// {{{ void Connection::SendCommandImmediate(const wxString &command, MessageArguments args, const char *data, size_t dataLength) throw (SocketError, SocketDestroyedError)
void Connection::SendCommandImmediate(const wxString &command, MessageArguments args, const char *data, size_t dataLength) throw (SocketError, SocketDestroyedError) {
	SendCommand(command, args, data, dataLength);
}
// }}}
// {{{ wxXmlDocument Connection::SendCommandWait(const wxString &command, MessageArguments args, const char *data, size_t dataLength) throw (EngineError, MalformedDocumentError, SocketError, SocketDestroyedError)
wxXmlDocument Connection::SendCommandWait(const wxString &command, MessageArguments args, const char *data, size_t dataLength) throw (EngineError, MalformedDocumentError, SocketError, SocketDestroyedError) {
	wxString txID;

	if (socket == NULL) {
		throw SocketDestroyedError();
	}

	/* Acquire BFL and disable socket notifications so we can get the
	 * response here without waking the input handler. */
	pendingMutex.Lock();
	socket->Notify(false);

	try {
		txID = IntToString(SendCommand(command, args, data, dataLength));
	}
	catch (Error e) {
		socket->Notify(true);
		pendingMutex.Unlock();

		throw;
	}

	while (true) {
		wxXmlDocument doc;
		wxXmlNode *root;
		try {
			doc = GetMessage();
			root = doc.GetRoot();

			HandleMessage(doc);
		}
		catch (Error e) {
			/* Clean up and rethrow. Oh, for a RAII wxMutex or C++
			 * supporting finally... */
			socket->Notify(true);
			pendingMutex.Unlock();

			throw;
		}
		if (root->GetName() == wxT("response")) {
			// Check the returned transaction ID.
			wxString recvTx(root->GetPropVal(wxT("transaction_id"), wxEmptyString));
			if (recvTx == txID) {
				socket->Notify(true);
				pendingMutex.Unlock();

				return doc;
			}
			else {
				wxLogDebug(wxT("Unexpected transaction ID '%s' in response."), recvTx.c_str());
			}
		}
		else {
			/* This should have been handled by HandleMessage(), so we'll
			 * ignore it. */
			wxLogDebug(wxT("Got non-response packet '%s' from debugger engine, ignoring."), root->GetName().c_str());
		}
	}
}
// }}}
// {{{ void Connection::TestCommand(const wxString &command) throw ()
void Connection::TestCommand(const wxString &command) throw () {
	try {
		supported[command] = (FeatureGet(command) == wxT("1"));
	}
	catch (Error e) {
		wxLogDebug(wxT("Error testing command: %s."), command.c_str());
		supported[command] = false;
	}
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
