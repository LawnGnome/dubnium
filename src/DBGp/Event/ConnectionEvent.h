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

#ifndef DBGP_EVENT_CONNECTIONEVENT_H
#define DBGP_EVENT_CONNECTIONEVENT_H

#include <wx/event.h>
#include <wx/string.h>

#include "DBGp/Event/Event.h"

/** The event type for connection events. */
extern const wxEventType wxEVT_DBGP_CONNECTION;

namespace DBGp {
	/** The class representing connection events. */
	class ConnectionEvent : public Event {
		public:
			/** 
			 * Constructs a new connection event with the
			 * properties from the init packet. Properties that
			 * weren't in the init packet will be empty strings.
			 *
			 * @param[in] conn The connection back to the debugging
			 * engine.
			 * @param[in] appID The application ID.
			 * @param[in] ideKey The IDE key.
			 * @param[in] session The reported session.
			 * @param[in] thread The thread ID.
			 * @param[in] parent The parent ID.
			 * @param[in] language The programming language in use.
			 * @param[in] protocolVersion The DBGp protocol version
			 * in use.
			 * @param[in] fileURI The initial source file URI.
			 */
			ConnectionEvent(Connection *conn, const wxString &appID, const wxString &ideKey, const wxString &session, const wxString &thread, const wxString &parent, const wxString &language, const wxString &protocolVersion, const wxString &fileURI);

			/** 
			 * Copy constructor for ConnectionEvent.
			 *
			 * @param[in] event The event to copy from.
			 */
			ConnectionEvent(const ConnectionEvent &event);

			/**
			 * Returns the application ID.
			 *
			 * @return The application ID.
			 */
			inline wxString GetAppID() const { return appID; }

			/**
			 * Returns the connection back to the debugging engine.
			 *
			 * @return A pointer to the connection.
			 */
			inline Connection *GetConnection() const { return conn; }

			/**
			 * Returns the file URI for the initial source file.
			 *
			 * @return The file URI.
			 * @todo For consistency, this should probably be
			 * GetFileUri.
			 */
			inline wxString GetFileURI() const { return fileURI; }

			/**
			 * Returns the IDE key.
			 *
			 * @return The IDE key.
			 */
			inline wxString GetIDEKey() const { return ideKey; }

			/**
			 * Returns the programming language.
			 *
			 * @return The programming language.
			 */
			inline wxString GetLanguage() const { return language; }

			/**
			 * Returns the parent. If these docblocks seem a little
			 * repetitive, just imagine writing them.
			 * 
			 * @return The parent.
			 */
			inline wxString GetParent() const { return parent; }

			/**
			 * Returns the DBGp protocol version. Generally 1.0.
			 *
			 * @return The protocol version.
			 */
			inline wxString GetProtocolVersion() const { return protocolVersion; }

			/**
			 * Returns the session information.
			 *
			 * @return The session information.
			 */
			inline wxString GetSession() const { return session; }

			/**
			 * Returns the thread ID.
			 *
			 * @return The thread ID.
			 */
			inline wxString GetThread() const { return thread; }

			/**
			 * Clones the event.
			 *
			 * @return A new copy of the event, instantiated with
			 * new.
			 */
			inline wxEvent *Clone() const { return new ConnectionEvent(*this); }

		protected:
			/** The application ID. */
			wxString appID;

			/** The file URI. */
			wxString fileURI;

			/** The IDE key. */
			wxString ideKey;

			/** The programming language in use. */
			wxString language;

			/** The parent. */
			wxString parent;

			/** The DBGp protocol version. */
			wxString protocolVersion;

			/** The session. */
			wxString session;

			/** The thread ID. */
			wxString thread;
	};
}

/** The typedef for connection event handlers. */
typedef void (wxEvtHandler::*DBGpConnectionFunction)(DBGp::ConnectionEvent &);

/** The event table macro for this event. */
#define EVT_DBGP_CONNECTION(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_DBGP_CONNECTION, id, -1, \
		(wxObjectEventFunction) (wxEventFunction) (DBGpFunction) \
		wxStaticCastEvent(DBGpConnectionFunction, &fn), (wxObject *) NULL),

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
