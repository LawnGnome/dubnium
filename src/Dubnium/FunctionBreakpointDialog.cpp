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

#include "FunctionBreakpointDialog.h"
#include "ID.h"

#include <wx/button.h>
#include <wx/gbsizer.h>

// {{{ Event table
BEGIN_EVENT_TABLE(FunctionBreakpointDialog, wxDialog)
	EVT_BUTTON(wxID_CANCEL, FunctionBreakpointDialog::OnCancel)
	EVT_BUTTON(wxID_OK, FunctionBreakpointDialog::OnOK)
	EVT_RADIOBOX(ID_FUNCTIONBREAKPOINTDIALOG_TYPE, FunctionBreakpointDialog::OnType)
END_EVENT_TABLE()
// }}}

// {{{ FunctionBreakpointDialog::FunctionBreakpointDialog(wxWindow *parent, const wxString &title)
FunctionBreakpointDialog::FunctionBreakpointDialog(wxWindow *parent, const wxString &title) : wxDialog(parent, -1, title) {
	wxGridBagSizer *sizer = new wxGridBagSizer(3, 3);

	wxArrayString choices;
	choices.Add(_("Class/object method"));
	choices.Add(_("Function"));
	
	type = new wxRadioBox(this, ID_FUNCTIONBREAKPOINTDIALOG_TYPE, _("Breakpoint Type:"), wxDefaultPosition, wxDefaultSize, choices, 2, wxRA_SPECIFY_COLS);
	type->SetSelection(1);
	sizer->Add(type, wxGBPosition(0, 0), wxGBSpan(1, 2), wxLEFT);

	clsLabel = new wxStaticText(this, -1, _("Class:"));
	clsLabel->Disable();
	cls = new wxTextCtrl(this, ID_FUNCTIONBREAKPOINTDIALOG_CLASS);
	cls->Disable();
	sizer->Add(clsLabel, wxGBPosition(1, 0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL | wxLEFT);
	sizer->Add(cls, wxGBPosition(1, 1), wxDefaultSpan, wxEXPAND);

	functionLabel = new wxStaticText(this, -1, _("Function:"));
	sizer->Add(functionLabel, wxGBPosition(2, 0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL | wxLEFT);
	function = new wxTextCtrl(this, ID_FUNCTIONBREAKPOINTDIALOG_FUNCTION);
	sizer->Add(function, wxGBPosition(2, 1), wxDefaultSpan, wxEXPAND);

	sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), wxGBPosition(3, 0), wxGBSpan(1, 2), wxRIGHT | wxEXPAND);

	SetAutoLayout(true);
	SetSizer(sizer);
	sizer->SetSizeHints(this);
}
// }}}

// {{{ wxString FunctionBreakpointDialog::GetClass() const
wxString FunctionBreakpointDialog::GetClass() const {
	return cls->GetValue();
}
// }}}
// {{{ wxString FunctionBreakpointDialog::GetFunction() const
wxString FunctionBreakpointDialog::GetFunction() const {
	return function->GetValue();
}
// }}}
// {{{ bool FunctionBreakpointDialog::IsMethod() const
bool FunctionBreakpointDialog::IsMethod() const {
	return (type->GetSelection() == 0);
}
// }}}

// {{{ void FunctionBreakpointDialog::OnCancel(wxCommandEvent &event)
void FunctionBreakpointDialog::OnCancel(wxCommandEvent &event) {
	EndModal(0);
}
// }}}
// {{{ void FunctionBreakpointDialog::OnOK(wxCommandEvent &event)
void FunctionBreakpointDialog::OnOK(wxCommandEvent &event) {
	EndModal(1);
}
// }}}
// {{{ void FunctionBreakpointDialog::OnType(wxCommandEvent &event)
void FunctionBreakpointDialog::OnType(wxCommandEvent &event) {
	if (type->GetSelection() == 0) {
		cls->Enable();
		clsLabel->Enable();

		functionLabel->SetLabel(_("Method:"));
	}
	else {
		cls->Disable();
		clsLabel->Disable();

		functionLabel->SetLabel(_("Function:"));
	}
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
