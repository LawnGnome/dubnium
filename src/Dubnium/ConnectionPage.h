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

#ifndef DUBNIUM_CONNECTIONPAGE_H
#define DUBNIUM_CONNECTIONPAGE_H

#include <wx/aui/aui.h>
#include <wx/confbase.h>
#include <wx/panel.h>

#include "DBGp/Connection.h"
#include "DBGp/Event.h"
#include "DBGp/StackLevel.h"

#include "BreakpointPanel.h"
#include "ID.h"
#include "OutputPanel.h"
#include "PropertiesPanel.h"
#include "SourcePanel.h"
#include "SourceTextCtrlHandler.h"
#include "StackPanel.h"

class ConnectionPage : public wxPanel, public SourceTextCtrlHandler {
	public:
		ConnectionPage(wxWindow *parent, DBGp::Connection *conn, const wxString &fileURI, const wxString &language);

		virtual void BreakpointAdd(int line, bool temporary = false);
		void BreakpointRemove(const wxString &file, int line);
		virtual void BreakpointRemove(int line);
		inline DBGp::Connection *GetConnection() { return conn; }
		inline wxString GetLastFile() const { return lastFile; }
		virtual wxString GetPropertyValue(const wxString &name) const;
		void SavePerspective();
		void SetStackLevel(DBGp::StackLevel *level);

	protected:
		BreakpointPanel *breakpoint;
		bool breakSupported;
		wxConfigBase *config;
		DBGp::Connection *conn;
		wxString language;
		wxString lastFile;
		DBGp::StackLevel *level;
		wxAuiManager *mgr;
		OutputPanel *output;
		PropertiesPanel *properties;
		SourcePanel *source;
		StackPanel *stack;
		wxToolBar *toolbar;
		bool unavailable;

		void CreateToolBar();
		void OnBreak(wxCommandEvent &event);
		void OnPanes(wxCommandEvent &event);
		void OnRun(wxCommandEvent &event);
		void OnRunToCursor(wxCommandEvent &event);
		void OnStatusChange(DBGp::StatusChangeEvent &event);
		void OnStderr(DBGp::StderrEvent &event);
		void OnStdout(DBGp::StdoutEvent &event);
		void OnStepInto(wxCommandEvent &event);
		void OnStepOut(wxCommandEvent &event);
		void OnStepOver(wxCommandEvent &event);
		void SetSource(const wxString &file, int line = -1);
		void UpdateStack();
		void UpdateToolBar(bool run, bool brk, bool stepInto, bool stepOver, bool stepOut);

		DECLARE_EVENT_TABLE()
};

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
