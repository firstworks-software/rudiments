// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#ifndef RUDIMENTS_MVC_H
#define RUDIMENTS_MVC_H 1

#include <rudiments/private/mvcincludes.h>

class RUDIMENTS_DLLSPEC mvcsecurity : virtual public object {
	public:
		mvcsecurity() {};
		virtual	~mvcsecurity() {};
		virtual const char	*getUserName()=0;
};

class RUDIMENTS_DLLSPEC httpbasicsecurity : public mvcsecurity {
	public:
		httpbasicsecurity();
		virtual ~httpbasicsecurity();

		void	setRequest(httprequest *req);
		httprequest	*getRequest();
		const char	*getUserName();

	#include <rudiments/private/httpbasicsecurity.h>
};

class RUDIMENTS_DLLSPEC mvcproperties : virtual public object {
	public:
		mvcproperties();
		virtual	~mvcproperties();

		bool		parseFile(const char *filename);
		bool		parseString(const char *string);

		const char	*getValue(const char *parameter);

	#include <rudiments/private/mvcproperties.h>
};

class RUDIMENTS_DLLSPEC mvctier : virtual public object {
	public:
		mvctier();
		virtual ~mvctier();

		void		setProperties(mvcproperties *prop);
		mvcproperties	*getProperties();

		void		setSecurity(mvcsecurity *sec);
		mvcsecurity	*getSecurity();

	#include <rudiments/private/mvctier.h>
};

class RUDIMENTS_DLLSPEC mvccontroller : public mvctier {
};

class RUDIMENTS_DLLSPEC mvcview : public mvctier {
	public:
		mvcview();
		virtual ~mvcview();

		void	setRequest(httprequest *req);
		httprequest	*getRequest();

		void	setResponse(httpresponse *resp);
		httpresponse	*getResponse();

	#include <rudiments/private/mvcview.h>
};

class RUDIMENTS_DLLSPEC mvcservice : public mvctier {
};

class RUDIMENTS_DLLSPEC mvcdao : public mvctier {
};

/** The mvccrud class defines an interface for child classes which implement
 *  the CRUD (create, read, update, and delete) paradigm.
 *
 *  A typical invocation of a mycrud class which implements mvccrud, from
 *  within the dao tier of an MVC application, would be something like:
 *
 *
 *  // initialize crud
 *  mycrud	*crud=new mycrud();
 *  mycrud->init(...some init parameters...);
 *
 *  // read data
 *  mycrud->doRead(...);
 *
 *  // return results via instance of mvcresults
 *  mvcr->setSuccess();
 *  mvcr->attachData("myresults","table",mycrud->getResultSetTable());
 *  mvcr->getWastebasket()->attach(crud);
 *
 *
 *  Various methods such as doRead(), doUpdate(), and doDelete() take a
 *  "criteria" argument.  This should be a JSON string in jsonlogic format
 *  (http://jsonlogic.com), and will be used to construct the where clause for
 *  these operations:
 *
 *    { "and" : [
 *      { "=" : [
 *        { "var" : "col1" },
 *        1
 *      ] },
 *      { "!=" : [
 *        { "var" : "col2" },
 *        "one"
 *      ] }
 *    ] }
 *
 *  The doRead() method also takes a "sort" argument.  This should be a JSON
 *  string conforming to the following format, and will be used to construct
 *  the order-by clause for these operations:
 *
 *    {
 *      "col1" : "asc",
 *      "col2" : "asc",
 *      "col3" : "desc"
 *    }
 */
class RUDIMENTS_DLLSPEC mvccrud : virtual public object {
	public:
		/** Executes the create operation utilizing "keys" and "values"
		 *  as appropriate.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doCreate(const char * const *keys,
						const char * const *values)=0;

		/** Executes the read opertation.
		 *
		 *  "criteria" should be a JSON string representing the
		 *  criteria that will be used to build the where clause,
		 *  conforming to the format described in the class description.
		 *
		 *  "sort" should be a JSON string representing the criteria
		 *  that will be used to build the order-by clause, conforming
		 *  to the format described in the class description.
		 *
		 *  "skip" indicates how many rows to skip immediately (useful
		 *  for paging).
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doRead(const char *criteria,
						const char *sort,
						uint64_t skip)=0;

		/** Executes the update opertaion.
		 *
		 *  "keys" and "values" should be set to the key/value pairs to
		 *  be updated.
		 *
		 *  "criteria" should be a JSON string representing the
		 *  criteria that will be used to build the where clause,
		 *  conforming to the format described in the class description.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doUpdate(const char * const * keys,
						const char * const *values,
						const char *criteria)=0;

		/** Executes the delete operation.
		 *
		 *  "criteria" should be a JSON string representing the
		 *  criteria that will be used to build the where clause,
		 *  conforming to the format described in the class description.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doDelete(const char *criteria)=0;

		/** Returns whatever error message may have been set by the
		 *  most recent failed method call. */
		virtual const char	*getErrorMessage()=0;

		/** Returns whatever error code may have been set by the
		 *  most recent failed method call. */
		virtual int64_t		getErrorCode()=0;

		/** Returns an instance of scalar, representing the first
		 *  field of the first row of the results if doRead() was
		 *  most recently called, or an empty scalar if doCreate(),
		 *  doUpdate(), or doDelete() was most recently called. */
		virtual const scalarcollection<const char *>
							*getScalar()=0;

		/** Returns an instance of linkedlist, representing the
		 *  first row of the results if doRead() was most recently
		 *  called, or an empty linkedlist if doCreate(), doUpdate(),
		 *  or doDelete() was most recently called. */
		virtual const listcollection<const char *>
							*getRowLinkedList()=0;

		/** Returns an instance of dictionary, representing the first
		 *  row of the results if doRead() was most recently
		 *  called, or an empty dictionary if doCreate(), doUpdate(),
		 *  or doDelete() was most recently called. */
		virtual const dictionarycollection<const char *, const char *>
							*getRowDictionary()=0;

		/** Returns an instance of dictionary, representing the first
		 *  column of each row of the results if doRead() was most
		 *  recently called, or an empty resultsetlinkedlist if
		 *  doCreate(), doUpdate(), or doDelete() was most recently
		 *  called. */
		virtual const listcollection<const char *>
						*getResultSetLinkedList()=0;

		/** Returns an instance of table, representing the results
		 *  if doRead() was most recently called, or an empty
		 *  resultsettable if doCreate(), doUpdate(), or doDelete() was
		 *  most recently called. */
		virtual const tablecollection<const char *>
						*getResultSetTable()=0;
};

class RUDIMENTS_DLLSPEC mvcresult : virtual public object {
	public:
		mvcresult();
		virtual ~mvcresult();

		void	setResult(bool success);
		void	setResult(bool success, uint32_t code);
		void	setResult(bool success, uint32_t code,
						const char *message);

		void	setSuccess();
		void	setSuccess(uint32_t code);
		void	setSuccess(uint32_t code, const char *message);

		void	setFailed();
		void	setFailed(uint32_t code);
		void	setFailed(uint32_t code, const char *message);

		void	setSuccess(bool success);
		bool	getSuccess();

		void		setCode(uint32_t code);
		uint32_t	getCode();

		void		setMessage(const char *message);
		const char	*getMessage();

		void	attachData(const char *key,
					const char *type,
					collection *data);

		linkedlist<char *>	*getKeys();
		const char		*getType(const char *key);
		collection		*getData(const char *key);

		wastebasket	*getWastebasket();

	#include <rudiments/private/mvcresult.h>
};

#endif
