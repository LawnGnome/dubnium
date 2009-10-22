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

#include "SourcePanel.h"
#include "ConnectionPage.h"

#include <wx/log.h>

// {{{ SourcePanel::SourcePanel(ConnectionPage *parent, wxWindowID id)
SourcePanel::SourcePanel(ConnectionPage *parent, wxWindowID id) : ToolbarPanel(parent, id) {
	tc = new SourceTextCtrl(this, ID_SOURCEPANEL_RTC, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_DONTWRAP | wxTE_MULTILINE | wxTE_RICH);
	sizer->Add(tc, 1, wxEXPAND);
}
// }}}

// {{{ void SourcePanel::BreakpointAdd(int line, bool temporary)
void SourcePanel::BreakpointAdd(int line, bool temporary) {
	parent->BreakpointAdd(line, temporary);
}
// }}}
// {{{ void SourcePanel::BreakpointRemove(int line)
void SourcePanel::BreakpointRemove(int line) {
	parent->BreakpointRemove(line);
}
// }}}
// {{{ wxString SourcePanel::GetPropertyValue(const wxString &name) const
wxString SourcePanel::GetPropertyValue(const wxString &name) const {
	return parent->GetPropertyValue(name);
}
// }}}
// {{{ void SourcePanel::Unavailable(const wxString &message)
void SourcePanel::Unavailable(const wxString &message) {
	tc->SetSource(message);
	tc->SetLexer(wxSTC_LEX_NULL);
}
// }}}

// vim:set fdm=marker ts=8 noet cin:
