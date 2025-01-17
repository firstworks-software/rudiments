// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#ifndef RUDIMENTS_MVC_H
#define RUDIMENTS_MVC_H 1

#include <rudiments/private/mvcincludes.h>


/** The mvcsecurity class provides a base class for obtaining the current user.
 *
 *  This class cannot be used directly, rather it must be overridden by a child
 *  class that can fetch the current user from a specific security framework,
 *  for example, the httpbasicsecurity class can fetch the current user from
 *  the HTTP Basic Authentication framework. */
class RUDIMENTS_DLLSPEC mvcsecurity : virtual public object {
	public:

		/** Creates an instance of the mvcsecurity class. */
		mvcsecurity() {};

		/** Deletes this instance of the mvcsecurity class. */
		virtual	~mvcsecurity() {};

		/** Defines a pure-virtual method for obtaining the user
		 *  name.  Must be overridden by a child class. */
		virtual const char	*getUserName()=0;

	#include <rudiments/private/mvcsecurity.h>
};


/** The httpbasicsecurity class provides an implementation of the mvcsecurity
 *  class which can fetch the current user from the HTTP Basic Authentication
 *  framework. */
class RUDIMENTS_DLLSPEC httpbasicsecurity : public mvcsecurity {
	public:

		/** Creates an instance of the httpbasicsecurity class. */
		httpbasicsecurity();

		/** Deletes this instance of the httpbasicsecurity class. */
		virtual ~httpbasicsecurity();

		/** Sets the httprequest to use when getUserName() is called. */
		void	setRequest(httprequest *req);

		/** Returns the httprequest set by a previous call to
		 *  setRequest() or NULL if none has been set. */
		httprequest	*getRequest();

		/** Returns the REMOTE_USER provided by the httprequest set by
		 *  a previous call to setRequest(), NULL if no httprequest has
		 *  been, or an empty string if the httprequest didn't include
		 *  a REMOTE_USER. */
		const char	*getUserName();

	#include <rudiments/private/httpbasicsecurity.h>
};


/** The mvcproperties class provides methods for parsing java-style properties
 *  files or strings, and returning property values.
 *
 *  The format of the property file or string should follow this example:
 *
 *  #pound comment
 *  !exclamation comment
 *  key1=value1
 *  key2=value2
 *
 *  key 3=value 3
 *  key 4=value 4
 */
class RUDIMENTS_DLLSPEC mvcproperties : virtual public object {
	public:

		/** Creates an instance of the mvcproperties class. */
		mvcproperties();

		/** Deletes this instance of the mvcproperties class. */
		virtual	~mvcproperties();

		/** Parses the properties file "filename", discarding
		 *  properties from any previously parsed file or string. */
		bool	parseFile(const char *filename);

		/** Parses the properties string "string", discarding
		 *  properties from any previously parsed file or string. */
		bool	parseString(const char *string);

		/** Returns the value of "property" as provided by the most
		 *  recently prased file or string. */
		const char	*getValue(const char *property);

	#include <rudiments/private/mvcproperties.h>
};


/** The mvctier class provides a base class for the tiers of an MVC (Model View
 *  Controller) application, and provides methods and functionality that is
 *  common to all tiers.
 *
 *  This class is not intended to be used directly, nor are its direct
 *  children: mvcservice, mvcdao, mvcview, and mvccontroller.  Rather, you
 *  should create children of those classes implementing specific MVC
 *  functionality for your application, and use them directly. */
class RUDIMENTS_DLLSPEC mvctier : virtual public object {
	public:

		/** Creates an instance of the mvctier class. */
		mvctier();

		/** Deletes this instance of the mvctier class. */
		virtual ~mvctier();

		/** Returns "mvctier" but should be overridden by child classes
		 *  to return a more specific type. */
		virtual const char	*getType();

		/** Sets the instance of mvcproperties to use. */
		void		setProperties(mvcproperties *prop);

		/** Returns the instance of mvcproperties set by a previous
		 *  call to setProperties(). */
		mvcproperties	*getProperties();

		/** Sets the instance of mvcsecurity to use. */
		void		setSecurity(mvcsecurity *sec);

		/** Returns the instance of mvcsecurity set by a previous
		 *  call to setSecurity(). */
		mvcsecurity	*getSecurity();

		/** Sets the instance of logger to use. */
		void		setLogger(logger *log);

		/** Returns the instance of logger set by a previous call
		 *  to setLogger(). */
		logger		*getLogger();

	#include <rudiments/private/mvctier.h>
};


/** The mvccontroller class provides a base class for MVC (Model View
 *  Controller) controllers.
 *
 *  This class is not intended to be used directly, rather you should create
 *  children of this class that implements specific controller tier
 *  functionality for your application, and use them directly. */
class RUDIMENTS_DLLSPEC mvccontroller : public mvctier {
	public:

		/** Creates an instance of the mvccontroller class. */
		mvccontroller() {};

		/** Deletes this instance of the mvccontroller class. */
		virtual ~mvccontroller() {};

	#include <rudiments/private/mvccontroller.h>
};


/** The mvcview class provides a base class for MVC (Model View Controller)
 *  views.
 *
 *  This class is not intended to be used directly, rather you should create
 *  children of this class that implements specific view tier functionality for
 *  your application, and use them directly. */
class RUDIMENTS_DLLSPEC mvcview : public mvctier {
	public:

		/** Creates an instance of the mvcview class. */
		mvcview();

		/** Deletes this instance of the mvcview class. */
		virtual ~mvcview();

		/** Sets the instance of httprequest to use. */
		void	setRequest(httprequest *req);

		/** Returns the instance of httprequest set by a previous call
		 *  to setRequest(). */
		httprequest	*getRequest();

		/** Sets the instance of httpresponse to use. */
		void	setResponse(httpresponse *resp);

		/** Returns the instance of httpresponse set by a previous call
		 *  to setResponse(). */
		httpresponse	*getResponse();

	#include <rudiments/private/mvcview.h>
};


/** The mvcservice class provides a base class for MVC (Model View Controller)
 *  services.
 *
 *  This class is not intended to be used directly, rather you should create
 *  children of this class that implements specific service tier functionality
 *  for your application, and use them directly. */
class RUDIMENTS_DLLSPEC mvcservice : public mvctier {
	public:

		/** Creates an instance of the mvcservice class. */
		mvcservice() {};

		/** Deletes this instance of the mvcservice class. */
		virtual ~mvcservice() {};
		
	#include <rudiments/private/mvcservice.h>
};


/** The mvcdao class provides a base class for MVC (Model View Controller)
 *  DAOs (Data Access Objects).
 *
 *  This class is not intended to be used directly, rather you should create
 *  children of this class that implements specific DAO tier functionality
 *  for your application, and use them directly. */
class RUDIMENTS_DLLSPEC mvcdao : public mvctier {
	public:

		/** Creates an instance of the mvcdao class. */
		mvcdao() {};

		/** Deletes this instance of the mvcdao class. */
		virtual ~mvcdao() {};
		
	#include <rudiments/private/mvcdao.h>
};


/** The mvcresult class provides an object to which "results" can be attached
 *  and passed up from the DAO tier to other MVC tiers.
 *
 *  In the Rudiments MVC framework, collections (lists, dictionaries, trees,
 *  scalars, doms, etc.) are passed around between tiers instead of DTOs and/or
 *  domain objects.  mvcresult objects provide a convenient object to attach
 *  collections to, as well as status, status codes and status messages.
 *
 *  Internally, each instance of mvcresult has:
 *
 *  status - true if the operation succeeded, and false if it failed
 *  status code - a specific success or error code
 *  status message - a specific success or error message
 *  data - a dictionary where each key is a string and each value
 *  		is a collection
 *  wastebasket - an instance of the wastebasked class to which parent obejcts
 *  		of the collections, or of members of the collections can be
 *  		attached for future disposal
 */
class RUDIMENTS_DLLSPEC mvcresult : virtual public object {
	public:

		/** Creates an instance of the mvcresult class. */
		mvcresult();

		/** Deletes this instance of the mvcresult class. */
		virtual ~mvcresult();

		/** Sets the status to "success", status code to 0, and status
		 *  message to NULL. */
		void	setStatus(bool success);

		/** Sets the status to "success", status code to "code", and
		 *  status message to NULL. */
		void	setStatus(bool success, uint32_t code);

		/** Sets the status to "success", status code to "code", and
		 *  status message to "message". */
		void	setStatus(bool success, uint32_t code,
						const char *message);

		/** Sets the status to true (success), status code to 0, and
		 *  status message to NULL. */
		void	setSuccess();

		/** Sets the status to true (success), status code to "code",
		 *  and status message to NULL. */
		void	setSuccess(uint32_t code);

		/** Sets the status to true (success), status code to "code",
		 *  and status message to "message". */
		void	setSuccess(uint32_t code, const char *message);

		/** Sets the status to false (failure), status code to 0, and
		 *  status message to NULL. */
		void	setFailed();

		/** Sets the status to false (failure), status code to "code",
		 *  and status message to NULL. */
		void	setFailed(uint32_t code);

		/** Sets the status to false (failure), status code to "code",
		 *  and status message to "message". */
		void	setFailed(uint32_t code, const char *message);

		/** Sets the status "success" but does not change the
		 *  status code or status message. */
		void		setSuccess(bool success);

		/** Returns the status as set by a previous call to
		 *  setStatus(), setSuccess(), setFailed(), or setSuccess(). */
		bool		getSuccess();

		/** Sets the status code to "code" but does not change the
		 *  status or status message. */
		void		setCode(uint32_t code);

		/** Returns the status code as set by a previous call to
		 *  setStatus(), setSuccess(), setFailed(), or setCode(). */
		uint32_t	getCode();

		/** Sets the status message to "message" but does not change
		 *  the status or status code. */
		void		setMessage(const char *message);

		/** Returns the status message as set by a previous call to
		 *  setStatus(), setSuccess(), setFailed(), or setMessage(). */
		const char	*getMessage();

		/** Associates with collection "data" with string "key".
		 *
		 *  When using this method, "data" will not be deleted when
		 *  this instance of mvcresult is deleted. */
		void		setData(const char *key,
						collection *data);

		/** Associates with collection "data" with string "key".
		 *
		 *  When using this method, "data" will be deleted when this
		 *  instance of mvcresult is deleted. */
		void		attachData(const char *key,
						collection *data);

		/** Returns the list of keys previously associated with
		 *  collections using setData() or attachData(). */
		linkedlist<char *>	*getKeys();

		/** Returns the collection associated with "key". */
		collection		*getData(const char *key);

		/** Writes a representation (currently JSON) of the contents of
		 *  the instance to standard output.
		 *
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write();

		/** Writes a representation (currently JSON) of the contents of
		 *  the instance to "out".
		 *
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write(output *out);

		/** Writes an indented JSON representation of the contents of
		 *  the instance to standard output.
		 *
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	writeJson();

		/** Writes a JSON representation of the contents of the
		 *  instance to standard output.
		 *
		 *  The representation is indented if "indent" is true and not
		 *  indented if "indent" is false.
		 *
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	writeJson(bool indent);

		/** Writes an indented JSON representation of the contents of
		 *  the instance to "out".
		 *
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	writeJson(output *out);

		/** Writes a JSON representation of the contents of the
		 *  instance to "out".
		 *
		 *  The representation is indented if "indent" is true and not
		 *  indented if "indent" is false.
		 *
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	writeJson(output *out, bool indent);

		/** Returns the wastebasket. */
		wastebasket	*getWastebasket();

	#include <rudiments/private/mvcresult.h>
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
 *  (http://jsonlogic.com), and will be used to constrain the results.
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
 *  string conforming to the following format, and will be used to order
 *  the results.
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
		 *  "types" should contain the corresponding data type for each
		 *  value:
		 *  * "n" for numeric
		 *  * "t" for true
		 *  * "f" for false
		 *  * "u" for null
		 *  * "s" (or any other value) for string
		 *  Otherwise "types" may be null, and the data type will be 
		 *  derived as "s", "n", or "u" from the value.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doCreate(const char * const *columns,
						const char * const *values,
						const char * const *types)=0;

		/** Executes a create (insert) operation.
		 *
		 *  "columns" should contain the set of columns that
		 *  corresponding elements of "values" will be inserted into.
		 *  The data type of each value will be derived as "s", "n", or
		 *  "u" from the value.
		 *
		 *  Returns true on success and false on error.  On error, the
		 *  code and message can be retrieved using getErrorCode() and
		 *  getErrorMessage(). */
		virtual bool	doCreate(const char * const *columns,
						const char * const *values);

		/** Executes a create (insert) operation.
		 *
		 *  Keys of "kvp" and values of "kvp" should be set to the
		 *  column/value pairs to be inserted.  The data type of each
		 *  value will be derived as "s", "n", or "u" from the value.
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
		 *  criteria that will be used to constrain the results,
		 *  conforming to the format described in the class
		 *  description.
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
		 *  pairs to be updated.  "types" should be set to the
		 *  corresponding data type for each value:
		 *  * "n" for numeric
		 *  * "t" for true
		 *  * "f" for false
		 *  * "u" for null
		 *  * "s" (or any other value) for string
		 *  Otherwise "types" may be null, and the data type will be
		 *  derived as "s", "n", or "u" from the value.
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
						const char * const *types,
						const char *criteria)=0;

		/** Executes an update operation.
		 *
		 *  "columns" and "values" should be set to the column/value
		 *  pairs to be updated.  The data type of each value will be
		 *  derived as "s", "n", or "u" from the value.
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
						const char *criteria);

		/** Executes an update operation.
		 *
		 *  Keys of "kvp" and values of "kvp" should be set to the
		 *  column/value pairs to be updated.  The data type of each
		 *  value will be derived as "s", "n", or "u" from the value.
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
		virtual scalarcollection<uint64_t>
					*getAffectedRowsScalar()=0;

		/** Returns an instance of listcollection with a single element,
		 *  containing the affected rows doCreate(), doUpdate(), or
		 *  doDelete() was most recently called or an empty list if
		 *  doRead() was most recently called. */
		virtual listcollection<uint64_t>
					*getAffectedRowsList()=0;

		/** Returns an instance of dictionarycollection with a single
		 *  element, containing the affected rows doCreate(),
		 *  doUpdate(), or doDelete() was most recently called or an
		 *  empty dictionary if doRead() was most recently called. */
		virtual dictionarycollection<const char *, uint64_t>
					*getAffectedRowsDictionary()=0;

		/** Returns an instance of tablecollection with a single
		 *  field, containing the affected rows doCreate(),
		 *  doUpdate(), or doDelete() was most recently called or an
		 *  empty table if doRead() was most recently called. */
		virtual tablecollection<uint64_t>
					*getAffectedRowsTable()=0;

		/** Returns an instance of scalarcollection, representing the
		 *  first field of the first row of the results if doRead() was
		 *  most recently called, or an empty scalar if doCreate(),
		 *  doUpdate(), or doDelete() was most recently called. */
		virtual scalarcollection<const char *>
					*getFirstFieldScalar()=0;

		/** Returns an instance of listcollection, representing the
		 *  first row of the results if doRead() was most recently
		 *  called, or an empty list if doCreate(), doUpdate(), or
		 *  doDelete() was most recently called. */
		virtual listcollection<const char *>
					*getFirstRowList()=0;

		/** Returns an instance of dictionarycollection, representing
		 *  the first row of the results if doRead() was most recently
		 *  called, or an empty dictionary if doCreate(), doUpdate(),
		 *  or doDelete() was most recently called. */
		virtual dictionarycollection<const char *, const char *>
						*getFirstRowDictionary()=0;

		/** Returns an instance of dictionarycollection, representing
		 *  the first column of each row of the results if doRead() was
		 *  most recently called, or an empty dictionary if doCreate(),
		 *  doUpdate(), or doDelete() was most recently called. */
		virtual listcollection<const char *>
					*getFirstColumnList()=0;

		/** Returns an instance of tablecollection, representing the
 		 *  results if doRead() was most recently called, or an empty
		 *  resultsettable if doCreate(), doUpdate(), or doDelete() was
		 *  most recently called. */
		virtual tablecollection<const char *>
					*getResultSetTable()=0;

	protected:
		/** Returns "u" if "value" is NULL, "n" if value is a numeric
		 *  string, and "s" otherwise. */
		virtual	const char	*deriveDataType(const char *value);
};

#include <rudiments/private/mvccrudinlines.h>

#endif
