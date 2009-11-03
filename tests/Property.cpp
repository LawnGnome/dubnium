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

#include "Property.h"

#include <cstring>

CPPUNIT_TEST_SUITE_REGISTRATION(Property);

// {{{ void Property::setUp()
void Property::setUp() {
	Stack::setUp();
	DBGp::StackLevel::ContextMap::const_iterator i = stack->GetLevel(0)->GetContexts().find(wxT("0"));
	context = new DBGp::Context(*(i->second));
}
// }}}
// {{{ void Property::tearDown()
void Property::tearDown() {
	delete context;
	Stack::tearDown();
}
// }}}

// {{{ void Property::testArray()
void Property::testArray() {
	DBGp::Property *arr = context->GetProperty(wxT("arr"));
	CPPUNIT_ASSERT(arr != NULL);
	CPPUNIT_ASSERT(arr->GetContext() == context);
	CPPUNIT_ASSERT(arr->GetDepth() == 0);
	CPPUNIT_ASSERT(arr->GetFullName() == wxT("arr"));
	CPPUNIT_ASSERT(arr->GetName() == wxT("arr"));
	CPPUNIT_ASSERT(arr->GetParent() == NULL);
	CPPUNIT_ASSERT(arr->GetType().GetCommonType() == DBGp::Type::ARRAY);
	CPPUNIT_ASSERT(arr->HasChildren() == true);

	DBGp::Property *zero = arr->GetChild(wxT("0"));
	CPPUNIT_ASSERT(zero != NULL);
	CPPUNIT_ASSERT(zero->GetData() == wxT("42"));
	CPPUNIT_ASSERT(zero->GetType().GetCommonType() == DBGp::Type::INT);
}
// }}}
// {{{ void Property::testContextGetProperties()
void Property::testContextGetProperties() {
	const DBGp::Property::PropertyMap &properties(context->GetProperties());
	CPPUNIT_ASSERT(properties.size() == 3);
}
// }}}
// {{{ void Property::testContextGetProperty()
void Property::testContextGetProperty() {
	DBGp::Property *prop = context->GetProperty(wxT("str"));
	CPPUNIT_ASSERT(prop != NULL);
	CPPUNIT_ASSERT(prop->GetData() == wxT("Group Four"));
	CPPUNIT_ASSERT(prop->GetType().GetCommonType() == DBGp::Type::STRING);
}
// }}}
// {{{ void Property::testContextGetPropertyNotFound()
void Property::testContextGetPropertyNotFound() {
	context->GetProperty(wxT("notFound"));
}
// }}}
// {{{ void Property::testGetChildNotFound()
void Property::testGetChildNotFound() {
	DBGp::Property *arr = context->GetProperty(wxT("arr"));
	CPPUNIT_ASSERT(arr != NULL);
	arr->GetChild(wxT("2"));
}
// }}}
// {{{ void Property::testObject()
void Property::testObject() {
	DBGp::Property *obj = context->GetProperty(wxT("obj"));
	CPPUNIT_ASSERT(obj != NULL);
	CPPUNIT_ASSERT(obj->GetClassName() == wxT("Object"));
	CPPUNIT_ASSERT(obj->GetContext() == context);
	CPPUNIT_ASSERT(obj->GetDepth() == 0);
	CPPUNIT_ASSERT(obj->GetFullName() == wxT("obj"));
	CPPUNIT_ASSERT(obj->GetName() == wxT("obj"));
	CPPUNIT_ASSERT(obj->GetParent() == NULL);
	CPPUNIT_ASSERT(obj->GetType().GetCommonType() == DBGp::Type::OBJECT);
	CPPUNIT_ASSERT(obj->HasChildren() == true);

	DBGp::Property *constant = obj->GetChild(wxT("constant"));
	CPPUNIT_ASSERT(constant != NULL);
	CPPUNIT_ASSERT(constant->GetData() == wxT("42"));
	CPPUNIT_ASSERT(constant->GetType().GetCommonType() == DBGp::Type::INT);
	CPPUNIT_ASSERT(constant->IsConstant() == true);
}
// }}}
// {{{ void Property::testUpdate()
void Property::testUpdate() {
	AddResponse(wxT("xml/property/get-str.xml"));
	DBGp::Property *prop = context->GetProperty(wxT("str"));
	CPPUNIT_ASSERT(prop != NULL);
	CPPUNIT_ASSERT(prop->GetData() == wxT("Group Four"));
	prop->Update();
	CPPUNIT_ASSERT(prop->GetData() == wxT("Group Five"));
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
