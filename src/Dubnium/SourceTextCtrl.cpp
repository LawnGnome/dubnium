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

#include "SourceTextCtrl.h"
#include "Languages.h"

#include <algorithm>
#include <string>

#include <wx/artprov.h>
#include <wx/log.h>
#include <wx/tipwin.h>

#include "ID.h"

using namespace Languages;

// {{{ Event table
BEGIN_EVENT_TABLE(SourceTextCtrl, wxStyledTextCtrl)
	EVT_CONTEXT_MENU(SourceTextCtrl::OnContextMenu)
	EVT_LEFT_DCLICK(SourceTextCtrl::OnDoubleClick)
	EVT_MENU(wxID_COPY, SourceTextCtrl::OnCopy)
	EVT_MENU(wxID_SELECTALL, SourceTextCtrl::OnSelectAll)
	EVT_MENU(ID_SOURCETEXTCTRL_RUN_TO_HERE, SourceTextCtrl::OnRunToHere)
	EVT_MENU(ID_SOURCETEXTCTRL_TOGGLE_BREAKPOINT, SourceTextCtrl::OnToggleBreakpoint)
	EVT_STC_DWELLEND(wxID_ANY, SourceTextCtrl::OnDwellEnd)
	EVT_STC_DWELLSTART(wxID_ANY, SourceTextCtrl::OnDwellStart)
	EVT_STC_MARGINCLICK(wxID_ANY, SourceTextCtrl::OnMarginClick)
END_EVENT_TABLE()
// }}}

// {{{ SourceTextCtrl::SourceTextCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
SourceTextCtrl::SourceTextCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name) : wxStyledTextCtrl(parent, id, pos, size, style, name), menuPos(wxSTC_INVALID_POSITION), tipWindow(NULL) {
	config = wxConfigBase::Get();
	handler = dynamic_cast<SourceTextCtrlHandler *>(parent);

	wxFont font(DefaultFont());
	StyleSetFont(wxSTC_STYLE_DEFAULT, font);
	
	SetMarginType(0, wxSTC_MARGIN_NUMBER);
	SetMarginWidth(0, TextWidth(wxSTC_STYLE_LINENUMBER, wxT("99999")));
	SetMarginSensitive(0, true);

	SetMarginType(1, wxSTC_MARGIN_SYMBOL);
	SetMarginWidth(1, 16);
	SetMarginSensitive(1, true);
	MarkerDefine(MARKER_BREAKPOINT, wxSTC_MARK_CIRCLE, wxT("red"), wxT("red"));
	MarkerDefine(MARKER_CURRENT, wxSTC_MARK_ARROW, wxT("green"), wxT("green"));

	SetMouseDwellTime(1000);
	SetReadOnly(true);
	UsePopUp(false);
}
// }}}

// {{{ void SourceTextCtrl::AddBreakpoint(int line)
void SourceTextCtrl::AddBreakpoint(int line) {
	if (handler) {
		handler->BreakpointAdd(line + 1);
	}
	breakpoints.push_back(line);
	MarkerAdd(line, MARKER_BREAKPOINT);
}
// }}}
// {{{ wxFont SourceTextCtrl::DefaultFont()
wxFont SourceTextCtrl::DefaultFont() {
	static wxConfigBase *config = wxConfigBase::Get();
	long defaultSize = 10;
	long defaultStyle = wxFONTSTYLE_NORMAL;
	long defaultWeight = wxFONTWEIGHT_NORMAL;
	wxFont font(defaultSize, wxFONTFAMILY_TELETYPE, defaultStyle, defaultWeight);
	wxString defaultFace(font.GetFaceName());

	font.SetFaceName(config->Read(wxT("SourcePanel/FontFace"), defaultFace));
	font.SetPointSize(config->Read(wxT("SourcePanel/FontSize"), defaultSize));
	font.SetStyle(config->Read(wxT("SourcePanel/FontStyle"), defaultStyle));
	font.SetWeight(config->Read(wxT("SourcePanel/FontWeight"), defaultWeight));

	return font;
}
// }}}
// {{{ void SourceTextCtrl::RemoveBreakpoint(int line)
void SourceTextCtrl::RemoveBreakpoint(int line) {
	if (handler) {
		handler->BreakpointRemove(line + 1);
	}
	for (BreakpointList::iterator i = breakpoints.begin(); i != breakpoints.end(); i++) {
		if (*i == line) {
			breakpoints.erase(i);
			break;
		}
	}
	MarkerDelete(line, MARKER_BREAKPOINT);
}
// }}}
// {{{ void SourceTextCtrl::SetBreakpointStyle(int line, bool isSet)
void SourceTextCtrl::SetBreakpointStyle(int line, bool isSet) {
	MarkerDelete(--line, MARKER_ALL);
	if (isSet) {
		if (line != current) {
			MarkerAdd(line, MARKER_BREAKPOINT); 
		}
	}
	else if (line == current) {
		MarkerAdd(line, MARKER_CURRENT);
	}
}
// }}}
// {{{ void SourceTextCtrl::SetLexerLanguage(const wxString &language)
void SourceTextCtrl::SetLexerLanguage(const wxString &language) {
	StyleClearAll();
	wxStyledTextCtrl::SetLexerLanguage(language.Lower());
	SetLexerOptions(GetLexer());
	SetStyleOptions(GetLexer());
}
// }}}
// {{{ void SourceTextCtrl::SetLine(int line)
void SourceTextCtrl::SetLine(int line) {
	// Convert the 1-indexed value from DBGp to a 0-indexed value.
	int start = PositionFromLine(--line);
	current = line;

	Freeze();
	GotoPos(start);
	MarkerDeleteAll(MARKER_CURRENT);
	MarkerAdd(line, MARKER_CURRENT);
	Thaw();
}
// }}}
// {{{ void SourceTextCtrl::SetSource(const wxString &source, int line)
void SourceTextCtrl::SetSource(const wxString &source, int line) {
	breakpoints.clear();
	Freeze();
	SetReadOnly(false);
	SetText(source);
	if (line == -1) {
		GotoPos(0);
		current = -1;
	}
	else {
		SetLine(line);
	}
	SetReadOnly(true);
	Thaw();
}
// }}}

// {{{ void SourceTextCtrl::OnCopy(wxCommandEvent &event)
void SourceTextCtrl::OnCopy(wxCommandEvent &event) {
	Copy();
}
// }}}
// {{{ void SourceTextCtrl::OnContextMenu(wxContextMenuEvent &event)
void SourceTextCtrl::OnContextMenu(wxContextMenuEvent &event) {
	wxMenu menu;
	wxPoint pos(event.GetPosition() - GetScreenPosition());

	menuPos = PositionFromPoint(pos);
	wxLogDebug(wxT("Point is line %d"), LineFromPosition(menuPos));

	menu.Append(wxID_COPY);
	menu.Append(wxID_SELECTALL);

	if (menuPos != wxSTC_INVALID_POSITION) {
		menu.AppendSeparator();
		menu.Append(ID_SOURCETEXTCTRL_RUN_TO_HERE, _("&Run to Here"));
		if (handler) {
			menu.Append(ID_SOURCETEXTCTRL_TOGGLE_BREAKPOINT, _("Toggle &Breakpoint"));
		}
	}

	PopupMenu(&menu);
}
// }}}
// {{{ void SourceTextCtrl::OnDoubleClick(wxMouseEvent &event)
void SourceTextCtrl::OnDoubleClick(wxMouseEvent &event) {
	int pos = PositionFromPointClose(event.GetPosition().x, event.GetPosition().y);

	if (pos != wxSTC_INVALID_POSITION) {
		ToggleBreakpoint(LineFromPosition(pos));
	}
}
// }}}
// {{{ void SourceTextCtrl::OnDwellEnd(wxStyledTextEvent &event)
void SourceTextCtrl::OnDwellEnd(wxStyledTextEvent &event) {
	if (tipWindow) {
		tipWindow->Destroy();
		tipWindow = NULL;
	}
}
// }}}
// {{{ void SourceTextCtrl::OnDwellStart(wxStyledTextEvent &event)
void SourceTextCtrl::OnDwellStart(wxStyledTextEvent &event) {
	int pos = event.GetPosition();
	if (pos != wxSTC_INVALID_POSITION) {
		/* We need to pull out the individual token for the variable
		 * under the mouse, if any. */
		int line = LineFromPosition(pos);
		int col = pos - PositionFromLine(line);
		wxString name(GetLine(line));
		size_t pos;
		// TODO: This should be language specific.
		static const wxString varChars(wxT("abcdefghijklmonpqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_->.$[]'"));

		pos = name.find_first_not_of(varChars, col);
		if (pos != std::string::npos) {
			name = name.Left(pos);
		}

		pos = name.find_last_not_of(varChars);
		if (pos != std::string::npos && pos < name.Length()) {
			name = name.Mid(pos + 1);
		}

		DBGp::Property *prop = handler->GetProperty(name);
		if (prop) {
			wxString text;

			text << name << wxT(" (") << prop->GetType().GetName() << wxT(") : ");

			if (prop->HasChildren()) {
				/* We'll do a shallow dump. People can look at
				 * the properties panel and dialog (or the
				 * context menu I, er, haven't implemented yet)
				 * if they want detailed information. */
				const DBGp::Property::PropertyMap children = prop->GetChildren();
				for (DBGp::Property::PropertyMap::const_iterator i = children.begin(); i != children.end(); i++) {
					DBGp::Property *child = i->second;
					text << wxT("\n") << child->GetName() << wxT(" (") << child->GetType().GetName() << wxT(") : ");
					if (child->HasChildren()) {
						text << _("<complex data structure>");
					}
					else {
						text << child->GetData();
					}
				}
			}
			else {
				text << prop->GetData();
			}

			tipWindow = new wxTipWindow(this, text, 640, &tipWindow);
			tipWindow->SetBoundingRect(wxRect(-1, -1, -1, -1));
		}
	}
}
// }}}
// {{{ void SourceTextCtrl::OnMarginClick(wxStyledTextEvent &event)
void SourceTextCtrl::OnMarginClick(wxStyledTextEvent &event) {
	ToggleBreakpoint(LineFromPosition(event.GetPosition()));
}
// }}}
// {{{ void SourceTextCtrl::OnRunToHere(wxCommandEvent &event)
void SourceTextCtrl::OnRunToHere(wxCommandEvent &event) {
	if (handler) {
		handler->BreakpointAdd(LineFromPosition(menuPos) + 1, true);
	}
}
// }}}
// {{{ void SourceTextCtrl::OnSelectAll(wxCommandEvent &event)
void SourceTextCtrl::OnSelectAll(wxCommandEvent &event) {
	SelectAll();
}
// }}}
// {{{ void SourceTextCtrl::OnToggleBreakpoint(wxCommandEvent &event)
void SourceTextCtrl::OnToggleBreakpoint(wxCommandEvent &event) {
	ToggleBreakpoint(LineFromPosition(menuPos));
}
// }}}
// {{{ void SourceTextCtrl::SetLexerOptions(int lexer)
void SourceTextCtrl::SetLexerOptions(int lexer) {
	for (const Lexers *l = lexers; l->lexer != -1; l++) {
		if (l->lexer == lexer) {
			SetKeyWords(4, l->keywords);
			SetStyleBits(l->bits);
		}
	}
}
// }}}
// {{{ void SourceTextCtrl::SetStyleOptions(int lexer)
void SourceTextCtrl::SetStyleOptions(int lexer) {
	for (const TokenTypes *t = tokenTypes; t->token != END; t++) {
		if (t->lexer == lexer) {
			switch (t->token) {
				case COMMENT:
					StyleSetForeground(t->stcType, wxT("blue"));
					break;

				case KEYWORD:
					StyleSetBold(t->stcType, true);
					StyleSetForeground(t->stcType, wxT("dark green"));
					break;

				case LITERAL:
					StyleSetForeground(t->stcType, wxT("magenta"));
					break;

				case OPERATOR:
					StyleSetBold(t->stcType, true);
					StyleSetForeground(t->stcType, wxT("brown"));
					break;

				case DEFAULT:
				case IDENTIFIER:
				default:
					;
			}
		}
	}
}
// }}}
// {{{ void SourceTextCtrl::ToggleBreakpoint(int line)
void SourceTextCtrl::ToggleBreakpoint(int line) {
	if (std::count(breakpoints.begin(), breakpoints.end(), line) > 0) {
		RemoveBreakpoint(line);
	}
	else {
		AddBreakpoint(line);
	}
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
