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

#include "ToolbarPanel.h"
#include "ArtProvider.h"
#include "ConnectionPage.h"
#include "Dubnium.h"

// {{{ ToolbarPanel::ToolbarPanel(ConnectionPage *parent, wxWindowID id)
ToolbarPanel::ToolbarPanel(ConnectionPage *parent, wxWindowID id) : wxPanel(dynamic_cast<wxWindow *>(parent), id), parent(parent), size(ArtProvider::toolbarSize, ArtProvider::toolbarSize) {
	config = wxConfigBase::Get();
	sizer = new wxBoxSizer(wxVERTICAL);

	toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxNO_BORDER | wxTB_FLAT | wxTB_NODIVIDER);
	toolbar->SetToolBitmapSize(size);
	sizer->Add(toolbar, 0, wxEXPAND | wxALL);

	SetSizer(sizer);
	sizer->SetSizeHints(this);
}
// }}}

// {{{ void ToolbarPanel::AddRadioTool(wxWindowID id, const wxString &label, const wxArtID &artID, const wxString &tooltip)
void ToolbarPanel::AddRadioTool(wxWindowID id, const wxString &label, const wxArtID &artID, const wxString &tooltip) {
	wxBitmap bitmap(wxArtProvider::GetBitmap(artID, wxART_TOOLBAR, size));

	toolbar->AddRadioTool(id, label, bitmap, bitmap, tooltip);
}
// }}}
// {{{ void ToolbarPanel::AddTool(wxWindowID id, const wxString &label, const wxArtID &artID, const wxString &tooltip)
void ToolbarPanel::AddTool(wxWindowID id, const wxString &label, const wxArtID &artID, const wxString &tooltip) {
	toolbar->AddTool(id, label, wxArtProvider::GetBitmap(artID, wxART_TOOLBAR, size), tooltip);
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
