// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_WASTEBASKET_H
#define RUDIMENTS_WASTEBASKET_H

#include <rudiments/private/wastebasketincludes.h>

/** The wastebasket class provides an garbage collection object.  Arbitrary
 *  arrays of primitives, objects, and arrays of objects can be attached to it
 *  and deleted at a later date.
 *
 *  Useful for holding on to dependencies of an object until the object no
 *  longer needs them.
 *
 *  For example:
 *
 *  int main() {
 *  	wastebasket w;
 *  	getInformation(&w).display();
 *  	w.empty();
 *  }
 *
 *  information	*getInformation(wastebasket *w) {
 *
 *  	// Assume that information, dbconnection and dbcursor all
 *	// inherit from the rudiments object class.
 *	// 
 *	// Assume also, that the dbresultset class inherits from the
 *	// information class and implements its display() method.
 *	
 *	dbconnection	*con=createDatabaseConnection();
 *	dbcursor	*cur=con->createDatabaseCursor();
 *	dbresultset	*rs=cur->runQuery("...some query...");
 *	w->attach(rs);
 *	w->attach(cur);
 *	w->attach(con);
 *	return rs;
 *  }
 *
 *  In this example, getInformation() creates a connection to a database,
 *  creates a cursor for running a query, and runs a query using that cursor
 *  to create a result set.  The result set is then passed back to main() which
 *  displays it.
 *
 *  In this code, the rs object requires that the cur and con objects still
 *  exist in order to execute its display() method.
 *
 *  While all three objects could be passed back to main(), or created in
 *  main() and passed down, using a wastebasket allows getInformation() to
 *  completely hide its implementation details from main().
 *
 *  This is especially useful in MVC applications.
 */
class RUDIMENTS_DLLSPEC wastebasket : virtual public object {
	public:

		/** Creates an instance of the wastebasket class. */
		wastebasket();

		/** Deletes this instance of the wastebasket class. */
		virtual	~wastebasket();

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(char *value);

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(wchar_t *value);

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(byte_t *value);

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(int16_t *value);

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(int32_t *value);

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(int64_t *value);

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(uint16_t *value);

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(uint32_t *value);

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(uint64_t *value);

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(float *value);

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(double *value);

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(long double *value);

		/** Attaches "value" such that it will be deleted when
		 *  empty() is called. */
		void	attach(object *o);

		/** Attaches "value" such that it will be array-deleted when
		 *  empty() is called. */
		void	attach(object **o);

		/** Deletes or array-deletes, as appropriate, all values
		 *  previously attached using the attach() methods. */
		void	empty();

		/** Writes a representation of the wastebasket to standard
		 *  output. */
		ssize_t	write();

		/** Writes a representation of the wastebasket to "out". */
		ssize_t	write(output *out);

	#include <rudiments/private/wastebasket.h>
};

#endif
