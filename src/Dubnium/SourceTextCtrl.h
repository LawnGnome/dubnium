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

#ifndef DUBNIUM_SOURCETEXTCTRL_SOURCETEXTCTRL_H
#define DUBNIUM_SOURCETEXTCTRL_SOURCETEXTCTRL_H

#include <list>

#include <wx/confbase.h>
#include <wx/stc/stc.h>
#include <wx/timer.h>
#include <wx/tipwin.h>

#include "SourceTextCtrlHandler.h"

class SourceTextCtrl : public wxStyledTextCtrl {
	public:
		SourceTextCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0, const wxString &name = wxTextCtrlNameStr);

		void AddBreakpoint(int line);
		static wxFont DefaultFont();
		void RemoveBreakpoint(int line);
		void SetBreakpointStyle(int line, bool isSet);
		virtual void SetLexerLanguage(const wxString &language);
		void SetLine(int line);
		void SetSource(const wxString &source, int line = -1);

	protected:
		typedef std::list<int> BreakpointList;

		BreakpointList breakpoints;
		wxConfigBase *config;
		int current;
		SourceTextCtrlHandler *handler;
		int menuPos;
		bool menuShown;
		wxString source;
		wxTipWindow *tipWindow;

		enum {
			MARKER_ALL = -1,
			MARKER_BREAKPOINT = 0,
			MARKER_CURRENT
		};

		DBGp::Property *GetPropertyAtPosition(int pos);
		void OnContextMenu(wxContextMenuEvent &event);
		void OnCopy(wxCommandEvent &event);
		void OnDoubleClick(wxMouseEvent &event);
		void OnDwellEnd(wxStyledTextEvent &event);
		void OnDwellStart(wxStyledTextEvent &event);
		void OnExamineValue(wxCommandEvent &event);
		void OnMarginClick(wxStyledTextEvent &event);
		void OnRunToHere(wxCommandEvent &event);
		void OnSelectAll(wxCommandEvent &event);
		void OnToggleBreakpoint(wxCommandEvent &event);
		void SetLexerOptions(int lexer);
		void SetStyleOptions(int lexer);
		void ToggleBreakpoint(int line);

		DECLARE_EVENT_TABLE()
};

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
