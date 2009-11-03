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

#include "PropertiesPanel.h"

#include "PropertyDialog.h"
#include "PropertyTreeItem.h"

#include <wx/log.h>

// {{{ Event table
BEGIN_EVENT_TABLE(PropertiesPanel, wxPanel)
	EVT_TREE_ITEM_ACTIVATED(ID_PROPERTIESPANEL_TREE, PropertiesPanel::OnItemActivated)
END_EVENT_TABLE()
// }}}

// {{{ PropertiesPanel::PropertiesPanel(ConnectionPage *parent, wxWindowID id)
PropertiesPanel::PropertiesPanel(ConnectionPage *parent, wxWindowID id) : ToolbarPanel(parent, id), toolTip(this, this, wxID_ANY) {
	tree = new wxTreeCtrl(this, ID_PROPERTIESPANEL_TREE, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);
	sizer->Add(tree, 1, wxEXPAND | wxALL);
}
// }}}

// {{{ wxString PropertiesPanel::GetPropertyValue(const wxString &name) const
wxString PropertiesPanel::GetPropertyValue(const wxString &name) const {
	if (properties.count(name) > 0) {
		return properties.find(name)->second->GetData();
	}
	return wxEmptyString;
}
// }}}
// {{{ wxString PropertiesPanel::GetTipText(const wxPoint &mousePos)
wxString PropertiesPanel::GetTipText(const wxPoint &mousePos) {
	int flags = 0;
	wxPoint pos(mousePos - tree->GetPosition());
	wxTreeItemId id(tree->HitTest(pos, flags));
	wxString text;

	if (id && ~(flags & wxTREE_HITTEST_NOWHERE)) {
		PropertyTreeItem *item = dynamic_cast<PropertyTreeItem *>(tree->GetItemData(id));
		if (item) {
			DBGp::Property *prop = item->GetProperty();
			if (!prop->HasChildren()) {
				text << prop->GetName() << wxT(" : ") << prop->GetData();
			}
		}
	}

	return text;
}
// }}}
// {{{ void PropertiesPanel::SetStackLevel(DBGp::StackLevel *level)
void PropertiesPanel::SetStackLevel(const DBGp::StackLevel *level) {
	tree->Freeze();

	properties.clear();
	tree->DeleteAllItems();
	wxTreeItemId root(tree->AddRoot(_("Root")));

	if (level) {
		const DBGp::StackLevel::ContextMap &contexts = level->GetContexts();

		for (DBGp::StackLevel::ContextMap::const_iterator i = contexts.begin(); i != contexts.end(); i++) {
			const DBGp::Property::PropertyMap &properties = i->second->GetProperties();

			// TODO: I18n.
			wxTreeItemId context(tree->AppendItem(root, i->second->GetName()));

			for (DBGp::Property::PropertyMap::const_iterator i = properties.begin(); i != properties.end(); i++) {
				AddProperty(context, i->second);
			}
		}
	}

	tree->Thaw();
}
// }}}

// {{{ void PropertiesPanel::AddProperty(const wxTreeItemId &parent, const DBGp::Property *prop)
void PropertiesPanel::AddProperty(const wxTreeItemId &parent, const DBGp::Property *prop) {
	if (prop->HasChildren()) {
		wxTreeItemId id(tree->AppendItem(parent, prop->GetName(), -1, -1, new PropertyTreeItem(prop)));
		const DBGp::Property::PropertyMap &children(prop->GetChildren());
		for (DBGp::Property::PropertyMap::const_iterator i = children.begin(); i != children.end(); i++) {
			AddProperty(id, i->second);
		}
	}
	else {
		wxString label(prop->GetName());

		label << wxT(" : ") << prop->GetData();
		tree->AppendItem(parent, label, -1, -1, new PropertyTreeItem(prop));
		wxLogDebug(wxT("Adding property: %s = %s"), prop->GetFullName().c_str(), prop->GetData().c_str());
		properties[prop->GetFullName()] = new DBGp::Property(*prop);
	}
}
// }}}
// {{{ void PropertiesPanel::OnItemActivated(wxTreeEvent &event)
void PropertiesPanel::OnItemActivated(wxTreeEvent &event) {
	PropertyTreeItem *item = dynamic_cast<PropertyTreeItem *>(tree->GetItemData(event.GetItem()));

	/* The cast can fail, as the root element and context elements don't
	 * have PropertyTreeItem instances associated with them. That's not an
	 * error; we'll just return without doing anything. */
	if (item) {
		PropertyDialog pd(this, wxID_ANY, item->GetProperty());
		pd.ShowModal();
	}
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
