// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CSVDOM_H
#define RUDIMENTS_CSVDOM_H

#include <rudiments/private/csvdomincludes.h>

/** The csvdom class implements a minimal CSV DOM parser. */
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

		/** Returns the number of rows in the CSV. */
		uint64_t	getRowCount();

		/** Returns row at "position". */
		domnode		*getRow(uint64_t position);

		/** Returns the value at "row","column" or NULL if no such
		 *  row/column exists. */
		const char	*getField(uint64_t row, uint64_t column);

		/** Returns the value at "row","column" or NULL if no such
		 *  row/column exists. */
		const char	*getField(uint64_t row, const char *column);

		/** Sets the value at "row","column" to "value" and marks
		 *  it "quoted".  Returns true on success and false if no such
		 *  row/column exists. */
		bool	setField(uint64_t row, uint64_t column,
					const char *value, bool quoted);

		/** Sets the value at "row","column" to "value" and marks
		 *  it "quoted".  Returns true on success and false if no such
		 *  row/column exists. */
		bool	setField(uint64_t row, const char *column,
					const char *value, bool quoted);

		/** Inserts a row before "position".  Returns true on success
		 *  and false if "position" doesn't exist. */
		bool	insertRowBefore(uint64_t position);

		/** Inserts a row after "position".  Returns true on success
		 *  and false if "position" doesn't exist. */
		bool	insertRowAfter(uint64_t position);

		/** Moves row at "oldposition" to "newposition".  Returns
		 *  true on success and false if "oldposition" doesn't exist
		 *  or if "newposition" is greater than one more than the last
		 *  position. */
		bool	moveRow(uint64_t oldposition, uint64_t newposition);

		/** Deletes the row at "position".  Returns true on success
		 *  and false if "position" doesn't exist. */
		bool	deleteRow(uint64_t position);

		/** Returns true if the row at "position" is composed
		 *  entirely of empty fields. */
		bool	getRowIsEmpty(uint64_t position);

		/** Deletes rows composed entirely of empty fields.
		 *  Returns true on success and false on failure. */
		bool	deleteEmptyRows();

		/** For each row, at the field in column "position", if the
		 *  value for the field in that row is empty, then replace
		 *  it with the value of the field at the same position from
		 *  the previous row. */
		void	carryValueDown(uint64_t position);

		/** For each row, at the field in column "name", if the
		 *  value for the field in that row is empty, then replace
		 *  it with the value of the field at the same position from
		 *  the previous row. */
		void	carryValueDown(const char *name);

		/** For each row, for all fields, if the value for the field in
		 *  that row is empty, then replace it with the value of the
		 *  field at the same position from the previous row. */
		void	carryAllValuesDown();

		/** Writes the current DOM tree to "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is output
		 *  as-is. */
		bool	write(output *out, bool indent) const;

	protected:

		/** Gets called when the start of the header is encountered. */
		virtual	bool	headerStart();

		/** Gets called when a column name is parsed. */
		virtual	bool	column(const char *name, bool quoted);

		/** Gets called when the end of the header is encountered. */
		virtual	bool	headerEnd();

		/** Gets called when the start of the body is encountered. */
		virtual	bool	bodyStart();

		/** Gets called when the start of a row is encountered. */
		virtual	bool	rowStart();

		/** Gets called when a field is parsed. */
		virtual	bool	field(const char *value, bool quoted);

		/** Gets called when the end of a row is encountered. */
		virtual	bool	rowEnd();

		/** Gets called when the end of the body is encountered. */
		virtual	bool	bodyEnd();

	#include <rudiments/private/csvdom.h>
};

#endif
