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

#ifndef DBGP_MESSAGEARGUMENTS_H
#define DBGP_MESSAGEARGUMENTS_H

#include <cstdarg>
#include <map>

#include <wx/string.h>

namespace DBGp {
	/**
	 * A class to encapsulate the arguments that are sent up as part of a
	 * DBGp command.
	 */
	class MessageArguments {
		public:
			/** Constructs an empty argument list. */
			MessageArguments();

			/**
			 * Constructs an argument list with a variable number
			 * of arguments. This function will expect numberArgs *
			 * 2 arguments of type <code>wxChar *</code>, which
			 * represent the key/value pairs for arguments. The
			 * value can be NULL if only the key should be sent.
			 *
			 * Example with one argument with both a key and value,
			 * which effectively results in <code>-n name</code>:
			 *
			 * <code>
			 * MessageArguments args(1, wxT("-n"), wxT("name"));
			 * </code>
			 *
			 * Example with two arguments; the first having no
			 * associated value, the second having on, thereby
			 * resulting in <code>-o -n name</code>:
			 *
			 * <code>
			 * MessageArguments args(2, wxT("-o"), NULL, wxT("-n"), wxT("name"));
			 * </code>
			 *
			 * @param[in] numberArgs The number of arguments to
			 * expect.
			 */
			MessageArguments(unsigned int numberArgs, ...);

			/**
			 * Copy constructor.
			 *
			 * @param[in] ma The MessageArguments instance to copy
			 * from.
			 */
			MessageArguments(const MessageArguments &ma);

			/**
			 * Appends a name-only argument to the list.
			 *
			 * @param[in] name The name to append.
			 * @return A reference to the MessageArguments
			 * instance. This allows chaining of Append calls.
			 */
			MessageArguments &Append(const wxString &name);

			/**
			 * Appends a name/value argument to the list.
			 * 
			 * @param[in] name The name to append.
			 * @param[in] value The value to associate with the
			 * name.
			 * @return A reference to the MessageArguments
			 * instance. This allows chaining of Append calls.
			 */
			MessageArguments &Append(const wxString &name, const wxString &value);

			/**
			 * Returns the arguments in a string form that can be
			 * sent to the debugging engine.
			 *
			 * @return The arguments as a command substring.
			 */
			wxString GetArguments() const;

		private:
			/**
			 * Typedef for the internal map that stores the
			 * arguments.
			 */
			typedef std::map<wxString, wxString> ArgumentMap;

			/** The data store for the arguments. */
			ArgumentMap arguments;
	};
}

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
