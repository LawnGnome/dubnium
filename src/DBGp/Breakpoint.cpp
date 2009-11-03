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

#include "DBGp/Breakpoint.h"
#include "DBGp/Connection.h"
#include "DBGp/Utility.h"

#include <cstring>

#include <wx/log.h>

using namespace DBGp;

// {{{ Breakpoint::Breakpoint(Connection *conn)
Breakpoint::Breakpoint(Connection *conn) : conn(conn), enabled(true), hitCondition(HIT_GE), hitCount(0), hitValue(0), isSet(false), temporary(false), type(LINE) {
}
// }}}
// {{{ Breakpoint::~Breakpoint()
Breakpoint::~Breakpoint() {
	if (isSet) {
		try {
			conn->SendCommandImmediate(wxT("breakpoint_remove"), MessageArguments().Append(wxT("-d"), id));
		}
		catch (SocketError e) {
			wxLogError(wxT("Socket error removing breakpoint ID '%s'."), id.c_str());
		}
	}
}
// }}}

// {{{ int Breakpoint::GetHitCount() throw (EngineError, SocketError)
int Breakpoint::GetHitCount() throw (EngineError, SocketError) {
	Get();
	return hitCount;
}
// }}}

// {{{ void Breakpoint::SetCallType(const wxString &function) throw (EngineError, SocketError)
void Breakpoint::SetCallType(const wxString &function) throw (EngineError, SocketError) {
	type = CALL;
	this->function = function;
	Set();
}
// }}}
// {{{ void Breakpoint::SetConditionalType(const wxString &expression) throw (EngineError, SocketError)
void Breakpoint::SetConditionalType(const wxString &expression) throw (EngineError, SocketError) {
	type = CONDITIONAL;
	this->expression = expression;
	Set();
}
// }}}
// {{{ void Breakpoint::SetExceptionType(const wxString &exception) throw (EngineError, SocketError)
void Breakpoint::SetExceptionType(const wxString &exception) throw (EngineError, SocketError) {
	type = EXCEPTION;
	this->exception = exception;
	Set();
}
// }}}
// {{{ void Breakpoint::SetHitCondition(HitCondition condition, int value) throw (EngineError, SocketError)
void Breakpoint::SetHitCondition(HitCondition condition, int value) throw (EngineError, SocketError) {
	hitCondition = condition;
	hitValue = value;
	Set();
}
// }}}
// {{{ void Breakpoint::SetLineType(const wxString &fileName, int lineNo) throw (EngineError, SocketError)
void Breakpoint::SetLineType(const wxString &fileName, int lineNo) throw (EngineError, SocketError) {
	type = LINE;
	this->fileName = fileName;
	this->lineNo = lineNo;
	Set();
}
// }}}
// {{{ void Breakpoint::SetReturnType(const wxString &function) throw (EngineError, SocketError)
void Breakpoint::SetReturnType(const wxString &function) throw (EngineError, SocketError) {
	type = RETURN;
	this->function = function;
	Set();
}
// }}}
// {{{ void Breakpoint::SetTemporary(bool temporary) throw (EngineError, SocketError)
void Breakpoint::SetTemporary(bool temporary) throw (EngineError, SocketError) {
	this->temporary = temporary;
	Set();
}
// }}}
// {{{ void Breakpoint::SetWatchType(const wxString &expression) throw (EngineError, SocketError)
void Breakpoint::SetWatchType(const wxString &expression) throw (EngineError, SocketError) {
	type = WATCH;
	this->expression = expression;
	Set();
}
// }}}

// {{{ void Breakpoint::Disable() throw (EngineError, SocketError)
void Breakpoint::Disable() throw (EngineError, SocketError) {
	enabled = false;
	Set();
}
// }}}
// {{{ void Breakpoint::Enable() throw (EngineError, SocketError)
void Breakpoint::Enable() throw (EngineError, SocketError) {
	enabled = true;
	Set();
}
// }}}

// {{{ void Breakpoint::Get() throw (EngineError, SocketError)
void Breakpoint::Get() throw (EngineError, SocketError) {
	if (!isSet) {
		return;
	}

	wxXmlDocument doc(conn->SendCommandWait(wxT("breakpoint_get"), MessageArguments().Append(wxT("-d"), id)));

	for (wxXmlNode *node = doc.GetRoot()->GetChildren(); node; node = node->GetNext()) {
		if (node->GetType() == wxXML_ELEMENT_NODE && node->GetName() == wxT("breakpoint")) {
			id = node->GetPropVal(wxT("id"), wxEmptyString);
			enabled = (node->GetPropVal(wxT("state"), wxT("disabled")) == wxT("enabled"));
			fileName = node->GetPropVal(wxT("filename"), wxEmptyString);
			function = node->GetPropVal(wxT("function"), wxEmptyString);
			exception = node->GetPropVal(wxT("exception"), wxEmptyString);
			lineNo = StringToInt(node->GetPropVal(wxT("lineno"), wxT("0")));
			hitCount = StringToInt(node->GetPropVal(wxT("hit_count"), wxT("0")));
			hitValue = StringToInt(node->GetPropVal(wxT("hit_value"), wxT("0")));
			hitCondition = StringToHitCondition(node->GetPropVal(wxT("hit_condition"), wxT(">=")));
			type = StringToType(node->GetPropVal(wxT("type"), wxT("line")));

			expression = wxEmptyString;
			for (wxXmlNode *child = node->GetChildren(); child != NULL; child = child->GetNext()) {
				if (child->GetType() == wxXML_ELEMENT_NODE && child->GetName() == wxT("expression")) {
					expression = child->GetNodeContent();
					break;
				}
			}

			break;
		}
	}
}
// }}}
// {{{ void Breakpoint::Set() throw (EngineError, SocketError) 
void Breakpoint::Set() throw (EngineError, SocketError) {
	char *data = NULL;
	size_t dataLength = 0;
	MessageArguments args(5,
			wxT("-t"), TypeToString(type).c_str(),
			wxT("-s"), enabled ? wxT("enabled") : wxT("disabled"),
			wxT("-h"), IntToString(hitValue).c_str(),
			wxT("-o"), HitConditionToString(hitCondition).c_str(),
			wxT("-r"), temporary ? wxT("1") : wxT("0"));

	switch (type) {
		case LINE:
			args.Append(wxT("-f"), fileName).Append(wxT("-n"), IntToString(lineNo));
			break;

		case CALL:
		case RETURN:
			args.Append(wxT("-m"), function);
			break;

		case EXCEPTION:
			args.Append(wxT("-x"), exception);
			break;

		default:
#if defined(wxUSE_UNICODE) && wxUSE_UNICODE == 1
			data = conn->conv->cWX2MB(expression.c_str()).release();
#else
			data = expression.c_str();
#endif
			dataLength = std::strlen(data);

	}

	if (isSet) {
		conn->SendCommandWait(wxT("breakpoint_update"), args.Append(wxT("-d"), id), data, dataLength);
	}
	else {
		wxXmlDocument doc(conn->SendCommandWait(wxT("breakpoint_set"), args, data, dataLength));
		id = doc.GetRoot()->GetPropVal(wxT("id"), wxEmptyString);
	}

#if defined(wxUSE_UNICODE) && wxUSE_UNICODE == 1
	if (data) {
		delete[] data;
	}
#endif

	isSet = true;
}
// }}}

// {{{ wxString Breakpoint::HitConditionToString(HitCondition cond)
wxString Breakpoint::HitConditionToString(HitCondition cond) {
	if (cond == HIT_MULT) {
		return wxT("%");
	}
	else if (cond == HIT_EQ) {
		return wxT("==");
	}
	return wxT(">=");
}
// }}}
// {{{ Breakpoint::HitCondition Breakpoint::StringToHitCondition(const wxString &s)
Breakpoint::HitCondition Breakpoint::StringToHitCondition(const wxString &s) {
	if (s == wxT("%")) {
		return HIT_MULT;
	}
	else if (s == wxT("==")) {
		return HIT_EQ;
	}
	return HIT_GE;
}
// }}}
// {{{ Breakpoint::Type Breakpoint::StringToType(const wxString &s)
Breakpoint::Type Breakpoint::StringToType(const wxString &s) {
	if (s == wxT("call")) {
		return CALL;
	}
	else if (s == wxT("return")) {
		return RETURN;
	}
	else if (s == wxT("exception")) {
		return EXCEPTION;
	}
	else if (s == wxT("conditional")) {
		return CONDITIONAL;
	}
	else if (s == wxT("watch")) {
		return WATCH;
	}
	return LINE;
}
// }}}
// {{{ wxString Breakpoint::TypeToString(Type type)
wxString Breakpoint::TypeToString(Type type) {
	if (type == CALL) {
		return wxT("call");
	}
	else if (type == RETURN) {
		return wxT("return");
	}
	else if (type == EXCEPTION) {
		return wxT("exception");
	}
	else if (type == CONDITIONAL) {
		return wxT("conditional");
	}
	else if (type == WATCH) {
		return wxT("watch");
	}
	return wxT("line");
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
