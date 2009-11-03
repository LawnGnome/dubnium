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

#ifndef DBGP_BREAKPOINT_H
#define DBGP_BREAKPOINT_H

#include <wx/string.h>

#include "DBGp/Error/Error.h"
#include "DBGp/MessageArguments.h"

namespace DBGp {
	class Connection;

	/**
	 * Class representing a set breakpoint within the debugger. This class
	 * (mostly) transparently handles the various breakpoint_get,
	 * breakpoint_set and breakpoint_update calls required.
	 */
	class Breakpoint {
		public:
			/** Possible hit conditions. */
			typedef enum {
				/** Greater than or equal to the value. */
				HIT_GE,
				/** Equal to the value. */
				HIT_EQ,
				/** An exact multiple of the value. */
				HIT_MULT
			} HitCondition;

			/** Possible breakpoint types. */
			typedef enum {
				/** Break on a specific line. */
				LINE,
				/** Break on a function call. */
				CALL,
				/** Break on a function return. */
				RETURN,
				/** Break on an exception being thrown. */
				EXCEPTION,
				/** Break on a condition being met. */
				CONDITIONAL,
				/** Break on the variable given being modified. */
				WATCH
			} Type;

			/**
			 * Constructs a new Breakpoint object. Most code should
			 * use Connection::CreateBreakpoint() rather than
			 * directly instantiating Breakpoint objects.
			 *
			 * @param[in] conn The DBGp connection.
			 */
			Breakpoint(Connection *conn);

			/**
			 * Destroys the breakpoint and removes it from the
			 * debugging engine.
			 */
			~Breakpoint();

			/**
			 * Returns the exception to break on.
			 *
			 * @return The exception to break on. The value of this
			 * is undefined if the exception type isn't in use.
			 */
			inline wxString GetException() const { return exception; }

			/**
			 * Returns the expression to watch.
			 *
			 * @return The expression to watch. The value of this
			 * is undefined if the conditional or watch type is not
			 * in use.
			 */
			inline wxString GetExpression() const { return expression; }

			/**
			 * Returns the file the current breakpoint is set
			 * within.
			 *
			 * @return The file the breakpoint is set in. This
			 * value is undefined if the line type is not in use.
			 */
			inline wxString GetFileName() const { return fileName; }

			/**
			 * Returns the function to break on.
			 *
			 * @return The function name to break on. This value is
			 * undefined if the call or return types are not in
			 * use.
			 */
			inline wxString GetFunction() const { return function; }

			/**
			 * Returns the current hit condition.
			 *
			 * @return The current hit condition.
			 */
			inline HitCondition GetHitCondition() const { return hitCondition; }

			/**
			 * Returns the current hit count.
			 *
			 * @return The current hit count.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			int GetHitCount() throw (EngineError, SocketError);

			/**
			 * Returns the current hit value.
			 *
			 * @return The current hit value.
			 */
			inline int GetHitValue() const { return hitValue; }

			/**
			 * Returns the current server-assigned ID. 
			 *
			 * @return The current server-assigned breakpoint ID.
			 * If the Set method has not yet been called, then this
			 * will be an empty string.
			 */
			inline wxString GetID() const { return id; }

			/**
			 * Returns the line number to break on.
			 *
			 * @return The 1-indexed line number. This value is
			 * undefined if the call type is not in use.
			 */
			inline int GetLineNo() const { return lineNo; }

			/**
			 * Returns a flag indicating whether the breakpoint is
			 * temporary or not.
			 *
			 * @return True if the breakpoint is temporary.
			 */
			inline bool GetTemporary() const { return temporary; }

			/**
			 * Returns the breakpoint type.
			 *
			 * @return The breakpoint type.
			 */
			inline Type GetType() const { return type; }

			/**
			 * Returns a flag indicating if the breakpoint is
			 * enabled.
			 *
			 * @return True if the breakpoint is enabled.
			 */
			inline bool IsEnabled() const { return enabled; }

			/**
			 * Sets the breakpoint to break when a specific
			 * function is called.
			 *
			 * @param[in] function The function to break on.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void SetCallType(const wxString &function) throw (EngineError, SocketError);

			/**
			 * Sets the breakpoint to break when a condition is
			 * true.
			 *
			 * @param[in] expression The expression to test.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void SetConditionalType(const wxString &expression) throw (EngineError, SocketError);

			/**
			 * Sets the breakpoint to break when an exception of
			 * the given type is thrown.
			 *
			 * @param[in] exception The exception to match.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void SetExceptionType(const wxString &exception) throw (EngineError, SocketError);

			/**
			 * Sets the hit condition and value to match when
			 * breaking.
			 *
			 * @param[in] condition The condition to match.
			 * @param[in] value The value to match.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void SetHitCondition(HitCondition condition, int value) throw (EngineError, SocketError);

			/**
			 * Sets the breakpoint to break on a specific line of
			 * code.
			 *
			 * @param[in] fileName The file to break in.
			 * @param[in] lineNo The 1-indexed line to break on.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void SetLineType(const wxString &fileName, int lineNo) throw (EngineError, SocketError);

			/**
			 * Sets the breakpoint to break when returning from the
			 * given function.
			 *
			 * @param[in] function The function to break on when
			 * returning.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void SetReturnType(const wxString &function) throw (EngineError, SocketError);

			/**
			 * Sets the temporary status of the breakpoint. A
			 * temporary breakpoint will break once, then be
			 * deleted.
			 *
			 * @param[in] temporary True to make the breakpoint temporary.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 * @todo This needs more testing, since I'm not sure
			 * I've got the behaviour of other methods quite right
			 * on this yet.
			 */
			void SetTemporary(bool temporary) throw (EngineError, SocketError);

			/**
			 * Sets the breakpoint to break when the given variable
			 * is modified.
			 *
			 * @param[in] expression The variable to monitor.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void SetWatchType(const wxString &expression) throw (EngineError, SocketError);

			/**
			 * Disables the breakpoint.
			 *
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void Disable() throw (EngineError, SocketError);

			/**
			 * Enables the breakpoint.
			 *
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void Enable() throw (EngineError, SocketError);

			/**
			 * Compares the breakpoint to the given ID.
			 *
			 * @param[in] id The ID to compare the breakpoint to.
			 * @return True if the breakpoints match.
			 */
			inline bool operator==(const wxString &id) const { return id == this->id; }

			/**
			 * Retrieves the current status of the breakpoint from
			 * the debugging engine, even if it doesn't seem to be
			 * out-of-date.
			 *
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void Get() throw (EngineError, SocketError);

			/**
			 * Sends the current status of the breakpoint to the
			 * debugging engine. This needs to be called after the
			 * breakpoint type has been set.
			 *
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void Set() throw (EngineError, SocketError);

			/**
			 * Converts a hit condition to a string matching the
			 * DBGp specification.
			 *
			 * @param[in] cond The condition to convert.
			 * @return The DBGp compliant string.
			 */
			static wxString HitConditionToString(HitCondition cond);
			
			/**
			 * Converts a hit condition string to an internal hit
			 * condition.
			 *
			 * @param[in] s The DBGp compliant string to convert.
			 * @return The hit condition.
			 */
			static HitCondition StringToHitCondition(const wxString &s);

			/**
			 * Converts the given DBGp type string to an internal
			 * type.
			 *
			 * @param[in] s The DBGp compliant type string to
			 * convert.
			 * @return The type.
			 */
			static Type StringToType(const wxString &s);

			/**
			 * Converts the given type to a DBGp type string.
			 *
			 * @param[in] type The type to convert.
			 * @return The DBGp compliant type string.
			 */
			static wxString TypeToString(Type type);

		private:
			/** The DBGp connection. */
			Connection *conn;

			/**
			 * The flag indicating whether the breakpoint is
			 * enabled.
			 */
			bool enabled;

			/** The exception to match. */
			wxString exception;

			/** The expression to monitor. */
			wxString expression;

			/** The file name to match. */
			wxString fileName;

			/** The function to match. */
			wxString function;

			/** The hit condition to match. */
			HitCondition hitCondition;

			/** The current hit count. */
			int hitCount;

			/** The hit value to match. */
			int hitValue;

			/** The server-assigned breakpoint ID. */
			wxString id;

			/** Indicates whether Set has been called. */
			bool isSet;

			/** The line number to break on. */
			int lineNo;

			/**
			 * A flag indicating whether the breakpoint is
			 * temporary.
			 */
			bool temporary;

			/** The breakpoint type. */
			Type type;

	};
}

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
