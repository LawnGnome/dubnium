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

#include "Dubnium.h"
#include "ArtProvider.h"
#include "Config.h"
#include "MainFrame.h"

#include <wx/gdicmn.h>
#include <wx/image.h>
#include <wx/sysopt.h>

#include <wx/msgdlg.h>

// {{{ bool Dubnium::OnInit()
bool Dubnium::OnInit() {
	// Set up our toolbar image handling.
	wxInitAllImageHandlers();
	wxArtProvider::Push(new ArtProvider);

#if defined(__WXMSW__)
	/* We're on Windows, but we can't do alpha-blended icons because
	 * Microsoft apparently can't make that work. Do the best we can and
	 * add a FAQ item later dealing with this. */
	wxSystemOptions::SetOption(wxT("msw.remap"), 0);
#endif

	// Set up the vendor and product names, which are used within wxConfig.
	SetVendorName(APPNAME);
	SetAppName(APPNAME);

	MainFrame *frame = new MainFrame;
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}
// }}}

IMPLEMENT_APP(Dubnium)

// vim:set fdm=marker ts=8 noet cin:
