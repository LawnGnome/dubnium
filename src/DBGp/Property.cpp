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

#include "DBGp/Property.h"
#include "DBGp/Connection.h"
#include "DBGp/Context.h"
#include "DBGp/Utility.h"

using namespace DBGp;

// {{{ Property::Property(Connection *conn, Context *context, unsigned int depth, Property *parent)
Property::Property(Connection *conn, Context *context, unsigned int depth, Property *parent) : conn(conn), context(context), depth(depth), parent(parent) {
	wxASSERT(conn != NULL);
}
// }}}
// {{{ Property::Property(const Property &p)
Property::Property(const Property &p) :
	address(p.address),
	className(p.className),
	conn(p.conn),
	constant(p.constant),
	context(p.context),
	data(p.data),
	depth(p.depth),
	fullName(p.fullName),
	hasChildren(p.hasChildren),
	key(p.key),
	name(p.name),
	parent(p.parent),
	size(p.size),
	type(p.type) {
	for (PropertyMap::const_iterator i = p.children.begin(); i != p.children.end(); i++) {
		Property *prop = new Property(*(i->second));
		prop->parent = this;
		children[i->first] = prop;
	}
}
// }}}
// {{{ Property::~Property()
Property::~Property() {
	for (PropertyMap::iterator i = children.begin(); i != children.end(); i++) {
		delete i->second;
	}
}
// }}}

// {{{ Property *Property::GetChild(const wxString &name) throw (NotFoundError)
Property *Property::GetChild(const wxString &name) throw (NotFoundError) {
	for (PropertyMap::iterator i = children.begin(); i != children.end(); i++) {
		if (i->second->GetName() == name) {
			return i->second;
		}
	}
	throw NotFoundError(wxT("Requested child property '") + name + wxT("' not found."));
}
// }}}

// {{{ void Property::Update() throw (EngineError, SocketError)
void Property::Update() throw (EngineError, SocketError) {
	wxXmlDocument doc(conn->SendCommandWait(wxT("property_get"), GetPropertyArguments()));

	for (wxXmlNode *node = doc.GetRoot()->GetChildren(); node != NULL; node = node->GetNext()) {
		if (node->GetType() == wxXML_ELEMENT_NODE && node->GetName() == wxT("property")) {
			ParsePropertyElement(node);
			break;
		}
	}
}
// }}}

// {{{ MessageArguments Property::GetPropertyArguments() const
MessageArguments Property::GetPropertyArguments() const {
	MessageArguments args(3,
			wxT("-d"), IntToString(depth).c_str(),
			wxT("-c"), context->GetID().c_str(),
			wxT("-n"), fullName.c_str());

	if (address != wxEmptyString) {
		args.Append(wxT("-a"), address);
	}

	if (key != wxEmptyString) {
		args.Append(wxT("-k"), key);
	}

	return args;
}
// }}}
// {{{ void Property::ParsePropertyElement(wxXmlNode *node)
void Property::ParsePropertyElement(wxXmlNode *node) {
	address = node->GetPropVal(wxT("address"), wxEmptyString);
	className = node->GetPropVal(wxT("classname"), wxEmptyString);
	constant = (node->GetPropVal(wxT("constant"), wxT("0")) == wxT("1"));
	fullName = node->GetPropVal(wxT("fullname"), wxEmptyString);
	hasChildren = (node->GetPropVal(wxT("children"), wxT("0")) == wxT("1"));
	key = node->GetPropVal(wxT("key"), wxEmptyString);
	name = node->GetPropVal(wxT("name"), wxEmptyString);
	size = StringToULong(node->GetPropVal(wxT("size"), wxT("0")));

	try {
		type = conn->TypemapGet().GetType(node->GetPropVal(wxT("type"), wxT("undefined")));
	}
	catch (Error e) {
		type = Type();
	}

	data = node->GetNodeContent();

	for (wxXmlNode *child = node->GetChildren(); child != NULL; child = child->GetNext()) {
		if (child->GetType() == wxXML_ELEMENT_NODE && child->GetName() == wxT("property")) {
			Property *prop = new Property(conn, context, depth, this);
			prop->ParsePropertyElement(child);
			children[prop->GetName()] = prop;
		}
	}
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
