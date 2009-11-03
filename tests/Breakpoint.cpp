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

#include "Breakpoint.h"

#include <algorithm>

CPPUNIT_TEST_SUITE_REGISTRATION(Breakpoint);

// {{{ void Breakpoint::setUp()
void Breakpoint::setUp() {
	DBGpFixture::setUp();
	conn->ProcessNextResponse();
	breakpoint = conn->CreateBreakpoint();
}
// }}}
// {{{ void Breakpoint::tearDown()
void Breakpoint::tearDown() {
	conn->RemoveBreakpoint(breakpoint);
	DBGpFixture::tearDown();
}
// }}}

// {{{ void Breakpoint::testCall()
void Breakpoint::testCall() {
	AddResponse(wxT("xml/breakpoint/set.xml"));
	AddResponse(wxT("xml/breakpoint/get-call.xml"));
	AddResponse(wxT("xml/breakpoint/get-call.xml"));
	AddResponse(wxT("xml/breakpoint/run.xml"));
	AddResponse(wxT("xml/breakpoint/remove.xml"));

	breakpoint->SetCallType(wxT("func"));
	breakpoint->Get();
	CPPUNIT_ASSERT(breakpoint->GetFunction() == wxT("func"));
	CPPUNIT_ASSERT(breakpoint->GetHitCount() == 1);
	CPPUNIT_ASSERT(breakpoint->GetID() == wxT("BP1"));
	CPPUNIT_ASSERT(breakpoint->GetType() == DBGp::Breakpoint::CALL);
	CPPUNIT_ASSERT(breakpoint->IsEnabled() == true);
	CPPUNIT_ASSERT(breakpoint->GetTemporary() == false);

	run();
}
// }}}
// {{{ void Breakpoint::testConditional()
void Breakpoint::testConditional() {
	AddResponse(wxT("xml/breakpoint/set.xml"));
	AddResponse(wxT("xml/breakpoint/get-conditional.xml"));
	AddResponse(wxT("xml/breakpoint/get-conditional.xml"));
	AddResponse(wxT("xml/breakpoint/run.xml"));
	AddResponse(wxT("xml/breakpoint/remove.xml"));

	breakpoint->SetConditionalType(wxT("answer == 42"));
	breakpoint->Get();
	CPPUNIT_ASSERT(breakpoint->GetExpression() == wxT("answer == 42"));
	CPPUNIT_ASSERT(breakpoint->GetHitCount() == 1);
	CPPUNIT_ASSERT(breakpoint->GetID() == wxT("BP1"));
	CPPUNIT_ASSERT(breakpoint->GetType() == DBGp::Breakpoint::CONDITIONAL);
	CPPUNIT_ASSERT(breakpoint->IsEnabled() == true);
	CPPUNIT_ASSERT(breakpoint->GetTemporary() == false);

	run();
}
// }}}
// {{{ void Breakpoint::testCreateRemove()
void Breakpoint::testCreateRemove() {
	DBGp::Breakpoint *breakpoint = conn->CreateBreakpoint();
	
	const DBGp::Connection::BreakpointList &breakpoints = conn->GetBreakpoints();
	CPPUNIT_ASSERT(std::find(breakpoints.begin(), breakpoints.end(), breakpoint) != breakpoints.end());

	conn->RemoveBreakpoint(breakpoint);
	CPPUNIT_ASSERT(std::find(breakpoints.begin(), breakpoints.end(), breakpoint) == breakpoints.end());
}
// }}}
// {{{ void Breakpoint::testException()
void Breakpoint::testException() {
	AddResponse(wxT("xml/breakpoint/set.xml"));
	AddResponse(wxT("xml/breakpoint/get-exception.xml"));
	AddResponse(wxT("xml/breakpoint/get-exception.xml"));
	AddResponse(wxT("xml/breakpoint/run.xml"));
	AddResponse(wxT("xml/breakpoint/remove.xml"));

	breakpoint->SetExceptionType(wxT("TestException"));
	breakpoint->Get();
	CPPUNIT_ASSERT(breakpoint->GetException() == wxT("TestException"));
	CPPUNIT_ASSERT(breakpoint->GetHitCount() == 1);
	CPPUNIT_ASSERT(breakpoint->GetID() == wxT("BP1"));
	CPPUNIT_ASSERT(breakpoint->GetType() == DBGp::Breakpoint::EXCEPTION);
	CPPUNIT_ASSERT(breakpoint->IsEnabled() == true);
	CPPUNIT_ASSERT(breakpoint->GetTemporary() == false);

	run();
}
// }}}
// {{{ void Breakpoint::testGetError()
void Breakpoint::testGetError() {
	AddResponse(wxT("xml/breakpoint/set.xml"));
	AddResponse(wxT("xml/breakpoint/get-error.xml"));
	AddResponse(wxT("xml/breakpoint/remove.xml"));

	breakpoint->SetCallType(wxT("func"));
	breakpoint->Get();
}
// }}}
// {{{ void Breakpoint::testHitCondition()
void Breakpoint::testHitCondition() {
	AddResponse(wxT("xml/breakpoint/set.xml"));
	AddResponse(wxT("xml/breakpoint/update.xml"));
	AddResponse(wxT("xml/breakpoint/get-hitcondition.xml"));
	AddResponse(wxT("xml/breakpoint/get-hitcondition.xml"));
	AddResponse(wxT("xml/breakpoint/run.xml"));
	AddResponse(wxT("xml/breakpoint/remove.xml"));

	breakpoint->SetCallType(wxT("func"));
	breakpoint->SetHitCondition(DBGp::Breakpoint::HIT_GE, 42);
	breakpoint->Get();
	CPPUNIT_ASSERT(breakpoint->GetFunction() == wxT("func"));
	CPPUNIT_ASSERT(breakpoint->GetHitCondition() == DBGp::Breakpoint::HIT_GE);
	CPPUNIT_ASSERT(breakpoint->GetHitCount() == 1);
	CPPUNIT_ASSERT(breakpoint->GetHitValue() == 42);
	CPPUNIT_ASSERT(breakpoint->GetID() == wxT("BP1"));
	CPPUNIT_ASSERT(breakpoint->GetType() == DBGp::Breakpoint::CALL);
	CPPUNIT_ASSERT(breakpoint->IsEnabled() == true);
	CPPUNIT_ASSERT(breakpoint->GetTemporary() == false);

	run();
}
// }}}
// {{{ void Breakpoint::testLine()
void Breakpoint::testLine() {
	AddResponse(wxT("xml/breakpoint/set.xml"));
	AddResponse(wxT("xml/breakpoint/get-line.xml"));
	AddResponse(wxT("xml/breakpoint/get-line.xml"));
	AddResponse(wxT("xml/breakpoint/run.xml"));
	AddResponse(wxT("xml/breakpoint/remove.xml"));

	breakpoint->SetLineType(wxT("dbgp://"), 42);
	breakpoint->Get();
	CPPUNIT_ASSERT(breakpoint->GetFileName() == wxT("dbgp://"));
	CPPUNIT_ASSERT(breakpoint->GetHitCount() == 1);
	CPPUNIT_ASSERT(breakpoint->GetID() == wxT("BP1"));
	CPPUNIT_ASSERT(breakpoint->GetLineNo() == 42);
	CPPUNIT_ASSERT(breakpoint->GetType() == DBGp::Breakpoint::LINE);
	CPPUNIT_ASSERT(breakpoint->IsEnabled() == true);
	CPPUNIT_ASSERT(breakpoint->GetTemporary() == false);

	run();
}
// }}}
// {{{ void Breakpoint::testReturn()
void Breakpoint::testReturn() {
	AddResponse(wxT("xml/breakpoint/set.xml"));
	AddResponse(wxT("xml/breakpoint/get-return.xml"));
	AddResponse(wxT("xml/breakpoint/get-return.xml"));
	AddResponse(wxT("xml/breakpoint/run.xml"));
	AddResponse(wxT("xml/breakpoint/remove.xml"));

	breakpoint->SetReturnType(wxT("func"));
	breakpoint->Get();
	CPPUNIT_ASSERT(breakpoint->GetFunction() == wxT("func"));
	CPPUNIT_ASSERT(breakpoint->GetHitCount() == 1);
	CPPUNIT_ASSERT(breakpoint->GetID() == wxT("BP1"));
	CPPUNIT_ASSERT(breakpoint->GetType() == DBGp::Breakpoint::RETURN);
	CPPUNIT_ASSERT(breakpoint->IsEnabled() == true);
	CPPUNIT_ASSERT(breakpoint->GetTemporary() == false);

	run();
}
// }}}
// {{{ void Breakpoint::testSetError()
void Breakpoint::testSetError() {
	AddResponse(wxT("xml/breakpoint/set-error.xml"));

	breakpoint->SetCallType(wxT("func"));
}
// }}}
// {{{ void Breakpoint::testTemporary()
void Breakpoint::testTemporary() {
	AddResponse(wxT("xml/breakpoint/set.xml"));
	AddResponse(wxT("xml/breakpoint/update.xml"));
	AddResponse(wxT("xml/breakpoint/get-temporary.xml"));
	AddResponse(wxT("xml/breakpoint/get-temporary.xml"));
	AddResponse(wxT("xml/breakpoint/run.xml"));
	AddResponse(wxT("xml/breakpoint/remove.xml"));

	breakpoint->SetCallType(wxT("func"));
	breakpoint->SetTemporary(true);
	breakpoint->Get();
	CPPUNIT_ASSERT(breakpoint->GetFunction() == wxT("func"));
	CPPUNIT_ASSERT(breakpoint->GetHitCount() == 1);
	CPPUNIT_ASSERT(breakpoint->GetID() == wxT("BP1"));
	CPPUNIT_ASSERT(breakpoint->GetType() == DBGp::Breakpoint::CALL);
	CPPUNIT_ASSERT(breakpoint->IsEnabled() == true);
	CPPUNIT_ASSERT(breakpoint->GetTemporary() == true);

	run();
}
// }}}
// {{{ void Breakpoint::testUpdate()
void Breakpoint::testUpdate() {
	AddResponse(wxT("xml/breakpoint/set.xml"));
	AddResponse(wxT("xml/breakpoint/update.xml"));
	AddResponse(wxT("xml/breakpoint/get-call.xml"));
	AddResponse(wxT("xml/breakpoint/get-call.xml"));
	AddResponse(wxT("xml/breakpoint/run.xml"));
	AddResponse(wxT("xml/breakpoint/remove.xml"));

	breakpoint->SetCallType(wxT("func"));
	breakpoint->Set();
	breakpoint->Get();
	CPPUNIT_ASSERT(breakpoint->GetFunction() == wxT("func"));
	CPPUNIT_ASSERT(breakpoint->GetHitCount() == 1);
	CPPUNIT_ASSERT(breakpoint->GetID() == wxT("BP1"));
	CPPUNIT_ASSERT(breakpoint->GetType() == DBGp::Breakpoint::CALL);
	CPPUNIT_ASSERT(breakpoint->IsEnabled() == true);
	CPPUNIT_ASSERT(breakpoint->GetTemporary() == false);

	run();
}
// }}}
// {{{ void Breakpoint::testWatchType()
void Breakpoint::testWatchType() {
	AddResponse(wxT("xml/breakpoint/set.xml"));
	AddResponse(wxT("xml/breakpoint/get-watch.xml"));
	AddResponse(wxT("xml/breakpoint/get-watch.xml"));
	AddResponse(wxT("xml/breakpoint/run.xml"));
	AddResponse(wxT("xml/breakpoint/remove.xml"));

	breakpoint->SetWatchType(wxT("answer == 42"));
	breakpoint->Get();
	CPPUNIT_ASSERT(breakpoint->GetExpression() == wxT("answer == 42"));
	CPPUNIT_ASSERT(breakpoint->GetHitCount() == 1);
	CPPUNIT_ASSERT(breakpoint->GetID() == wxT("BP1"));
	CPPUNIT_ASSERT(breakpoint->GetType() == DBGp::Breakpoint::WATCH);
	CPPUNIT_ASSERT(breakpoint->IsEnabled() == true);
	CPPUNIT_ASSERT(breakpoint->GetTemporary() == false);

	run();
}
// }}}

// {{{ void Breakpoint::run()
void Breakpoint::run() {
	conn->Run();
	CPPUNIT_ASSERT(conn->Status() == DBGp::Connection::BREAK);
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
