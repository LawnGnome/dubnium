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

#ifndef DBGP_CONTEXT_H
#define DBGP_CONTEXT_H

#include <wx/string.h>

#include "DBGp/Property.h"

namespace DBGp {
	class Connection;
	class StackLevel;

	/** A class representing a context within a stack frame. */
	class Context {
		public:
			/**
			 * Constructs a new Context object.
			 *
			 * @param[in] conn The DBGp connection.
			 * @param[in] level The stack level the context exists
			 * on.
			 * @param[in] id The context ID.
			 * @param[in] name The context name.
			 */
			Context(Connection *conn, StackLevel *level, const wxString &id, const wxString &name);

			/**
			 * Context copy constructor.
			 *
			 * @param[in] context The context to copy.
			 */
			Context(const Context &context);

			/** Context destructor. */
			virtual ~Context();

			/**
			 * Returns the context ID.
			 *
			 * @return The context ID.
			 */
			inline wxString GetID() const { return id; }

			/**
			 * Returns the context name.
			 *
			 * @return The context name.
			 */
			inline wxString GetName() const { return name; }

			/**
			 * Returns the properties defined within the context.
			 *
			 * @return The defined properties.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			const Property::PropertyMap &GetProperties() const throw (EngineError, SocketError);

			/**
			 * Retrieves a specific property from the context.
			 *
			 * @param[in] name The property to look for.
			 * @return The property.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws NotFoundError Thrown if the property doesn't
			 * exist within the context.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			Property *GetProperty(const wxString &name) throw (EngineError, NotFoundError, SocketError);

			/**
			 * Returns the stack level the context belongs to.
			 *
			 * @return The stack level.
			 */
			inline StackLevel *GetStackLevel() const { return level; }

			/**
			 * Forces an update of the properties within the
			 * context.
			 *
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void UpdateProperties() throw (EngineError, SocketError);

		private:
			/** The DBGp connection. */
			Connection *conn;

			/** The context ID. */
			wxString id;

			/** The context's stack level. */
			StackLevel *level;

			/** The context name. */
			wxString name;

			/** The properties defined within the context. */
			Property::PropertyMap properties;

			/**
			 * Checks if we have the properties within the context
			 * already and, if not, retrieves them.
			 *
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 * @todo We need to be smarter about page handling,
			 * rather than just setting the features to high values
			 * in Connection.
			 */
			void RetrieveProperties() throw (EngineError, SocketError);
	};
}

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
