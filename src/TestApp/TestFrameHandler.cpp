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

#include "TestFrameHandler.h"

#include <wx/aboutdlg.h>

// {{{ TestFrameHandler::TestFrameHandler()
TestFrameHandler::TestFrameHandler() : TestFrame(NULL), conn(NULL) {
	server = new DBGp::Server(9000, this);
	statusBar->SetStatusText(_("Ready"));
	wxLog::SetActiveTarget(new wxLogTextCtrl(debugTextCtrl));
}
// }}}
// {{{ TestFrameHandler::~TestFrameHandler()
TestFrameHandler::~TestFrameHandler() {
	if (conn) {
		delete conn;
	}
	delete server;
	wxLog::SetActiveTarget(NULL);
}
// }}}

// {{{ void TestFrameHandler::OnConnect(DBGp::ConnectionEvent &event)
void TestFrameHandler::OnConnect(DBGp::ConnectionEvent &event) {
	if (conn) {
		delete conn;
	}
	conn = event.GetConnection();
	run->Enable();
	statusBar->SetStatusText(_("Connected"));

	output = wxEmptyString;
	outputHTML->SetPage(output);

	try {
		sourceTextCtrl->SetValue(conn->Source(event.GetFileURI()));
	}
	catch (DBGp::Error e) {
		sourceTextCtrl->SetValue(wxEmptyString);
		wxLogError(_("Error getting source: %s."), e.GetMessage().c_str());
	}
}
// }}}
// {{{ void TestFrameHandler::OnFileExit(wxCommandEvent &event)
void TestFrameHandler::OnFileExit(wxCommandEvent &event) {
	Destroy();
}
// }}}
// {{{ void TestFrameHandler::OnHelpAbout(wxCommandEvent &event)
void TestFrameHandler::OnHelpAbout(wxCommandEvent &event) {
	wxAboutDialogInfo info;

	info.AddDeveloper(wxT("Adam Harvey"));
	info.SetCopyright(wxT("(C) 2007 Adam Harvey"));
	info.SetDescription(wxT("A simple test application to demonstrate libDBGp."));
	info.SetName(_("libDBGp Ad-Hoc Test Thingy"));
	info.SetVersion(_("Pre-Release"));

	wxAboutBox(info);
}
// }}}
// {{{ void TestFrameHandler::OnRun(wxCommandEvent &event)
void TestFrameHandler::OnRun(wxCommandEvent &event) {
	wxASSERT(conn != NULL);

	run->Disable();
	statusBar->SetStatusText(_("Running"));

	try {
		conn->Detach();
	}
	catch (DBGp::UnsupportedFeatureError e) {
		conn->Run();
	}
}
// }}}
// {{{ void TestFrameHandler::OnStatus(DBGp::StatusChangeEvent &event)
void TestFrameHandler::OnStatus(DBGp::StatusChangeEvent &event) {
	DBGp::Connection::EngineStatus status = event.GetStatus();

	wxLogDebug(wxT("In OnStatus"));
	statusBar->SetStatusText(DBGp::Connection::EngineStatusToString(status));

	if (status == DBGp::Connection::STOPPING || status == DBGp::Connection::STOPPED) {
		//conn->Destroy();
		//conn = NULL;
	}
}
// }}}
// {{{ void TestFrameHandler::OnStdout(DBGp::StdoutEvent &event)
void TestFrameHandler::OnStdout(DBGp::StdoutEvent &event) {
	output += event.GetData();
	outputHTML->SetPage(output);
}
// }}}

// {{{ Event table
/* We need to redefine the event handlers for the built-ins due to the fact
 * that events don't bubble up the inheritance tree. */
BEGIN_EVENT_TABLE(TestFrameHandler, wxFrame)
	EVT_BUTTON(ID_RUN, TestFrameHandler::OnRun)
	EVT_MENU(ID_FILE_EXIT, TestFrameHandler::OnFileExit)
	EVT_MENU(ID_HELP_ABOUT, TestFrameHandler::OnHelpAbout)
	EVT_DBGP_CONNECTION(wxID_ANY, TestFrameHandler::OnConnect)
	EVT_DBGP_STATUSCHANGE(wxID_ANY, TestFrameHandler::OnStatus)
	EVT_DBGP_STDOUT(wxID_ANY, TestFrameHandler::OnStdout)
END_EVENT_TABLE()
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
