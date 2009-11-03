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

#include <wx/app.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/log.h>
#include <wx/socket.h>
#include <wx/string.h>

#include "DBGp/Server.h"
#include "DBGp/Connection.h"
#include "DBGp/Event/ConnectionEvent.h"
#include "DBGp/Event/StatusChangeEvent.h"
#include "DBGp/Event/StderrEvent.h"
#include "DBGp/Event/StdoutEvent.h"
#include "DBGp/Event/StreamEvent.h"

wxLogStderr *logTarget;

class SocketServerTestFrame : public wxFrame {
	public:
		SocketServerTestFrame();
		virtual ~SocketServerTestFrame();

	private:
		DBGp::Connection *conn;
		DBGp::Server *server;

		void OnConnect(DBGp::ConnectionEvent &event);
		void OnStatus(wxCommandEvent &event);
		void OnStderr(DBGp::StderrEvent &event);
		void OnStdout(DBGp::StdoutEvent &event);
		void OnStream(DBGp::StreamEvent &event);

		DECLARE_EVENT_TABLE()
};

class SocketServerTestApp : public wxApp {
	public:
		virtual bool OnInit();
};

// vim:set fdm=marker ts=8 sw=8 noet cin:
