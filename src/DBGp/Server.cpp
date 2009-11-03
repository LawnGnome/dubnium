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

#include "DBGp/Server.h"

#include <algorithm>

#include <wx/log.h>

using namespace DBGp;

BEGIN_EVENT_TABLE(Server, wxEvtHandler)
	EVT_SOCKET(1, Server::OnServerEvent)
END_EVENT_TABLE()

// {{{ Server::Server(wxUint16 port, wxEvtHandler *parent)
Server::Server(wxUint16 port, wxEvtHandler *parent) : wxEvtHandler(), parent(parent), port(port) {
	wxIPV4address addr;

	addr.AnyAddress();
	addr.Service(port);

	server = new wxSocketServer(addr, wxSOCKET_REUSEADDR);
	if (!server) {
		wxLogFatalError(wxT("Unable to create server object."));
	}
	else if (!server->IsOk()) {
		wxLogError(wxT("Error instantiating server object."));
	}
	server->SetEventHandler(*this, 1);
	server->SetNotify(wxSOCKET_CONNECTION_FLAG);
	server->Notify(true);

	if (parent) {
		SetNextHandler(parent);
	}
}
// }}}
// {{{ Server::~Server()
Server::~Server() {
	server->Notify(false);

	for (ConnectionList::iterator i = connections.begin(); i != connections.end(); i++)
		delete *i;

	server->Destroy();
}
// }}}

// {{{ void Server::RemoveConnection(Connection *conn)
void Server::RemoveConnection(Connection *conn) {
	std::remove(connections.begin(), connections.end(), conn);
	delete conn;
}
// }}}

// {{{ Connection *Server::CreateConnectionObject(wxSocketBase *socket, Server *server)
Connection *Server::CreateConnectionObject(wxSocketBase *socket, Server *server) {
	return new Connection(socket, server);
}
// }}}
// {{{ void Server::OnServerEvent(wxSocketEvent &event)
void Server::OnServerEvent(wxSocketEvent &event) {
	wxSocketClient *socket;

	// We're only interested in connections at this level.
	if (event.GetSocketEvent() != wxSOCKET_CONNECTION) {
		event.Skip(true);
		return;
	}

	socket = new wxSocketClient;
	if (!socket) {
		wxLogFatalError(wxT("Unable to create client socket."));
	}

	if (server->AcceptWith(*socket, false)) {
		wxIPV4address addr;
		socket->GetPeer(addr);
		wxLogDebug(wxT("Got connection from %s:%hu."), addr.Hostname().c_str(), addr.Service());
		Connection *conn = CreateConnectionObject(socket, this);
		connections.push_back(conn);
	}
	else {
		wxLogWarning(wxT("Got server socket event but no connection is in the accept queue."));
	}
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
