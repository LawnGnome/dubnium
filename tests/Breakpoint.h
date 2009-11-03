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

#ifndef TEST_BREAKPOINT_H
#define TEST_BREAKPOINT_H

#include "DBGpFixture.h"

class Breakpoint : public DBGpFixture {
	CPPUNIT_TEST_SUITE(Breakpoint);
	CPPUNIT_TEST(testCall);
	CPPUNIT_TEST(testConditional);
	CPPUNIT_TEST(testCreateRemove);
	CPPUNIT_TEST(testException);
	CPPUNIT_TEST_EXCEPTION(testGetError, DBGp::EngineError);
	CPPUNIT_TEST(testHitCondition);
	CPPUNIT_TEST(testLine);
	CPPUNIT_TEST(testReturn);
	CPPUNIT_TEST_EXCEPTION(testSetError, DBGp::EngineError);
	CPPUNIT_TEST(testTemporary);
	CPPUNIT_TEST(testUpdate);
	CPPUNIT_TEST(testWatchType);
	CPPUNIT_TEST_SUITE_END();

	public:
		virtual void setUp();
		virtual void tearDown();

		void testCall();
		void testConditional();
		void testCreateRemove();
		void testException();
		void testGetError();
		void testHitCondition();
		void testLine();
		void testReturn();
		void testSetError();
		void testTemporary();
		void testUpdate();
		void testWatchType();

	protected:
		DBGp::Breakpoint *breakpoint;

		void run();
};

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
