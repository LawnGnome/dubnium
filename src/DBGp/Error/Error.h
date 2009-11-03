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

#ifndef DBGP_ERROR_ERROR_H
#define DBGP_ERROR_ERROR_H

#include <wx/socket.h>
#include <wx/string.h>

namespace DBGp {
	/** The base class for all exceptions thrown within DBGp. */
	class Error {
		public:
			/**
			 * Constructs a new error object.
			 *
			 * @param[in] message The error message.
			 */
			Error(const wxString &message);

			/**
			 * Returns the error message.
			 * 
			 * @return The error message.
			 */
			wxString GetMessage() const;

		protected:
			/** The error message. */
			wxString message;
	};

	/** Represents errors returned from the debugging engine. */
	class EngineError : public Error {
		public:
			/**
			 * Constructs a new error based on the fields returned
			 * via DBGp error messages.
			 *
			 * @param[in] code The error code. This should be a
			 * three-digit number, based on the spec.
			 * @param[in] appErr Any application-specific error
			 * information.
			 * @param[in] message The "UI-usable" error message.
			 */
			EngineError(wxUint16 code, const wxString &appErr, const wxString &message);

			/**
			 * Returns the error code.
			 * 
			 * @return The numeric error code.
			 */
			wxUint16 GetCode() const;

			/**
			 * Returns the application-specific error information,
			 * if any.
			 * 
			 * @return The error information.
			 */
			wxString GetAppErr() const;

		protected:
			/** The error code. */
			wxUint16 code;

			/** The application-specific error information. */
			wxString appErr;
	};

	/** Represents malformed XML document errors. */
	class MalformedDocumentError : public Error {
		public:
			/**
			 * Constructs a new malformed document error.
			 *
			 * @param[in] message A message describing the context
			 * within which the error occurred.
			 */
			inline MalformedDocumentError(const wxString &message) : Error(message) {}
	};

	/** Represents file/property-not-found errors. */
	class NotFoundError : public Error {
		public:
			/**
			 * Constructs a new file not found error.
			 * 
			 * @param[in] message A message describing what
			 * couldn't be found.
			 */
			inline NotFoundError(const wxString &message) : Error(message) {}
	};

	/** Represents errors when getting or setting properties. */
	class PropertyError : public Error {
		public:
			/**
			 * Constructs a new property error object.
			 * 
			 * @param[in] message The error message.
			 */
			inline PropertyError(const wxString &message) : Error(message) {}
	};

	/** Represents errors in the socket communications. */
	class SocketError : public Error {
		public:
			/**
			 * Constructs a new socket error object.
			 * 
			 * @param[in] message The error message.
			 */
			inline SocketError(const wxString &message) : Error(message) {}

			/**
			 * Constructs a new socket error object with a message
			 * corresponding to a wxSocketError value.
			 * 
			 * @param[in] error The socket error.
			 */
			SocketError(wxSocketError error);
	};

	/**
	 * Represents the error occurring when an operation is attempted on a
	 * previously destroyed socket.
	 */
	class SocketDestroyedError : public SocketError {
		public:
			inline SocketDestroyedError() : SocketError(wxT("Socket already destroyed.")) {}
	};

	/**
	 * Represents errors caused by attempting to retrieve unsupported
	 * features.
	 */
	class UnsupportedFeatureError : public Error {
		public:
			/**
			 * Constructs a new unsupported feature error object.
			 * 
			 * @param[in] message The error message.
			 */
			inline UnsupportedFeatureError(const wxString &message) : Error(message) {}
	};
}

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
