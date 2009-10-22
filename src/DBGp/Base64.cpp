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

#include "DBGp/Base64.h"
#include "DBGp/Utility.h"

#include <cstring>

namespace Base64 {
	char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	// {{{ char AlphabetPosition(char c) throw (DecoderError)
	char AlphabetPosition(char c) throw (DecoderError) {
		char *pos = std::strchr(alphabet, c);
		if (pos) {
			return pos - alphabet;
		}
		throw DecoderError(wxString(wxT("Unexpected character in input: ") + static_cast<wxChar>(c)));
	}
	// }}}
	// {{{ size_t DataLength(const wxString &base64)
	size_t DataLength(const wxString &base64) {
		size_t length = 3 * (base64.Length() / 4);

		if (base64.Right(2) == wxT("==")) {
			length -= 2;
		}
		else if (base64.Right(1) == wxT("=")) {
			--length;
		}

		return length;
	}
	// }}}
	// {{{ void Decode(const wxString &base64, char *data) throw (DecoderError, InsufficientBufferError)
	void Decode(const wxString &base64, char *data, size_t dataLength) throw (DecoderError, InsufficientBufferError) {
		char *c = data;
		size_t reqLen = DataLength(base64);

		wxASSERT(data != NULL);

		if (reqLen < dataLength) {
			throw InsufficientBufferError(wxT("Buffer too small (required ") + DBGp::IntToString(reqLen) + wxT(" bytes, got ") + DBGp::IntToString(dataLength) + wxT(" bytes."));
		}

		for (size_t i = 0; i < base64.Length(); i += 4) {
			wxUint32 buffer;
			wxString slice(base64.Mid(i, 4));

			buffer = AlphabetPosition(slice[0]) * 67108864 + AlphabetPosition(slice[1]) * 1048576;
			*c = static_cast<char>(buffer >> 24);
			++c;
			if (slice[2] != wxT('=')) {
				buffer += AlphabetPosition(slice[2]) * 16384;
				*c = static_cast<char>(buffer >> 16);
				++c;
				if (slice[3] != wxT('=')) {
					buffer += AlphabetPosition(slice[3]) * 256;
					*c = static_cast<char>(buffer >> 8);
					++c;
				}
			}
		}

		if (dataLength > reqLen) {
			*c = 0;
		}
	}
	// }}}
	// {{{ wxString Encode(const char *s, size_t length)
	wxString Encode(const char *s, size_t length) {
		const char *ptr = s;
		wxString enc;

		for (; ptr <= (s + length - 3); ptr += 3) {
			wxUint32 bufVal = ptr[0] * 16777216 + ptr[1] * 65536 + ptr[2] * 256;
			for (size_t i = 0; i < 4; ++i) {
				unsigned char c = bufVal >> 26;
				bufVal = bufVal << 6;
				enc += alphabet[c];
			}
		}

		if (ptr < (s + length)) {
			char rem = (s + length) - ptr;
			wxUint32 bufVal = ptr[0] * 16777216;
			if (rem == 2) {
				bufVal += ptr[1] * 65536;
			}
			for (char i = 0; i <= rem; i++) {
				unsigned char c = bufVal >> 26;
				bufVal = bufVal << 6;
				enc += alphabet[c];
			}
			enc << (rem == 1 ? wxT("==") : wxT("="));
		}

		return enc;
	}
	// }}}
}

// vim:set fdm=marker ts=8 noet cin:
