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

#include "Dwell.h"

#include <wx/log.h>

// {{{ Dwell::Dwell()
Dwell::Dwell(DwellHandler *handler) : wxEvtHandler(), handler(handler), lastPosition(NULL), timer(NULL) {
	wxASSERT(handler != NULL);
	ConnectEvents();
}
// }}}
// {{{ Dwell::~Dwell()
Dwell::~Dwell() {
	if (lastPosition) {
		delete lastPosition;
	}
}
// }}}

// {{{ void Dwell::ConnectEvents()
void Dwell::ConnectEvents() {
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(Dwell::OnMotion));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(Dwell::OnLeave));
	Connect(wxEVT_MOTION, wxMouseEventHandler(Dwell::OnMotion));
	Connect(wxEVT_TIMER, wxTimerEventHandler(Dwell::OnTimer));

	timer = new wxTimer(this);
}
// }}}
// {{{ void Dwell::OnEnter(wxMouseEvent &event)
void Dwell::OnEnter(wxMouseEvent &event) {
	if (lastPosition) {
		delete lastPosition;
	}

	wxLogDebug(wxT("Dwell::OnEnter: %d,%d"), event.GetPosition().x, event.GetPosition().y);
	lastPosition = new wxPoint(event.GetPosition());
}
// }}}
// {{{ void Dwell::OnLeave(wxMouseEvent &event)
void Dwell::OnLeave(wxMouseEvent &event) {
	if (lastPosition) {
		delete lastPosition;
		lastPosition = NULL;
	}

	timer->Stop();
}
// }}}
// {{{ void Dwell::OnMotion(wxMouseEvent &event)
void Dwell::OnMotion(wxMouseEvent &event) {
	if (lastPosition) {
		delete lastPosition;
	}

	wxLogDebug(wxT("Dwell::OnMotion: %d,%d"), event.GetPosition().x, event.GetPosition().y);
	lastPosition = new wxPoint(event.GetPosition());
	timer->Start(500, true);
}
// }}}
// {{{ void Dwell::OnTimer(wxTimerEvent &event)
void Dwell::OnTimer(wxTimerEvent &event) {
	wxLogDebug(wxT("Dwell::OnTimerEvent: fired"));
	if (handler) {
		handler->OnDwell(*lastPosition);
	}
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
