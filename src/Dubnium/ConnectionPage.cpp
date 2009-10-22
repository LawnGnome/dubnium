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

#include "ConnectionPage.h"
#include "ArtProvider.h"
#include "PaneMenu.h"

#include <wx/artprov.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/toolbar.h>

// {{{ Event table
BEGIN_EVENT_TABLE(ConnectionPage, wxPanel)
	EVT_DBGP_STATUSCHANGE(wxID_ANY, ConnectionPage::OnStatusChange)
	EVT_DBGP_STDERR(wxID_ANY, ConnectionPage::OnStderr)
	EVT_DBGP_STDOUT(wxID_ANY, ConnectionPage::OnStdout)
	EVT_TOOL(ID_CONNECTIONPAGE_BREAK, ConnectionPage::OnBreak)
	EVT_TOOL(ID_CONNECTIONPAGE_PANES, ConnectionPage::OnPanes)
	EVT_TOOL(ID_CONNECTIONPAGE_RUN, ConnectionPage::OnRun)
	EVT_TOOL(ID_CONNECTIONPAGE_RUN_TO_CURSOR, ConnectionPage::OnRunToCursor)
	EVT_TOOL(ID_CONNECTIONPAGE_STEPINTO, ConnectionPage::OnStepInto)
	EVT_TOOL(ID_CONNECTIONPAGE_STEPOUT, ConnectionPage::OnStepOut)
	EVT_TOOL(ID_CONNECTIONPAGE_STEPOVER, ConnectionPage::OnStepOver)
END_EVENT_TABLE()
// }}}

// {{{ ConnectionPage::ConnectionPage(wxWindow *parent, DBGp::Connection *conn, const wxString &fileURI, const wxString &language)
ConnectionPage::ConnectionPage(wxWindow *parent, DBGp::Connection *conn, const wxString &fileURI, const wxString &language) : wxPanel(parent, ID_CONNECTIONPAGE), conn(conn), language(language), level(level), unavailable(true) {
	config = wxConfigBase::Get();

	conn->SetEventHandler(this);
	breakSupported = conn->CommandSupported(wxT("break"));

	CreateToolBar();
	breakpoint = new BreakpointPanel(this);
	output = new OutputPanel(this);
	properties = new PropertiesPanel(this);
	source = new SourcePanel(this);
	stack = new StackPanel(this);

	SetSource(fileURI);

	wxAuiPaneInfo defaultPane;
	defaultPane.Floatable(false).CloseButton(true);

	mgr = new wxAuiManager(this);
	mgr->AddPane(toolbar, wxAuiPaneInfo().ToolbarPane().Top().Position(0).Floatable(false));
	mgr->AddPane(output, wxAuiPaneInfo(defaultPane).Bottom().Position(0).Caption(_("Output")).MinSize(wxSize(1, 150)));
	mgr->AddPane(breakpoint, wxAuiPaneInfo(defaultPane).Bottom().Position(2).Caption(_("Breakpoints")).MinSize(wxSize(1, 150)));
	mgr->AddPane(stack, wxAuiPaneInfo(defaultPane).Right().Position(0).Caption(_("Call Stack")).MinSize(wxSize(200, 1)));
	mgr->AddPane(properties, wxAuiPaneInfo(defaultPane).Right().Position(1).Caption(_("Properties")).MinSize(wxSize(200, 1)));
	mgr->AddPane(source, wxAuiPaneInfo().CentrePane().Caption(_("Source")).CaptionVisible(true));

#ifdef PERSPECTIVE
	wxString perspective(config->Read(wxT("Perspective"), wxEmptyString));
	if (perspective.Len() > 0) {
		mgr->LoadPerspective(perspective);
	}
#endif

	mgr->Update();
}
// }}}
// {{{ ConnectionPage::~ConnectionPage()
ConnectionPage::~ConnectionPage() {
	conn->Close();
}
// }}}

// {{{ void ConnectionPage::BreakpointAdd(int line, bool temporary)
void ConnectionPage::BreakpointAdd(int line, bool temporary) {
	if (!unavailable) {
		DBGp::Breakpoint *bp = conn->CreateBreakpoint();

		bp->SetLineType(lastFile, line);
		if (temporary) {
			bp->SetTemporary(true);
		}
		bp->Set();

		if (temporary) {
			conn->Run();
		}
		else {
			breakpoint->Update();
		}
	}
}
// }}}
// {{{ void ConnectionPage::BreakpointRemove(const wxString &file, int line)
void ConnectionPage::BreakpointRemove(const wxString &file, int line) {
	source->tc->SetBreakpointStyle(line, false);
}
// }}}
// {{{ void ConnectionPage::BreakpointRemove(int line)
void ConnectionPage::BreakpointRemove(int line) {
	if (!unavailable) {
		DBGp::Breakpoint *bp = breakpoint->GetFileBreakpoint(lastFile, line);

		if (bp) {
			conn->RemoveBreakpoint(bp);
			breakpoint->Update();
		}
	}
}
// }}}
// {{{ wxString ConnectionPage::GetPropertyValue(const wxString &name) const
wxString ConnectionPage::GetPropertyValue(const wxString &name) const {
	return properties->GetPropertyValue(name);
}
// }}}
// {{{ void ConnectionPage::SavePerspective()
void ConnectionPage::SavePerspective() {
	config->Write(wxT("Perspective"), mgr->SavePerspective());
}
// }}}
// {{{ void ConnectionPage::SetStackLevel(DBGp::StackLevel *level)
void ConnectionPage::SetStackLevel(DBGp::StackLevel *level) {
	this->level = level;
	properties->SetStackLevel(level);
	SetSource(level->GetFileName(), level->GetLineNo());
}
// }}}

// {{{ void ConnectionPage::CreateToolBar()
void ConnectionPage::CreateToolBar() {
	wxSize size(ArtProvider::toolbarSize, ArtProvider::toolbarSize);
	toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxNO_BORDER | wxTB_FLAT | wxTB_NODIVIDER);
	toolbar->SetToolBitmapSize(size);

	toolbar->AddTool(ID_CONNECTIONPAGE_OPEN, _("Open"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, size), _("Open a source file"));
	toolbar->AddSeparator();
	toolbar->AddTool(ID_CONNECTIONPAGE_RUN, _("Run"), wxArtProvider::GetBitmap(wxT("run"), wxART_TOOLBAR, size), _("Run"));
	if (breakSupported) {
		toolbar->AddTool(ID_CONNECTIONPAGE_BREAK, _("Break"), wxArtProvider::GetBitmap(wxT("break"), wxART_TOOLBAR, size), _("Break"));
	}
	toolbar->AddTool(ID_CONNECTIONPAGE_STEPINTO, _("Step Into"), wxArtProvider::GetBitmap(wxT("step-into"), wxART_TOOLBAR, size), _("Step Into"));
	toolbar->AddTool(ID_CONNECTIONPAGE_STEPOVER, _("Step Over"), wxArtProvider::GetBitmap(wxT("step-over"), wxART_TOOLBAR, size), _("Step Over"));
	toolbar->AddTool(ID_CONNECTIONPAGE_STEPOUT, _("Step Out"), wxArtProvider::GetBitmap(wxT("step-out"), wxART_TOOLBAR, size), _("Step Out"));
	toolbar->AddSeparator();
	toolbar->AddTool(ID_CONNECTIONPAGE_RUN_TO_CURSOR, _("Run to Cursor"), wxArtProvider::GetBitmap(wxT("run-to-cursor"), wxART_TOOLBAR, size), _("Run to Cursor"));
	toolbar->AddSeparator();
	toolbar->AddTool(ID_CONNECTIONPAGE_PANES, _("Panes"), wxArtProvider::GetBitmap(wxT("panes"), wxART_TOOLBAR, size), _("Toggle which panes are displayed"));
	toolbar->Realize();

	// Disable the tools that don't make sense at the start.
	UpdateToolBar(true, false, true, false, false);
}
// }}}
// {{{ void ConnectionPage::OnBreak(wxCommandEvent &event)
void ConnectionPage::OnBreak(wxCommandEvent &event) {
	conn->Break();
}
// }}}
// {{{ void ConnectionPage::OnPanes(wxCommandEvent &event)
void ConnectionPage::OnPanes(wxCommandEvent &event) {
	PaneMenu menu(this, _("Panes"), mgr->GetAllPanes());
	PopupMenu(&menu);
	wxAuiPaneInfo *pane = menu.GetPane();
	if (pane) {
		pane->Show(!(pane->IsShown()));
		mgr->Update();
	}
}
// }}}
// {{{ void ConnectionPage::OnRun(wxCommandEvent &event)
void ConnectionPage::OnRun(wxCommandEvent &event) {
	UpdateToolBar(false, true, false, false, false);
	conn->Run();
}
// }}}
// {{{ void ConnectionPage::OnRunToCursor(wxCommandEvent &event)
void ConnectionPage::OnRunToCursor(wxCommandEvent &event) {
	DBGp::Breakpoint *bp;
	int line = source->tc->LineFromPosition(source->tc->GetCurrentPos()) + 1;

	bp = conn->CreateBreakpoint();
	bp->SetLineType(lastFile, line);
	bp->SetTemporary(true);
	bp->Set();

	UpdateToolBar(false, true, false, false, false);
	conn->Run();
}
// }}}
// {{{ void ConnectionPage::OnStatusChange(DBGp::StatusChangeEvent &event)
void ConnectionPage::OnStatusChange(DBGp::StatusChangeEvent &event) {
	if (event.GetStatus() == DBGp::Connection::BREAK) {
		UpdateToolBar(true, false, true, true, true);
		UpdateStack();
	}
	else if (event.GetStatus() == DBGp::Connection::RUNNING) {
		UpdateToolBar(false, true, false, false, false);
	}
	else {
		UpdateToolBar(false, false, false, false, false);
		stack->SetStack(NULL);
		properties->SetStackLevel(NULL);
		source->Unavailable(_("No source is available as execution has finished."));
		unavailable = true;
	}
}
// }}}
// {{{ void ConnectionPage::OnStderr(DBGp::StderrEvent &event)
void ConnectionPage::OnStderr(DBGp::StderrEvent &event) {
	output->AppendStderr(event.GetData());
}
// }}}
// {{{ void ConnectionPage::OnStdout(DBGp::StdoutEvent &event)
void ConnectionPage::OnStdout(DBGp::StdoutEvent &event) {
	output->AppendStdout(event.GetData());
}
// }}}
// {{{ void ConnectionPage::OnStepInto(wxCommandEvent &event)
void ConnectionPage::OnStepInto(wxCommandEvent &event) {
	conn->StepInto();
}
// }}}
// {{{ void ConnectionPage::OnStepOut(wxCommandEvent &event)
void ConnectionPage::OnStepOut(wxCommandEvent &event) {
	conn->StepOut();
}
// }}}
// {{{ void ConnectionPage::OnStepOver(wxCommandEvent &event)
void ConnectionPage::OnStepOver(wxCommandEvent &event) {
	conn->StepOver();
}
// }}}
// {{{ void ConnectionPage::SetSource(const wxString &file, int line)
void ConnectionPage::SetSource(const wxString &file, int line) {
	if (file != lastFile) {
		try {
			source->tc->SetSource(conn->Source(file), line);
			source->tc->SetLexerLanguage(language);
			lastFile = file;
			DBGp::Connection::BreakpointList list(breakpoint->GetFileBreakpoints(file));
			for (DBGp::Connection::BreakpointList::const_iterator i = list.begin(); i != list.end(); i++) {
				source->tc->SetBreakpointStyle((*i)->GetLineNo(), true);
			}
			unavailable = false;
		}
		catch (DBGp::Error e) {
			source->Unavailable(_("No source is available for this stack frame."));
			unavailable = true;
		}
	}
	else if (line >= 0) {
		source->tc->SetLine(line);
	}
}
// }}}
// {{{ void ConnectionPage::UpdateStack()
void ConnectionPage::UpdateStack() {
	DBGp::Stack stack(conn->StackGet());
	this->stack->SetStack(&stack);
	SetStackLevel(stack.GetLevel(0));
}
// }}}
// {{{ void ConnectionPage::UpdateToolBar(bool run, bool brk, bool stepInto, bool stepOver, bool stepOut)
void ConnectionPage::UpdateToolBar(bool run, bool brk, bool stepInto, bool stepOver, bool stepOut) {
	toolbar->EnableTool(ID_CONNECTIONPAGE_RUN, run);
	toolbar->EnableTool(ID_CONNECTIONPAGE_BREAK, brk);
	toolbar->EnableTool(ID_CONNECTIONPAGE_STEPINTO, stepInto);
	toolbar->EnableTool(ID_CONNECTIONPAGE_STEPOVER, stepOver);
	toolbar->EnableTool(ID_CONNECTIONPAGE_STEPOUT, stepOut);
	toolbar->EnableTool(ID_CONNECTIONPAGE_RUN_TO_CURSOR, run);
}
// }}}

// vim:set fdm=marker ts=8 noet cin:
