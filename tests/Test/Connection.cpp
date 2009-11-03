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

#include "Test/Connection.h"

#include <wx/log.h>

#ifdef __WXDEBUG__
#include <wx/sstream.h>
#endif

using namespace Test;

// {{{ Connection::Connection(DBGp::ConnectionID id, wxSocketBase *socket, Server *server)
Connection::Connection(wxSocketBase *socket, Server *server) : DBGp::Connection(socket, server), first(true) {
}
// }}}

// {{{ void Connection::AddResponse(const wxXmlDocument &doc)
void Connection::AddResponse(const wxXmlDocument &doc) {
	responses.push_back(doc);
}
// }}}
// {{{ void Connection::ProcessNextResponse()
void Connection::ProcessNextResponse() {
	try {
		wxXmlDocument doc(GetMessage());
		HandleMessage(doc);
	}
	catch (DBGp::Error e) {
	}
}
// }}}

// {{{ wxXmlDocument Connection::GetMessage() throw (DBGp::MalformedDocumentError, DBGp::SocketError)
wxXmlDocument Connection::GetMessage() throw (DBGp::MalformedDocumentError, DBGp::SocketError) {
	if (first) {
		currentResponse = responses.begin();
		first = false;
	}
	else {
		currentResponse++;
	}

	if (currentResponse != responses.end()) {
		wxXmlDocument doc(*currentResponse);

		// Munge the transaction ID.
		wxString transaction;
		transaction << (txID - 1);
		doc.GetRoot()->DeleteProperty(wxT("transaction_id"));
		doc.GetRoot()->AddProperty(wxT("transaction_id"), transaction);

#ifdef __WXDEBUG__
		// Log if it it makes sense to.
		wxStringOutputStream os;
		doc.Save(os);
		wxLogDebug(wxT("Fake RX: %s"), os.GetString().c_str());
#endif

		return doc;
	}
	throw DBGp::SocketError(wxSOCKET_IOERR);
}
// }}}
// {{{ DBGp::TransactionID Connection::SendCommand(const wxString &command, DBGp::MessageArguments args, const char *data, size_t dataLength) throw (DBGp::SocketError)
DBGp::TransactionID Connection::SendCommand(const wxString &command, DBGp::MessageArguments args, const char *data, size_t dataLength) throw (DBGp::SocketError) {
	return GetTransactionID();
}
// }}}
// {{{ void Connection::SendCommandImmediate(const wxString &command, DBGp::MessageArguments args, const char *data, size_t dataLength) throw (DBGp::SocketError)
void Connection::SendCommandImmediate(const wxString &command, DBGp::MessageArguments args, const char *data, size_t dataLength) throw (DBGp::SocketError) {
	wxLogDebug(wxT("TX: %s %s"), command.c_str(), args.GetArguments().c_str());
	// We need to absorb a response so it doesn't cause problems later.
	try {
		wxXmlDocument doc(GetMessage());
		HandleMessage(doc);
	}
	catch (DBGp::SocketError e) {
		throw;
	}
	catch (...) {
	}
}
// }}}
// {{{ wxXmlDocument Connection::SendCommandWait(const wxString &command, DBGp::MessageArguments args, const char *data, size_t dataLength) throw (DBGp::EngineError, DBGp::MalformedDocumentError, DBGp::SocketError)
wxXmlDocument Connection::SendCommandWait(const wxString &command, DBGp::MessageArguments args, const char *data, size_t dataLength) throw (DBGp::EngineError, DBGp::MalformedDocumentError, DBGp::SocketError) {
	wxLogDebug(wxT("TX: %s %s"), command.c_str(), args.GetArguments().c_str());
	wxXmlDocument doc(GetMessage());
	HandleMessage(doc);
	return doc;
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
