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

#include "StackPanel.h"
#include "ConnectionPage.h"
#include "StackLevelClientData.h"

// {{{ Event table
BEGIN_EVENT_TABLE(StackPanel, wxPanel)
	EVT_LISTBOX(ID_STACKPANEL_LIST, StackPanel::OnListBox)
END_EVENT_TABLE()
// }}}

// {{{ StackPanel::StackPanel(ConnectionPage *parent, wxWindowID id)
StackPanel::StackPanel(ConnectionPage *parent, wxWindowID id) : ToolbarPanel(parent, id), ToolTipProvider(), toolTip(this, this, ID_STACKPANEL_TOOLTIP) {
	list = new wxListBox(this, ID_STACKPANEL_LIST, wxDefaultPosition, wxDefaultSize, wxArrayString(), wxLB_SINGLE);
	sizer->Add(list, 1, wxEXPAND | wxALL);
}
// }}}

// {{{ wxString StackPanel::GetTipText(const wxPoint &mousePos)
wxString StackPanel::GetTipText(const wxPoint &mousePos) {
	return wxEmptyString;
}
// }}}
// {{{ void StackPanel::SetStack(DBGp::Stack *stack)
void StackPanel::SetStack(DBGp::Stack *stack) {
	list->Freeze();

	list->Clear();
	if (stack) {
		for (unsigned int i = 0; i < stack->GetDepth(); i++) {
			DBGp::StackLevel *level = stack->GetLevel(i);
			wxString doc;
			wxString file(level->GetFileName());
			wxString label(wxT("#"));
			wxString::size_type slashPos = file.rfind(wxT('/'));

			if (slashPos == wxString::npos) {
				doc = file;
			}
			else {
				doc = file.Mid(slashPos + 1);
			}

			label << i << wxT(": ") << doc << wxT(":") << level->GetLineNo() << wxT(" (") << file << wxT(")");
			list->Append(label, new StackLevelClientData(level));
		}
		list->SetSelection(0);
	}

	list->Thaw();
}
// }}}

// {{{ void StackPanel::OnListBox(wxCommandEvent &event)
void StackPanel::OnListBox(wxCommandEvent &event) {
	if (list->GetSelection() != wxNOT_FOUND) {
		wxClientData *data = list->GetClientObject(list->GetSelection());
		if (data) {
			StackLevelClientData *levelData = dynamic_cast<StackLevelClientData *>(data);
			if (levelData) {
				DBGp::StackLevel *level = levelData->GetStackLevel();
				parent->SetStackLevel(level);
			}
		}
	}
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
