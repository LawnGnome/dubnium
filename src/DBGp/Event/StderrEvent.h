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

#ifndef DBGP_EVENT_STDERREVENT_H
#define DBGP_EVENT_STDERREVENT_H

#include <wx/event.h>
#include <wx/string.h>

#include "DBGp/Event/StreamEvent.h"

/** The event type for stderr events. */
extern const wxEventType wxEVT_DBGP_STDERR;

namespace DBGp {
	/**
	 * A class representing data being sent to stderr by the program being
	 * debugged.
	 */
	class StderrEvent : public StreamEvent {
		public:
			/**
			 * Constructs a new stderr event.
			 *
			 * @param[in] conn The DBGp connection.
			 * @param[in] data The data sent to stderr.
			 */
			StderrEvent(Connection *conn, const wxString &data);

			/**
			 * Copy constructor for stderr events.
			 *
			 * @param[in] event The event to copy.
			 */
			StderrEvent(const StderrEvent &event);

			/**
			 * Creates a new copy of the event.
			 *
			 * @return A new copy of the event.
			 */
			inline wxEvent *Clone() const { return new StderrEvent(*this); }
	};
}

/** Typedef for stderr event handlers. */
typedef void (wxEvtHandler::*DBGpStderrFunction)(DBGp::StderrEvent &);

/** Event table macro. */
#define EVT_DBGP_STDERR(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_DBGP_STDERR, id, -1, \
		(wxObjectEventFunction) (wxEventFunction) (DBGpFunction) (DBGpStreamFunction) \
		wxStaticCastEvent(DBGpStderrFunction, &fn), (wxObject *) NULL),

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
