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

		/** Executes a create (insert) operation.
		 *
		 *  "columns" should contain the set of columns that
		 *  corresponding elements of "values" will be inserted into.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doCreate(const char * const *columns,
						const char * const *values)=0;

		/** Executes a create (insert) operation.
		 *
		 * "kvp" should contain the column/value pairs to be inserted.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doCreate(
				dictionary<const char *, const char *> *kvp)=0;

		/** Executes a create (insert) operation.
		 *
		 *  "j" should be a jsondom containing 1 object:
		 *
		 *  "data" should be a JSON object consisting of the
		 *  column/value pairs to be inserted.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doCreate(jsondom *j)=0;

		/** Executes a read operation.
		 *
		 *  "criteria" should be a JSON string representing the
		 *  criteria that will be used to build the where clause,
		 *  conforming to the format described in the class description.
		 *
		 *  "sort" should be a JSON object representing the criteria
		 *  that will be used to order the results, conforming to the
		 *  format described in the class description.
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

		/** Executes a read operation.
		 *
		 *  "j" should be a jsondom containing 3 objects:
		 *
		 *  "criteria" should be a JSON object representing the
		 *  criteria that will be used to determine what to read,
		 *  conforming to the format described in the class description.
		 *
		 *  "sort" should be a JSON object representing the criteria
		 *  that will be used to order the results, conforming to the
		 *  format described in the class description.
		 *
		 *  "skip" should be a number indicating how many rows to skip
		 *  immediately (useful for paging).
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doRead(jsondom *j)=0;

		/** Executes an update operation.
		 *
		 *  "columns" and "values" should be set to the column/value
		 *  pairs to be updated.
		 *
		 *  "criteria" should be a JSON string representing the
		 *  criteria that will be used to determine what to update,
		 *  conforming to the format described in the class description.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doUpdate(const char * const * columns,
						const char * const *values,
						const char *criteria)=0;

		/** Executes an update operation.
		 *
		 *  Keys of "kvp" and values of "kvp" should be set to the
		 *  column/value pairs to be updated.
		 *
		 *  "criteria" should be a JSON string representing the
		 *  criteria that will be used to determine what to update,
		 *  conforming to the format described in the class description.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doUpdate(
				dictionary<const char *, const char *> *kvp,
				const char *criteria)=0;

		/** Executes an update operation.
		 *
		 *  "j" should be a jsondom containing 2 objects:
		 *
		 *  "criteria" should be a JSON object representing the
		 *  criteria that will be used to determine what to update,
		 *  conforming to the format described in the class description.
		 *
		 *  "data" should be a JSON object consisting of the
		 *  column/value pairs to be updated.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doUpdate(jsondom *j)=0;

		/** Executes a delete operation.
		 *
		 *  "criteria" should be a JSON string representing the
		 *  criteria that will be used to determine what to delete,
		 *  conforming to the format described in the class description.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doDelete(const char *criteria)=0;

		/** Executes a delete operation.
		 *
		 *  "j" should be a jsondom containing 1 object:
		 *
		 *  "criteria" should be a JSON object representing the
		 *  criteria that will be used to determine what to delete,
		 *  conforming to the format described in the class description.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doDelete(jsondom *j)=0;

		/** Returns whatever error message may have been set by the
		 *  most recent failed method call. */
		virtual const char	*getErrorMessage()=0;

		/** Returns whatever error code may have been set by the
		 *  most recent failed method call. */
		virtual int64_t		getErrorCode()=0;

		/** Returns an instance of scalarcollection, containing the
		 *  number of affected rows if doCreate(), doUpdate(), or
		 *  doDelete() was most recently called or an empty scalar if
		 *  doRead() was most recently called. */
		virtual const scalarcollection<uint64_t>
						*getAffectedRowsScalar()=0;

		/** Returns an instance of listcollection with a single element,
		 *  containing the affected rows doCreate(), doUpdate(), or
		 *  doDelete() was most recently called or an empty list if
		 *  doRead() was most recently called. */
		virtual const listcollection<uint64_t>
						*getAffectedRowsList()=0;

		/** Returns an instance of dictionarycollection with a single
		 *  element, containing the affected rows doCreate(),
		 *  doUpdate(), or doDelete() was most recently called or an
		 *  empty dictionary if doRead() was most recently called. */
		virtual const dictionarycollection<const char *, uint64_t>
						*getAffectedRowsDictionary()=0;

		/** Returns an instance of tablecollection with a single
		 *  field, containing the affected rows doCreate(),
		 *  doUpdate(), or doDelete() was most recently called or an
		 *  empty table if doRead() was most recently called. */
		virtual const tablecollection<uint64_t>
						*getAffectedRowsTable()=0;

		/** Returns an instance of scalarcollection, representing the
		 *  first field of the first row of the results if doRead() was
		 *  most recently called, or an empty scalar if doCreate(),
		 *  doUpdate(), or doDelete() was most recently called. */
		virtual const scalarcollection<const char *>
						*getFirstFieldScalar()=0;

		/** Returns an instance of listcollection, representing the
		 *  first row of the results if doRead() was most recently
		 *  called, or an empty list if doCreate(), doUpdate(), or
		 *  doDelete() was most recently called. */
		virtual const listcollection<const char *>
						*getFirstRowList()=0;

		/** Returns an instance of dictionarycollection, representing
		 *  the first row of the results if doRead() was most recently
		 *  called, or an empty dictionary if doCreate(), doUpdate(),
		 *  or doDelete() was most recently called. */
		virtual const dictionarycollection<const char *, const char *>
						*getFirstRowDictionary()=0;

		/** Returns an instance of dictionarycollection, representing
		 *  the first column of each row of the results if doRead() was
		 *  most recently called, or an empty dictionary if doCreate(),
		 *  doUpdate(), or doDelete() was most recently called. */
		virtual const listcollection<const char *>
						*getFirstColumnList()=0;

		/** Returns an instance of tablecollection, representing the
 		 *  results if doRead() was most recently called, or an empty
		 *  resultsettable if doCreate(), doUpdate(), or doDelete() was
		 *  most recently called. */
		virtual const tablecollection<const char *>
						*getResultSetTable()=0;
};

class RUDIMENTS_DLLSPEC mvcresult : virtual public object {
	public:
		mvcresult();
		virtual ~mvcresult();

		void	setStatus(bool success);
		void	setStatus(bool success, uint32_t code);
		void	setStatus(bool success, uint32_t code,
						const char *message);

		void	setSuccess();
		void	setSuccess(uint32_t code);
		void	setSuccess(uint32_t code, const char *message);

		void	setFailed();
		void	setFailed(uint32_t code);
		void	setFailed(uint32_t code, const char *message);

		void		setSuccess(bool success);
		bool		getSuccess();

		void		setCode(uint32_t code);
		uint32_t	getCode();

		void		setMessage(const char *message);
		const char	*getMessage();

		void		setData(const char *key,
						const collection *data);
		void		attachData(const char *key,
						collection *data);

		linkedlist<char *>	*getKeys();
		const collection	*getData(const char *key);

		ssize_t	write() const;
		ssize_t	write(output *out) const;

		ssize_t	writeJson() const;
		ssize_t	writeJson(bool indent) const;
		ssize_t	writeJson(output *out) const;
		ssize_t	writeJson(output *out, bool indent) const;

		ssize_t	writeXml() const;
		ssize_t	writeXml(bool indent) const;
		ssize_t	writeXml(output *out) const;
		ssize_t	writeXml(output *out, bool indent) const;

		wastebasket	*getWastebasket();

	#include <rudiments/private/mvcresult.h>
};

#endif
