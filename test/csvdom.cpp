// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/csvdom.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>
#include "test.cpp"

const char normal[]=
"\"\"\"col1\"\"\",c\"ol\"2,col3,col4,col5\n"
"val11,\"\"\"val12\"\"\",v\"al1\"3,\"val\n14\",val15\n"
"val21,val22,\"\"\"val23\"\"\",v\"al2\"4,\"val\n25\"\n";

const char testcsv[]=
"blah\nblah\nblah\nblah\n"
"col1,col2,col3,col4,col5\n"
"val11,val12,val13,val14,val15\n"
"val21,val22,val23,val24,val25\n"
"val31,val32,val33,val34,val35\n"
"val41,val42,val43,val44,val45\n"
"val51,val52,val53,val54,val55\n"
"blah1\nblah2\nblah3\nblah4\n";

const char carrydown[]=
"col1,col2,col3,col4,col5\n"
"val1,val2,val3,val4,val5\n"
",val2,,val4,\n"
",val2,,val4,\n"
",val2,,val4,\n"
",val2,,val4,\n"
"vala,valb,valc,vald,vale\n"
"vala,,valc,,vale\n"
"vala,,valc,,vale\n"
"vala,,valc,,vale\n"
"vala,,valc,,vale\n";

int main() {

	header("csvdom");

	csvdom	c;

	stdoutput.printf("normal...\n");
	c.parseString(normal);
	c.writeFile("normal.csv",permissions::evalPermString("rw-r--r--"));
	char	*normalcsv=file::getContents("normal.csv");
	test("file contents",!charstring::compare(normal,normalcsv));
	delete[] normalcsv;
	test("parse file",c.parseFile("normal.csv"));
	file::remove("normal.csv");

	c.setIgnoreHeaderLines(4);
	c.setIgnoreFooterLines(4);
	for (uint16_t i=0; i<2; i++) {

		if (!i) {
			c.parseString(testcsv);
		} else {
			file	f;
			test("create file",f.create("test.csv",
				permissions::evalPermString("rw-r--r--")));
			test("write to file",
				f.write(testcsv,charstring::length(testcsv))==
					(ssize_t)charstring::length(testcsv));
			test ("parse file",c.parseFile("test.csv"));
			file::remove("test.csv");
		}

		stdoutput.printf("column manipulation - %s...\n",
						(!i)?"string":"file");

		test("column count",c.getColumnCount()==5);
		test("column name 0",
			!charstring::compare(c.getColumnName(0),"col1"));
		test("column name 1",
			!charstring::compare(c.getColumnName(1),"col2"));
		test("column name 2",
			!charstring::compare(c.getColumnName(2),"col3"));
		test("column name 3",
			!charstring::compare(c.getColumnName(3),"col4"));
		test("column name 4",
			!charstring::compare(c.getColumnName(4),"col5"));
		uint64_t	position;
		test("column position col1 success",
				c.getColumnPosition("col1",&position));
		test("column position col1 value",position==0);
		test("column position col2 success",
				c.getColumnPosition("col2",&position));
		test("column position col2 value",position==1);
		test("column position col3 success",
				c.getColumnPosition("col3",&position));
		test("column position col3 value",position==2);
		test("column position col4 success",
				c.getColumnPosition("col4",&position));
		test("column position col4 value",position==3);
		test("column position col5 success",
				c.getColumnPosition("col5",&position));
		test("column position col5 value",position==4);
		test("rename column col1 -> c1",
				c.renameColumn("col1","c1",false));
		test("rename column col2 -> c2",
				c.renameColumn("col2","c2",false));
		test("rename column col3 -> c3",
				c.renameColumn("col3","c3",false));
		test("rename column col4 -> c4",
				c.renameColumn("col4","c4",false));
		test("rename column col5 -> c5",
				c.renameColumn("col5","c5",false));
		test("column name 0",
			!charstring::compare(c.getColumnName(0),"c1"));
		test("column name 1",
			!charstring::compare(c.getColumnName(1),"c2"));
		test("column name 2",
			!charstring::compare(c.getColumnName(2),"c3"));
		test("column name 3",
			!charstring::compare(c.getColumnName(3),"c4"));
		test("column name 4",
			!charstring::compare(c.getColumnName(4),"c5"));
		test("row count",c.getRowCount()==5);
		test("field 0,0",
			!charstring::compare(
				c.getField(0,(uint64_t)0),"val11"));
		test("field 0,1",
			!charstring::compare(c.getField(0,1),"val12"));
		test("field 0,2",
			!charstring::compare(c.getField(0,2),"val13"));
		test("field 0,3",
			!charstring::compare(c.getField(0,3),"val14"));
		test("field 0,4",
			!charstring::compare(c.getField(0,4),"val15"));
		test("field 1,0",
			!charstring::compare(
				c.getField(1,(uint64_t)0),"val21"));
		test("field 1,1",
			!charstring::compare(c.getField(1,1),"val22"));
		test("field 1,2",
			!charstring::compare(c.getField(1,2),"val23"));
		test("field 1,3",
			!charstring::compare(c.getField(1,3),"val24"));
		test("field 1,4",
			!charstring::compare(c.getField(1,4),"val25"));
		test("field 2,0",
			!charstring::compare(
				c.getField(2,(uint64_t)0),"val31"));
		test("field 2,1",
			!charstring::compare(c.getField(2,1),"val32"));
		test("field 2,2",
			!charstring::compare(c.getField(2,2),"val33"));
		test("field 2,3",
			!charstring::compare(c.getField(2,3),"val34"));
		test("field 2,4",
			!charstring::compare(c.getField(2,4),"val35"));
		test("field 3,0",
			!charstring::compare(
				c.getField(3,(uint64_t)0),"val41"));
		test("field 3,1",
			!charstring::compare(c.getField(3,1),"val42"));
		test("field 3,2",
			!charstring::compare(c.getField(3,2),"val43"));
		test("field 3,3",
			!charstring::compare(c.getField(3,3),"val44"));
		test("field 3,4",
			!charstring::compare(c.getField(3,4),"val45"));
		test("field 4,0",
			!charstring::compare(
				c.getField(4,(uint64_t)0),"val51"));
		test("field 4,1",
			!charstring::compare(c.getField(4,1),"val52"));
		test("field 4,2",
			!charstring::compare(c.getField(4,2),"val53"));
		test("field 4,3",
			!charstring::compare(c.getField(4,3),"val54"));
		test("field 4,4",
			!charstring::compare(c.getField(4,4),"val55"));

		test("move 4 -> 0",c.moveColumn(4,0));
		test("column name 0",
			!charstring::compare(c.getColumnName(0),"c5"));
		test("column name 1",
			!charstring::compare(c.getColumnName(1),"c1"));
		test("column name 2",
			!charstring::compare(c.getColumnName(2),"c2"));
		test("column name 3",
			!charstring::compare(c.getColumnName(3),"c3"));
		test("column name 4",
			!charstring::compare(c.getColumnName(4),"c4"));
		test("field 0,0",
			!charstring::compare(
				c.getField(0,(uint64_t)0),"val15"));
		test("field 0,1",
			!charstring::compare(c.getField(0,1),"val11"));
		test("field 0,2",
			!charstring::compare(c.getField(0,2),"val12"));
		test("field 0,3",
			!charstring::compare(c.getField(0,3),"val13"));
		test("field 0,4",
			!charstring::compare(c.getField(0,4),"val14"));

		test("move 4 -> 1",c.moveColumn(4,1));
		test("column name 0",
			!charstring::compare(c.getColumnName(0),"c5"));
		test("column name 1",
			!charstring::compare(c.getColumnName(1),"c4"));
		test("column name 2",
			!charstring::compare(c.getColumnName(2),"c1"));
		test("column name 3",
			!charstring::compare(c.getColumnName(3),"c2"));
		test("column name 4",
			!charstring::compare(c.getColumnName(4),"c3"));
		test("field 0,0",
			!charstring::compare(
				c.getField(0,(uint64_t)0),"val15"));
		test("field 0,1",
			!charstring::compare(c.getField(0,1),"val14"));
		test("field 0,2",
			!charstring::compare(c.getField(0,2),"val11"));
		test("field 0,3",
			!charstring::compare(c.getField(0,3),"val12"));
		test("field 0,4",
			!charstring::compare(c.getField(0,4),"val13"));

		test("move 4 -> 2",c.moveColumn(4,2));
		test("column name 0",
			!charstring::compare(c.getColumnName(0),"c5"));
		test("column name 1",
			!charstring::compare(c.getColumnName(1),"c4"));
		test("column name 2",
			!charstring::compare(c.getColumnName(2),"c3"));
		test("column name 3",
			!charstring::compare(c.getColumnName(3),"c1"));
		test("column name 4",
			!charstring::compare(c.getColumnName(4),"c2"));
		test("field 0,0",
			!charstring::compare(
				c.getField(0,(uint64_t)0),"val15"));
		test("field 0,1",
			!charstring::compare(c.getField(0,1),"val14"));
		test("field 0,2",
			!charstring::compare(c.getField(0,2),"val13"));
		test("field 0,3",
			!charstring::compare(c.getField(0,3),"val11"));
		test("field 0,4",
			!charstring::compare(c.getField(0,4),"val12"));

		test("move 4 -> 3",c.moveColumn(4,3));
		test("column name 0",
			!charstring::compare(c.getColumnName(0),"c5"));
		test("column name 1",
			!charstring::compare(c.getColumnName(1),"c4"));
		test("column name 2",
			!charstring::compare(c.getColumnName(2),"c3"));
		test("column name 3",
			!charstring::compare(c.getColumnName(3),"c2"));
		test("column name 4",
			!charstring::compare(c.getColumnName(4),"c1"));
		test("field 0,0",
			!charstring::compare(
				c.getField(0,(uint64_t)0),"val15"));
		test("field 0,1",
			!charstring::compare(c.getField(0,1),"val14"));
		test("field 0,2",
			!charstring::compare(c.getField(0,2),"val13"));
		test("field 0,3",
			!charstring::compare(c.getField(0,3),"val12"));
		test("field 0,4",
			!charstring::compare(c.getField(0,4),"val11"));

		test("insert before 0",
			c.insertColumnBefore((uint64_t)0,"new",false));
		test("column count",c.getColumnCount()==6);
		test("field 0,0",
			!charstring::compare(c.getField(0,(uint64_t)0),""));
		test("field 1,0",
			!charstring::compare(c.getField(1,(uint64_t)0),""));
		test("field 2,0",
			!charstring::compare(c.getField(2,(uint64_t)0),""));
		test("field 3,0",
			!charstring::compare(c.getField(3,(uint64_t)0),""));
		test("field 4,0",
			!charstring::compare(c.getField(4,(uint64_t)0),""));
		test("field 0,1",
			!charstring::compare(c.getField(0,1),"val15"));
		test("field 1,1",
			!charstring::compare(c.getField(0,2),"val14"));
		test("field 2,1",
			!charstring::compare(c.getField(0,3),"val13"));
		test("field 3,1",
			!charstring::compare(c.getField(0,4),"val12"));
		test("field 4,1",
			!charstring::compare(c.getField(0,5),"val11"));

		test("insert after 5",c.insertColumnAfter(5,"new",false));
		test("column count",c.getColumnCount()==7);
		test("field 0,6",!charstring::compare(c.getField(0,6),""));
		test("field 1,6",!charstring::compare(c.getField(1,6),""));
		test("field 2,6",!charstring::compare(c.getField(2,6),""));
		test("field 3,6",!charstring::compare(c.getField(3,6),""));
		test("field 4,6",!charstring::compare(c.getField(4,6),""));

		test("delete column 0",c.deleteColumn((uint64_t)0));
		test("column count",c.getColumnCount()==6);
		test("delete column 5",c.deleteColumn(5));
		test("column count",c.getColumnCount()==5);
		test("field 0,0",!charstring::compare(
				c.getField(0,(uint64_t)0),"val15"));
		test("field 0,4",!charstring::compare(c.getField(0,4),"val11"));

		test("insert before 0",
				c.insertColumnBefore((uint64_t)0,"new",false));
		test("insert before 2",c.insertColumnBefore(2,"new",false));
		test("insert before 4",c.insertColumnBefore(4,"new",false));
		test("insert before 6",c.insertColumnBefore(6,"new",false));
		test("insert before 8",c.insertColumnBefore(8,"new",false));
		test("column count",c.getColumnCount()==10);
		test("empty column 0",c.getColumnIsEmpty((uint64_t)0));
		test("empty column 1",!c.getColumnIsEmpty(1));
		test("empty column 2",c.getColumnIsEmpty(2));
		test("empty column 3",!c.getColumnIsEmpty(3));
		test("empty column 4",c.getColumnIsEmpty(4));
		test("empty column 5",!c.getColumnIsEmpty(5));
		test("empty column 6",c.getColumnIsEmpty(6));
		test("empty column 7",!c.getColumnIsEmpty(7));
		test("empty column 8",c.getColumnIsEmpty(8));
		test("empty column 9",!c.getColumnIsEmpty(9));
		test("delete empty columns",c.deleteEmptyColumns());
		test("column count",c.getColumnCount()==5);
		test("empty column 0",!c.getColumnIsEmpty((uint64_t)0));
		test("empty column 1",!c.getColumnIsEmpty(1));
		test("empty column 2",!c.getColumnIsEmpty(2));
		test("empty column 3",!c.getColumnIsEmpty(3));
		test("empty column 4",!c.getColumnIsEmpty(4));

		test("upper c5 success",c.upperCaseColumn((uint64_t)0));
		test("upper c5 value",!charstring::compare(
					c.getColumnName((uint64_t)0),"C5"));
		test("lower C5 success",c.lowerCaseColumn((uint64_t)0));
		test("lower C5 value",!charstring::compare(
					c.getColumnName((uint64_t)0),"c5"));
		test("upper c5 success",c.upperCaseColumn("c5"));
		test("lower C5 success",c.lowerCaseColumn("C5"));
		c.upperCaseColumns();
		test("upper all columns C5",
			!charstring::compare(
				c.getColumnName((uint64_t)0),"C5"));
		test("upper all columns C4",
			!charstring::compare(
				c.getColumnName((uint64_t)1),"C4"));
		test("upper all columns C3",
			!charstring::compare(
				c.getColumnName((uint64_t)2),"C3"));
		test("upper all columns C2",
			!charstring::compare(
				c.getColumnName((uint64_t)3),"C2"));
		test("upper all columns C1",
			!charstring::compare(
				c.getColumnName((uint64_t)4),"C1"));
		c.lowerCaseColumns();
		test("lower all columns c5",
			!charstring::compare(
				c.getColumnName((uint64_t)0),"c5"));
		test("lower all columns c4",
			!charstring::compare(
				c.getColumnName((uint64_t)1),"c4"));
		test("lower all columns c3",
			!charstring::compare(
				c.getColumnName((uint64_t)2),"c3"));
		test("lower all columns c2",
			!charstring::compare(
				c.getColumnName((uint64_t)3),"c2"));
		test("lower all columns c1",
			!charstring::compare(
				c.getColumnName((uint64_t)4),"c1"));

		stdoutput.printf("row manipulation - %s...\n",
						(!i)?"string":"file");
		test("insert before 0",c.insertRowBefore((uint64_t)0));
		test("insert before 2",c.insertRowBefore(2));
		test("insert before 4",c.insertRowBefore(4));
		test("insert before 6",c.insertRowBefore(6));
		test("insert before 8",c.insertRowBefore(8));
		test("row count",c.getRowCount()==10);
		test("field 0,0",
			!charstring::compare(c.getField(0,(uint64_t)0),""));
		test("field 0,1",!charstring::compare(c.getField(0,1),""));
		test("field 0,2",!charstring::compare(c.getField(0,2),""));
		test("field 0,3",!charstring::compare(c.getField(0,3),""));
		test("field 0,4",!charstring::compare(c.getField(0,4),""));
		test("field 1,0",
			!charstring::compare(
					c.getField(1,(uint64_t)0),"val15"));
		test("field 1,1",!charstring::compare(c.getField(1,1),"val14"));
		test("field 1,2",!charstring::compare(c.getField(1,2),"val13"));
		test("field 1,3",!charstring::compare(c.getField(1,3),"val12"));
		test("field 1,4",!charstring::compare(c.getField(1,4),"val11"));

		test("field 2,0",!charstring::compare(
					c.getField(2,(uint64_t)0),""));
		test("field 2,1",!charstring::compare(c.getField(2,1),""));
		test("field 2,2",!charstring::compare(c.getField(2,2),""));
		test("field 2,3",!charstring::compare(c.getField(2,3),""));
		test("field 2,4",!charstring::compare(c.getField(2,4),""));
		test("delete row 0",c.deleteRow(0));
		test("row count",c.getRowCount()==9);
		test("field 0,0",!charstring::compare(
					c.getField(0,(uint64_t)0),"val15"));
		test("delete row 1",c.deleteRow(1));
		test("row count",c.getRowCount()==8);
		test("field 1,0",!charstring::compare(
					c.getField(1,(uint64_t)0),"val25"));
		test("delete row 6",c.deleteRow(6));
		test("row count",c.getRowCount()==7);
		test("field 6,0",!charstring::compare(
					c.getField(6,(uint64_t)0),"val55"));
		test("empty row 0",!c.getRowIsEmpty((uint64_t)0));
		test("empty row 1",!c.getRowIsEmpty((uint64_t)1));
		test("empty row 2",c.getRowIsEmpty((uint64_t)2));
		test("empty row 3",!c.getRowIsEmpty((uint64_t)3));
		test("empty row 4",c.getRowIsEmpty((uint64_t)4));
		test("empty row 5",!c.getRowIsEmpty((uint64_t)5));
		test("empty row 6",!c.getRowIsEmpty((uint64_t)6));
		test("delete empty rows",c.deleteEmptyRows());
		test("empty row 0",!c.getRowIsEmpty((uint64_t)0));
		test("empty row 1",!c.getRowIsEmpty((uint64_t)1));
		test("empty row 2",!c.getRowIsEmpty((uint64_t)2));
		test("empty row 3",!c.getRowIsEmpty((uint64_t)3));
		test("empty row 4",!c.getRowIsEmpty((uint64_t)4));
		test("move 4 -> 0",c.moveRow(4,0));
		test("field 0,0",!charstring::compare(
					c.getField(0,(uint64_t)0),"val55"));
		test("move 4 -> 1",c.moveRow(4,1));
		test("field 1,0",!charstring::compare(
					c.getField(1,(uint64_t)0),"val45"));
		test("move 4 -> 2",c.moveRow(4,2));
		test("field 2,0",!charstring::compare(
					c.getField(2,(uint64_t)0),"val35"));
		test("move 4 -> 3",c.moveRow(4,3));
		test("field 3,0",!charstring::compare(
					c.getField(3,(uint64_t)0),"val25"));
	}

	stdoutput.printf("carry value down...\n");
	c.setIgnoreHeaderLines(0);
	c.setIgnoreFooterLines(0);
	c.parseString(carrydown);
	bool	success=
		!charstring::compare(c.getField(0,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(0,1),"val2") &&
		!charstring::compare(c.getField(0,2),"val3") &&
		!charstring::compare(c.getField(0,3),"val4") &&
		!charstring::compare(c.getField(0,4),"val5") &&
		!charstring::compare(c.getField(1,(uint64_t)0),"") &&
		!charstring::compare(c.getField(1,1),"val2") &&
		!charstring::compare(c.getField(1,2),"") &&
		!charstring::compare(c.getField(1,3),"val4") &&
		!charstring::compare(c.getField(1,4),"") &&
		!charstring::compare(c.getField(2,(uint64_t)0),"") &&
		!charstring::compare(c.getField(2,1),"val2") &&
		!charstring::compare(c.getField(2,2),"") &&
		!charstring::compare(c.getField(2,3),"val4") &&
		!charstring::compare(c.getField(2,4),"") &&
		!charstring::compare(c.getField(3,(uint64_t)0),"") &&
		!charstring::compare(c.getField(3,1),"val2") &&
		!charstring::compare(c.getField(3,2),"") &&
		!charstring::compare(c.getField(3,3),"val4") &&
		!charstring::compare(c.getField(3,4),"") &&
		!charstring::compare(c.getField(4,(uint64_t)0),"") &&
		!charstring::compare(c.getField(4,1),"val2") &&
		!charstring::compare(c.getField(4,2),"") &&
		!charstring::compare(c.getField(4,3),"val4") &&
		!charstring::compare(c.getField(4,4),"") &&
		!charstring::compare(c.getField(5,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(5,1),"valb") &&
		!charstring::compare(c.getField(5,2),"valc") &&
		!charstring::compare(c.getField(5,3),"vald") &&
		!charstring::compare(c.getField(5,4),"vale") &&
		!charstring::compare(c.getField(6,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(6,1),"") &&
		!charstring::compare(c.getField(6,2),"valc") &&
		!charstring::compare(c.getField(6,3),"") &&
		!charstring::compare(c.getField(6,4),"vale") &&
		!charstring::compare(c.getField(7,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(7,1),"") &&
		!charstring::compare(c.getField(7,2),"valc") &&
		!charstring::compare(c.getField(7,3),"") &&
		!charstring::compare(c.getField(7,4),"vale") &&
		!charstring::compare(c.getField(8,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(8,1),"") &&
		!charstring::compare(c.getField(8,2),"valc") &&
		!charstring::compare(c.getField(8,3),"") &&
		!charstring::compare(c.getField(8,4),"vale") &&
		!charstring::compare(c.getField(9,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(9,1),"") &&
		!charstring::compare(c.getField(9,2),"valc") &&
		!charstring::compare(c.getField(9,3),"") &&
		!charstring::compare(c.getField(9,4),"vale");
	test("original state",success);
	c.carryValueDown((uint64_t)0);
	success=
		!charstring::compare(c.getField(0,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(0,1),"val2") &&
		!charstring::compare(c.getField(0,2),"val3") &&
		!charstring::compare(c.getField(0,3),"val4") &&
		!charstring::compare(c.getField(0,4),"val5") &&
		!charstring::compare(c.getField(1,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(1,1),"val2") &&
		!charstring::compare(c.getField(1,2),"") &&
		!charstring::compare(c.getField(1,3),"val4") &&
		!charstring::compare(c.getField(1,4),"") &&
		!charstring::compare(c.getField(2,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(2,1),"val2") &&
		!charstring::compare(c.getField(2,2),"") &&
		!charstring::compare(c.getField(2,3),"val4") &&
		!charstring::compare(c.getField(2,4),"") &&
		!charstring::compare(c.getField(3,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(3,1),"val2") &&
		!charstring::compare(c.getField(3,2),"") &&
		!charstring::compare(c.getField(3,3),"val4") &&
		!charstring::compare(c.getField(3,4),"") &&
		!charstring::compare(c.getField(4,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(4,1),"val2") &&
		!charstring::compare(c.getField(4,2),"") &&
		!charstring::compare(c.getField(4,3),"val4") &&
		!charstring::compare(c.getField(4,4),"") &&
		!charstring::compare(c.getField(5,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(5,1),"valb") &&
		!charstring::compare(c.getField(5,2),"valc") &&
		!charstring::compare(c.getField(5,3),"vald") &&
		!charstring::compare(c.getField(5,4),"vale") &&
		!charstring::compare(c.getField(6,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(6,1),"") &&
		!charstring::compare(c.getField(6,2),"valc") &&
		!charstring::compare(c.getField(6,3),"") &&
		!charstring::compare(c.getField(6,4),"vale") &&
		!charstring::compare(c.getField(7,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(7,1),"") &&
		!charstring::compare(c.getField(7,2),"valc") &&
		!charstring::compare(c.getField(7,3),"") &&
		!charstring::compare(c.getField(7,4),"vale") &&
		!charstring::compare(c.getField(8,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(8,1),"") &&
		!charstring::compare(c.getField(8,2),"valc") &&
		!charstring::compare(c.getField(8,3),"") &&
		!charstring::compare(c.getField(8,4),"vale") &&
		!charstring::compare(c.getField(9,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(9,1),"") &&
		!charstring::compare(c.getField(9,2),"valc") &&
		!charstring::compare(c.getField(9,3),"") &&
		!charstring::compare(c.getField(9,4),"vale");
	test("carry down 0",success);
	c.carryValueDown("col3");
	success=
		!charstring::compare(c.getField(0,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(0,1),"val2") &&
		!charstring::compare(c.getField(0,2),"val3") &&
		!charstring::compare(c.getField(0,3),"val4") &&
		!charstring::compare(c.getField(0,4),"val5") &&
		!charstring::compare(c.getField(1,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(1,1),"val2") &&
		!charstring::compare(c.getField(1,2),"val3") &&
		!charstring::compare(c.getField(1,3),"val4") &&
		!charstring::compare(c.getField(1,4),"") &&
		!charstring::compare(c.getField(2,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(2,1),"val2") &&
		!charstring::compare(c.getField(2,2),"val3") &&
		!charstring::compare(c.getField(2,3),"val4") &&
		!charstring::compare(c.getField(2,4),"") &&
		!charstring::compare(c.getField(3,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(3,1),"val2") &&
		!charstring::compare(c.getField(3,2),"val3") &&
		!charstring::compare(c.getField(3,3),"val4") &&
		!charstring::compare(c.getField(3,4),"") &&
		!charstring::compare(c.getField(4,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(4,1),"val2") &&
		!charstring::compare(c.getField(4,2),"val3") &&
		!charstring::compare(c.getField(4,3),"val4") &&
		!charstring::compare(c.getField(4,4),"") &&
		!charstring::compare(c.getField(5,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(5,1),"valb") &&
		!charstring::compare(c.getField(5,2),"valc") &&
		!charstring::compare(c.getField(5,3),"vald") &&
		!charstring::compare(c.getField(5,4),"vale") &&
		!charstring::compare(c.getField(6,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(6,1),"") &&
		!charstring::compare(c.getField(6,2),"valc") &&
		!charstring::compare(c.getField(6,3),"") &&
		!charstring::compare(c.getField(6,4),"vale") &&
		!charstring::compare(c.getField(7,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(7,1),"") &&
		!charstring::compare(c.getField(7,2),"valc") &&
		!charstring::compare(c.getField(7,3),"") &&
		!charstring::compare(c.getField(7,4),"vale") &&
		!charstring::compare(c.getField(8,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(8,1),"") &&
		!charstring::compare(c.getField(8,2),"valc") &&
		!charstring::compare(c.getField(8,3),"") &&
		!charstring::compare(c.getField(8,4),"vale") &&
		!charstring::compare(c.getField(9,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(9,1),"") &&
		!charstring::compare(c.getField(9,2),"valc") &&
		!charstring::compare(c.getField(9,3),"") &&
		!charstring::compare(c.getField(9,4),"vale");
	test("carry down col3",success);
	c.carryAllValuesDown();
	success=
		!charstring::compare(c.getField(0,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(0,1),"val2") &&
		!charstring::compare(c.getField(0,2),"val3") &&
		!charstring::compare(c.getField(0,3),"val4") &&
		!charstring::compare(c.getField(0,4),"val5") &&
		!charstring::compare(c.getField(1,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(1,1),"val2") &&
		!charstring::compare(c.getField(1,2),"val3") &&
		!charstring::compare(c.getField(1,3),"val4") &&
		!charstring::compare(c.getField(1,4),"val5") &&
		!charstring::compare(c.getField(2,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(2,1),"val2") &&
		!charstring::compare(c.getField(2,2),"val3") &&
		!charstring::compare(c.getField(2,3),"val4") &&
		!charstring::compare(c.getField(2,4),"val5") &&
		!charstring::compare(c.getField(3,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(3,1),"val2") &&
		!charstring::compare(c.getField(3,2),"val3") &&
		!charstring::compare(c.getField(3,3),"val4") &&
		!charstring::compare(c.getField(3,4),"val5") &&
		!charstring::compare(c.getField(4,(uint64_t)0),"val1") &&
		!charstring::compare(c.getField(4,1),"val2") &&
		!charstring::compare(c.getField(4,2),"val3") &&
		!charstring::compare(c.getField(4,3),"val4") &&
		!charstring::compare(c.getField(4,4),"val5") &&
		!charstring::compare(c.getField(5,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(5,1),"valb") &&
		!charstring::compare(c.getField(5,2),"valc") &&
		!charstring::compare(c.getField(5,3),"vald") &&
		!charstring::compare(c.getField(5,4),"vale") &&
		!charstring::compare(c.getField(6,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(6,1),"valb") &&
		!charstring::compare(c.getField(6,2),"valc") &&
		!charstring::compare(c.getField(6,3),"vald") &&
		!charstring::compare(c.getField(6,4),"vale") &&
		!charstring::compare(c.getField(7,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(7,1),"valb") &&
		!charstring::compare(c.getField(7,2),"valc") &&
		!charstring::compare(c.getField(7,3),"vald") &&
		!charstring::compare(c.getField(7,4),"vale") &&
		!charstring::compare(c.getField(8,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(8,1),"valb") &&
		!charstring::compare(c.getField(8,2),"valc") &&
		!charstring::compare(c.getField(8,3),"vald") &&
		!charstring::compare(c.getField(8,4),"vale") &&
		!charstring::compare(c.getField(9,(uint64_t)0),"vala") &&
		!charstring::compare(c.getField(9,1),"valb") &&
		!charstring::compare(c.getField(9,2),"valc") &&
		!charstring::compare(c.getField(9,3),"vald") &&
		!charstring::compare(c.getField(9,4),"vale");
	test("carry all down",success);

	stdoutput.printf("\n");
}
