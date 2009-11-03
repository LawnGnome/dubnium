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

#include "DBGp/MessageArguments.h"

using namespace DBGp;

MessageArguments::MessageArguments() {
}

MessageArguments::MessageArguments(unsigned int numberArgs, ...) {
	va_list argList;

	va_start(argList, numberArgs);
	for (unsigned int i = 0; i < numberArgs * 2; i += 2) {
		wxString name(va_arg(argList, const wxChar *));
		const wxChar *value = va_arg(argList, const wxChar *);

		if (value != NULL) {
			arguments[name] = wxString(value);
		}
		else {
			arguments[name] = wxEmptyString;
		}
	}
	va_end(argList);
}

MessageArguments::MessageArguments(const MessageArguments &ma) : arguments(ma.arguments) {
}

MessageArguments &MessageArguments::Append(const wxString &name) {
	return Append(name, wxEmptyString);
}

MessageArguments &MessageArguments::Append(const wxString &name, const wxString &value) {
	arguments[name] = value;
	return *this;
}

wxString MessageArguments::GetArguments() const {
	wxString argStr;

	for (ArgumentMap::const_iterator i = arguments.begin(); i != arguments.end(); i++) {
		if (i != arguments.begin()) {
			argStr << wxT(" ");
		}
		argStr << i->first << wxT(" ");
		if (i->second.Length() > 0) {
			argStr << wxT('"') << i->second << wxT('"');
		}
	}

	return argStr;
}

// vim:set fdm=marker ts=8 sw=8 noet cin:
