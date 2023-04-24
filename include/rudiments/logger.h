// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LOGGER_H
#define RUDIMENTS_LOGGER_H

#include <rudiments/private/loggerincludes.h>

/** The logdestination class is a virtual base class that just defines methods
 *  that the real logdestination classes must implement. */
class RUDIMENTS_DLLSPEC logdestination : virtual public object {
	#include <rudiments/private/logdestination.h>
};

/** The syslogdestination class writes log data to syslog. */
class RUDIMENTS_DLLSPEC syslogdestination : public logdestination {
	public:

		/** Creates an instance of the syslogdestination class. */
		syslogdestination();

		/** Deletes this instance of the syslogdestination class. */
		virtual		~syslogdestination();

		/** Opens the syslog as a logging destination using "option",
		 *  "facility" and "priority".  The string "ident" will be
		 *  prepended to each message. */
		void	open(const char *ident, int32_t option,
					int32_t facility, int32_t priority);

		/** Closes the syslog as a loggin destination. */
		void	close();

	#include <rudiments/private/syslogdestination.h>
};


/** The filedestination class writes log data to a file. */
class RUDIMENTS_DLLSPEC filedestination : public logdestination {
	public:

		/** Creates an instance of the filedestination class. */
		filedestination();

		/** Deletes this instance of the filedestination class. */
		virtual		~filedestination();

		/** Opens file "filename" as a logging destination with
		 *  "perms" permissions. */
		bool	open(const char *filename, mode_t perms);

		/** Closes the file as a logging destination. */
		void	close();

	#include <rudiments/private/filedestination.h>
};


/** The stdoutdestination class writes log data to standard output. */
class RUDIMENTS_DLLSPEC stdoutdestination : public logdestination {
	#include <rudiments/private/stdoutdestination.h>
};


/** The stderrdestination class writes log data to standard error. */
class RUDIMENTS_DLLSPEC stderrdestination : public logdestination {
	#include <rudiments/private/stderrdestination.h>
};


typedef listnode<logdestination *>	loggerlistnode;
typedef linkedlist<logdestination *>	loggerlist;

/** The logger class and associated logdestination classes provide methods for
 *  logging data from an application.  The following destinations are supported:
 *  	file, syslog, stdout and stderr
 * 
 *  To use these classes: instantiate one or more logdestinations,
 *  instantiate a logger, and add the logdestination instance(s) to the
 *  logger class instance using addLogDestination().  Then, each call to a
 *  logger class method will output logging information to each of the
 *  logdestinations in seqence. */
class RUDIMENTS_DLLSPEC logger : public object {
	public:

		/** Creates an instance of the logger class. */
		logger();

		/** Deletes this instance of the logger class. */
		~logger();

		/** Add "logdest" to the list of destinations that
		 *  log entries will be written to. */
		void	addLogDestination(logdestination *logdest);

		/** Remove "logdest" from the list of destinations that
		 *  log entries will be written to. */
		void	removeLogDestination(logdestination *logdest);

		/** Remove all entries from the list of destinations that
		 *  log entries will be written to. */
		void	removeAllLogDestinations();


		/** Sets the character to be used when indenting to "ch". */
		void	setIndent(char ch);

		/** Returns the character currently used when indenting. */
		char	getIndent();

		/** Sets the wide character to be used when indenting
		 *  to "ch". */
		void	setWideIndent(wchar_t ch);

		/** Returns the wide character currently used when indenting. */
		char	getWideIndent();


		/** Sets the logging level to "level". */
		void	setLogLevel(uint8_t level);

		/** Returns the logging level. */
		uint8_t	getLogLevel();


		/** Returns a string containing the current date,
		 *  followed by "name", followed by the process id in
		 *  brackets.  For example:
		 * 
		 *  01/05/2002 12:54:23 myprogram [2421]
		 * 
		 *  Note that this method allocates a string internally
		 *  and returns it.  This string must be deleted by the
		 *  calling program.
		 * 
		 *  Note that this method uses getSystemDateAndTime()
		 *  from the datetime class.  If you are using
		 *  getLogHeader() in a multi-threaded application, you
		 *  must supply a mutex to the datetime class using it's
		 *  setMutex() method. */
		static char	*getLogHeader(const char *name);

		/** Returns a string containing the current date,
		 *  followed by "name", followed by the process id in
		 *  brackets.  For example:
		 * 
		 *  01/05/2002 12:54:23 myprogram [2421]
		 * 
		 *  Note that this method allocates a string internally
		 *  and returns it.  This string must be deleted by the
		 *  calling program.
		 * 
		 *  Note that this method uses getSystemDateAndTime()
		 *  from the datetime class.  If you are using
		 *  getLogHeader() in a multi-threaded application, you
		 *  must supply a mutex to the datetime class using it's
		 *  setMutex() method. */
		static wchar_t	*getLogHeader(const wchar_t *name);


		/** If the current logging level is equal to or greater than
		 * "level", then writes "header", followed by "indent"
		 * indentations, followed by "string" followed by " {" as a
		 * single log entry. */
		void	start(uint8_t level,
				const char *header,
				uint32_t indent,
				const char *string);

		/** If the current logging level is equal to or greater than
		 * "level", then writes "header", followed by "indent"
		 * indentations, followed by "string" followed by " {" as a
		 * single log entry. */
		void	start(uint8_t level,
				const wchar_t *header,
				uint32_t indent,
				const wchar_t *string);

		/** If the current logging level is equal to or greater than
		 *  "level", then writes "header", followed by "indent"
		 *  indentations, followed by "..." formatted using "format" as
		 *  a single log entry. */
		void	write(uint8_t level,
				const char *header,
				uint32_t indent,
				const char *format, ...);

		/** If the current logging level is equal to or greater than
		 *  "level", then writes "header", followed by "indent"
		 *  indentations, followed by "..." formatted using "format" as
		 *  a single log entry. */
		void	write(uint8_t level,
				const wchar_t *header,
				uint32_t indent,
				const wchar_t *format, ...);

		/** If the current logging level is equal to or greater than
		 *  "level", then writes "header", followed by "indent"
		 *  indentations, followed by "va_list" formatted using "format"
		 *  as a single log entry. */
		void	write(uint8_t level,
				const char *header,
				uint32_t indent,
				const char *format,
				va_list *argp);

		/** If the current logging level is equal to or greater than
		 *  "level", then writes "header", followed by "indent"
		 *  indentations, followed by "va_list" formatted using "format"
		 *  as a single log entry. */
		void	write(uint8_t level,
				const wchar_t *header,
				uint32_t indent,
				const wchar_t *format,
				va_list *argp);

		/** If the current logging level is equal to or greater than
 		 *  "level", then writes "header", followed by "indent"
 		 *  indentations, followed by "}" as a single log entry. */
		void	end(uint8_t level,
				const char *header,
				uint32_t indent);

		/** If the current logging level is equal to or greater than
 		 *  "level", then writes "header", followed by "indent"
 		 *  indentations, followed by "}" as a single log entry. */
		void	end(uint8_t level,
				const wchar_t *header,
				uint32_t indent);

	#include <rudiments/private/logger.h>
};

#endif
