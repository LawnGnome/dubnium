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

#include "DBGp/Stack.h"
#include "DBGp/Connection.h"
#include "DBGp/Utility.h"

#include <stdexcept>

#include <wx/log.h>
#include <wx/string.h>

using namespace DBGp;

// {{{ Stack::Stack(Connection *conn) throw (EngineError, MalformedDocumentError, SocketError)
Stack::Stack(Connection *conn) throw (EngineError, MalformedDocumentError, SocketError) : conn(conn) {
	unsigned int depth;
	wxXmlDocument doc(conn->SendCommandWait(wxT("stack_depth"), MessageArguments()));
	
	depth = StringToInt(doc.GetRoot()->GetPropVal(wxT("depth"), wxT("0")));
	for (unsigned int i = 0; i < depth; i++) {
		try {
			levels.push_back(new StackLevel(conn, i));
		}
		catch (NotFoundError e) {
			wxLogWarning(wxT("Expected stack depth %u not found."), i);
		}
	}
}
// }}}
// {{{ Stack::~Stack()
Stack::~Stack() {
	for (StackLevelDeque::iterator i = levels.begin(); i != levels.end(); i++) {
		delete *i;
	}
}
// }}}

// {{{ StackLevel *Stack::GetLevel(unsigned int level) throw (NotFoundError)
StackLevel *Stack::GetLevel(unsigned int level) throw (NotFoundError) {
	try {
		return levels.at(level);
	}
	catch (std::out_of_range e) {
		throw NotFoundError(wxString(wxT("The requested stack level ")) + IntToString(level) + wxT(" does not exist."));
	}
}
// }}}

// vim:set fdm=marker ts=8 noet cin:
