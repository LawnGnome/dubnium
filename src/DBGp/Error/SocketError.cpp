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

#include "DBGp/Error/Error.h"

using namespace DBGp;

SocketError::SocketError(wxSocketError error) : Error(wxEmptyString) {
	switch (error) {
		case wxSOCKET_NOERROR:
			message = wxT("No error happened.");
			break;

		case wxSOCKET_INVOP:
			message = wxT("Invalid operation.");
			break;

		case wxSOCKET_IOERR:
			message = wxT("Input/Output error.");
			break;

		case wxSOCKET_INVADDR:
			message = wxT("Invalid address passed to wxSocket.");
			break;

		case wxSOCKET_INVSOCK:
			message = wxT("Invalid socket (uninitialised).");
			break;

		case wxSOCKET_NOHOST:
			message = wxT("No corresponding host.");
			break;

		case wxSOCKET_INVPORT:
			message = wxT("Invalid port.");
			break;

		case wxSOCKET_WOULDBLOCK:
			message = wxT("The socket is non-blocking and the operation would block.");
			break;

		case wxSOCKET_TIMEDOUT:
			message = wxT("The timeout for this operation expired.");
			break;

		case wxSOCKET_MEMERR:
			message = wxT("Memory exhausted.");
			break;

		default:
			message = wxT("Unknown error.");
	}
}

// vim:set fdm=marker ts=8 noet cin:
