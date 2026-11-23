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

class tracecsvsax : public csvsax {
	public:
		bool headerStart() {
			trace.append("hs;");
			return true;
		}
		bool column(const char *name, size_t namelength, bool quoted) {
			trace.append((quoted)?"cq(":"c(");
			trace.append(name,namelength)->append(");");
			return true;
		}
		bool headerEnd() {
			trace.append("he;");
			return true;
		}
		bool bodyStart() {
			trace.append("bs;");
			return true;
		}
		bool recordStart() {
			trace.append("rs;");
			return true;
		}
		bool field(const char *value, size_t valuelength, bool quoted) {
			trace.append((quoted)?"fq(":"f(");
			trace.append(value,valuelength)->append(");");
			return true;
		}
		bool recordEnd() {
			trace.append("re;");
			return true;
		}
		bool bodyEnd() {
			trace.append("be;");
			return true;
		}
		stringbuffer	trace;
};

// parses "input" as a string and as a file and checks that the callbacks
// were called in the "expected" order, with the "expected" values
void testTrace(const char *description,
			const char *input, const char *expected) {

	stringbuffer	name;

	tracecsvsax	s;
	name.append(description)->append(" (string)");
	test(name.getString(),s.parseString(input));
	name.append(" callbacks");
	test(name.getString(),
		!charstring::compare(s.trace.getString(),expected));

	file	f;
	test("create file",f.create("csvsaxtest.csv",
			permissions::parsePermString("rw-r--r--")));
	test("write to file",
		f.write(input,charstring::getLength(input))==
			(ssize_t)charstring::getLength(input));
	f.close();

	tracecsvsax	fs;
	name.clear();
	name.append(description)->append(" (file)");
	test(name.getString(),fs.parseFile("csvsaxtest.csv"));
	name.append(" callbacks");
	test(name.getString(),
		!charstring::compare(fs.trace.getString(),expected));
	file::remove("csvsaxtest.csv");
}

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

	// input that ends with no line end: the open column or field is
	// given and the header or record is ended as at a line end
	testTrace("last row has no line end",
		"name,phone\nAl,555\nBo,556",
		"hs;c(name);c(phone);he;bs;"
		"rs;f(Al);f(555);re;rs;f(Bo);f(556);re;be;");
	testTrace("crlf, last row has no line end",
		"name,phone\r\nAl,555\r\nBo,556",
		"hs;c(name);c(phone);he;bs;"
		"rs;f(Al);f(555);re;rs;f(Bo);f(556);re;be;");
	testTrace("quoted last field at end",
		"name,phone\nAl,555\nBo,\"556\"",
		"hs;c(name);c(phone);he;bs;"
		"rs;f(Al);f(555);re;rs;f(Bo);fq(556);re;be;");
	testTrace("empty last field at end",
		"name,phone\nAl,555\nBo,",
		"hs;c(name);c(phone);he;bs;"
		"rs;f(Al);f(555);re;rs;f(Bo);f();re;be;");
	testTrace("unclosed quote at end",
		"a,b\n1,\"2",
		"hs;c(a);c(b);he;bs;rs;f(1);fq(2);re;be;");
	testTrace("header only, no line end",
		"name,phone",
		"hs;c(name);c(phone);he;bs;be;");
	testTrace("header only, quoted last column, no line end",
		"name,\"phone\"",
		"hs;c(name);cq(phone);he;bs;be;");
	testTrace("header only, trailing comma, no line end",
		"name,",
		"hs;c(name);c();he;bs;be;");

	// input that ends with a line end: no extra record
	testTrace("last row ends with lf",
		"name,phone\nAl,555\nBo,556\n",
		"hs;c(name);c(phone);he;bs;"
		"rs;f(Al);f(555);re;rs;f(Bo);f(556);re;be;");
	testTrace("last row ends with crlf",
		"name,phone\r\nAl,555\r\nBo,556\r\n",
		"hs;c(name);c(phone);he;bs;"
		"rs;f(Al);f(555);re;rs;f(Bo);f(556);re;be;");
	testTrace("header only, lf",
		"name,phone\n",
		"hs;c(name);c(phone);he;bs;be;");
	testTrace("header only, crlf",
		"name,phone\r\n",
		"hs;c(name);c(phone);he;bs;be;");
	testTrace("header only, cr alone",
		"name,phone\r",
		"hs;c(name);c(phone);he;bs;be;");

	// empty input: no columns and no records
	testTrace("empty input","","hs;he;bs;be;");
}
