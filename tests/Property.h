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

#ifndef TEST_PROPERTY_H
#define TEST_PROPERTY_H

#include "Stack.h"

class Property : public Stack {
	CPPUNIT_TEST_SUITE(Property);
	CPPUNIT_TEST(testArray);
	CPPUNIT_TEST(testContextGetProperties);
	CPPUNIT_TEST(testContextGetProperty);
	CPPUNIT_TEST_EXCEPTION(testContextGetPropertyNotFound, DBGp::NotFoundError);
	CPPUNIT_TEST_EXCEPTION(testGetChildNotFound, DBGp::NotFoundError);
	CPPUNIT_TEST(testObject);
	CPPUNIT_TEST(testUpdate);
	CPPUNIT_TEST_SUITE_END();

	public:
		virtual void setUp();
		virtual void tearDown();

		void testArray();
		void testContextGetProperties();
		void testContextGetProperty();
		void testContextGetPropertyNotFound();
		void testGetChildNotFound();
		void testObject();
		void testString();
		void testUpdate();

	protected:
		DBGp::Context *context;
};

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
