// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_COMMANDLINE_H
#define RUDIMENTS_COMMANDLINE_H

#include <rudiments/private/commandlineincludes.h>

/** The commandline class provides methods for parsing command line
 *  arguments. */
class RUDIMENTS_DLLSPEC commandline {
	public:

		/** Creates an instance of the commandline class. */
		commandline();

		/** Creates an instance of the commandline class and sets its
		 *  argument count and arguments to "argc" and "argv". */
		commandline(int32_t argc, const char **argv);

		/** Deletes this instance of the commandline class. */
		~commandline();

		/** Sets the argument count and arguments to
		 *  "argc" and "argv". */
		void initialize(int32_t argc, const char **argv);

		/** Looks for either:
		 * 	-arg value
		 *  or
		 * 	--arg=value
		 *  on the command line and returns value.
		 *
		 *  Returns an empty string if just -arg or --arg is found, or
		 *  NULL if neither are found. */
		const char	*getValue(const char *arg) const;

		/** Looks for the argument or an abbreviated version of it on
		 *  the command line and returns the value.
		 *
		 *  For example, if "arg" is "name" and "abbr" is "n" then it
		 *  looks for either:
		 *
		 * 	-name value
		 *  or
		 * 	--name=value
		 *  or
		 *  	-n value
		 *  or
		 *  	--n==value
		 *
		 *  on the command line and returns value.
		 *
		 *  If both "arg" and "abbr" are found then the value for
		 *  "arg" is returned.
		 * 
		 *  Returns an empty string if just -name, --name, -n, or --n
		 *  is found, or NULL if none are found. */
		const char	*getValue(const char *arg,
						const char *abbr) const;

		/** Returns true if "-arg", "--arg" or "--arg=value" was found
		 *  on the command line and false if it was not found.
		 *
		 *  This is useful for processing command line switches. */
		bool	found(const char *arg) const;

		/** Returns true if the argument or an abbreviated version of
		 *  it was found on the command line and false if neither were
		 *  found.
		 *
		 *  For example, if "arg" is "recursive" and "abbr" is "r" then
		 *  it returns true if "-recursive", "--recursive", "-r", or
		 *  "--r" was found on the command line.
		 *
		 *  This is useful for processing command line switches. */
		bool	found(const char *arg, const char *abbr) const;

		/** Parses the command line into the supplied dictionary.
		 *
		 *  If an argument has no value, then the value for that
		 *  key in the dictionary is set to an empty string. */
		void	toDictionary(dictionary<char *,char *> *dict);

	#include <rudiments/private/commandline.h>

};

#endif
