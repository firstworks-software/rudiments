// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CSVDOM_H
#define RUDIMENTS_CSVDOM_H

#include <rudiments/private/csvdomincludes.h>

/** The csvdom class implements a minimal CSV DOM parser.
 *
 *  It parses a file or string of CSV-formatted data and produces a dom tree
 *  representing the data.  It creates an "h" element for the header, "c"
 *  element for each column, "r" element for each record, and "f" element for
 *  each field.  To each "c" and "f" element, it adds a "v" attribute,
 *  containing the value, and a "q" element which can contain "y" or "n"
 *  indicating whether or not the value is quoted.
 *
 *  For example, the following CSV:
 *
 *  "col1","col2",col3,col4
 *  field11,field12,"field13","field14"
 *  "field21",field22,field23,"field24"
 *  field31,"field32",field33,field34
 *
 *  would produce the following dom tree:
 *
 *  <h>
 *    <c v="col1" q="y"/>
 *    <c v="col2" q="y"/>
 *    <c v="col3" q="n"/>
 *    <c v="col4" q="n"/>
 *  </h>
 *  <r>
 *    <f v="field11" q="n"/>
 *    <f v="field12" q="n"/>
 *    <f v="field13" q="y"/>
 *    <f v="field14" q="y"/>
 *  </r>
 *  <r>
 *    <f v="field21" q="y"/>
 *    <f v="field22" q="n"/>
 *    <f v="field23" q="n"/>
 *    <f v="field24" q="y"/>
 *  </r>
 *  <r>
 *    <f v="field31" q="n"/>
 *    <f v="field32" q="y"/>
 *    <f v="field33" q="n"/>
 *    <f v="field34" q="n"/>
 *  </r>
 *
 */
class RUDIMENTS_DLLSPEC csvdom : public csvsax, public dom {
	public:

		/** Creates a new instance of the csvdom class. */
		csvdom();

		/** Creates a new instance of the csvdom class, allowing the
		 *  user to enable or disable the "string cache" for this
		 *  instance.
		 *
		 *  By default, as each csvdomnode is added to the csvdom tree,
		 *  tag and attribute names and values are stored in a string
		 *  cache and pointers into the cache are assigned to each node.
		 *  Reference counters are kept and the string is removed from
		 *  the cache when no more nodes are using it.  If the data is 
		 *  highly repetitive this conserves memory at the cost of
		 *  speed.
		 *
		 *  If the string cache is disabled then memory is allocated in
		 *  each csvdomnode for names and values and freed when the
		 *  csvdomnode is freed.  This is faster but uses much more
		 *  memory. */
		csvdom(bool stringcacheenabled);

		/** Deletes this instance of the csvdom class. */
		virtual	~csvdom();

		/** Returns "csvdom". */
		virtual const char	*getType();

		/** Parses generic input "in" and generates a DOM tree. */
		bool	parse(input *in);

		/** Parses file "filename" and generates a DOM tree. */
		bool	parseFile(const char *filename);

		/** Parses string "string" and generates a DOM tree. */
		bool	parseString(const char *string);

		/** Creates a new root node.  This is useful for building a
		 *  tree from scratch. */
		void	createRootNode();

		/** Returns the number of columns in the CSV. */
		uint64_t	getColumnCount();

		/** Returns the column name of the column at "position".
		 *  Returns NULL if "position" doesn't exist. */
		const char	*getColumnName(uint64_t position);

		/** Sets "position" to the position of the column named "name".
		 *  Returns true on success and false if "name" doesn't
		 *  exist. */
		bool	getColumnPosition(const char *name, uint64_t *position);

		/** Upper-cases the name of the column at "position".  Returns
		 *  true on success and false if "position" doesn't exist. */
		bool	upperCaseColumn(uint64_t position);

		/** Upper-cases the name of the column "name".  Returns true on
		 *  success and false if "name" doesn't exist. */
		bool	upperCaseColumn(const char *name);

		/** Upper-cases all column names. */
		bool	upperCaseColumns();

		/** Lower-cases the name of the column at "position".  Returns
		 *  true on success and false if "position" doesn't exist. */
		bool	lowerCaseColumn(uint64_t position);

		/** Lower-cases the name of the column "name".  Returns true on
		 *  success and false if "name" doesn't exist. */
		bool	lowerCaseColumn(const char *name);

		/** Lower-cases all column names. */
		bool	lowerCaseColumns();

		/** Right-trims the name of the column at "position".  Returns
		 *  true on success and false if "position" doesn't exist. */
		bool	rightTrimColumn(uint64_t position);

		/** Right-trims the name of the column "name".  Returns true on
		 *  success and false if "name" doesn't exist. */
		bool	rightTrimColumn(const char *name);

		/** Right-trims all column names. */
		bool	rightTrimColumns();

		/** Left-trims the name of the column at "position".  Returns
		 *  true on success and false if "position" doesn't exist. */
		bool	leftTrimColumn(uint64_t position);

		/** Left-trims the name of the column "name".  Returns true on
		 *  success and false if "name" doesn't exist. */
		bool	leftTrimColumn(const char *name);

		/** Left-trims all column names. */
		bool	leftTrimColumns();

		/** Right-trims and left-trims the name of the column at
		 *  "position".  Returns true on success and false if
		 *  "position" doesn't exist. */
		bool	bothTrimColumn(uint64_t position);

		/** Right-trims and left-trims the name of the column "name".
		 *  Returns true on success and false if "name" doesn't
		 *  exist. */
		bool	bothTrimColumn(const char *name);

		/** Right-trims and left-trims all column names. */
		bool	bothTrimColumns();

		/** Renames the column at "position" to "name".  Sets the
		 *  column name to be quoted if "quoted" is true, or false
		 *  otherwise.  Returns true on success and false if
		 *  "position" doesn't exist. */
		bool	renameColumn(uint64_t position,
					const char *name, bool quoted);

		/** Renames the first column named "oldname" to "newname".
		 *  Sets the column name to be quoted if "quoted" is true, or
		 *  false otherwise.  Returns true on success and false if
		 *  "oldname" doesn't exist. */
		bool	renameColumn(const char *oldname,
					const char *newname, bool quoted);

		/** Renames the first column named any of the names in the
		 *  NULL-terminated array of names "oldname" to "newname".
		 *  Sets the column name to be quoted if "quoted" is true, or
		 *  false otherwise.  Returns true on success and false if
		 *  "oldname" doesn't exist. */
		bool	renameColumn(const char * const *oldnames,
					const char *newname, bool quoted);

		/** Renames all columns named "oldname" to "newname".
		 *  Sets the column name to be quoted if "quoted" is true, or
		 *  false otherwise.  Returns true on success and false if
		 *  "oldname" doesn't exist. */
		bool	renameAllColumns(const char *oldname,
					const char *newname, bool quoted);

		/** Renames all columns named any of the names in the
		 *  NULL-terminated array of names "oldname" to "newname".
		 *  Sets the column name to be quoted if "quoted" is true, or
		 *  false otherwise.  Returns true on success and false if
		 *  "oldname" doesn't exist. */
		bool	renameAllColumns(const char * const *oldnames,
					const char *newname, bool quoted);

		/** Inserts a column before "position" and names it "name".
 		 *  Sets the column name to be quoted if "quoted" is true, or
 		 *  false otherwise.  If "name" is null or empty then the
 		 *  column name will be set to an empty string.  Returns true
 		 *  on success and false if "position" doesn't exist. */
		bool	insertColumnBefore(uint64_t position,
					const char *name, bool quoted);

		/** Inserts a column before column named "name" and names it
		 *  "newname".  Sets the column name to be quoted if "quoted"
		 *  is true, or false otherwise.  If "name" is null or empty
		 *  then the column name will be set to an empty string.
		 *  Returns true on success and false if "name" doesn't
		 *  exist. */
		bool	insertColumnBefore(const char *name,
					const char *newname, bool quoted);

		/** Inserts a column after "position" with name "name".  Sets
		 *  the column name to be quoted if "quoted" is true, or false
		 *  otherwise.  If "name" is null or empty then the column name
		 *  will be set to an empty string.  Returns true on success
		 *  and false if "position" doesn't exist. */
		bool	insertColumnAfter(uint64_t position,
					const char *name, bool quoted);

		/** Inserts a column after column named "name" and names it
		 *  "newname".  Sets the column name to be quoted if "quoted"
		 *  is true, or false otherwise.  If "name" is null or empty
		 *  then the column name will be set to an empty string.
		 *  Returns true on success and false if "name" doesn't
		 *  exist. */
		bool	insertColumnAfter(const char *name,
					const char *newname, bool quoted);

		/** Moves column at "oldposition" to "newposition".  Returns
		 *  true on success and false if "oldposition" doesn't exist
		 *  or if "newposition" is greater than one more than the last
		 *  position. */
		bool	moveColumn(uint64_t oldposition, uint64_t newposition);

		/** Moves column named "name" to "position".  Returns
		 *  true on success and false if "name" doesn't exist, or if
		 *  position is greater than the one more than the last
		 *  position. */
		bool	moveColumn(const char *name, uint64_t position);

		/** Deletes the column at "position".  Returns true on success
		 *  and false if "position" doesn't exist. */
		bool	deleteColumn(uint64_t position);

		/** Deletes the column named "name".  Returns true on success
		 *  and false if "name" doesn't exist. */
		bool	deleteColumn(const char *name);

		/** Returns true if the column at "position" is composed
		 *  entirely of empty fields. */
		bool	getColumnIsEmpty(uint64_t position);

		/** Returns true if the column "name" is composed
		 *  entirely of empty fields. */
		bool	getColumnIsEmpty(const char *name);

		/** Deletes columns composed entirely of empty fields.
		 *  Returns true on success and false on failure. */
		bool	deleteEmptyColumns();

		/** Returns the number of records in the CSV. */
		uint64_t	getRecordCount();

		/** Returns record at "position". */
		domnode		*getRecord(uint64_t position);

		/** Returns the value at "record","column" or NULL if no such
		 *  record/column exists. */
		const char	*getField(uint64_t record, uint64_t column);

		/** Returns the value at "record","column" or NULL if no such
		 *  record/column exists. */
		const char	*getField(uint64_t record, const char *column);

		/** Sets the value at "record","column" to "value" and marks
		 *  it "quoted".  Returns true on success and false if no such
		 *  record/column exists. */
		bool	setField(uint64_t record, uint64_t column,
					const char *value, bool quoted);

		/** Sets the value at "record","column" to "value" and marks
		 *  it "quoted".  Returns true on success and false if no such
		 *  record/column exists. */
		bool	setField(uint64_t record, const char *column,
					const char *value, bool quoted);

		/** Right-trims the value at "record","column".  Returns true on
		 *  success and false if no such record/column exists. */
		bool	rightTrimField(uint64_t record, uint64_t column);

		/** Right-trims the value at "record","column".  Returns true on
		 *  success and false if no such record/column exists. */
		bool	rightTrimField(uint64_t record, const char *column);

		/** Left-trims the value at "record","column".  Returns true on
		 *  success and false if no such record/column exists. */
		bool	leftTrimField(uint64_t record, uint64_t column);

		/** Left-trims the value at "record","column".  Returns true on
		 *  success and false if no such record/column exists. */
		bool	leftTrimField(uint64_t record, const char *column);

		/** Right-trims and left-trims the value at "record","column".
		 *  Returns true on success and false if no such record/column
		 *  exists. */
		bool	bothTrimField(uint64_t record, uint64_t column);

		/** Right-trims and left-trims the value at "record","column".
		 *  Returns true on success and false if no such record/column
		 *  exists. */
		bool	bothTrimField(uint64_t record, const char *column);

		/** Right-trims all values. */
		void	rightTrimFields();

		/** Left-trims all values. */
		void	leftTrimFields();

		/** Right-trims and left-trims all values. */
		void	bothTrimFields();

		/** Inserts a record before "position".  Returns true on success
		 *  and false if "position" doesn't exist. */
		bool	insertRecordBefore(uint64_t position);

		/** Inserts a record after "position".  Returns true on success
		 *  and false if "position" doesn't exist. */
		bool	insertRecordAfter(uint64_t position);

		/** Moves record at "oldposition" to "newposition".  Returns
		 *  true on success and false if "oldposition" doesn't exist
		 *  or if "newposition" is greater than one more than the last
		 *  position. */
		bool	moveRecord(uint64_t oldposition, uint64_t newposition);

		/** Deletes the record at "position".  Returns true on success
		 *  and false if "position" doesn't exist. */
		bool	deleteRecord(uint64_t position);

		/** Returns true if the record at "position" is composed
		 *  entirely of empty fields. */
		bool	getRecordIsEmpty(uint64_t position);

		/** Deletes records composed entirely of empty fields.
		 *  Returns true on success and false on failure. */
		bool	deleteEmptyRecords();

		/** For each record, at the field in column "position", if the
		 *  value for the field in that record is empty, then replace
		 *  it with the value of the field at the same position from
		 *  the previous record. */
		void	carryValueDown(uint64_t position);

		/** For each record, at the field in column "name", if the
		 *  value for the field in that record is empty, then replace
		 *  it with the value of the field at the same position from
		 *  the previous record. */
		void	carryValueDown(const char *name);

		/** For each record, for all fields, if the value for the field
 		 *  in that record is empty, then replace it with the value of
 		 *  the field at the same position from the previous record. */
		void	carryAllValuesDown();

	protected:

		/** Gets called when the start of the header is encountered. */
		virtual	bool	headerStart();

		/** Gets called when a column name is parsed. */
		virtual	bool	column(const char *name, bool quoted);

		/** Gets called when the end of the header is encountered. */
		virtual	bool	headerEnd();

		/** Gets called when the start of the body is encountered. */
		virtual	bool	bodyStart();

		/** Gets called when the start of a record is encountered. */
		virtual	bool	recordStart();

		/** Gets called when a field is parsed. */
		virtual	bool	field(const char *value, bool quoted);

		/** Gets called when the end of a record is encountered. */
		virtual	bool	recordEnd();

		/** Gets called when the end of the body is encountered. */
		virtual	bool	bodyEnd();

	#include <rudiments/private/csvdom.h>
};

#endif
