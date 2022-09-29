// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_OBJECT_H
#define RUDIMENTS_OBJECT_H

//#define PARADIGM 1

#include <rudiments/private/objectincludes.h>

/** The object class is the parent class for all rudiments classes. */
class object {
	public:

		/** Deletes this instance of the object class. */
		virtual	~object() {};

#ifdef PARADIGM
		/** Clears any current working data, but does not reset any
		 *  parameters.
		 *
		 *  This degenerate implentation does nothing and just returns
		 *  true, but may be overridden by a child class to do something
		 *  and return false if that fails, and possibly set an error.
		 *  */
		virtual bool	clear();

		/** Resets the object to the state that it was in when it was
		 *  constructed.
		 *
		 *  This implentation just calls clear() and construct() and
		 *  returns true unless clear() fails, but may be overridden by
		 *  a child class to do something else, or to do this more
		 *  efficiently, return false if something else fails, and
		 *  possibly set an error. */
		virtual bool	reset();

	protected:

		/** Initialize the internals of this instance to a default
		 *  state.  Should be called by the constructor.
		 *
		 *  This degenerate implementation does nothing, but may be
		 *  overridden by a child class to do something. */
		virtual	void	construct();
#endif

	#include <rudiments/private/object.h>
};

#endif
