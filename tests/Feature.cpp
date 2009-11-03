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

#include "Feature.h"

CPPUNIT_TEST_SUITE_REGISTRATION(Feature);

// {{{ void Feature::testGetError()
void Feature::testGetError() {
	AddResponse(wxT("xml/feature/get-error.xml"));
	conn->ProcessNextResponse();
	conn->FeatureGet(wxT("test"));
}
// }}}
// {{{ void Feature::testGetSuccess()
void Feature::testGetSuccess() {
	AddResponse(wxT("xml/feature/get-success.xml"));
	conn->ProcessNextResponse();
	wxString test(conn->FeatureGet(wxT("test")));
	CPPUNIT_ASSERT(test == wxT("Unit testing FTW"));
}
// }}}
// {{{ void Feature::testGetUnsupported()
void Feature::testGetUnsupported() {
	AddResponse(wxT("xml/feature/get-unsupported.xml"));
	conn->ProcessNextResponse();
	conn->FeatureGet(wxT("test"));
}
// }}}
// {{{ void Feature::testSetError()
void Feature::testSetError() {
	AddResponse(wxT("xml/feature/set-error.xml"));
	conn->ProcessNextResponse();
	conn->FeatureSet(wxT("test"), wxT("value"));
}
// }}}
// {{{ void Feature::testSetFailure()
void Feature::testSetFailure() {
	AddResponse(wxT("xml/feature/set-failure.xml"));
	conn->ProcessNextResponse();
	bool success = conn->FeatureSet(wxT("test"), wxT("value"));
	CPPUNIT_ASSERT(success == false);
}
// }}}
// {{{ void Feature::testSetSuccess()
void Feature::testSetSuccess() {
	AddResponse(wxT("xml/feature/set-success.xml"));
	conn->ProcessNextResponse();
	bool success = conn->FeatureSet(wxT("test"), wxT("value"));
	CPPUNIT_ASSERT(success == true);
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
