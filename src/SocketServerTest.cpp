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

#include "SocketServerTest.h"
#include <wx/utils.h>

SocketServerTestFrame::SocketServerTestFrame() : wxFrame(NULL, -1, wxT("Test Application")), conn(NULL) {
	server = new DBGp::Server(9000, this);

	new wxButton(this, 101, wxT("Status"));
}

SocketServerTestFrame::~SocketServerTestFrame() {
	delete server;
	wxLogDebug(wxT("Dying, goodbye."));
	wxLog::SetActiveTarget(NULL);
	delete logTarget;
}

void SocketServerTestFrame::OnConnect(DBGp::ConnectionEvent &event) {
	conn = event.GetConnection();

	wxLogDebug(wxT("Source: %s"), conn->Source(event.GetFileURI()).c_str());

	DBGp::Breakpoint *bp = conn->CreateBreakpoint();
	bp->SetCallType(wxT("phpinfo"));
	bp->Set();

	conn->Run();
	wxSleep(1);
	conn->StackGet();
	conn->Detach();
}

void SocketServerTestFrame::OnStatus(wxCommandEvent &event) {
	if (conn) {
		try {
			conn->Status();
		}
		catch (DBGp::Error e) {
			wxLogError(wxT("Error getting status: %s"), e.GetMessage().c_str());
		}
	}
}

void SocketServerTestFrame::OnStderr(DBGp::StderrEvent &event) {
	wxLogDebug(wxT("Stderr: %s"), event.GetData().c_str());
}

void SocketServerTestFrame::OnStdout(DBGp::StdoutEvent &event) {
	wxLogDebug(wxT("Stdout: %s"), event.GetData().c_str());
}

void SocketServerTestFrame::OnStream(DBGp::StreamEvent &event) {
	wxLogDebug(wxT("Stream: %s"), event.GetData().c_str());
}

bool SocketServerTestApp::OnInit() {
	SocketServerTestFrame *frame = new SocketServerTestFrame;
	frame->Show(true);
	SetTopWindow(frame);
	wxLog::SetActiveTarget(logTarget = new wxLogStderr);
	wxLogDebug(wxT("Up and running."));
	return true;
}

BEGIN_EVENT_TABLE(SocketServerTestFrame, wxFrame)
	EVT_BUTTON(101, SocketServerTestFrame::OnStatus)
	EVT_DBGP_CONNECTION(wxID_ANY, SocketServerTestFrame::OnConnect)
	EVT_DBGP_STDERR(wxID_ANY, SocketServerTestFrame::OnStderr)
	EVT_DBGP_STDOUT(wxID_ANY, SocketServerTestFrame::OnStdout)
	EVT_DBGP_STREAM(wxID_ANY, SocketServerTestFrame::OnStream)
END_EVENT_TABLE()

IMPLEMENT_APP(SocketServerTestApp)

// vim:set fdm=marker ts=8 sw=8 noet cin:
