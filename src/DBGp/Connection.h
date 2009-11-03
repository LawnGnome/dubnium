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

#ifndef DBGP_CONNECTION_H
#define DBGP_CONNECTION_H

#include <list>
#include <map>

#include <wx/event.h>
#include <wx/socket.h>
#include <wx/string.h>
#include <wx/xml/xml.h>

#include "DBGp/Base64.h"
#include "DBGp/Breakpoint.h"
#include "DBGp/Error/Error.h"
#include "DBGp/MessageArguments.h"
#include "DBGp/Property.h"
#include "DBGp/Stack.h"
#include "DBGp/Typemap.h"

namespace DBGp {
	/** Represents a unique transaction ID within the connection. */
	typedef unsigned long TransactionID;

	class Server;
	
	/**
	 * The class for an individual connection to a DBGp engine.
	 *
	 * @todo Implement spawnpoint support.
	 */
	class Connection : public wxEvtHandler {
		public:
			/**
			 * Allow Breakpoint instances to directly communicate
			 * with the debugging engine.
			 */
			friend class Breakpoint;

			/**
			 * Allow Context instances to directly communicate with
			 * the debugging engine.
			 */
			friend class Context;

			/**
			 * Allow Property instances to directly communicate
			 * with the debugging engine.
			 */
			friend class Property;

			/**
			 * Allow Stack instances to directly communicate with
			 * the debugging engine.
			 */
			friend class Stack;

			/**
			 * Allow StackLevel instances to directly communicate
			 * with the debugging engine.
			 */
			friend class StackLevel;

			/** Container for breakpoints within the connection. */
			typedef std::list<Breakpoint *> BreakpointList;

			/** The possible states of the DBGp engine. */
			typedef enum {
				/** Engine starting, yet to run. */
				STARTING,
				/** Run complete, engine stopping. */
				STOPPING,
				/** Detached, no further interaction possible. */
				STOPPED,
				/** Code is running. */
				RUNNING,
				/** Code execution has paused. */
				BREAK
			} EngineStatus;

			/**
			 * Constructs a new Connection object. Generally, this
			 * should only be called from
			 * Server::CreateConnectionObject() or a subclass
			 * constructor.
			 *
			 * @param[in] socket The client socket back to the
			 * debugging engine.
			 * @param[in] server The server that accepted this
			 * connection.
			 */
			Connection(wxSocketBase *socket, Server *server);

			/**
			 * Closes down the client socket and removes the
			 * connection from the active list in the server
			 * object.
			 */
			virtual ~Connection();

			void Close();
			bool CommandSupported(const wxString &command);
			inline void Destroy() { socket->Destroy(); }

			/**
			 * Sets the event handler.
			 *
			 * @param[in] handler The new event handler, or NULL to
			 * disable event generation.
			 */
			void SetEventHandler(wxEvtHandler *handler);

			/**
			 * Tells the debugging engine to immediately break.
			 *
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 * @throws UnsupportedFeatureError Thrown if the break 
			 * command isn't supported.
			 */
			void Break() throw (SocketError, UnsupportedFeatureError);

			/**
			 * Tells the debugging engine to detach the debugger.
			 *
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 * @throws UnsupportedFeatureError Thrown if the detach
			 * command isn't supported.
			 */
			void Detach() throw (SocketError, UnsupportedFeatureError);

			/**
			 * Retrieves the current value of a feature from the
			 * debugging engine via the feature_get command.
			 *
			 * @param[in] name The feature to retrieve.
			 * @return The current value of the feature.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 * @throws UnsupportedFeatureError Thrown if the
			 * feature comes back from the debugging engine as
			 * unsupported.
			 */
			wxString FeatureGet(const wxString &name) throw (EngineError, MalformedDocumentError, SocketError, UnsupportedFeatureError);

			/**
			 * Sets the value of a feature via the feature_set
			 * command.
			 *
			 * @param[in] name The name of the feature to set.
			 * @param[in] value The value to set the feature to.
			 * @return Whether the feature was set.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			bool FeatureSet(const wxString &name, const wxString &value) throw (EngineError, MalformedDocumentError, SocketError);

			/**
			 * Tells the debugging engine to run the script until a
			 * breakpoint is reached.
			 *
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void Run() throw (SocketError);

			/**
			 * Retrieves part of a source file from the debugging
			 * engine.
			 *
			 * @param[in] fileUri The file to retrieve. If omitted,
			 * the source file for the current context will be
			 * returned.
			 * @param[in] beginLine The 1-indexed line to start at.
			 * @param[in] endLine The 1-indexed line to end at.
			 * @return The file data.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws NotFoundError Thrown if the file cannot be
			 * found.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			wxString Source(const wxString &fileUri = wxEmptyString, int beginLine = -1, int endLine = -1) throw (EngineError, NotFoundError, SocketError);

			/**
			 * Retrieves the current status of the debugging
			 * engine.
			 *
			 * @param[out] reason The reason why the engine is in
			 * the state it is.
			 * @return The current status of the engine.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			EngineStatus Status(wxString *reason = NULL) throw (EngineError, MalformedDocumentError, SocketError);

			/**
			 * Tells the debugging engine to step into the next
			 * statement.
			 *
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void StepInto() throw (SocketError);

			/**
			 * Tells the debugging engine to step out of the
			 * current frame.
			 *
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void StepOut() throw (SocketError);

			/**
			 * Tells the debugging engine to step over the next
			 * statement.
			 *
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void StepOver() throw (SocketError);

			/**
			 * Tells the debugging engine to stop execution.
			 *
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void Stop() throw (SocketError);

			/**
			 * Retrieves the typemap from the debugging engine.
			 *
			 * @return The debugging engine's typemap.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			Typemap &TypemapGet() throw (EngineError, MalformedDocumentError, SocketError);

			/**
			 * Creates a new breakpoint object. This breakpoint
			 * will not be active until Breakpoint::Set is called.
			 * The returned Breakpoint object will be owned and
			 * deleted by the Connection object.
			 *
			 * @return The new breakpoint object.
			 */
			Breakpoint *CreateBreakpoint();

			/**
			 * Retrieves a breakpoint with the given ID. Note that
			 * unset breakpoints do not have an ID.
			 * 
			 * @param[in] id The ID to search for.
			 * @return The breakpoint if found, or NULL.
			 */
			Breakpoint *GetBreakpoint(const wxString &id);

			/**
			 * Returns the entire list of breakpoints.
			 *
			 * @return The list of breakpoints.
			 */
			const BreakpointList &GetBreakpoints() const;

			/**
			 * Removes a breakpoint. The breakpoint will be deleted
			 * within this function, so it should not be referenced
			 * after calling this function.
			 *
			 * @param[in] breakpoint The breakpoint to remove.
			 */
			void RemoveBreakpoint(Breakpoint *breakpoint);

			/**
			 * Retrieves the current call stack.
			 *
			 * @return The current call stack.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			Stack StackGet() throw (EngineError, MalformedDocumentError, SocketError);

			/**
			 * Converts an EngineStatus into the DBGp standard
			 * string.
			 *
			 * @param[in] status The status to convert.
			 * @return The string as defined by the standard.
			 */
			static wxString EngineStatusToString(EngineStatus status);

			/**
			 * Converts a DBGp status string to an EngineStatus.
			 *
			 * @param[in] s The status string.
			 * @return The status.
			 * @throws NotFoundError Thrown if the status string is
			 * unknown.
			 */
			static EngineStatus StringToEngineStatus(const wxString &s) throw (NotFoundError);

		protected:
			/** Breakpoints defined within the connection. */
			BreakpointList breakpoints;

			/** The conversion object for the encoding in use. */
			wxMBConv *conv;

			/** The event handler to call. */
			wxEvtHandler *handler;

			/**
			 * Mutex to prevent multiple commands being sent up
			 * simultaneously. We don't <em>absolutely</em> need
			 * this, given the transaction ID support in DBGp, but
			 * it simplifies things greatly, since we're not that
			 * interested in asynchronous requests being sent up to
			 * the debugging engine.
			 */
			wxMutex pendingMutex;

			/**
			 * A pointer back to the server that spawned this
			 * connection.
			 */
			Server *server;

			/** The TCP socket to the debugging engine. */
			wxSocketBase *socket;

			/** The current engine status. */
			EngineStatus status;

			/** Support status of extended commands. */
			std::map<wxString, bool> supported;

			/** The current transaction ID. */
			TransactionID txID;

			/** The debugging engine's typemap. */
			Typemap typemap;

			/**
			 * The mutex preventing duplicate transaction IDs when
			 * multi-threading.
			 */
			wxMutex txMutex;

			/**
			 * Configures the debugging engine to copy data
			 * destined for stdout and stderr to us.
			 */
			void CopyOutput() throw ();

			/**
			 * Retrieves the next DBGp message waiting on the
			 * socket. This will block if no message if waiting.
			 *
			 * @return The XML document created from the message.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 * @throws SocketDestroyedError Thrown if the socket
			 * has already been destroyed.
			 * @todo Create a new exception type and throw it if
			 * the document is malformed.
			 */
			virtual wxXmlDocument GetMessage() throw (MalformedDocumentError, SocketError, SocketDestroyedError);

			/**
			 * Creates a new transaction ID.
			 *
			 * @return The new transaction ID.
			 */
			TransactionID GetTransactionID();

			/**
			 * Decodes encoded data within a message. Base64 errors
			 * will be logged but otherwise ignored.
			 *
			 * @param[in,out] node The response element in the XML
			 * document.
			 */
			void HandleEncodedMessage(wxXmlNode *node);

			/**
			 * Examines a message returned from the debugging
			 * engine and performs any actions that are
			 * appropriate. In the event of non-error response
			 * messages, this function will do nothing and leave
			 * processing to the caller.
			 *
			 * @param[in] doc The message to examine.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 */
			void HandleMessage(wxXmlDocument &doc) throw (EngineError, MalformedDocumentError, SocketError);

			/**
			 * Handles a response element.
			 *
			 * @param[in] resp The response element in the XML
			 * document.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 */
			void HandleResponse(wxXmlNode *resp) throw (EngineError);

			/**
			 * Handles a response error. This function will never
			 * return.
			 *
			 * @param[in] error The error node.
			 * @throws EngineError Thrown with the error
			 * information.
			 */
			void HandleResponseError(wxXmlNode *error) throw (EngineError);

			/**
			 * Negotiates the features that we want with the
			 * debugging engine.
			 *
			 * @todo Actually retrieve the encoding if we can't set
			 * it to UTF-8 and see if we have a conversion class in
			 * wxWidgets for it.
			 */
			void NegotiateFeatures() throw ();

			/**
			 * Event handler when there is a socket event: either
			 * input or a lost connection. It's important that this
			 * function does not throw an exception.
			 *
			 * @param[in] event The socket event.
			 */
			void OnSocket(wxSocketEvent &event) throw ();

			/**
			 * Low-level function to send a command.
			 * 
			 * @param[in] command The command to execute.
			 * @param[in] args The arguments to the command.
			 * @param[in] data The data to be encoded and sent up
			 * with the command, if any.
			 * @param[in] dataLength The length of the data to be
			 * sent. Ignored if data is NULL.
			 * @return The transaction ID associated with the
			 * command.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 * @throws SocketDestroyedError Thrown if the socket
			 * has already been destroyed.
			 * @todo Test non-Unicode builds; they <em>should</em>
			 * work in theory, but hey, it's well after midnight.
			 */
			virtual TransactionID SendCommand(const wxString &command, MessageArguments args, const char *data, size_t dataLength) throw (SocketError, SocketDestroyedError);

			/**
			 * Sends a command to the debugging engine and does not
			 * wait for a response. Any response will be picked up
			 * by OnInput and ignored.
			 *
			 * @param[in] command The command to execute.
			 * @param[in] args The arguments to the command.
			 * @param[in] data The data to be encoded and sent up
			 * with the command, if any.
			 * @param[in] dataLength The length of the data to be
			 * sent. Ignored if data is NULL.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 * @throws SocketDestroyedError Thrown if the socket
			 * has already been destroyed.
			 */
			virtual void SendCommandImmediate(const wxString &command, MessageArguments args, const char *data = NULL, size_t dataLength = 0) throw (SocketError, SocketDestroyedError);

			/**
			 * Sends a command to the debugging engine and waits
			 * for a response.
			 *
			 * @note Section 7.5 of the DBGp specification notes
			 * that certain commands may not return a response
			 * immediately. In that case, this function will block
			 * until it receives a response. Some callbacks may be
			 * called while waiting if stream messages are
			 * received, though.
			 *
			 * @param[in] command The command to execute.
			 * @param[in] args The arguments to the command.
			 * @param[in] data The data to be encoded and sent up
			 * with the command, if any.
			 * @param[in] dataLength The length of the data to be
			 * sent. Ignored if data is NULL.
			 * @return The XML document sent back by the server.
			 * @throws EngineError Thrown if the debugging engine
			 * returns an error.
			 * @throws SocketError Thrown if a communications error
			 * occurs.
			 * @throws SocketDestroyedError Thrown if the socket
			 * has already been destroyed.
			 * @todo Consider inserting a timeout, or at least some
			 * way of getting back to the wxWidgets main loop.
			 */
			virtual wxXmlDocument SendCommandWait(const wxString &command, MessageArguments args, const char *data = NULL, size_t dataLength = 0) throw (EngineError, MalformedDocumentError, SocketError, SocketDestroyedError);

			/**
			 * Tests if a command is supported.
			 *
			 * @param[in] command The command to test for.
			 */
			void TestCommand(const wxString &command) throw ();

			//DECLARE_EVENT_TABLE()
	};
}

#endif

// vim:set fdm=marker ts=8 sw=8 noet cin:
