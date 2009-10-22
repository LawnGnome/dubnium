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

#ifndef DBGP_STACKLEVEL_H
#define DBGP_STACKLEVEL_H

#include <map>

#include <wx/string.h>

#include "DBGp/Context.h"
#include "DBGp/Error/Error.h"
#include "DBGp/Location.h"
#include "DBGp/Property.h"

namespace DBGp {
	class Connection;

	class StackLevel {
		public:
			typedef enum {
				FILE,
				EVAL
			} Type;

			typedef std::map<wxString, Context *> ContextMap;

			StackLevel(Connection *conn, unsigned int level) throw (EngineError, MalformedDocumentError, NotFoundError, SocketError);
			StackLevel(const StackLevel &level);
			virtual ~StackLevel();
			
			inline Location GetCmdBegin() const { return cmdBegin; }
			inline Location GetCmdEnd() const { return cmdEnd; }
			inline const ContextMap &GetContexts() const { return contexts; }
			inline wxString GetFileName() const { return fileName; }
			inline unsigned int GetLevel() const { return level; }
			inline unsigned int GetLineNo() const { return lineNo; }
			inline Type GetType() const { return type; }
			inline wxString GetWhere() const { return where; }

			static Type StringToType(const wxString &s);
			static wxString TypeToString(Type type);

		private:
			Location cmdBegin;
			Location cmdEnd;
			Connection *conn;
			ContextMap contexts;
			wxString fileName;
			unsigned int level;
			unsigned int lineNo;
			Type type;
			wxString where;

			void GetEngineContexts() throw (EngineError, SocketError);
			void GetStack() throw (EngineError, MalformedDocumentError, NotFoundError, SocketError);
	};
}

#endif

// vim:set fdm=marker ts=8 noet cin:
