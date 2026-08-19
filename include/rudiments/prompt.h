// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROMPT_H
#define RUDIMENTS_PROMPT_H

#include <rudiments/private/promptincludes.h>

/** The prompt class provides methods for displaying a prompt and accepting
 *  user input.
 *
 *  Input history and tab-completion are supported if the underlying system
 *  supports them.  On Unix-like systems, this is enabled by the libedit
 *  library.  On many Windows systems, this is provided by the OS. */
class RUDIMENTS_DLLSPEC prompt : virtual public object {
	public:

		/** Creates an instance of the prompt class. */
		prompt();

		/** Deletes this instance of the prompt class.
		 *
 		 *  Calls flushHistory(). */
		virtual	~prompt();

		/** Sets the name of the file used to store the input histroy.
		 *  On Unix-like systems, this is traditionally a dot-file in
		 *  the user's home directory.
		 *
		 *  Does nothing if rudiments is compiled without support for
		 *  libedit. */
		void	setHistoryFile(const char *filename);

		/** Returns the name of the file used to store the input
 		 *  history. */
		const char	*getHistoryFile();

		/** Sets the maximum number of lines of input history that will
 		 *  be stored in the input history file.  The history file will
 		 *  be truncated to this number of lines when flushHistory() is
 		 *  called, either explicitly or implicitly.
 		 *
 		 *  Defaults to 1024.
 		 *
 		 *  If set to 0 then the input history file will not be
 		 *  truncated during calls to flushHistory().
		 *
		 *  Does nothing if rudiments is compiled without support for
		 *  libedit. */
		void	setMaxHistoryLines(uint32_t lines);

		/** Returns the maximum number of lines to store in the input
 		 *  history file. */
		uint32_t	getMaxHistoryLines();

		/** Sets the maximum number of lines of input history that will
		 *  be queued in memory before flushHistory() is called
		 *  automatically.
 		 *
 		 *  Defaults to 1024.
 		 *
 		 *  If set to 0 then an unlimited number of lines will be
 		 *  queued, and will only be flushed when flushHistory() is
 		 *  called manually, or when the instance of the class is
 		 *  deleted.
		 *
		 *  Does nothing if rudiments is compiled without support for
		 *  libedit. */
		void	setMaxHistoryQueue(uint32_t queue);

		/** Returns the maximum number of lines of input history that
 		 *  will be queued in memory before flushHistory() will be
 		 *  called automatically. */
		uint32_t	getMaxHistoryQueue();

		/** Sets the prompt that will be displayed during the next
		 *  call to read(). */
		void	setPrompt(const char *prompt);

		/** Returns the prompt that will be displayed during the next
		 *  call to read(). */
		const char	*getPrompt();

		/** Sets the function that will be called to generate
		 *  tab-completion candidates during subsequent calls to
		 *  read(), and the argument that will be passed to it.
		 *
		 *  When the user hits the tab key, "tabhandler" is called
		 *  with:
		 *  * "text" - the word being completed
		 *  * "line" - the entire line of input so far
		 *  * "start" - the offset in "line" where "text" begins
		 *  * "end" - the offset in "line" of the cursor
		 *  * "arg" - the argument passed to setTabHandler()
		 *
		 *  "tabhandler" must return a NULL-terminated array of
		 *  candidate completions, or NULL if there are none.  Each
		 *  candidate must be the complete word that should replace
		 *  "text", not just the portion of the word after "text" -
		 *  the typed text is replaced, not appended to.  The array
		 *  and each string in it must be allocated with new[]
		 *  (charstring::duplicate() is convenient) and will be
		 *  deleted by the prompt class.
		 *
		 *  If the candidates share a common prefix, that prefix is
		 *  inserted into the line.  If there is exactly one
		 *  candidate, it is inserted in its entirety.  Otherwise the
		 *  candidates are displayed.
		 *
		 *  Only one prompt's tab handler can be active at a time.
		 *  The underlying library provides a single, process-wide
		 *  tab-completion hook, so if two instances of the prompt
		 *  class each have a tab handler, only the handler belonging
		 *  to the instance whose read() method is currently running
		 *  will be called.  Tab-completion is not thread-safe -
		 *  don't call read() on tab-handler-enabled instances from
		 *  more than one thread at a time.
		 *
		 *  Each time setTabHandler() is called, the tab handler
		 *  replaces the previously designated handler.  Passing NULL
		 *  disables tab-completion for this instance.
		 *
		 *  Does nothing if rudiments is compiled without support for
		 *  libedit. */
		void	setTabHandler(char **(*tabhandler)(const char *text,
						const char *line,
						int32_t start,
						int32_t end,
						void *arg),
					void *arg);

		/** Displays the prompt configured by setPrompt(), reads a
		 *  line from standard input, queues the line in memory, and
		 *  calls flushHistory() if the queue has grown beyond the
		 *  number of lines configured by getMaxHistoryQueue().
		 *
		 *  Returns the line that was read from standard input without
		 *  the trailing carriage return and/or line feed.
		 *
		 *  Returns an empty string if return/enter is hit by itself.
		 *
		 *  Returns NULL if ctrl-D was hit.
		 *
		 *  Note that return value is allocated internally and returned.
		 *  The calling program must free the buffer. */
		char	*read();

		/** Flushes all lines of input history queued in memory to the
		 *  file configured by setHistoryFile(). */
		void	flushHistory();

	#include <rudiments/private/prompt.h>
};

#endif
