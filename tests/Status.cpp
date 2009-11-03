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

#include "Status.h"

CPPUNIT_TEST_SUITE_REGISTRATION(Status);

// {{{ void Status::testBreak()
void Status::testBreak() {
	wxString reason;
	AddResponse(wxT("xml/status/break.xml"));
	conn->ProcessNextResponse();
	DBGp::Connection::EngineStatus status(conn->Status(&reason));
	CPPUNIT_ASSERT(status == DBGp::Connection::BREAK);
	CPPUNIT_ASSERT(reason == wxT("ok"));
}
// }}}
// {{{ void Status::testRunning()
void Status::testRunning() {
	wxString reason;
	AddResponse(wxT("xml/status/running.xml"));
	conn->ProcessNextResponse();
	DBGp::Connection::EngineStatus status(conn->Status(&reason));
	CPPUNIT_ASSERT(status == DBGp::Connection::RUNNING);
	CPPUNIT_ASSERT(reason == wxT("ok"));
}
// }}}
// {{{ void Status::testStarting()
void Status::testStarting() {
	wxString reason;
	AddResponse(wxT("xml/status/starting.xml"));
	conn->ProcessNextResponse();
	DBGp::Connection::EngineStatus status(conn->Status(&reason));
	CPPUNIT_ASSERT(status == DBGp::Connection::STARTING);
	CPPUNIT_ASSERT(reason == wxT("ok"));
}
// }}}
// {{{ void Status::testStatusChange()
void Status::testStatusChange() {
	AddResponse(wxT("xml/status/run.xml"));
	conn->ProcessNextResponse();
	conn->Run();
	CPPUNIT_ASSERT(lastEvent != NULL);
	DBGp::StatusChangeEvent *e = dynamic_cast<DBGp::StatusChangeEvent *>(lastEvent);
	CPPUNIT_ASSERT(e != NULL);
	CPPUNIT_ASSERT(e->GetStatus() == DBGp::Connection::BREAK);
}
// }}}
// {{{ void Status::testStopped()
void Status::testStopped() {
	wxString reason;
	AddResponse(wxT("xml/status/stopped.xml"));
	conn->ProcessNextResponse();
	DBGp::Connection::EngineStatus status(conn->Status(&reason));
	CPPUNIT_ASSERT(status == DBGp::Connection::STOPPED);
	CPPUNIT_ASSERT(reason == wxT("ok"));
}
// }}}
// {{{ void Status::testStopping()
void Status::testStopping() {
	wxString reason;
	AddResponse(wxT("xml/status/stopping.xml"));
	conn->ProcessNextResponse();
	DBGp::Connection::EngineStatus status(conn->Status(&reason));
	CPPUNIT_ASSERT(status == DBGp::Connection::STOPPING);
	CPPUNIT_ASSERT(reason == wxT("ok"));
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
