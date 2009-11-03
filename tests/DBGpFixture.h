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

#ifndef TEST_DBGPFIXTURE_H
#define TEST_DBGPFIXTURE_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <wx/event.h>

#include "DBGp/Event.h"

#include "Test/Connection.h"
#include "Test/Server.h"

class DBGpFixture : public wxEvtHandler, public CppUnit::TestFixture {
	public:
		void setUp();
		void tearDown();

		virtual void OnConnectionEvent(DBGp::ConnectionEvent &event);
		virtual void OnStatusChangeEvent(DBGp::StatusChangeEvent &event);
		virtual void OnStderrEvent(DBGp::StderrEvent &event);
		virtual void OnStdoutEvent(DBGp::StdoutEvent &event);

	protected:
		Test::Connection *conn;
		DBGp::Event *lastEvent;
		Test::Server *server;

		void AddResponse(const wxString &file);
		virtual void OnEvent(DBGp::Event &event);

		DECLARE_EVENT_TABLE()
};

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
