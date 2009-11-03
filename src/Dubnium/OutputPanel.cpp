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

#include "OutputPanel.h"

#include <wx/artprov.h>
#include <wx/bmpbuttn.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>

// {{{ Event table
BEGIN_EVENT_TABLE(OutputPanel, wxPanel)
	EVT_TOOL(wxID_SAVE, OutputPanel::OnSave)
	EVT_TOOL(ID_OUTPUTPANEL_SHOW_ALL, OutputPanel::OnShowAll)
	EVT_TOOL(ID_OUTPUTPANEL_SHOW_STDERR, OutputPanel::OnShowStderr)
	EVT_TOOL(ID_OUTPUTPANEL_SHOW_STDOUT, OutputPanel::OnShowStdout)
END_EVENT_TABLE()
// }}}

// {{{ OutputPanel::OutputPanel(ConnectionPage *parent, wxWindowID id)
OutputPanel::OutputPanel(ConnectionPage *parent, wxWindowID id) : ToolbarPanel(parent, id), mode(ALL) {
	AddRadioTool(ID_OUTPUTPANEL_SHOW_ALL, _("All"), wxT("output-all"), _("Display all output"));
	AddRadioTool(ID_OUTPUTPANEL_SHOW_STDERR, _("Stderr"), wxT("output-stderr"), _("Display output sent to standard error"));
	AddRadioTool(ID_OUTPUTPANEL_SHOW_STDOUT, _("Stdout"), wxT("output-stdout"), _("Display output sent to standard output"));
	toolbar->AddSeparator();
	toolbar->AddTool(wxID_SAVE, _("Save"), wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR, size), _("Save output to a file"));
	toolbar->Realize();

	output = new wxRichTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxRE_READONLY | wxRE_MULTILINE);
	sizer->Add(output, 1, wxEXPAND);
}
// }}}

// {{{ void OutputPanel::AppendStderr(const wxString &data)
void OutputPanel::AppendStderr(const wxString &data) {
	outputMutex.Lock();
	OutputChunk chunk;

	chunk.mode = STDERR;
	chunk.data = data;
	this->data.push_back(chunk);

	if (mode & STDERR) {
		AppendChunk(chunk);
	}
	outputMutex.Unlock();
}
// }}}
// {{{ void OutputPanel::AppendStdout(const wxString &data)
void OutputPanel::AppendStdout(const wxString &data) {
	outputMutex.Lock();
	OutputChunk chunk;

	chunk.mode = STDOUT;
	chunk.data = data;
	this->data.push_back(chunk);

	if (mode & STDOUT) {
		AppendChunk(chunk);
	}
	outputMutex.Unlock();
}
// }}}
// {{{ void OutputPanel::ClearOutput()
void OutputPanel::ClearOutput() {
	outputMutex.Lock();
	data.clear();
	output->Clear();
	outputMutex.Unlock();
}
// }}}

// {{{ void OutputPanel::OnSave(wxCommandEvent &event)
void OutputPanel::OnSave(wxCommandEvent &event) {
	wxFileDialog fd(this, _("Save Output"), wxEmptyString, wxEmptyString, wxT("*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (fd.ShowModal() == wxID_OK) {
		if (!output->SaveFile(fd.GetPath(), wxRICHTEXT_TYPE_TEXT)) {
			wxMessageBox(_("The output could not be saved."), _("Error"), wxICON_ERROR | wxOK);
		}
	}
}
// }}}
// {{{ void OutputPanel::OnShowAll(wxCommandEvent &event)
void OutputPanel::OnShowAll(wxCommandEvent &event) {
	mode = ALL;
	RedisplayOutput();
}
// }}}
// {{{ void OutputPanel::OnShowStderr(wxCommandEvent &event)
void OutputPanel::OnShowStderr(wxCommandEvent &event) {
	mode = STDERR;
	RedisplayOutput();
}
// }}}
// {{{ void OutputPanel::OnShowStdout(wxCommandEvent &event)
void OutputPanel::OnShowStdout(wxCommandEvent &event) {
	mode = STDOUT;
	RedisplayOutput();
}
// }}}

// {{{ void OutputPanel::AppendChunk(const OutputChunk &chunk)
void OutputPanel::AppendChunk(const OutputChunk &chunk) {
	if (chunk.mode & STDERR) {
		output->BeginItalic();
	}
	output->AppendText(chunk.data);
	if (chunk.mode & STDERR) {
		output->EndItalic();
	}
}
// }}}
// {{{ void OutputPanel::RedisplayOutput()
void OutputPanel::RedisplayOutput() {
	outputMutex.Lock();
	output->Clear();
	for (OutputList::const_iterator i = data.begin(); i != data.end(); i++) {
		if (i->mode & mode) {
			AppendChunk(*i);
		}
	}
	outputMutex.Unlock();
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
