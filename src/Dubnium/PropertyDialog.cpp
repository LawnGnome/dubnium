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

#include <wx/grid.h>
#include <wx/sizer.h>

#include "PropertyDialog.h"

// {{{ Event table
BEGIN_EVENT_TABLE(PropertyDialog, wxDialog)
	EVT_BUTTON(wxID_OK, PropertyDialog::OnOK)
END_EVENT_TABLE()
// }}}

// {{{ PropertyDialog::PropertyDialog(wxWindow *parent, wxWindowID id, const DBGp::Property *prop, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
PropertyDialog::PropertyDialog(wxWindow *parent, wxWindowID id, const DBGp::Property *prop, const wxPoint &pos, const wxSize &size, long style, const wxString &name) : wxDialog(parent, id, prop->GetFullName(), pos, size, style | wxMAXIMIZE_BOX | wxRESIZE_BORDER, name) {
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	grid = new wxGrid(this, wxID_ANY);
	grid->Freeze();

	grid->EnableEditing(false);
	grid->CreateGrid(0, 3);
	grid->SetColLabelValue(0, _("Name"));
	grid->SetColLabelValue(1, _("Type"));
	grid->SetColLabelValue(2, _("Value"));
	grid->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
	grid->SetRowLabelSize(0);

	AddProperty(prop, 0);

	grid->AutoSizeColumns();
	grid->Thaw();

	SetAutoLayout(true);
	SetSizer(sizer);
	sizer->SetSizeHints(this);

	sizer->Add(grid, 1, wxEXPAND | wxALL);
	sizer->Add(CreateSeparatedButtonSizer(wxID_CLOSE), 0, wxEXPAND | wxALL);
}
// }}}

// {{{ void PropertyDialog::AddProperty(const DBGp::Property *prop, unsigned long indent)
void PropertyDialog::AddProperty(const DBGp::Property *prop, unsigned long indent) {
	static int row = -1;
	wxString prefix;

	if (indent == 0) {
		row = -1;
	}
	else {
		for (unsigned long i = 0; i < indent; i++) {
			prefix += wxT('>');
		}
		prefix += wxT(' ');
	}

	grid->AppendRows(1);
	grid->SetCellValue(++row, 0, prefix + prop->GetName());
	grid->SetCellValue(row, 1, prop->GetType().GetName());
	grid->SetCellValue(row, 2, prop->GetData());
	
	for (DBGp::Property::PropertyMap::const_iterator i = prop->GetChildren().begin(); i != prop->GetChildren().end(); i++) {
		AddProperty(i->second, indent + 1);
	}
}
// }}}
// {{{ void PropertyDialog::OnOK(wxCommandEvent &event)
void PropertyDialog::OnOK(wxCommandEvent &event) {
	EndModal(0);
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
