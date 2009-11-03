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

#include "Init.h"

CPPUNIT_TEST_SUITE_REGISTRATION(Init);

// {{{ void Init::testInit()
void Init::testInit() {
	conn->ProcessNextResponse();
	CPPUNIT_ASSERT(lastEvent != NULL);
	CPPUNIT_ASSERT(lastEvent->GetEventType() == wxEVT_DBGP_CONNECTION);
	DBGp::ConnectionEvent *e = dynamic_cast<DBGp::ConnectionEvent *>(lastEvent);
	CPPUNIT_ASSERT(e != NULL);
	CPPUNIT_ASSERT(e->GetAppID() == wxT("Test AppID"));
	CPPUNIT_ASSERT(e->GetConnection() == conn);
	CPPUNIT_ASSERT(e->GetFileURI() == wxT("dbgp://"));
	CPPUNIT_ASSERT(e->GetIDEKey() == wxT("Test IDE Key"));
	CPPUNIT_ASSERT(e->GetLanguage() == wxT("Test Language"));
	CPPUNIT_ASSERT(e->GetProtocolVersion() == wxT("1.0"));
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
