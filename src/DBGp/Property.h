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

#ifndef DBGP_PROPERTY_H
#define DBGP_PROPERTY_H

#include <wx/string.h>
#include <wx/xml/xml.h>

#include "DBGp/Error/Error.h"
#include "DBGp/MessageArguments.h"
#include "DBGp/Type.h"

namespace DBGp {
	class Connection;
	class Context;

	class Property {
		friend class Connection;
		friend class Context;

		public:
			typedef std::map<wxString, Property *> PropertyMap;

			Property(Connection *conn, Context *context, unsigned int depth, Property *parent = NULL);
			Property(const Property &p);
			virtual ~Property();

			inline wxString GetAddress() const { return address; }
			inline const PropertyMap &GetChildren() const { return children; }
			inline wxString GetClassName() const { return className; }
			inline Context *GetContext() const { return context; }
			inline wxString GetData() const { return data; }
			inline unsigned int GetDepth() const { return depth; }
			inline wxString GetFullName() const { return fullName; }
			inline wxString GetKey() const { return key; }
			inline wxString GetName() const { return name; }
			inline Property *GetParent() { return parent; }
			inline unsigned long GetSize() const { return size; }
			inline Type GetType() const { return type; }

			inline bool HasChildren() const { return hasChildren; }
			inline bool HasParent() const { return (parent != NULL); }
			inline bool IsConstant() const { return constant; }

			Property *GetChild(const wxString &name) throw (NotFoundError);

			void Update() throw (EngineError, SocketError);

		private:
			wxString address;
			PropertyMap children;
			wxString className;
			Connection *conn;
			bool constant;
			Context *context;
			wxString data;
			unsigned int depth;
			wxString fullName;
			bool hasChildren;
			wxString key;
			wxString name;
			Property *parent;
			unsigned long size;
			Type type;

			MessageArguments GetPropertyArguments() const;
			void ParsePropertyElement(wxXmlNode *node);
	};
}

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
