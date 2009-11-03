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

#ifndef DUBNIUM_PROPERTIESPANEL_H
#define DUBNIUM_PROPERTIESPANEL_H

#include <wx/treectrl.h>

#include "DBGp/Property.h"
#include "DBGp/StackLevel.h"

#include "ID.h"
#include "ToolbarPanel.h"
#include "ToolTipWindow.h"

class PropertiesPanel : public ToolbarPanel, public ToolTipProvider {
	public:
		PropertiesPanel(ConnectionPage *parent, wxWindowID id = wxID_ANY);

		DBGp::Property *GetProperty(const wxString &name);
		wxString GetPropertyValue(const wxString &name) const;
		virtual wxString GetTipText(const wxPoint &mousePos);
		void SetStackLevel(const DBGp::StackLevel *level);

	protected:
		DBGp::Property::PropertyMap properties;
		wxTreeCtrl *tree;
		ToolTipWindow toolTip;

		void AddProperty(const wxTreeItemId &parent, const DBGp::Property *prop);
		void OnItemActivated(wxTreeEvent &event);

		DECLARE_EVENT_TABLE()
};

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
