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

#ifndef DBGP_EVENT_EVENT_H
#define DBGP_EVENT_EVENT_H

#include <wx/event.h>

#include "DBGp/Connection.h"

/** The event type constant for generic DBGp events. */
extern const wxEventType wxEVT_DBGP;

namespace DBGp {
	/**
	 * The base class for libDBGp events. In general, this isn't going to
	 * be used directly, but there's a full set of wxEvent macros and
	 * typedefs in case we want to use it.
	 */
	class Event : public wxEvent {
		public:
			/**
			 * Instantiates a new libDBGp event.
			 *
			 * @param[in] conn The connection back to the debugging
			 * engine.
			 * @param[in] type The event type.
			 */
			Event(Connection *conn, wxEventType type = wxEVT_DBGP);

			/**
			 * DBGp event copy constructor.
			 *
			 * @param[in] event The event to copy.
			 */
			Event(const Event &event);

			/**
			 * Returns the connection to the debugging engine.
			 *
			 * @return A pointer to the connection.
			 */
			inline Connection *GetConnection() const { return conn; }

		protected:
			/**
			 * The connection to use for communication with the
			 * debugging engine.
			 */
			Connection *conn;
	};
}

/** DBGp event handler typedef. */
typedef void (wxEvtHandler::*DBGpFunction)(DBGp::Event &);

/** The event table macro for this event. */
#define EVT_DBGP(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_DBGP, id, -1, \
		(wxObjectEventFunction) (wxEventFunction) \
		wxStaticCastEvent(DBGpFunction, &fn), (wxObject *) NULL), 

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
