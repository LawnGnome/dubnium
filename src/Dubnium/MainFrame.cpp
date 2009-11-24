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

#include "MainFrame.h"
#include "Config.h"
#include "ConnectionPage.h"
#include "DebugPage.h"
#include "ID.h"
#include "PrefDialog.h"
#include "WelcomePage.h"

#include <wx/aboutdlg.h>
#include <wx/filename.h>
#include <wx/sizer.h>
#include <wx/uri.h>

#include "DBGp/Connection.h"

// {{{ Event table
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_CLOSE(MainFrame::OnClose)
	EVT_DBGP_CONNECTION(wxID_ANY, MainFrame::OnConnection)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_MENU(wxID_EXIT, MainFrame::OnQuit)
	EVT_MENU(wxID_PREFERENCES, MainFrame::OnPreferences)
END_EVENT_TABLE()
// }}}

// {{{ MainFrame::MainFrame()
MainFrame::MainFrame() : wxFrame(NULL, ID_MAINFRAME, _("Dubnium")) {
	config = wxConfigBase::Get();

	server = new DBGp::Server(static_cast<wxUint16>(config->Read(wxT("Network/Port"), 9000)), this);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(notebook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_WINDOWLIST_BUTTON | wxAUI_NB_CLOSE_ON_ALL_TABS), 1, wxEXPAND | wxALL);

	notebook->AddPage(new WelcomePage(notebook), _("Welcome"));
#ifdef __WXDEBUG__
	notebook->AddPage(new DebugPage(notebook), _("Debug Log"));
#endif

	SetMenuBar(CreateMenuBar());
	SetAutoLayout(true);
	SetSizer(sizer);
	sizer->SetSizeHints(this);

#ifndef __WXMAC__
	wxIcon icon;
	icon.CopyFromBitmap(wxArtProvider::GetBitmap(wxT("icon"), wxART_TOOLBAR, wxSize(32, 32)));
	SetIcon(icon);
#endif

	LoadSize();
}
// }}}
// {{{ MainFrame::~MainFrame()
MainFrame::~MainFrame() {
	delete server;
}
// }}}

// {{{ wxMenuBar *MainFrame::CreateMenuBar()
wxMenuBar *MainFrame::CreateMenuBar() {
	wxMenuBar *menuBar = new wxMenuBar;

#ifndef __WXMAC__
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(wxID_EXIT);
	menuBar->Append(fileMenu, _("&File"));
#endif

	wxMenu *toolMenu = new wxMenu;
	toolMenu->Append(wxID_PREFERENCES);
	menuBar->Append(toolMenu, _("&Tools"));

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_ABOUT);
	menuBar->Append(helpMenu, _("&Help"));

#ifdef __WXMAC__
	wxApp::s_macHelpMenuTitleName = _("&Help");
#endif

	return menuBar;
}
// }}}
// {{{ void MainFrame::LoadSize()
void MainFrame::LoadSize() {
	bool max = false;

	config->Read(wxT("Maximised"), &max);
	if (max) {
		Maximize(true);
	}
	else {
		SetSize(config->Read(wxT("Position/X"), 10L), 
			config->Read(wxT("Position/Y"), 10L),
			config->Read(wxT("Size/Width"), 780L),
			config->Read(wxT("Size/Height"), 550L));
	}
}
// }}}
// {{{ void MainFrame::OnAbout(wxCommandEvent &event)
void MainFrame::OnAbout(wxCommandEvent &event) {
	wxAboutDialogInfo info;

	info.AddDeveloper(wxT("Adam Harvey"));
	info.SetName(APPNAME);
	info.SetVersion(VERSION);

	wxAboutBox(info);
}
// }}}
// {{{ void MainFrame::OnClose(wxCloseEvent &event)
void MainFrame::OnClose(wxCloseEvent &event) {
	config->Write(wxT("Maximised"), IsMaximized());
	config->Write(wxT("Position/X"), GetPosition().x);
	config->Write(wxT("Position/Y"), GetPosition().y);
	config->Write(wxT("Size/Width"), GetSize().x);
	config->Write(wxT("Size/Height"), GetSize().y);

	config->Flush();

	Destroy();
}
// }}}
// {{{ void MainFrame::OnConnection(DBGp::ConnectionEvent &event)
void MainFrame::OnConnection(DBGp::ConnectionEvent &event) {
	wxString expectedKey(config->Read(wxT("Network/IDEKey"), wxEmptyString));
	if (expectedKey.Len() == 0 || expectedKey == event.GetIDEKey()) {
		wxFileName name(wxURI::Unescape(wxURI(event.GetFileURI()).GetPath()));
		notebook->AddPage(new ConnectionPage(notebook, event.GetConnection(), event.GetFileURI(), event.GetLanguage()), name.GetFullName(), true);

		if (!IsActive()) {
			RequestUserAttention(wxUSER_ATTENTION_INFO);
		}
	}
	else {
		// TODO: Ponder how we want to notify the user of this. Indeed,
		// ponder if we want to notify the user of this.
		wxLogDebug(wxT("Connection refused due to IDE key mismatch: expected %s; got %s."), expectedKey.c_str(), event.GetIDEKey().c_str());
		event.GetConnection()->Close();
	}
}
// }}}
// {{{ void MainFrame::OnPreferences(wxCommandEvent &event)
void MainFrame::OnPreferences(wxCommandEvent &event) {
	PrefDialog dialog(this, -1, _("Preferences"));
	dialog.ShowModal();
}
// }}}
// {{{ void MainFrame::OnQuit(wxCommandEvent &event)
void MainFrame::OnQuit(wxCommandEvent &event) {
	Close(false);
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
