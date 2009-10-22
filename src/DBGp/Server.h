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

#ifndef DBGP_SERVER_H
#define DBGP_SERVER_H

#include <list>

#include <wx/event.h>
#include <wx/socket.h>
#include <wx/thread.h>

#include "DBGp/Connection.h"

namespace DBGp {
	/**
	 * The server that accepts new DBGp connections and spawns Connection
	 * objects to handle them.
	 */
	class Server : public wxEvtHandler {
		/**
		 * Required for connections to be able to update the active
		 * list within the object.
		 */
		friend class Connection;

		public:
			/**
			 * Constructs a new listening server.
			 *
			 * @param[in] port The TCP port to listen on.
			 * @param[in] parent The parent event handler, if any.
			 * @todo Allow binding to a particular interface.
			 */
			Server(wxUint16 port, wxEvtHandler *parent = NULL);

			/** Shuts down the server. */
			virtual ~Server();

			/**
			 * Removes a connection from the active list. This also
			 * destroys the connection; the connection should not
			 * be used after this.
			 *
			 * @param[in] conn The connection to remove.
			 */
			void RemoveConnection(Connection *conn);

		protected:
			/**
			 * Creates a new connection object. This can be
			 * overridden in a subclass to allow for children of
			 * Connection to be created instead of Connection
			 * itself.
			 *
			 * @param[in] socket The client socket to use within
			 * the connection.
			 * @param[in] server A pointer to the server.
			 * @return A Connection object (or child) created using
			 * the new operator.
			 */
			virtual Connection *CreateConnectionObject(wxSocketBase *socket, Server *server);

		private:
			/** Typedef for the internal connection map. */
			typedef std::list<Connection *> ConnectionList;

			/** The internal list of active connections. */
			ConnectionList connections;

			/** The parent event handler. */
			wxEvtHandler *parent;

			/** The TCP port to listen on. */
			wxUint16 port;

			/** The socket server listening for DBGp connections. */
			wxSocketServer *server;

			/**
			 * Called when connections are made to the listening
			 * socket.
			 *
			 * @param[in] event The socket event object.
			 */
			void OnServerEvent(wxSocketEvent &event);

			DECLARE_EVENT_TABLE()
	};
}

#endif

// vim:set fdm=marker ts=8 noet cin:
