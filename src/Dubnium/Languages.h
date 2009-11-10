// {{{ Copyright notice
/* Copyright (c) 2007-2009, Adam Harvey
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

#ifndef LANGUAGES_H
#define LANGUAGES_H

/* The main aim of this header is to define mappings between the lexers in
 * wxStyledTextCtrl, their defined lexical states (token types, if you prefer),
 * and some standard internal types we can use for highlighting. Realistically,
 * we're reinventing the wheel here, and would be far better off just adapting
 * the lists that are part of most higher-level controls that wrap wxSTC, like
 * wxSTEdit, but for now, this is a quick and dirty way of getting the job
 * done. */

/* The lexer used by wxSTC when given a language of "PHP" is lexer number 30,
 * which isn't actually in the header file. Useful. And, by useful, I mean
 * irritating. */
#define wxSTC_LEX_PHP 30

/* WINNT.H apparently defines a TokenTypes type, so we'll wrap this in a
 * namespace. */
namespace Languages {
	// Standard internal types.
	typedef enum {
		COMMENT,
		DEFAULT,
		IDENTIFIER,
		KEYWORD,
		LITERAL,
		OPERATOR,
		END
	} TokenType;

	typedef struct {
		int lexer;
		int stcType;
		TokenType token;
	} TokenTypes;

	// Predefined mappings between languages/tokens and internal types.
	static const TokenTypes tokenTypes[] = {
		{wxSTC_LEX_PHP, wxSTC_HPHP_DEFAULT, DEFAULT},
		{wxSTC_LEX_PHP, wxSTC_HPHP_HSTRING, LITERAL},
		{wxSTC_LEX_PHP, wxSTC_HPHP_SIMPLESTRING, LITERAL},
		{wxSTC_LEX_PHP, wxSTC_HPHP_WORD, KEYWORD},
		{wxSTC_LEX_PHP, wxSTC_HPHP_NUMBER, LITERAL},
		{wxSTC_LEX_PHP, wxSTC_HPHP_VARIABLE, IDENTIFIER},
		{wxSTC_LEX_PHP, wxSTC_HPHP_COMMENT, COMMENT},
		{wxSTC_LEX_PHP, wxSTC_HPHP_COMMENTLINE, COMMENT},
		{wxSTC_LEX_PHP, wxSTC_HPHP_HSTRING_VARIABLE, IDENTIFIER},
		{wxSTC_LEX_PHP, wxSTC_HPHP_OPERATOR, OPERATOR},
		{-1, -1, END}
	};

	typedef struct {
		int lexer;
		unsigned bits : 4;
		const wxChar *keywords;
	} Lexers;

	/* Known lexers. The default number of style bits is generally 5, but
	 * some lexers (notably those based on the hypertext lexer, such as
	 * PHP) use 7. Full details are at
	 * http://www.yellowbrain.com/stc/styling.html, as part of the wxPython
	 * documentation for wxSTC (also known as the only useful wxSTC
	 * documentation in existence, as far as I can tell). */
	static const Lexers lexers[] = {
		{
			wxSTC_LEX_PHP,
			7,
			// PHP keywords, per
			// http://au2.php.net/manual/en/reserved.keywords.php
			wxT("abstract and array as break case catch ")
			wxT("cfunction class clone const continue declare ")
			wxT("default do else elseif enddeclare endfor ")
			wxT("endforeach endif endswitch endwhile extends ")
			wxT("final for foreach function global goto if ")
			wxT("implements interface instanceof namespace new ")
			wxT("old_function or private protected public static ")
			wxT("switch throw try use var while xor ")

			// PHP compile time constants.
			wxT("__CLASS__ __DIR__ __FILE__ __FUNCTION__ ")
			wxT("__METHOD__ __NAMESPACE__ ")

			// Language constructs.
			wxT("die echo empty exit eval include include_once ")
			wxT("isset list require require_once return print ")
			wxT("unset")
		},
		{-1, 0, NULL}
	};
}

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
