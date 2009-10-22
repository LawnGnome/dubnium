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

#include "DBGp/Context.h"
#include "DBGp/Connection.h"
#include "DBGp/StackLevel.h"
#include "DBGp/Utility.h"

#include <wx/xml/xml.h>

using namespace DBGp;

// {{{ Context::Context(Connection *conn, StackLevel *level, const wxString &id, const wxString &name)
Context::Context(Connection *conn, StackLevel *level, const wxString &id, const wxString &name) : conn(conn), id(id), level(level), name(name) {
	RetrieveProperties();
}
// }}}
// {{{ Context::Context(const Context &context)
Context::Context(const Context &context) : conn(context.conn), id(context.id), level(context.level), name(context.name) {
	for (Property::PropertyMap::const_iterator i = context.properties.begin(); i != context.properties.end(); i++) {
		Property *prop = new Property(*(i->second));
		prop->context = this;
		properties[i->first] = prop;
	}
}
// }}}
// {{{ Context::~Context()
Context::~Context() {
	for (Property::PropertyMap::iterator i = properties.begin(); i != properties.end(); i++) {
		delete i->second;
	}
}
// }}}

// {{{ const Property::PropertyMap &Context::GetProperties() const throw (EngineError, SocketError)
const Property::PropertyMap &Context::GetProperties() const throw (EngineError, SocketError) {
	return properties;
}
// }}}
// {{{ Property *Context::GetProperty(const wxString &name) throw (EngineError, NotFoundError, SocketError)
Property *Context::GetProperty(const wxString &name) throw (EngineError, NotFoundError, SocketError) {
	RetrieveProperties();
	Property::PropertyMap::iterator i = properties.find(name);
	if (i == properties.end()) {
		throw NotFoundError(wxT("Property '") + name + wxT("' not found."));
	}
	return i->second;
}
// }}}

// {{{ void Context::UpdateProperties() throw (EngineError, SocketError)
void Context::UpdateProperties() throw (EngineError, SocketError) {
	for (Property::PropertyMap::iterator i = properties.begin(); i != properties.end(); i++) {
		delete i->second;
	}
	RetrieveProperties();
}
// }}}

// {{{ void Context::RetrieveProperties() throw (EngineError, SocketError)
void Context::RetrieveProperties() throw (EngineError, SocketError) {
	if (properties.size() > 0) {
		return;
	}

	MessageArguments args(2, wxT("-d"), IntToString(level->GetLevel()).c_str(), wxT("-c"), id.c_str());
	wxXmlDocument doc(conn->SendCommandWait(wxT("context_get"), args));

	for (wxXmlNode *node = doc.GetRoot()->GetChildren(); node != NULL; node = node->GetNext()) {
		if (node->GetType() == wxXML_ELEMENT_NODE && node->GetName() == wxT("property")) {
			Property *prop = new Property(conn, this, level->GetLevel());
			prop->ParsePropertyElement(node);
			properties[prop->GetName()] = prop;
		}
	}
}
// }}}

// vim:set fdm=marker ts=8 noet cin:
