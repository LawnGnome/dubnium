// {{{ Copyright notice
/* Copyright (c) 2007-2009, Adam Harvey
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

#include "DBGp/Event/ConnectionEvent.h"

const wxEventType wxEVT_DBGP_CONNECTION = wxNewEventType();

using namespace DBGp;

// {{{ ConnectionEvent::ConnectionEvent(Connection *conn, const wxString &appID, const wxString &ideKey, const wxString &session, const wxString &thread, const wxString &parent, const wxString &language, const wxString &protocolVersion, const wxString &fileURI)
ConnectionEvent::ConnectionEvent(Connection *conn, const wxString &appID, const wxString &ideKey, const wxString &session, const wxString &thread, const wxString &parent, const wxString &language, const wxString &protocolVersion, const wxString &fileURI) :
	Event(conn, wxEVT_DBGP_CONNECTION),
	appID(appID),
	fileURI(fileURI),
	ideKey(ideKey),
	language(language),
	parent(parent),
	protocolVersion(protocolVersion),
	session(session),
	thread(thread) {
}
// }}}
// {{{ ConnectionEvent::ConnectionEvent(const ConnectionEvent &event)
ConnectionEvent::ConnectionEvent(const ConnectionEvent &event) :
	Event(event),
	appID(event.appID),
	fileURI(event.fileURI),
	ideKey(event.ideKey),
	language(event.language),
	parent(event.parent),
	protocolVersion(event.protocolVersion),
	session(event.session),
	thread(event.thread) {
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
