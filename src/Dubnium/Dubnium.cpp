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

#include "Dubnium.h"
#include "ArtProvider.h"
#include "Config.h"
#include "MainFrame.h"

#include <algorithm>
#include <stdexcept>

#include <wx/gdicmn.h>
#include <wx/image.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/sysopt.h>

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

// {{{ StickyBreakpoint BreakpointToSticky(const wxString &script, const DBGp::Breakpoint *bp) throw (std::invalid_argument)
StickyBreakpoint BreakpointToSticky(const wxString &script, const DBGp::Breakpoint *bp) throw (std::invalid_argument) {
	if (bp) {
		wxString arg;

		switch (bp->GetType()) {
			case DBGp::Breakpoint::CALL:
			case DBGp::Breakpoint::RETURN:
				arg = bp->GetFunction();
				break;

			case DBGp::Breakpoint::EXCEPTION:
				arg = bp->GetException();
				break;

			default:
				wxLogDebug(wxT("Bad breakpoint type: %s."), DBGp::Breakpoint::TypeToString(bp->GetType()).c_str());
				throw std::invalid_argument("Bad breakpoint type");
		}

		return StickyBreakpoint(script, bp->GetType(), arg);
}
	else {
		wxLogDebug(wxT("NULL breakpoint."));
		throw std::invalid_argument("NULL breakpoint");
	}
}
// }}}

// {{{ void Dubnium::AddStickyBreakpoint(const wxString &script, const DBGp::Breakpoint *bp)
void Dubnium::AddStickyBreakpoint(const wxString &script, const DBGp::Breakpoint *bp) {
	try {
		AddStickyBreakpoint(BreakpointToSticky(script, bp));
	}
	catch (std::invalid_argument e) {}
}
// }}}
// {{{ void Dubnium::AddStickyBreakpoint(const StickyBreakpoint &sb)
void Dubnium::AddStickyBreakpoint(const StickyBreakpoint &sb) {
	wxLogDebug(wxT("Adding sticky breakpoint: script %s; type %s; argument %s."), sb.GetScript().c_str(), DBGp::Breakpoint::TypeToString(sb.GetType()).c_str(), sb.GetArgument().c_str());
	stickyBreakpoints.push_back(sb);
}
// }}}
// {{{ std::vector<StickyBreakpoint> Dubnium::GetStickyBreakpoints(const wxString &script) const
std::vector<StickyBreakpoint> Dubnium::GetStickyBreakpoints(const wxString &script) const {
	/* This isn't optimal, admittedly, but if you're using hundreds of
	 * sticky breakpoints... */
	std::vector<StickyBreakpoint> breakpoints;

	for (std::vector<StickyBreakpoint>::const_iterator i = stickyBreakpoints.begin(); i != stickyBreakpoints.end(); i++) {
		if (i->GetScript() == script) {
			breakpoints.push_back(*i);
		}
	}

	return breakpoints;
}
// }}}
// {{{ bool Dubnium::IsStickyBreakpoint(const wxString &script, const DBGp::Breakpoint *bp) const
bool Dubnium::IsStickyBreakpoint(const wxString &script, const DBGp::Breakpoint *bp) const {
	try {
		return IsStickyBreakpoint(BreakpointToSticky(script, bp));
	}
	catch (std::invalid_argument e) {
		return false;
	}
}
// }}}
// {{{ bool Dubnium::IsStickyBreakpoint(const StickyBreakpoint &sb) const
bool Dubnium::IsStickyBreakpoint(const StickyBreakpoint &sb) const {
	return std::count(stickyBreakpoints.begin(), stickyBreakpoints.end(), sb);
}
// }}}
// {{{ void Dubnium::RemoveStickyBreakpoint(const wxString &script, const DBGp::Breakpoint *bp)
void Dubnium::RemoveStickyBreakpoint(const wxString &script, const DBGp::Breakpoint *bp) {
	try {
		RemoveStickyBreakpoint(BreakpointToSticky(script, bp));
	}
	catch (std::invalid_argument e) {}
}
// }}}
// {{{ void Dubnium::RemoveStickyBreakpoint(const StickyBreakpoint &sb) 
void Dubnium::RemoveStickyBreakpoint(const StickyBreakpoint &sb) {
	wxLogDebug(wxT("Removing sticky breakpoint: script %s; type %s; argument %s."), sb.GetScript().c_str(), DBGp::Breakpoint::TypeToString(sb.GetType()).c_str(), sb.GetArgument().c_str());
	std::vector<StickyBreakpoint>::iterator end(std::remove(stickyBreakpoints.begin(), stickyBreakpoints.end(), sb));
	stickyBreakpoints.erase(end, stickyBreakpoints.end());
}
// }}}

IMPLEMENT_APP(Dubnium)

// vim:set fdm=marker ts=8 sw=8 noet cin:
