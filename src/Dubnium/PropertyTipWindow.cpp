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

#include "PropertyTipWindow.h"

#include <wx/intl.h>

static const int MAXIMUM_CHILD_ELEMENTS = 20;

// {{{ wxString TruncateValue(const wxString &value, size_t maxLength = 60)
wxString TruncateValue(const wxString &value, size_t maxLength = 60) {
	if (value.Len() > maxLength) {
		wxString truncatedValue(value.Left(maxLength));
		wxString remaining;

		remaining.Printf(_("<remaining %u character(s) omitted>"), value.Len() - maxLength);
		truncatedValue << wxT("... ") << remaining;

		return truncatedValue;
	}

	return value;
}
// }}}

#if wxUSE_POPUPWIN

#include <wx/gbsizer.h>
#include <wx/settings.h>
#include <wx/stattext.h>

// {{{ Event table
BEGIN_EVENT_TABLE(PropertyTipWindow, wxPopupTransientWindow)
	EVT_MOTION(PropertyTipWindow::OnMouseMove)
END_EVENT_TABLE()
// }}}

// {{{ PropertyTipWindow::PropertyTipWindow(wxWindow *parent, const DBGp::Property *prop)
PropertyTipWindow::PropertyTipWindow(wxWindow *parent, const DBGp::Property *prop) : wxPopupTransientWindow(parent) {
	wxGridBagSizer *sizer = new wxGridBagSizer(0, 3);

	SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOTEXT));
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK));

	wxStaticText *name = new wxStaticText(this, -1, prop->GetFullName() + wxT(" (") + prop->GetType().GetName() + wxT(") :"));
	wxFont font(name->GetFont());
	font.SetWeight(wxFONTWEIGHT_BOLD);
	name->SetFont(font);

	sizer->Add(name, wxGBPosition(0, 0), wxGBSpan(1, 3));

	if (prop->HasChildren()) {
		/* We'll do a shallow dump, since detailed information is
		 * available through the properties panel and context menu. */
		const DBGp::Property::PropertyMap children = prop->GetChildren();
		int numShown = 0;

		sizer->Add(20, 10, wxGBPosition(1, 0));
		
		for (DBGp::Property::PropertyMap::const_iterator i = children.begin(); i != children.end(); i++) {
			DBGp::Property *child = i->second;

			sizer->Add(new wxStaticText(this, -1, child->GetName() + wxT(" (") + child->GetType().GetName() << wxT(")")), wxGBPosition(++numShown, 1));

			if (child->HasChildren()) {
				sizer->Add(new wxStaticText(this, -1, wxString(wxT(": ")) + _("<complex data structure>")), wxGBPosition(numShown, 2));
			}
			else {
				sizer->Add(new wxStaticText(this, -1, wxString(wxT(": ")) + TruncateValue(child->GetData())), wxGBPosition(numShown, 2));
			}

			// We'll truncate at an arbitrarily chosen 20 elements.
			if (numShown >= MAXIMUM_CHILD_ELEMENTS) {
				wxString rem;

				rem.Printf(_("<remaining %d element(s) omitted>"), children.size() - MAXIMUM_CHILD_ELEMENTS);
				sizer->Add(new wxStaticText(this, -1, rem), wxGBPosition(++numShown, 0), wxGBSpan(1, 3));

				break;
			}
		}
	}
	else {
		sizer->Add(new wxStaticText(this, -1, TruncateValue(prop->GetData())), wxGBPosition(0, 3));
	}

	SetAutoLayout(true);
	SetSizer(sizer);
	sizer->SetSizeHints(this);
}
// }}}

// {{{ void PropertyTipWindow::Popup(wxWindow *focus)
void PropertyTipWindow::Popup(wxWindow *focus) {
	wxPopupTransientWindow::Popup(focus);

	CaptureMouse();
	Position(CalculatePosition(), wxSize(0, 0));

	/* We have to call RecalcSizes() explicitly here to avoid wxGTK
	 * breakage. */
	GetSizer()->RecalcSizes();
}
// }}}

// {{{ void PropertyTipWindow::OnMouseMove(wxMouseEvent &event)
void PropertyTipWindow::OnMouseMove(wxMouseEvent &event) {
	ReleaseMouse();
	Dismiss();
}
// }}}

// {{{ wxPoint PropertyTipWindow::CalculatePosition()
wxPoint PropertyTipWindow::CalculatePosition() {
	int x, y;

	// Code nicked from wxTipWindow.
	wxGetMousePosition(&x, &y);
	y += wxSystemSettings::GetMetric(wxSYS_CURSOR_Y);

	return wxPoint(x, y);
}
// }}}

#else

// {{{ wxString PropertyAsString(const DBGp::Property *prop)
wxString PropertyAsString(const DBGp::Property *prop) {
	/* It might be nice to unify this with the codepath in the
	 * PropertyTipWindow constructor at some point, since they're doing
	 * very similar things. */
	wxString text;

	wxASSERT(prop != NULL);
	text << prop->GetFullName() << wxT(" (") << prop->GetType().GetName() << wxT(") : ");

	if (prop->HasChildren()) {
		const DBGp::Property::PropertyMap children = prop->GetChildren();
		int numShown = 0;

		for (DBGp::Property::PropertyMap::const_iterator i = children.begin(); i != children.end(); i++) {
			const DBGp::Property *child = i->second;

			text << wxT("\n\t") << child->GetName() << wxT(" (") << child->GetType().GetName() << wxT(") : ");
			if (child->HasChildren()) {
				text << _("<complex data structure>");
			}
			else {
				text << TruncateValue(child->GetData());
			}

			if (++numShown >= MAXIMUM_CHILD_ELEMENTS) {
				wxString rem;

				rem.Printf(_("<remaining %d element(s) omitted>"), children.size() - MAXIMUM_CHILD_ELEMENTS);
				text << wxT("\n") << rem;

				break;
			}
		}
	}
	else {
		text << TruncateValue(prop->GetData());
	}

	return text;
}
// }}}

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
