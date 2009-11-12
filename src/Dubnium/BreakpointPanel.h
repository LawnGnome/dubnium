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

#ifndef DUBNIUM_BREAKPOINTPANEL_H
#define DUBNIUM_BREAKPOINTPANEL_H

#include <wx/grid.h>

#include "DBGp/Connection.h"

#include "ID.h"
#include "ToolbarPanel.h"

class ConnectionPage;

class BreakpointPanel : public ToolbarPanel {
	public:
		BreakpointPanel(ConnectionPage *parent, wxWindowID id = wxID_ANY);

		DBGp::Breakpoint *GetFileBreakpoint(const wxString &file, int line);
		DBGp::Connection::BreakpointList GetFileBreakpoints(const wxString &file);
		void Update();

	protected:
		DBGp::Connection::BreakpointList breakpoints;
		wxGrid *grid;

		void OnAddCall(wxCommandEvent &event);
		void OnAddException(wxCommandEvent &event);
		void OnAddReturn(wxCommandEvent &event);
		void OnAddWatch(wxCommandEvent &event);
		void OnClickCell(wxGridEvent &event);
		void OnRemove(wxCommandEvent &event);
		void OnSelectCell(wxGridEvent &event);
		void ResetGrid();

		DECLARE_EVENT_TABLE()
};

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
