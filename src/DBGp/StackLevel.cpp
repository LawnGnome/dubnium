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

#include "DBGp/StackLevel.h"
#include "DBGp/Connection.h"
#include "DBGp/Utility.h"

using namespace DBGp;

// {{{ StackLevel::StackLevel(Connection *conn, unsigned int level) throw (EngineError, MalformedDocumentError, NotFoundError, SocketError)
StackLevel::StackLevel(Connection *conn, unsigned int level) throw (EngineError, MalformedDocumentError, NotFoundError, SocketError) : conn(conn), level(level) {
	GetStack();
	GetEngineContexts();
}
// }}}
// {{{ StackLevel::StackLevel(const StackLevel &level)
StackLevel::StackLevel(const StackLevel &level) : cmdBegin(level.cmdBegin), cmdEnd(level.cmdEnd), conn(level.conn), fileName(level.fileName), level(level.level), lineNo(level.lineNo), type(level.type), where(level.where) {
	for (ContextMap::const_iterator i = level.contexts.begin(); i != level.contexts.end(); i++) {
		contexts[i->first] = new Context(*(i->second));
	}
}
// }}}
// {{{ StackLevel::~StackLevel()
StackLevel::~StackLevel() {
	for (ContextMap::iterator i = contexts.begin(); i != contexts.end(); i++) {
		delete i->second;
	}
}
// }}}

// {{{ StackLevel::Type StackLevel::StringToType(const wxString &s)
StackLevel::Type StackLevel::StringToType(const wxString &s) {
	if (s == wxT("file")) {
		return FILE;
	}
	return EVAL;
}
// }}}
// {{{ wxString StackLevel::TypeToString(StackLevel::Type type)
wxString StackLevel::TypeToString(StackLevel::Type type) {
	if (type == FILE) {
		return wxT("file");
	}
	return wxT("eval");
}
// }}}

// {{{ void StackLevel::GetEngineContexts() throw (EngineError, SocketError)
void StackLevel::GetEngineContexts() throw (EngineError, SocketError) {
	wxXmlDocument doc(conn->SendCommandWait(wxT("context_names"), MessageArguments().Append(wxT("-d"), IntToString(level))));

	contexts.clear();

	for (wxXmlNode *node = doc.GetRoot()->GetChildren(); node != NULL; node = node->GetNext()) {
		if (node->GetType() == wxXML_ELEMENT_NODE && node->GetName() == wxT("context")) {
			wxString id(node->GetPropVal(wxT("id"), wxEmptyString));
			contexts[id] = new Context(conn, this, id, node->GetPropVal(wxT("name"), wxEmptyString));
		}
	}
}
// }}}
// {{{ void StackLevel::GetStack() throw (EngineError, MalformedDocumentError, NotFoundError, SocketError)
void StackLevel::GetStack() throw (EngineError, MalformedDocumentError, NotFoundError, SocketError) {
	wxXmlDocument doc;
	wxXmlNode *stack = NULL;

	try {
		doc = conn->SendCommandWait(wxT("stack_get"), MessageArguments().Append(wxT("-d"), IntToString(level)));
	}
	catch (EngineError e) {
		if (e.GetCode() == 301) {
			throw NotFoundError(wxString(wxT("Stack depth ")) + IntToString(level) + wxT(" not found."));
		}
		throw;
	}
	catch (...) {
		throw;
	}

	for (wxXmlNode *node = doc.GetRoot()->GetChildren(); node != NULL; node = node->GetNext()) {
		if (node->GetType() == wxXML_ELEMENT_NODE && node->GetName() == wxT("stack")) {
			stack = node;
			break;
		}
	}

	if (!stack) {
		throw MalformedDocumentError(wxT("Invalid stack_get response."));
	}

	type = StringToType(stack->GetPropVal(wxT("type"), wxT("file")));
	fileName = stack->GetPropVal(wxT("filename"), wxEmptyString);
	lineNo = StringToInt(stack->GetPropVal(wxT("lineno"), wxT("0")));
	where = stack->GetPropVal(wxT("where"), wxEmptyString);
	cmdBegin = Location(stack->GetPropVal(wxT("cmdbegin"), wxEmptyString));
	cmdEnd = Location(stack->GetPropVal(wxT("cmdend"), wxEmptyString));
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
