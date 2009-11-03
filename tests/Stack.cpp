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

#include "Stack.h"

CPPUNIT_TEST_SUITE_REGISTRATION(Stack);

// {{{ void Stack::setUp()
void Stack::setUp() {
	DBGpFixture::setUp();
	AddResponse(wxT("xml/stack/depth.xml"));
	AddResponse(wxT("xml/stack/get-0.xml"));
	AddResponse(wxT("xml/stack/context.xml"));
	AddResponse(wxT("xml/stack/context-get-0.xml"));
	AddResponse(wxT("xml/stack/context-get-1.xml"));
	AddResponse(wxT("xml/stack/get-1.xml"));
	AddResponse(wxT("xml/stack/context.xml"));
	AddResponse(wxT("xml/stack/context-get-0.xml"));
	AddResponse(wxT("xml/stack/context-get-1.xml"));
	conn->ProcessNextResponse();

	/* Not the generally recommended way of doing this, but it avoids a
	 * duplicate call to stack_depth. */
	stack = new DBGp::Stack(conn);
}
// }}}
// {{{ void Stack::tearDown()
void Stack::tearDown() {
	delete stack;
	DBGpFixture::tearDown();
}
// }}}

// {{{ void Stack::testContext()
void Stack::testContext() {
	DBGp::StackLevel *level = stack->GetLevel(0);
	CPPUNIT_ASSERT(level != NULL);

	const DBGp::StackLevel::ContextMap &contexts(level->GetContexts());
	DBGp::StackLevel::ContextMap::const_iterator i = contexts.find(wxT("0"));
	CPPUNIT_ASSERT(i != contexts.end());

	DBGp::Context *context = i->second;
	CPPUNIT_ASSERT(context->GetID() == wxT("0"));
	CPPUNIT_ASSERT(context->GetName() == wxT("Local"));
	CPPUNIT_ASSERT(context->GetStackLevel() == level);
}
// }}}
// {{{ void Stack::testStackGet()
void Stack::testStackGet() {
	CPPUNIT_ASSERT(stack != NULL);
	CPPUNIT_ASSERT(stack->GetDepth() == 2);
}
// }}}
// {{{ void Stack::testStackLevel()
void Stack::testStackLevel() {
	DBGp::StackLevel *level = stack->GetLevel(0);
	CPPUNIT_ASSERT(level != NULL);
	CPPUNIT_ASSERT(level->GetCmdBegin() == DBGp::Location());
	CPPUNIT_ASSERT(level->GetCmdEnd() == DBGp::Location());
	CPPUNIT_ASSERT(level->GetLevel() == 0);
	CPPUNIT_ASSERT(level->GetLineNo() == 42);
	CPPUNIT_ASSERT(level->GetType() == DBGp::StackLevel::FILE);
	CPPUNIT_ASSERT(level->GetWhere() == wxEmptyString);

	level = stack->GetLevel(1);
	CPPUNIT_ASSERT(level != NULL);
	CPPUNIT_ASSERT(level->GetCmdBegin() == DBGp::Location(13, 8));
	CPPUNIT_ASSERT(level->GetCmdEnd() == DBGp::Location(13, 13));
	CPPUNIT_ASSERT(level->GetLevel() == 1);
	CPPUNIT_ASSERT(level->GetLineNo() == 13);
	CPPUNIT_ASSERT(level->GetType() == DBGp::StackLevel::FILE);
	CPPUNIT_ASSERT(level->GetWhere() == wxT("func"));
}
// }}}
// {{{ void Stack::testStackLevelNotFound()
void Stack::testStackLevelNotFound() {
	stack->GetLevel(2);
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
