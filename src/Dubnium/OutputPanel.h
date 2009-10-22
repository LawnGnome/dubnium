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

#ifndef DUBNIUM_OUTPUTPANEL_H
#define DUBNIUM_OUTPUTPANEL_H

#include <list>

#include <wx/richtext/richtextctrl.h>
#include <wx/string.h>
#include <wx/tglbtn.h>
#include <wx/thread.h>

#include "ID.h"
#include "ToolbarPanel.h"

class OutputPanel : public ToolbarPanel {
	public:
		OutputPanel(ConnectionPage *parent, wxWindowID id = wxID_ANY);

		void AppendStderr(const wxString &data);
		void AppendStdout(const wxString &data);
		void ClearOutput();

		void OnSave(wxCommandEvent &event);
		void OnShowAll(wxCommandEvent &event);
		void OnShowStderr(wxCommandEvent &event);
		void OnShowStdout(wxCommandEvent &event);

	protected:
		typedef struct {
			unsigned short mode;
			wxString data;
		} OutputChunk;
		typedef std::list<OutputChunk> OutputList;

		const static unsigned short STDERR = 0x1;
		const static unsigned short STDOUT = 0x2;
		const static unsigned short ALL = STDERR | STDOUT;

		OutputList data;
		unsigned short mode;
		wxRichTextCtrl *output;
		wxMutex outputMutex;

		void AppendChunk(const OutputChunk &chunk);
		void RedisplayOutput();

		DECLARE_EVENT_TABLE()
};

#endif

// vim:set fdm=marker ts=8 noet cin:
