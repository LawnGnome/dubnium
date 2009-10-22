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

#ifndef DBGP_LOCATION_H
#define DBGP_LOCATION_H

#include <wx/string.h>

namespace DBGp {
	/** A location within a source file. */
	class Location {
		public:
			/** The line number of the location. */
			unsigned int lineNo;
			/** The offset on the line of the current location. */
			unsigned int textOffset;

			/** Constructs a new location. */
			inline Location() : lineNo(0), textOffset(0) {}

			/**
			 * Constructs a new location.
			 *
			 * @param[in] lineNo The 1-indexed line number.
			 * @param[in] textOffset The offset within the line.
			 */
			inline Location(unsigned int lineNo, unsigned int textOffset) : lineNo(lineNo), textOffset(textOffset) {}
			
			/**
			 * Constructs an offset based on a DBGp string.
			 *
			 * @param[in] offset The offset string, in
			 * line:character format.
			 */
			Location(const wxString &offset);

			/**
			 * Location copy constructor.
			 *
			 * @param[in] location The location to copy.
			 */
			inline Location(const Location &location) : lineNo(location.lineNo), textOffset(location.textOffset) {}

			/**
			 * Equality operator.
			 *
			 * @param[in] location The location to compare to.
			 * @return True if the locations match.
			 */
			bool operator==(const Location &location);
	};
}

#endif

// vim:set fdm=marker ts=8 noet cin:
