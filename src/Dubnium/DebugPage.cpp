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

#include "DebugPage.h"
#include "SourceTextCtrl.h"

#include <wx/log.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>

// {{{ DebugPage::DebugPage(wxWindow *parent)
DebugPage::DebugPage(wxWindow *parent) : wxPanel(parent, ID_DEBUGPAGE) {
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	wxTextCtrl *text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxTE_DONTWRAP | wxHSCROLL);
	
	text->SetFont(SourceTextCtrl::DefaultFont());
	sizer->Add(text, 1, wxEXPAND | wxALL);

	wxLog::SetActiveTarget(new wxLogTextCtrl(text));
	new wxLogChain(new wxLogStderr);

	SetAutoLayout(true);
	SetSizer(sizer);
	sizer->SetSizeHints(this);
}
// }}}
// {{{ DebugPage::~DebugPage()
DebugPage::~DebugPage() {
	wxLog::SetActiveTarget(NULL);
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
