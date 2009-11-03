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

#include "ToolTipWindow.h"

#include <wx/log.h>

// {{{ ToolTipWindow::ToolTipWindow(ToolTipProvider *provider, wxWindowID id)
ToolTipWindow::ToolTipWindow(ToolTipProvider *provider, wxWindow *window, wxWindowID id) : wxEvtHandler(), id(id), mouseIn(false), provider(provider), timer(this, id), tipWindow(NULL), window(window) {
	wxASSERT(provider != NULL);
	wxASSERT(window != NULL);
	timer.Start(1000, wxTIMER_CONTINUOUS);
	BindEvents();
}
// }}}
// {{{ ToolTipWindow::~ToolTipWindow()
ToolTipWindow::~ToolTipWindow() {
	if (tipWindow) {
		tipWindow->Destroy();
	}
}
// }}}

// {{{ void ToolTipWindow::BindEvents()
void ToolTipWindow::BindEvents() {
	Connect(id, wxEVT_TIMER, wxTimerEventHandler(ToolTipWindow::OnTimer), NULL, this);
	window->Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(ToolTipWindow::OnMouseMove), NULL, this);
	window->Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(ToolTipWindow::OnMouseLeave), NULL, this);
	window->Connect(wxEVT_MOTION, wxMouseEventHandler(ToolTipWindow::OnMouseMove), NULL, this);
}
// }}}
// {{{ void ToolTipWindow::OnMouseLeave(wxMouseEvent &event)
void ToolTipWindow::OnMouseLeave(wxMouseEvent &event) {
	mouseIn = false;
	mousePos = wxDefaultPosition;
	event.Skip();
}
// }}}
// {{{ void ToolTipWindow::OnMouseMove(wxMouseEvent &event)
void ToolTipWindow::OnMouseMove(wxMouseEvent &event) {
	mouseIn = true;
	mousePos = event.GetPosition();
	event.Skip();
}
// }}}
// {{{ void ToolTipWindow::OnTimer(wxTimerEvent &event)
void ToolTipWindow::OnTimer(wxTimerEvent &event) {
	if (mouseIn && !tipWindow) {
		/* Only display the tooltip if the mouse has been stationary
		 * for the last second or more. */
		if (mouseLastPos == mousePos) {
			wxString tip(provider->GetTipText(mousePos));

			if (tip != wxEmptyString) {
				tipWindow = new wxTipWindow(window, tip, 640, &tipWindow);
				/* By setting the bounding rectangle to be
				 * (hopefully) off-screen, the tool tip will
				 * disappear as soon as the mouse is moved. */
				tipWindow->SetBoundingRect(wxRect(-1, -1, 1, 1));
			}
		}
		else {
			mouseLastPos = mousePos;
		}
	}
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
