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

#ifndef DUBNIUM_ID_H
#define DUBNIUM_ID_H

#include <wx/defs.h>

// Program-wide window IDs.
enum {
	ID_DUBNIUM_LOWEST = wxID_HIGHEST + 1,
	ID_BREAKPOINTPANEL_ADD_CALL,
	ID_BREAKPOINTPANEL_ADD_CONDITIONAL,
	ID_BREAKPOINTPANEL_ADD_EXCEPTION,
	ID_BREAKPOINTPANEL_ADD_EXPRESSION,
	ID_BREAKPOINTPANEL_ADD_RETURN,
	ID_BREAKPOINTPANEL_GRID,
	ID_BREAKPOINTPANEL_REMOVE,
	ID_CONNECTIONPAGE,
	ID_CONNECTIONPAGE_BREAK,
	ID_CONNECTIONPAGE_OPEN,
	ID_CONNECTIONPAGE_PANES,
	ID_CONNECTIONPAGE_RUN,
	ID_CONNECTIONPAGE_RUN_TO_CURSOR,
	ID_CONNECTIONPAGE_STEPINTO,
	ID_CONNECTIONPAGE_STEPOUT,
	ID_CONNECTIONPAGE_STEPOVER,
	ID_DEBUGPAGE,
	ID_MAINFRAME,
	ID_MAINFRAME_SERVER,
	ID_OUTPUTPANEL_SHOW_ALL,
	ID_OUTPUTPANEL_SHOW_STDERR,
	ID_OUTPUTPANEL_SHOW_STDOUT,
	ID_OUTPUTPANEL_SAVE,
	ID_PREFDIALOG_FONT,
	ID_PREFDIALOG_PORT,
	ID_PROPERTIESPANEL_TREE,
	ID_SOURCEPANEL,
	ID_SOURCEPANEL_RTC,
	ID_SOURCETEXTCTRL_RUN_TO_HERE,
	ID_SOURCETEXTCTRL_TOGGLE_BREAKPOINT,
	ID_STACKPANEL_LIST,
	ID_STACKPANEL_TOOLTIP,
	ID_DUBNIUM_HIGHEST
};

#endif

// vim:set fdm=marker ts=8 noet cin:
