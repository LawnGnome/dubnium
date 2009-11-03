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

#include "images.h"

#include "break.png.data.c"
#include "breakpoint-exception.png.data.c"
#include "breakpoint-function.png.data.c"
#include "breakpoint-return.png.data.c"
#include "icon.png.data.c"
#include "output-all.png.data.c"
#include "output-stderr.png.data.c"
#include "output-stdout.png.data.c"
#include "panes.png.data.c"
#include "run-to-cursor.png.data.c"
#include "run.png.data.c"
#include "save.png.data.c"
#include "step-into.png.data.c"
#include "step-out.png.data.c"
#include "step-over.png.data.c"

static Images::Image knownImages[] = {
	{wxT("break"), break_png, sizeof(break_png)},
	{wxT("breakpoint-exception"), breakpoint_exception_png, sizeof(breakpoint_exception_png)},
	{wxT("breakpoint-function"), breakpoint_function_png, sizeof(breakpoint_function_png)},
	{wxT("breakpoint-return"), breakpoint_return_png, sizeof(breakpoint_return_png)},
	{wxT("icon"), icon_png, sizeof(icon_png)},
	{wxT("output-all"), output_all_png, sizeof(output_all_png)},
	{wxT("output-stderr"), output_stderr_png, sizeof(output_stderr_png)},
	{wxT("output-stdout"), output_stdout_png, sizeof(output_stdout_png)},
	{wxT("panes"), panes_png, sizeof(panes_png)},
	{wxT("run-to-cursor"), run_to_cursor_png, sizeof(run_to_cursor_png)},
	{wxT("run"), run_png, sizeof(run_png)},
	{wxT("save"), save_png, sizeof(save_png)},
	{wxT("step-into"), step_into_png, sizeof(step_into_png)},
	{wxT("step-out"), step_out_png, sizeof(step_out_png)},
	{wxT("step-over"), step_over_png, sizeof(step_over_png)},
	{NULL, NULL}
};

// {{{ const Images::Image *Images::GetImage(const wxString &id)
const Images::Image *Images::GetImage(const wxString &id) {
	for (Image *img = knownImages; img->id && img->image; img++) {
		if (id == img->id) {
			return img;
		}
	}
	return NULL;
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
