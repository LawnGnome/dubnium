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

#ifndef DBGP_BASE64_H
#define DBGP_BASE64_H

#include <wx/string.h>

#include "DBGp/Error/Error.h"

/**
 * General Base64 handling functions. These are clean-room implementations,
 * which means that bugs have doubtless been resurrected that other
 * implementations have long since dealt with.
 */
namespace Base64 {
	/** Exception class for Base64 decoding errors. */
	class DecoderError : public DBGp::Error {
		public:
			/**
			 * Constructs a new Base64::DecoderError object.
			 * 
			 * @param[in] message The detailed error message.
			 */
			inline DecoderError(const wxString &message) : Error(message) {}
	};

	/** Exception class thrown when insufficient buffer space is allocated. */
	class InsufficientBufferError : public DBGp::Error {
		public:
			/**
			 * Constructs a new Base64::InsufficientBufferError
			 * object.
			 * 
			 * @param[in] message The detailed error message.
			 */
			inline InsufficientBufferError(const wxString &message) : Error(message) {}
	};

	/**
	 * Calculates the number of bytes encoded within a Base64 string.
	 *
	 * @param[in] base64 The Base64 encoded string.
	 * @return The length of the encoded data.
	 */
	size_t DataLength(const wxString &base64);

	/**
	 * Decodes a Base64 encoded string.
	 *
	 * @param[in] base64 The Base64 encoded string.
	 * @param[in,out] data The buffer to write the decoded data to. The
	 * required length can be calculated by DataLength. The buffer will be
	 * NULL-terminated if large enough.
	 * @param[in] dataLength The allocated size of the data parameter.
	 * @throws DecoderError Thrown if the Base64 string included an unknown
	 * character.
	 * @throws InsufficientBufferError Thrown if the buffer passed in is
	 * not large enough.
	 */
	void Decode(const wxString &base64, char *data, size_t dataLength) throw (DecoderError, InsufficientBufferError);

	/**
	 * Encodes a binary string to Base64.
	 *
	 * @param[in] s The binary string to encode.
	 * @param[in] length The length of the binary string.
	 * @return The Base64 encoded string.
	 */
	wxString Encode(const char *s, size_t length);
}

#endif

// vim:set fdm=marker ts=8 noet cin:
