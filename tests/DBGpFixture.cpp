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

#include "DBGpFixture.h"
#include <wx/arrstr.h>
#include <wx/filefn.h>
#include <wx/log.h>

// {{{ Event table
BEGIN_EVENT_TABLE(DBGpFixture, wxEvtHandler)
	EVT_DBGP_CONNECTION(wxID_ANY, DBGpFixture::OnConnectionEvent)
	EVT_DBGP_STATUSCHANGE(wxID_ANY, DBGpFixture::OnStatusChangeEvent)
	EVT_DBGP_STDERR(wxID_ANY, DBGpFixture::OnStderrEvent)
	EVT_DBGP_STDOUT(wxID_ANY, DBGpFixture::OnStdoutEvent)
END_EVENT_TABLE()
// }}}

// {{{ void DBGpFixture::setUp()
void DBGpFixture::setUp() {
	lastEvent = NULL;
	server = new Test::Server(this);
	conn = new Test::Connection(new wxSocketClient, server);
	
	// Send expected initialisation responses.
	wxArrayString files;
	for (wxString file(wxFindFirstFile(wxT("xml/init/*.xml"))); !file.IsEmpty(); file = wxFindNextFile()) {
		files.Add(file);
	}
	files.Sort();
	for (size_t i = 0; i < files.GetCount(); ++i) {
		wxString file(files[i]);
		wxXmlDocument doc(file);
		if (doc.IsOk()) {
			conn->AddResponse(doc);
		}
	}
}
// }}}
// {{{ void DBGpFixture::tearDown()
void DBGpFixture::tearDown() {
	if (lastEvent) {
		delete lastEvent;
	}

	delete conn;
	delete server;
}
// }}}

// {{{ void DBGpFixture::OnConnectionEvent(DBGp::ConnectionEvent &event)
void DBGpFixture::OnConnectionEvent(DBGp::ConnectionEvent &event) {
	OnEvent(event);
}
// }}}
// {{{ void DBGpFixture::OnStatusChangeEvent(DBGp::StatusChangeEvent &event)
void DBGpFixture::OnStatusChangeEvent(DBGp::StatusChangeEvent &event) {
	OnEvent(event);
}
// }}}
// {{{ void DBGpFixture::OnStderrEvent(DBGp::StderrEvent &event)
void DBGpFixture::OnStderrEvent(DBGp::StderrEvent &event) {
	OnEvent(event);
}
// }}}
// {{{ void DBGpFixture::OnStdoutEvent(DBGp::StdoutEvent &event)
void DBGpFixture::OnStdoutEvent(DBGp::StdoutEvent &event) {
	OnEvent(event);
}
// }}}

// {{{ void DBGpFixture::AddResponse(const wxString &file)
void DBGpFixture::AddResponse(const wxString &file) {
	wxXmlDocument doc(file);
	if (doc.IsOk()) {
		conn->AddResponse(doc);
	}
	else {
		CPPUNIT_FAIL("XML response file not found.");
	}
}
// }}}
// {{{ void DBGpFixture::OnEvent(DBGp::Event &event)
void DBGpFixture::OnEvent(DBGp::Event &event) {
	if (lastEvent) {
		delete lastEvent;
	}

	lastEvent = dynamic_cast<DBGp::Event *>(event.Clone());
}
// }}}

// vim:set fdm=marker ts=8 noet cin:
