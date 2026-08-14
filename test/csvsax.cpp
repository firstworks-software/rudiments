// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/csvsax.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include "test.cpp"

class nulltrackingcsvsax : public csvsax {
	public:
		nulltrackingcsvsax() : csvsax() {
			clear();
		}
		void clear() {
			fieldcount=0;
			embeddednulllength=0;
			embeddednullvalue.clear();
			embeddednullcolumnlength=0;
			embeddednullcolumnvalue.clear();
			recordcount=0;
		}
		bool column(const char *name, size_t namelength, bool quoted) {
			// remember the column name whose real length is greater
			// than its length as a null-terminated string, ie the
			// column name containing an embedded null byte
			if (namelength>charstring::getLength(name)) {
				embeddednullcolumnlength=namelength;
				embeddednullcolumnvalue.clear();
				embeddednullcolumnvalue.append(name,namelength);
			}
			return true;
		}
		bool field(const char *value, size_t valuelength, bool quoted) {
			fieldcount++;
			// remember the field whose real length is greater than
			// its length as a null-terminated string, ie the field
			// containing an embedded null byte
			if (valuelength>charstring::getLength(value)) {
				embeddednulllength=valuelength;
				embeddednullvalue.clear();
				embeddednullvalue.append(value,valuelength);
			}
			return true;
		}
		bool recordEnd() {
			recordcount++;
			return true;
		}
		uint64_t	fieldcount;
		size_t		embeddednulllength;
		stringbuffer	embeddednullvalue;
		size_t		embeddednullcolumnlength;
		stringbuffer	embeddednullcolumnvalue;
		uint64_t	recordcount;
};

class oldstylecsvsax : public csvsax {
	public:
		oldstylecsvsax() : csvsax(), fieldcount(0) {}
		bool field(const char *value, bool quoted) {
			fieldcount++;
			return true;
		}
		uint64_t	fieldcount;
};

int main() {

	header("csvsax");

	// a csv where the last field of the second record contains an
	// embedded null byte
	const char	header1[]="col1,col2,col3\n";
	const char	row1[]="val1,val2,val3\n";
	const char	row2part1[]="val4,val5,x";
	const char	embeddednull[]="\0";
	const char	row2part2[]="y\n";
	const char	row3[]="val7,val8,val9\n";

	stringbuffer	buf;
	buf.append(header1);
	buf.append(row1);
	buf.append(row2part1);
	buf.append(embeddednull,1);
	buf.append(row2part2);
	buf.append(row3);

	nulltrackingcsvsax	c;
	test("parse string with embedded null",
			c.parseString(buf.getString(),buf.getSize()));
	test("all 9 fields seen",c.fieldcount==9);
	test("all 3 records seen",c.recordcount==3);
	test("embedded-null field length",c.embeddednulllength==3);
	test("embedded-null field bytes",
			!charstring::compare(
				c.embeddednullvalue.getString(),"x\0y",3));

	// a subclass that overrides only the old, two-arg field() should
	// still work unchanged
	oldstylecsvsax	o;
	test("old-style field() still called",
			o.parseString(buf.getString(),buf.getSize()) &&
			o.fieldcount==9);

	// the same data, read back from a file
	file	f;
	test("create file",f.create("csvsaxtest.csv",
			permissions::parsePermString("rw-r--r--")));
	test("write to file",
			f.write(buf.getString(),buf.getSize())==
				(ssize_t)buf.getSize());
	f.close();

	nulltrackingcsvsax	fc;
	test("parse file with embedded null",fc.parseFile("csvsaxtest.csv"));
	test("all 9 fields seen (file)",fc.fieldcount==9);
	test("all 3 records seen (file)",fc.recordcount==3);
	test("embedded-null field length (file)",fc.embeddednulllength==3);
	test("embedded-null field bytes (file)",
			!charstring::compare(
				fc.embeddednullvalue.getString(),"x\0y",3));
	file::remove("csvsaxtest.csv");

	// a quoted column name and a quoted field value, each containing
	// an embedded null byte
	const char	qheader1[]="\"c";
	const char	qheader2[]="1\",col2\n";
	const char	qrow1[]="\"v";
	const char	qrow2[]="1\",val2\n";

	stringbuffer	qbuf;
	qbuf.append(qheader1);
	qbuf.append(embeddednull,1);
	qbuf.append(qheader2);
	qbuf.append(qrow1);
	qbuf.append(embeddednull,1);
	qbuf.append(qrow2);

	nulltrackingcsvsax	qc;
	test("parse string with embedded null in quoted values",
			qc.parseString(qbuf.getString(),qbuf.getSize()));
	test("quoted column name length",qc.embeddednullcolumnlength==3);
	test("quoted column name bytes",
			!charstring::compare(
				qc.embeddednullcolumnvalue.getString(),"c\0" "1",3));
	test("quoted field length",qc.embeddednulllength==3);
	test("quoted field bytes",
			!charstring::compare(
				qc.embeddednullvalue.getString(),"v\0" "1",3));
}
