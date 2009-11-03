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

#include "Typemap.h"

CPPUNIT_TEST_SUITE_REGISTRATION(Typemap);

// {{{ void Typemap::setUp()
void Typemap::setUp() {
	DBGpFixture::setUp();
	conn->ProcessNextResponse();
	typemap = new DBGp::Typemap(conn->TypemapGet());
}
// }}}
// {{{ void Typemap::tearDown()
void Typemap::tearDown() {
	delete typemap;
	DBGpFixture::tearDown();
}
// }}}

// {{{ void Typemap::testAddType()
void Typemap::testAddType() {
	DBGp::Type type(DBGp::Type::FLOAT, wxT("test-float"), wxT("xsd:float"));
	typemap->AddType(type);

	DBGp::Type addedType(typemap->GetType(wxT("test-float")));
	CPPUNIT_ASSERT(addedType.GetCommonType() == type.GetCommonType());
	CPPUNIT_ASSERT(addedType.GetName() == type.GetName());
	CPPUNIT_ASSERT(addedType.GetXsiType() == type.GetXsiType());

	const DBGp::Typemap::TypeMap &types = typemap->GetTypes();
	bool found = false;
	for (DBGp::Typemap::TypeMap::const_iterator i = types.begin(); !found && i != types.end(); i++) {
		if (i->second.GetName() == type.GetName()) {
			found = true;
		}
	}
	CPPUNIT_ASSERT(found == true);
}
// }}}
// {{{ void Typemap::testGetMissingType()
void Typemap::testGetMissingType() {
	typemap->GetType(wxT("does not exist"));
}
// }}}
// {{{ void Typemap::testGetType()
void Typemap::testGetType() {
	DBGp::Type type(typemap->GetType(wxT("bool")));
	CPPUNIT_ASSERT(type.GetCommonType() == DBGp::Type::BOOL);
	CPPUNIT_ASSERT(type.GetName() == wxT("bool"));
	CPPUNIT_ASSERT(type.GetXsiType() == wxT("xsd:boolean"));
}
// }}}
// {{{ void Typemap::testGetTypes()
void Typemap::testGetTypes() {
	const DBGp::Typemap::TypeMap &types = typemap->GetTypes();
	CPPUNIT_ASSERT(types.size() == 8);

	DBGp::Typemap::TypeMap::const_iterator i = types.find(wxT("resource"));
	CPPUNIT_ASSERT(i != types.end());
	CPPUNIT_ASSERT(i->first == wxT("resource"));
	CPPUNIT_ASSERT(i->second.GetCommonType() == DBGp::Type::RESOURCE);
	CPPUNIT_ASSERT(i->second.GetXsiType() == wxEmptyString);
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
