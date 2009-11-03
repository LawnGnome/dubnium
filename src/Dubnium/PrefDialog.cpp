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

#include "PrefDialog.h"
#include "ID.h"
#include "SourceTextCtrl.h"

#include <wx/button.h>
#include <wx/fontdlg.h>
#include <wx/gbsizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// {{{ Event table
BEGIN_EVENT_TABLE(PrefDialog, wxDialog)
	EVT_BUTTON(wxID_CLOSE, PrefDialog::OnClose)
	EVT_BUTTON(ID_PREFDIALOG_FONT, PrefDialog::OnFont)
	EVT_SPINCTRL(ID_PREFDIALOG_PORT, PrefDialog::OnPort)
END_EVENT_TABLE()
// }}}

// {{{ PrefDialog::PrefDialog(MainFrame *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
PrefDialog::PrefDialog(MainFrame *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name) : wxDialog(dynamic_cast<wxWindow *>(parent), id, title, pos, size, style, name), config(wxConfigBase::Get()), parent(parent) {
	wxGridBagSizer *sizer = new wxGridBagSizer(3, 3);

	sizer->Add(new wxStaticText(this, -1, _("Font used for source code:")), wxGBPosition(0, 0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL | wxLEFT);
	sizer->Add(fontButton = new wxButton(this, ID_PREFDIALOG_FONT, _("Change Source Code Font")), wxGBPosition(0, 1), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL | wxEXPAND);
	UpdateFontButton();

	sizer->Add(new wxStaticText(this, -1, _("Port to listen on:")), wxGBPosition(1, 0), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL | wxLEFT);
	sizer->Add(new wxSpinCtrl(this, ID_PREFDIALOG_PORT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, config->Read(wxT("Network/Port"), 9000)), wxGBPosition(1, 1), wxDefaultSpan, wxALIGN_CENTRE_VERTICAL | wxEXPAND);

	sizer->Add(new wxStaticText(this, -1, _("Note: Port changes require a restart to take effect.")), wxGBPosition(2, 0), wxGBSpan(1, 2), wxLEFT);

	sizer->Add(new wxButton(this, wxID_CLOSE), wxGBPosition(3, 1), wxDefaultSpan, wxEXPAND | wxRIGHT);

	SetAutoLayout(true);
	SetSizer(sizer);
	sizer->SetSizeHints(this);
}
// }}}

// {{{ void PrefDialog::OnClose(wxCommandEvent &event)
void PrefDialog::OnClose(wxCommandEvent &event) {
	EndModal(0);
}
// }}}
// {{{ void PrefDialog::OnFont(wxCommandEvent &event)
void PrefDialog::OnFont(wxCommandEvent &event) {
	wxFont font(wxGetFontFromUser(this, SourceTextCtrl::DefaultFont(), _("Select Font")));
	if (font.IsOk()) {
		config->Write(wxT("SourcePanel/FontFace"), font.GetFaceName());
		config->Write(wxT("SourcePanel/FontSize"), font.GetPointSize());
		config->Write(wxT("SourcePanel/FontStyle"), font.GetStyle());
		config->Write(wxT("SourcePanel/FontWeight"), font.GetWeight());
		UpdateFontButton();
	}
}
// }}}
// {{{ void PrefDialog::OnPort(wxSpinEvent &event)
void PrefDialog::OnPort(wxSpinEvent &event) {
	config->Write(wxT("Network/Port"), event.GetPosition());
}
// }}}
// {{{ void PrefDialog::UpdateFontButton()
void PrefDialog::UpdateFontButton() {
	wxFont font(SourceTextCtrl::DefaultFont());
	wxString label(font.GetFaceName());

	label << wxT(' ') << font.GetPointSize();

	fontButton->SetFont(font);
	fontButton->SetLabel(label);
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
