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

#ifndef TEST_CONNECTION_H
#define TEST_CONNECTION_H

#include <list>

#include "DBGp/Connection.h"
#include "Test/Server.h"

namespace Test {
	class Connection : public DBGp::Connection {
		public:
			Connection(wxSocketBase *socket, Server *server);

			void AddResponse(const wxXmlDocument &doc);
			void ProcessNextResponse();

		protected:
			typedef std::list<wxXmlDocument> ResponseList;
			ResponseList::const_iterator currentResponse;
			bool first;
			ResponseList responses;

			wxXmlDocument GetMessage() throw (DBGp::MalformedDocumentError, DBGp::SocketError);
			DBGp::TransactionID SendCommand(const wxString &command, DBGp::MessageArguments args, const char *data = NULL, size_t dataLength = 0) throw (DBGp::SocketError);
			void SendCommandImmediate(const wxString &command, DBGp::MessageArguments args, const char *data = NULL, size_t dataLength = 0) throw (DBGp::SocketError);
			wxXmlDocument SendCommandWait(const wxString &command, DBGp::MessageArguments args, const char *data = NULL, size_t dataLength = 0) throw (DBGp::EngineError, DBGp::MalformedDocumentError, DBGp::SocketError);
	};
}

#endif

// vim:set fdm=marker ts=8 noet cin:
