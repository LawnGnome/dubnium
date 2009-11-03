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

#include "DBGp/Type.h"

using namespace DBGp;

// {{{ Type::Type()
Type::Type() : commonType(UNDEFINED), name(wxT("?")), xsiType(wxEmptyString) {
}
// }}}
// {{{ Type::Type(CommonType commonType, const wxString &name, const wxString &xsiType)
Type::Type(CommonType commonType, const wxString &name, const wxString &xsiType) : commonType(commonType), name(name), xsiType(xsiType) {
}
// }}}
// {{{ Type::Type(const Type &type)
Type::Type(const Type &type) : commonType(type.commonType), name(type.name), xsiType(type.xsiType) {
}
// }}}

// {{{ wxString Type::CommonTypeToString(CommonType commonType)
wxString Type::CommonTypeToString(CommonType commonType) {
	switch (commonType) {
		case BOOL:
			return wxT("bool");
		case INT:
			return wxT("int");
		case FLOAT:
			return wxT("float");
		case STRING:
			return wxT("string");
		case NULLTYPE:
			return wxT("null");
		case ARRAY:
			return wxT("array");
		case HASH:
			return wxT("hash");
		case OBJECT:
			return wxT("object");
		case RESOURCE:
			return wxT("resource");
		default:
			return wxT("undefined");
	}
}
// }}}
// {{{ Type::CommonType Type::StringToCommonType(const wxString &s)
Type::CommonType Type::StringToCommonType(const wxString &s) {
	if (s == wxT("bool")) {
		return BOOL;
	}
	if (s == wxT("int")) {
		return INT;
	}
	if (s == wxT("float")) {
		return FLOAT;
	}
	if (s == wxT("string")) {
		return STRING;
	}
	if (s == wxT("null")) {
		return NULLTYPE;
	}
	if (s == wxT("array")) {
		return ARRAY;
	}
	if (s == wxT("hash")) {
		return HASH;
	}
	if (s == wxT("object")) {
		return OBJECT;
	}
	if (s == wxT("resource")) {
		return RESOURCE;
	}
	return UNDEFINED;
}
// }}}

// vim:set fdm=marker ts=8 sw=8 noet cin:
