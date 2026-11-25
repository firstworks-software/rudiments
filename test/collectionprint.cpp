// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/scalar.h>
#include <rudiments/linkedlist.h>
#include <rudiments/singlylinkedlist.h>
#include <rudiments/avltree.h>
#include <rudiments/dictionary.h>
#include <rudiments/table.h>
#include <rudiments/dynamicarray.h>
#include <rudiments/staticarray.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/bytestring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/jsondom.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/locale.h>
#include <float.h>
#include "test.cpp"

const char *numbers[]={
	"zero","one","two","three","four","five","six","seven","eight","nine"
	//"0","1","2","3","4","5","6","7","8","9"
};

struct test {
	const char	*blah;
};

// Counts writes and fails call number "failat" and every call after it
// with "failcode".  If "shortwrite" is set, call number "failat" writes
// half of its data instead (none of a single byte) and returns that count,
// and the calls after it succeed.  Counts the calls made after the failure
// and records the size of call number "failat".
class failbuffer : public stringbuffer {
	public:
				failbuffer(size_t failat, bool shortwrite,
						ssize_t failcode=RESULT_ERROR);
		ssize_t	write(const byte_t *string, size_t size);
		ssize_t	write(const char *string, size_t length);
		size_t	writes;
		size_t	writesafterfail;
		size_t	failsize;
	private:
		ssize_t	failWrite(const byte_t *data, size_t size);
		size_t	failat;
		bool	shortwrite;
		ssize_t	failcode;
};

failbuffer::failbuffer(size_t failat, bool shortwrite, ssize_t failcode) :
								stringbuffer() {
	writes=0;
	writesafterfail=0;
	failsize=0;
	this->failat=failat;
	this->shortwrite=shortwrite;
	this->failcode=failcode;
}

ssize_t failbuffer::write(const byte_t *string, size_t size) {
	return failWrite(string,size);
}

ssize_t failbuffer::write(const char *string, size_t length) {
	return failWrite((const byte_t *)string,length);
}

ssize_t failbuffer::failWrite(const byte_t *data, size_t size) {
	writes++;
	if (failat && writes>failat) {
		writesafterfail++;
	}
	if (failat && writes==failat) {
		failsize=size;
	}
	if (failat && writes==failat && shortwrite) {
		return stringbuffer::write(data,size/2);
	}
	if (failat && writes>=failat && !shortwrite) {
		return failcode;
	}
	return stringbuffer::write(data,size);
}

// "wide" counts the output with getSize(), not getStringLength()
template <class collectiontype>
void testFailWrite(const char *name, collectiontype *c, bool wide=false) {

	for (uint16_t i=0; i<4; i++) {

		bool	indent=(i%2);
		bool	shortwrite=(i/2);

		stringbuffer	prefix;
		prefix.append(name);
		prefix.append((indent)?": writeJson indent":": writeJson");

		// count the writes of a clean run
		failbuffer	clean(0,shortwrite);
		ssize_t	cleanresult=c->writeJson(&clean,indent);
		size_t	total=clean.writes;
		stringbuffer	title;
		title.append(prefix.getString())->append(": clean run");
		test(title.getString(),
			cleanresult==(ssize_t)((wide)?clean.getSize():
						clean.getStringLength()) &&
			total>0);

		// fail or shorten each write
		for (size_t failat=1; failat<=total; failat++) {
			failbuffer	fb(failat,shortwrite);
			ssize_t	result=c->writeJson(&fb,indent);
			title.clear();
			title.append(prefix.getString());
			title.append((shortwrite)?
					": short write ":": fail write ");
			title.append((uint64_t)failat)->append(" of ");
			title.append((uint64_t)total);
			test(title.getString(),
				result==-1 && fb.writesafterfail==0);
		}
	}
}

const char *treemethods[]={
	"write","writeJson","writeJson indent","writeXml","writeXml indent"
};

template <class valuetype>
ssize_t writeTree(avltree<valuetype> *t, uint16_t method, output *out) {
	switch (method) {
		case 0:
			return t->write(out);
		case 1:
			return t->writeJson(out,false);
		case 2:
			return t->writeJson(out,true);
		case 3:
			return t->writeXml(out,false);
		default:
			return t->writeXml(out,true);
	}
}

// "expected" is the output of each of treemethods, or NULL to skip checking it
template <class valuetype>
void testTreeFailWrite(const char *name, avltree<valuetype> *t,
					const char * const *expected) {

	// hard error codes, 0 for a short write
	const ssize_t	codes[]={RESULT_ERROR,RESULT_TIMEOUT,RESULT_ABORT,0};

	for (uint16_t m=0; m<5; m++) {

		stringbuffer	prefix;
		prefix.append(name)->append(": ")->append(treemethods[m]);

		// count the writes of a clean run
		failbuffer	clean(0,false);
		ssize_t	cleanresult=writeTree(t,m,&clean);
		size_t	total=clean.writes;
		stringbuffer	title;
		title.append(prefix.getString())->append(": clean run");
		test(title.getString(),
			cleanresult==(ssize_t)clean.getStringLength() &&
			total>0 &&
			(!expected ||
			!charstring::compare(clean.getString(),expected[m])));

		// fail or shorten each write
		for (uint16_t c=0; c<4; c++) {
			bool	shortwrite=!codes[c];
			for (size_t failat=1; failat<=total; failat++) {
				failbuffer	fb(failat,shortwrite,
						(shortwrite)?RESULT_ERROR:codes[c]);
				ssize_t	result=writeTree(t,m,&fb);
				title.clear();
				title.append(prefix.getString());
				if (shortwrite) {
					title.append(": short write ");
				} else {
					title.append(": fail write (");
					title.append((int64_t)codes[c]);
					title.append(") ");
				}
				title.append((uint64_t)failat)->append(" of ");
				title.append((uint64_t)total);

				// A zero-byte write, such as an empty string
				// value, can't be made short, so the run must
				// complete.
				bool	success;
				if (shortwrite && !fb.failsize) {
					title.append(" (zero-byte)");
					success=(result==cleanresult);
				} else {
					success=(result==((shortwrite)?
							RESULT_ERROR:codes[c]) &&
						fb.writesafterfail==0);
				}
				test(title.getString(),success);
			}
		}
	}
}

// "expected" is the output of treemethods[method] as raw bytes, or NULL to
// skip checking it.  Each method converts a wide value to the character set
// of the locale and counts the output in bytes.
template <class valuetype>
void testWideWrite(const char *name, avltree<valuetype> *t, uint16_t method,
							bytebuffer *expected) {

	// hard error codes, 0 for a short write
	const ssize_t	codes[]={RESULT_ERROR,RESULT_TIMEOUT,RESULT_ABORT,0};

	stringbuffer	prefix;
	prefix.append(name)->append(": ")->append(treemethods[method]);

	// count the writes of a clean run
	failbuffer	clean(0,false);
	ssize_t	cleanresult=writeTree(t,method,&clean);
	size_t	total=clean.writes;
	stringbuffer	title;
	title.append(prefix.getString())->append(": bytes");
	test(title.getString(),
		!expected || (clean.getSize()==expected->getSize() &&
		!bytestring::compare(clean.getBuffer(),expected->getBuffer(),
							expected->getSize())));
	title.clear();
	title.append(prefix.getString())->append(": count");
	test(title.getString(),cleanresult==(ssize_t)clean.getSize());

	// fail or shorten each write
	for (uint16_t c=0; c<4; c++) {
		bool	shortwrite=!codes[c];
		for (size_t failat=1; failat<=total; failat++) {
			failbuffer	fb(failat,shortwrite,
					(shortwrite)?RESULT_ERROR:codes[c]);
			ssize_t	result=writeTree(t,method,&fb);
			title.clear();
			title.append(prefix.getString());
			if (shortwrite) {
				title.append(": short write ");
			} else {
				title.append(": fail write (");
				title.append((int64_t)codes[c]);
				title.append(") ");
			}
			title.append((uint64_t)failat)->append(" of ");
			title.append((uint64_t)total);
			test(title.getString(),
				result==((shortwrite)?RESULT_ERROR:codes[c]) &&
				fb.writesafterfail==0);
		}
	}
}

const char	*widefile="collectionprint.json";

// writes "c" to "out" with treemethods[method], where "method" is 0 (write)
// or 1 (writeJson)
template <class collectiontype>
ssize_t writeMethod(collectiontype *c, uint16_t method, output *out) {
	return (method)?c->writeJson(out,false):c->write(out);
}

// writes "t" to "out" with treemethods[method]
template <class valuetype>
ssize_t writeMethod(avltree<valuetype> *t, uint16_t method, output *out) {
	return writeTree(t,method,out);
}

// writes treemethods[method] of "c" to widefile, reads the file back into
// "contents" and "size", and returns the result of the method
template <class collectiontype>
ssize_t writeFile(collectiontype *c, uint16_t method,
					byte_t **contents, size_t *size) {
	*contents=NULL;
	*size=0;
	file::remove(widefile);
	file	f;
	ssize_t	result=RESULT_ERROR;
	if (f.create(widefile,permissions::parsePermString("rw-------"))) {
		result=writeMethod(c,method,&f);
		f.close();
		f.getContents(widefile,contents,size);
	}
	file::remove(widefile);
	return result;
}

// checks treemethods[method] of "c" to a file against "expected", or NULL to
// skip that check, and its return value against the size of the file in bytes
template <class collectiontype>
void testWideFile(const char *name, collectiontype *c, uint16_t method,
							bytebuffer *expected) {

	stringbuffer	prefix;
	prefix.append(name)->append(": ")->append(treemethods[method]);

	byte_t	*contents;
	size_t	size;
	ssize_t	result=writeFile(c,method,&contents,&size);
	stringbuffer	title;
	if (expected) {
		title.append(prefix.getString())->append(": file bytes");
		test(title.getString(),
			contents && size==expected->getSize() &&
			!bytestring::compare(contents,expected->getBuffer(),
							expected->getSize()));
		title.clear();
	}
	title.append(prefix.getString())->append(": file count");
	test(title.getString(),contents && result==(ssize_t)size);
	delete[] contents;
}

// checks treemethods[method] of "t" to a stringbuffer and to a file
template <class valuetype>
void testWideTree(const char *name, avltree<valuetype> *t, uint16_t method,
							bytebuffer *expected) {
	testWideWrite(name,t,method,expected);
	testWideFile(name,t,method,expected);
}

// sets "expected" to the output of treemethods[method], 0 (write) or 3
// (writeXml), of a tree that has the one value "value" of "size" bytes
void wideNode(bytebuffer *expected, uint16_t method,
				const char *value, size_t size) {
	expected->clear();
	expected->append("<t v=\"");
	expected->append(value,size);
	expected->append((method)?"\"/>\n":
				"\" lh=\"0\" rh=\"0\" bf=\"0\"/>\n");
}

// checks write(), writeJson(), and writeXml() of a tree that has one value:
// "value", converted to the character set of the locale by write() and
// writeXml(), and "jsonvalue" as writeJson() writes it
void testWideValue(const char *name, avltree<const wchar_t *> *t,
				const wchar_t *value, const char *jsonvalue) {

	stringbuffer	title;
	title.append(name)->append(": convert");
	char	*converted=NULL;
	ssize_t	length=charstring::printf(&converted,"%ls",value);
	test(title.getString(),length>=0);
	if (length<0) {
		delete[] converted;
		return;
	}

	bytebuffer	w;
	wideNode(&w,0,converted,length);
	testWideTree(name,t,0,&w);
	bytebuffer	x;
	wideNode(&x,3,converted,length);
	testWideTree(name,t,3,&x);
	delete[] converted;

	stringbuffer	j;
	j.append("[")->append(jsonvalue)->append("]");
	testWideWrite(name,t,1,&j);
	testWideFile(name,t,1,&j);
}

// a quote, a backslash, and a newline, as a raw C string and as a JSON string
#define ESCRAW "q\"b\\s\nl"
#define ESCENC "\"q\\\"b\\\\s\\u000al\""

// returns true if "n" or a node below it has the name "name", or NULL to skip
// that check, and a "v" attribute of "value"
bool hasNode(domnode *n, const char *name, const char *value) {
	if (n->isNullNode()) {
		return false;
	}
	if ((!name || !charstring::compare(n->getName(),name)) &&
		!charstring::compare(n->getAttributeValue("v"),value)) {
		return true;
	}
	for (domnode *c=n->getFirstTagChild();
			!c->isNullNode(); c=c->getNextTagSibling()) {
		if (hasNode(c,name,value)) {
			return true;
		}
	}
	return false;
}

// checks writeJson() of "c" against "expected" and "expectedindent", parses
// each with jsondom, and checks that the parsed tree has a node with the
// name "name", or NULL to skip that check, and the value "value"
template <class collectiontype>
void testEscape(const char *name, collectiontype *c,
				const char *expected, const char *expectedindent,
				const char *nodename, const char *value) {

	for (uint16_t i=0; i<2; i++) {

		bool	indent=(i==1);

		stringbuffer	prefix;
		prefix.append(name);
		prefix.append((indent)?": writeJson indent":": writeJson");

		stringbuffer	sb;
		ssize_t	result=c->writeJson(&sb,indent);
		stringbuffer	title;
		title.append(prefix.getString())->append(": output");
		test(title.getString(),
			!charstring::compare(sb.getString(),
					(indent)?expectedindent:expected));

		title.clear();
		title.append(prefix.getString())->append(": return value");
		test(title.getString(),result==(ssize_t)sb.getStringLength());

		jsondom	j;
		title.clear();
		title.append(prefix.getString())->append(": parses");
		test(title.getString(),
			j.parseString(sb.getString()) &&
			hasNode(j.getRootNode(),nodename,value));
	}
}

// checks writeJson() of "c" to a stringbuffer and to a file against
// "expected", and its return value against the size of the output in bytes.
// A wide key or value is converted to the character set of the locale.
template <class collectiontype>
void testWideJson(const char *name, collectiontype *c, const char *expected) {

	stringbuffer	prefix;
	prefix.append(name)->append(": writeJson");

	stringbuffer	sb;
	ssize_t	result=c->writeJson(&sb,false);
	size_t	expectedsize=charstring::getLength(expected);
	stringbuffer	title;
	title.append(prefix.getString())->append(": stringbuffer bytes");
	test(title.getString(),
		sb.getSize()==expectedsize &&
		!bytestring::compare(sb.getBuffer(),expected,expectedsize));
	title.clear();
	title.append(prefix.getString())->append(": stringbuffer count");
	test(title.getString(),result==(ssize_t)sb.getSize());

	stringbuffer	e;
	e.append(expected);
	testWideFile(name,c,1,&e);

	testFailWrite(name,c,true);
}

// checks that treemethods[method] of "c" to a stringbuffer and to a file fails
// on a wide character that the locale cannot convert, and writes only
// "expected"
template <class collectiontype>
void testWideError(const char *name, collectiontype *c, uint16_t method,
						const char *expected) {

	stringbuffer	prefix;
	prefix.append(name)->append(": ")->append(treemethods[method]);

	stringbuffer	sb;
	ssize_t	result=writeMethod(c,method,&sb);
	size_t	expectedsize=charstring::getLength(expected);
	stringbuffer	title;
	title.append(prefix.getString())->append(": stringbuffer bytes");
	test(title.getString(),
		sb.getSize()==expectedsize &&
		!bytestring::compare(sb.getBuffer(),expected,expectedsize));
	title.clear();
	title.append(prefix.getString())->append(": stringbuffer error");
	test(title.getString(),result==RESULT_ERROR);

	byte_t	*contents;
	size_t	size;
	result=writeFile(c,method,&contents,&size);
	title.clear();
	title.append(prefix.getString())->append(": file bytes");
	test(title.getString(),
		contents && size==expectedsize &&
		!bytestring::compare(contents,expected,expectedsize));
	title.clear();
	title.append(prefix.getString())->append(": file error");
	test(title.getString(),result==RESULT_ERROR);
	delete[] contents;
}

// set a UTF-8 character type locale, if the host has one
static bool setUtf8CType() {
	const char	*names[]={
		"C.UTF-8","C.utf8","en_US.UTF-8","en_US.utf8",
	#ifdef _WIN32
		".UTF8",
	#endif
		NULL
	};
	for (const char **name=names; *name; name++) {
		if (locale::setCType(*name)) {
			return true;
		}
	}
	return false;
}

// strings that look like numbers: only a json number is written bare
const char *numstrs[]={
	"007","02134",".5","-.5","0","-0","12","1.5","-1.5"
};
const wchar_t *wnumstrs[]={
	L"007",L"02134",L".5",L"-.5",L"0",L"-0",L"12",L"1.5",L"-1.5"
};
const char *numjson[]={
	"\"007\"","\"02134\"","\".5\"","\"-.5\"",
	"0","-0","12","1.5","-1.5"
};
#define NUMSTRJSON \
	"[\"007\",\"02134\",\".5\",\"-.5\",0,-0,12,1.5,-1.5]"
#define NUMSTRJSONINDENT \
	"[\n\t\"007\",\n\t\"02134\",\n\t\".5\",\n\t\"-.5\",\n" \
	"\t0,\n\t-0,\n\t12,\n\t1.5,\n\t-1.5\n]\n"

// checks writeJson() of a list of numstrs, with values of "valuetype"
template <class valuetype>
void testNumericStrings(const char *name) {
	linkedlist<valuetype>	ll;
	for (uint16_t i=0; i<sizeof(numstrs)/sizeof(numstrs[0]); i++) {
		ll.append((valuetype)numstrs[i]);
	}
	testEscape(name,&ll,NUMSTRJSON,NUMSTRJSONINDENT,"v","02134");
	testEscape(name,&ll,NUMSTRJSON,NUMSTRJSONINDENT,"v","-1.5");
	testFailWrite(name,&ll);
}

// checks that nan, inf, and -inf values of "floattype" are written as null,
// and that keys keep the text that "format" gives them
template <class floattype>
void testNonFinite(const char *name, const char *format) {

	volatile floattype	zero=0;
	floattype		nan=zero/zero;
	floattype		inf=1/zero;

	stringbuffer	title;
	title.append("linkedlist (")->append(name)->append(")");
	linkedlist<floattype>	ll;
	ll.append(nan);
	ll.append(inf);
	ll.append(-inf);
	ll.append(1.5);
	testEscape(title.getString(),&ll,
			"[null,null,null,1.5]",
			"[\n\tnull,\n\tnull,\n\tnull,\n\t1.5\n]\n",
			"v","1.5");
	testFailWrite(title.getString(),&ll);

	title.clear();
	title.append("avltree (")->append(name)->append(")");
	avltree<floattype>	at;
	at.insert(inf);
	at.insert(1.5);
	at.insert(-inf);
	testEscape(title.getString(),&at,
			"[1.5,[null],[null]]",
			"[\n  1.5,\n  [\n    null\n  ],\n"
			"  [\n    null\n  ]\n]\n",
			"v","1.5");
	testTreeFailWrite(title.getString(),&at,NULL);

	title.clear();
	title.append("dictionary (const char *, ")->append(name)->append(")");
	dictionary<const char *,floattype>	dv;
	dv.setValue("n",nan);
	dv.setValue("i",inf);
	dv.setValue("m",-inf);
	dv.setValue("x",1.5);
	testEscape(title.getString(),&dv,
			"{\"n\":null,\"i\":null,\"m\":null,\"x\":1.5}",
			"{\n\t\"n\":null,\n\t\"i\":null,\n\t\"m\":null,\n"
			"\t\"x\":1.5\n}\n",
			"x","1.5");
	testFailWrite(title.getString(),&dv);

	// keys are written with printf text, not as null

	stringbuffer	nantext;
	nantext.printf(format,nan);
	stringbuffer	expected;
	expected.append("{\"")->append(nantext.getString());
	expected.append("\":\"v\"}");
	stringbuffer	expectedindent;
	expectedindent.append("{\n\t\"")->append(nantext.getString());
	expectedindent.append("\":\"v\"\n}\n");
	title.clear();
	title.append("dictionary (")->append(name)->append(" nan key)");
	dictionary<floattype,const char *>	dn;
	dn.setValue(nan,"v");
	testEscape(title.getString(),&dn,
			expected.getString(),expectedindent.getString(),
			nantext.getString(),"v");
	testFailWrite(title.getString(),&dn);

	stringbuffer	inftext;
	inftext.printf(format,inf);
	stringbuffer	minftext;
	minftext.printf(format,-inf);
	expected.clear();
	expected.append("{\"")->append(inftext.getString());
	expected.append("\":1,\"")->append(minftext.getString());
	expected.append("\":2}");
	expectedindent.clear();
	expectedindent.append("{\n\t\"")->append(inftext.getString());
	expectedindent.append("\":1,\n\t\"")->append(minftext.getString());
	expectedindent.append("\":2\n}\n");
	title.clear();
	title.append("dictionary (")->append(name)->append(" inf keys)");
	dictionary<floattype,int32_t>	di;
	di.setValue(inf,1);
	di.setValue(-inf,2);
	testEscape(title.getString(),&di,
			expected.getString(),expectedindent.getString(),
			inftext.getString(),"1");
	testEscape(title.getString(),&di,
			expected.getString(),expectedindent.getString(),
			minftext.getString(),"2");
	testFailWrite(title.getString(),&di);
}

// checks write() of "c" against "expected" and its return value
template <class collectiontype>
void testWrite(const char *name, collectiontype *c, const char *expected) {
	stringbuffer	sb;
	ssize_t		result=c->write(&sb);
	stringbuffer	title;
	title.append(name)->append(": write output");
	test(title.getString(),
		!charstring::compare(sb.getString(),expected));
	title.clear();
	title.append(name)->append(": write return value");
	test(title.getString(),result==(ssize_t)sb.getStringLength());
}

// checks that the keys "a" and "b" of "floattype", which differ, are written
// as the different member names "atext" and "btext" by write() and writeJson()
template <class floattype>
void testFloatKeys(const char *name, floattype a, floattype b,
				const char *atext, const char *btext) {

	stringbuffer	title;
	title.append(name)->append(" keys differ");
	test(title.getString(),charstring::compare(atext,btext));

	stringbuffer	expected;
	expected.append("{\"")->append(atext)->append("\":1,\"");
	expected.append(btext)->append("\":2}");
	stringbuffer	expectedindent;
	expectedindent.append("{\n\t\"")->append(atext)->append("\":1,\n\t\"");
	expectedindent.append(btext)->append("\":2\n}\n");

	dictionary<floattype,int32_t>	d;
	d.setValue(a,1);
	d.setValue(b,2);
	testEscape(name,&d,expected.getString(),expectedindent.getString(),
								atext,"1");
	testEscape(name,&d,expected.getString(),expectedindent.getString(),
								btext,"2");
	testWrite(name,&d,expectedindent.getString());
	testFailWrite(name,&d);
}

// checks that "value" of "floattype" is written by write() and writeJson() as
// the text that "format" gives it, and that the text reads back as "value"
template <class floattype>
void testFloatValue(const char *name, floattype value, const char *format) {

	stringbuffer	text;
	text.printf(format,value);

	stringbuffer	title;
	title.append(name)->append(": reads back");
	test(title.getString(),
		(floattype)charstring::convertToFloat(text.getString())==value);

	stringbuffer	expected;
	expected.append("[")->append(text.getString())->append("]");
	stringbuffer	expectedindent;
	expectedindent.append("[\n\t")->append(text.getString());
	expectedindent.append("\n]\n");

	linkedlist<floattype>	ll;
	ll.append(value);
	testEscape(name,&ll,expected.getString(),expectedindent.getString(),
							"v",text.getString());
	testWrite(name,&ll,expectedindent.getString());
	testFailWrite(name,&ll);
}

int main(int argc, const char **argv) {

	header("collectionprint");

	stdoutput.printf("scalar (bool):\n");
	scalar<bool>	sb;
	sb.setValue(true);
	sb.write();
	sb.setValue(false);
	sb.write();
	stdoutput.printf("\n");

	stdoutput.printf("scalar (null/non-null):\n");
	scalar<const char *>	sn;
	sn.setValue(NULL);
	sn.write();
	sn.setValue("not-null");
	sn.write();
	stdoutput.printf("\n");

	stdoutput.printf("scalar (int32_t):\n");
	scalar<int32_t>	si32;
	for (int32_t i=-5; i<5; i++) {
		si32.setValue(i);
		si32.write();
	}
	stdoutput.printf("\n");

	stdoutput.printf("scalar (const char *):\n");
	scalar<const char *>	sc;
	for (uint32_t i=0; i<10; i++) {
		sc.setValue(numbers[i]);
		sc.write();
	}
	stdoutput.printf("\n");



	stdoutput.printf("linkedlist (int32_t):\n");
	linkedlist<int32_t>	lli32;
	for (int32_t i=-5; i<5; i++) {
		lli32.append(i);
	}
	lli32.write();
	stdoutput.printf("\n");

	stdoutput.printf("linkedlist (const char *):\n");
	linkedlist<const char *>	llc;
	for (uint32_t i=0; i<10; i++) {
		llc.append(numbers[i]);
	}
	llc.write();
	stdoutput.printf("\n");



	stdoutput.printf("singlylinkedlist (int32_t):\n");
	singlylinkedlist<int32_t>	slli32;
	for (int32_t i=-5; i<5; i++) {
		slli32.append(i);
	}
	slli32.write();
	stdoutput.printf("\n");

	stdoutput.printf("singlylinkedlist (const char *):\n");
	singlylinkedlist<const char *>	sllc;
	for (uint32_t i=0; i<10; i++) {
		sllc.append(numbers[i]);
	}
	sllc.write();
	stdoutput.printf("\n");



	stdoutput.printf("avltree (int32_t):\n");
	avltree<int32_t>	ati32;
	for (int32_t i=-5; i<5; i++) {
		ati32.insert(i);
	}
	ati32.write();
	ati32.writeXml();
	ati32.writeJson();
	stdoutput.printf("\n");

	stdoutput.printf("avltree (const char *):\n");
	avltree<const char *>	atc;
	for (uint32_t i=0; i<10; i++) {
		atc.insert(numbers[i]);
	}
	atc.write();
	atc.writeXml();
	atc.writeJson();
	stdoutput.printf("\n");



	stdoutput.printf("dictionary (int32_t, const char *):\n");
	dictionary<int32_t,const char *>	d;
	for (int32_t i=0; i<10; i++) {
		d.setValue(i,numbers[i]);
	}
	d.write();
	stdoutput.printf("\n");



	stdoutput.printf("table (int32_t):\n");
	table<int32_t>	ti32;
	for (int32_t i=0; i<10; i++) {
		ti32.setColumnName(i,numbers[i]);
		for (int32_t j=0; j<10; j++) {
			ti32.setValue(i,j,j);
		}
	}
	ti32.write();
	stdoutput.printf("\n");

	stdoutput.printf("table (const char *):\n");
	table<const char *>	tc;
	for (uint32_t i=0; i<10; i++) {
		tc.setColumnName(i,numbers[i]);
		for (int32_t j=0; j<10; j++) {
			tc.setValue(i,j,numbers[j]);
		}
	}
	tc.write();
	stdoutput.printf("\n");



	stdoutput.printf("dynamicarray (int32_t):\n");
	dynamicarray<int32_t>	dai32;
	for (int32_t i=0; i<10; i++) {
		dai32[i]=i;
	}
	dai32.write();
	stdoutput.printf("\n");



	stdoutput.printf("dynamicarray (const char *):\n");
	dynamicarray<const char *>	dac;
	for (int32_t i=0; i<10; i++) {
		dac[i]=numbers[i];
	}
	dac.write();
	stdoutput.printf("\n");



	stdoutput.printf("dynamicarray (struct test):\n");
	dynamicarray<struct test>	dt;
	for (uint32_t i=0; i<10; i++) {
		dt[i].blah=NULL;
	}
	dt.write();
	stdoutput.printf("\n");



	stdoutput.printf("staticarray (int32_t):\n");
	staticarray<int32_t,10>	sai32;
	for (int32_t i=0; i<10; i++) {
		sai32[i]=i;
	}
	sai32.write();
	stdoutput.printf("\n");



	stdoutput.printf("staticarray (const char *):\n");
	staticarray<const char *,10>	sac;
	for (int32_t i=0; i<10; i++) {
		sac[i]=numbers[i];
	}
	sac.write();
	stdoutput.printf("\n");



	stdoutput.printf("staticarray (struct test):\n");
	staticarray<struct test, 10>	st;
	st.write();
	stdoutput.printf("\n");

	stdoutput.printf("nested staticarray (int32_t):\n");
	staticarray<staticarray<int32_t,10>,10>	nsai32;
	for (int32_t i=0; i<10; i++) {
		for (int32_t j=0; j<10; j++) {
			nsai32[i][j]=j;
		}
	}
	nsai32.write();
	stdoutput.printf("\n");



	// write errors and short writes
	stdoutput.printf("write errors...\n");

	testFailWrite("scalar (int32_t)",&si32);
	testFailWrite("scalar (const char *)",&sc);
	sn.setValue(NULL);
	testFailWrite("scalar (null)",&sn);

	testFailWrite("dynamicarray (int32_t)",&dai32);
	testFailWrite("dynamicarray (const char *)",&dac);
	dynamicarray<int32_t>	edai32;
	testFailWrite("dynamicarray (empty)",&edai32);

	staticarray<int32_t,3>	sai32s;
	for (int32_t i=0; i<3; i++) {
		sai32s[i]=i;
	}
	testFailWrite("staticarray (int32_t)",&sai32s);

	testFailWrite("linkedlist (int32_t)",&lli32);
	testFailWrite("linkedlist (const char *)",&llc);
	linkedlist<int32_t>	elli32;
	testFailWrite("linkedlist (empty)",&elli32);

	testFailWrite("singlylinkedlist (int32_t)",&slli32);
	singlylinkedlist<int32_t>	eslli32;
	testFailWrite("singlylinkedlist (empty)",&eslli32);

	testFailWrite("dictionary (int32_t, const char *)",&d);
	dictionary<int32_t,const char *>	ed;
	testFailWrite("dictionary (empty)",&ed);

	testTreeFailWrite("avltree (int32_t)",&ati32,NULL);
	testTreeFailWrite("avltree (const char *)",&atc,NULL);
	avltree<const char *>	atce;
	atce.insert("b");
	atce.insert("");
	atce.insert("c");
	testTreeFailWrite("avltree (empty string)",&atce,NULL);
	avltree<int32_t>	ati32s;
	ati32s.insert(7);
	const char * const	sexpected[]={
		"<t v=\"7\" lh=\"0\" rh=\"0\" bf=\"0\"/>\n",
		"[7]","[\n  7\n]\n","<t v=\"7\"/>\n","<t v=\"7\"/>\n"
	};
	testTreeFailWrite("avltree (single)",&ati32s,sexpected);
	avltree<uint16_t>	atu0;
	atu0.insert((uint16_t)-1);
	const char * const	umexpected0[]={
		"<t v=\"65535\" lh=\"0\" rh=\"0\" bf=\"0\"/>\n",
		"[65535]","[\n  65535\n]\n","<t v=\"65535\"/>\n",
		"<t v=\"65535\"/>\n"
	};
	testTreeFailWrite("avltree (uint16_t max)",&atu0,umexpected0);
	avltree<uint32_t>	atu1;
	atu1.insert((uint32_t)-1);
	const char * const	umexpected1[]={
		"<t v=\"4294967295\" lh=\"0\" rh=\"0\" bf=\"0\"/>\n",
		"[4294967295]","[\n  4294967295\n]\n","<t v=\"4294967295\"/>\n",
		"<t v=\"4294967295\"/>\n"
	};
	testTreeFailWrite("avltree (uint32_t max)",&atu1,umexpected1);
	avltree<uint64_t>	atu2;
	atu2.insert((uint64_t)-1);
	const char * const	umexpected2[]={
		"<t v=\"18446744073709551615\" lh=\"0\" rh=\"0\" bf=\"0\"/>\n",
		"[18446744073709551615]","[\n  18446744073709551615\n]\n","<t v=\"18446744073709551615\"/>\n",
		"<t v=\"18446744073709551615\"/>\n"
	};
	testTreeFailWrite("avltree (uint64_t max)",&atu2,umexpected2);
	avltree<int32_t>	eati32;
	const char * const	eexpected[]={
		"<t/>\n","[]","[\n]\n","<t/>\n","<t/>\n"
	};
	testTreeFailWrite("avltree (empty)",&eati32,eexpected);

	stdoutput.printf("\n");

	// a quote, a backslash, and a newline in values and keys
	stdoutput.printf("escaped values...\n");

	linkedlist<const char *>	elle;
	elle.append(ESCRAW);
	elle.append("plain");
	elle.append("42");
	elle.append((const char *)NULL);
	testEscape("linkedlist (const char *)",&elle,
			"[" ESCENC ",\"plain\",42,null]",
			"[\n\t" ESCENC ",\n\t\"plain\",\n\t42,\n\tnull\n]\n",
			"v",ESCRAW);
	testFailWrite("linkedlist (escaped)",&elle);

	linkedlist<uint16_t>	llu0;
	llu0.append((uint16_t)-1);
	testEscape("linkedlist (uint16_t max)",&llu0,
			"[65535]","[\n\t65535\n]\n","v","65535");
	testFailWrite("linkedlist (uint16_t max)",&llu0);
	linkedlist<uint32_t>	llu1;
	llu1.append((uint32_t)-1);
	testEscape("linkedlist (uint32_t max)",&llu1,
			"[4294967295]","[\n\t4294967295\n]\n","v","4294967295");
	testFailWrite("linkedlist (uint32_t max)",&llu1);
	linkedlist<uint64_t>	llu2;
	llu2.append((uint64_t)-1);
	testEscape("linkedlist (uint64_t max)",&llu2,
			"[18446744073709551615]","[\n\t18446744073709551615\n]\n","v","18446744073709551615");
	testFailWrite("linkedlist (uint64_t max)",&llu2);

	dynamicarray<const char *>	edae;
	edae[0]=ESCRAW;
	edae[1]="plain";
	edae[2]="42";
	edae[3]=NULL;
	testEscape("dynamicarray (const char *)",&edae,
			"[" ESCENC ",\"plain\",42,null]",
			"[\n\t" ESCENC ",\n\t\"plain\",\n\t42,\n\tnull\n]\n",
			"v",ESCRAW);
	testFailWrite("dynamicarray (escaped)",&edae);

	dictionary<const char *,const char *>	ede;
	ede.setValue(ESCRAW,ESCRAW);
	ede.setValue("plain","42");
	ede.setValue("n",(const char *)NULL);
	const char	*edeexpected=
			"{" ESCENC ":" ESCENC ",\"plain\":42,\"n\":null}";
	const char	*edeindentexpected=
			"{\n\t" ESCENC ":" ESCENC ",\n"
			"\t\"plain\":42,\n\t\"n\":null\n}\n";
	testEscape("dictionary (escaped key and value)",&ede,
			edeexpected,edeindentexpected,ESCRAW,ESCRAW);
	testEscape("dictionary (plain key)",&ede,
			edeexpected,edeindentexpected,"plain","42");
	testFailWrite("dictionary (escaped)",&ede);

	// keys that are not strings are written as strings
	dictionary<int32_t,const char *>	edi;
	edi.setValue(1,"a");
	testEscape("dictionary (int32_t key)",&edi,
			"{\"1\":\"a\"}","{\n\t\"1\":\"a\"\n}\n",
			"1","a");
	testFailWrite("dictionary (int32_t key)",&edi);

	dictionary<const char *,int32_t>	edk;
	edk.setValue("x",1);
	edk.setValue("42",2);
	edk.setValue((const char *)NULL,3);
	const char	*edkexpected="{\"x\":1,\"42\":2,\"null\":3}";
	const char	*edkindentexpected=
			"{\n\t\"x\":1,\n\t\"42\":2,\n\t\"null\":3\n}\n";
	testEscape("dictionary (string key)",&edk,
			edkexpected,edkindentexpected,"x","1");
	testEscape("dictionary (numeric string key)",&edk,
			edkexpected,edkindentexpected,"42","2");
	testEscape("dictionary (NULL key)",&edk,
			edkexpected,edkindentexpected,"null","3");
	testFailWrite("dictionary (string, numeric, and NULL keys)",&edk);

	dictionary<char,int32_t>	edc;
	edc.setValue('7',1);
	edc.setValue('x',2);
	const char	*edcexpected="{\"7\":1,\"x\":2}";
	const char	*edcindentexpected="{\n\t\"7\":1,\n\t\"x\":2\n}\n";
	testEscape("dictionary (digit char key)",&edc,
			edcexpected,edcindentexpected,"7","1");
	testEscape("dictionary (char key)",&edc,
			edcexpected,edcindentexpected,"x","2");
	testFailWrite("dictionary (char key)",&edc);

	dictionary<bool,int32_t>	edb;
	edb.setValue(true,1);
	edb.setValue(false,2);
	const char	*edbexpected="{\"true\":1,\"false\":2}";
	const char	*edbindentexpected=
			"{\n\t\"true\":1,\n\t\"false\":2\n}\n";
	testEscape("dictionary (bool key)",&edb,
			edbexpected,edbindentexpected,"true","1");
	testEscape("dictionary (false bool key)",&edb,
			edbexpected,edbindentexpected,"false","2");
	testFailWrite("dictionary (bool key)",&edb);

	dictionary<double,int32_t>	edd;
	edd.setValue(1.5,1);
	testEscape("dictionary (double key)",&edd,
			"{\"1.5\":1}","{\n\t\"1.5\":1\n}\n",
			"1.5","1");
	testFailWrite("dictionary (double key)",&edd);

	// float keys that are close together are written as different names
	stringbuffer	fa;
	stringbuffer	fb;
	fa.printf("%.9g",(double)1e-7f);
	fb.printf("%.9g",(double)1.0000001e-7f);
	testFloatKeys<float>("dictionary (float colliding keys)",
				1e-7f,1.0000001e-7f,
				fa.getString(),fb.getString());

	stringbuffer	da;
	stringbuffer	db;
	da.printf("%.17g",1e-7);
	db.printf("%.17g",2e-7);
	testFloatKeys<double>("dictionary (double colliding keys)",
				1e-7,2e-7,da.getString(),db.getString());

	stringbuffer	ldfmt;
	ldfmt.append("%.")->append((int32_t)(2+LDBL_MANT_DIG*30103/100000));
	ldfmt.append("Lg");
	stringbuffer	la;
	stringbuffer	lb;
	la.printf(ldfmt.getString(),1e-7L);
	lb.printf(ldfmt.getString(),2e-7L);
	testFloatKeys<long double>("dictionary (long double colliding keys)",
				1e-7L,2e-7L,la.getString(),lb.getString());

	// int8_t keys and values are written as numbers
	dictionary<int8_t,int32_t>	ed8;
	ed8.setValue(1,10);
	ed8.setValue(-2,20);
	ed8.setValue(127,30);
	ed8.setValue(-128,40);
	const char	*ed8expected=
			"{\"1\":10,\"-2\":20,\"127\":30,\"-128\":40}";
	const char	*ed8indentexpected=
			"{\n\t\"1\":10,\n\t\"-2\":20,\n\t\"127\":30,\n"
			"\t\"-128\":40\n}\n";
	testEscape("dictionary (int8_t key)",&ed8,
			ed8expected,ed8indentexpected,"1","10");
	testEscape("dictionary (negative int8_t key)",&ed8,
			ed8expected,ed8indentexpected,"-2","20");
	testEscape("dictionary (max int8_t key)",&ed8,
			ed8expected,ed8indentexpected,"127","30");
	testEscape("dictionary (min int8_t key)",&ed8,
			ed8expected,ed8indentexpected,"-128","40");
	testWrite("dictionary (int8_t key)",&ed8,ed8indentexpected);
	testFailWrite("dictionary (int8_t key)",&ed8);

	dictionary<int32_t,int8_t>	ed8v;
	ed8v.setValue(1,5);
	ed8v.setValue(2,-3);
	testEscape("dictionary (int8_t values)",&ed8v,
			"{\"1\":5,\"2\":-3}","{\n\t\"1\":5,\n\t\"2\":-3\n}\n",
			"2","-3");
	testWrite("dictionary (int8_t values)",&ed8v,
			"{\n\t\"1\":5,\n\t\"2\":-3\n}\n");
	testFailWrite("dictionary (int8_t values)",&ed8v);

	linkedlist<int8_t>	ll8;
	ll8.append(5);
	ll8.append(-3);
	testEscape("linkedlist (int8_t)",&ll8,
			"[5,-3]","[\n\t5,\n\t-3\n]\n","v","-3");
	testWrite("linkedlist (int8_t)",&ll8,"[\n\t5,\n\t-3\n]\n");
	testFailWrite("linkedlist (int8_t)",&ll8);

	// NULL and "null" are written as the same name, and "" as an empty name
	dictionary<const char *,int32_t>	edn;
	edn.setValue((const char *)NULL,1);
	edn.setValue("null",2);
	edn.setValue("",3);
	testEscape("dictionary (NULL, \"null\" and empty keys)",&edn,
			"{\"null\":1,\"null\":2,\"\":3}",
			"{\n\t\"null\":1,\n\t\"null\":2,\n\t\"\":3\n}\n",
			"","3");
	testWrite("dictionary (NULL, \"null\" and empty keys)",&edn,
			"{\n\t\"null\":1,\n\t\"null\":2,\n\t\"\":3\n}\n");
	testFailWrite("dictionary (NULL, \"null\" and empty keys)",&edn);

	dictionary<uint16_t,int32_t>	edu0;
	edu0.setValue((uint16_t)-1,1);
	testEscape("dictionary (uint16_t max key)",&edu0,
			"{\"65535\":1}","{\n\t\"65535\":1\n}\n",
			"65535","1");
	testFailWrite("dictionary (uint16_t max key)",&edu0);

	dictionary<uint32_t,int32_t>	edu1;
	edu1.setValue((uint32_t)-1,1);
	testEscape("dictionary (uint32_t max key)",&edu1,
			"{\"4294967295\":1}","{\n\t\"4294967295\":1\n}\n",
			"4294967295","1");
	testFailWrite("dictionary (uint32_t max key)",&edu1);

	dictionary<uint64_t,int32_t>	edu2;
	edu2.setValue((uint64_t)-1,1);
	testEscape("dictionary (uint64_t max key)",&edu2,
			"{\"18446744073709551615\":1}","{\n\t\"18446744073709551615\":1\n}\n",
			"18446744073709551615","1");
	testFailWrite("dictionary (uint64_t max key)",&edu2);

	avltree<const char *>	ate;
	ate.insert(ESCRAW);
	ate.insert("plain");
	ate.insert("42");
	testEscape("avltree (const char *)",&ate,
			"[\"plain\",[42],[" ESCENC "]]",
			"[\n  \"plain\",\n  [\n    42\n  ],\n"
			"  [\n    " ESCENC "\n  ]\n]\n",
			"v",ESCRAW);
	testTreeFailWrite("avltree (escaped)",&ate,NULL);

	linkedlist<char>	ellc;
	ellc.append('"');
	ellc.append('\\');
	ellc.append('\n');
	ellc.append('7');
	ellc.append('a');
	testEscape("linkedlist (char)",&ellc,
			"[\"\\\"\",\"\\\\\",\"\\u000a\",7,\"a\"]",
			"[\n\t\"\\\"\",\n\t\"\\\\\",\n\t\"\\u000a\",\n"
			"\t7,\n\t\"a\"\n]\n",
			"v","\"");
	testFailWrite("linkedlist (escaped char)",&ellc);

	stdoutput.printf("\n");

	// only a string that is a json number is written bare
	stdoutput.printf("numeric strings...\n");
	testNumericStrings<const char *>("linkedlist (const char * numbers)");
	testNumericStrings<char *>("linkedlist (char * numbers)");
	testNumericStrings<const byte_t *>(
				"linkedlist (const byte_t * numbers)");
	testNumericStrings<byte_t *>("linkedlist (byte_t * numbers)");
	stdoutput.printf("\n");

	// nan and inf values are written as null
	stdoutput.printf("nan and inf...\n");
	testNonFinite<float>("float","%f");
	testNonFinite<double>("double","%f");
	testNonFinite<long double>("long double","%Lf");
	stdoutput.printf("\n");

	// float values are written with enough digits to read back as is
	stdoutput.printf("float values...\n");
	testFloatValue<float>("linkedlist (float 1e-7)",1e-7f,"%.9g");
	testFloatValue<float>("linkedlist (float 1.0000001)",
					1.0000001f,"%.9g");
	testFloatValue<double>("linkedlist (double 1e-7)",1e-7,"%.17g");
	testFloatValue<double>("linkedlist (double 1.0000001)",
					1.0000001,"%.17g");
	testFloatValue<long double>("linkedlist (long double 1e-7)",
					1e-7L,ldfmt.getString());
	testFloatValue<long double>("linkedlist (long double 1.0000001)",
					1.0000001L,ldfmt.getString());
	stdoutput.printf("\n");

	if (wcharstring::supportsPrintf()) {
		// wide values
		stdoutput.printf("wide values...\n");

		avltree<const wchar_t *>	atw;
		atw.insert(L"hello");
		testWideValue("avltree (const wchar_t *)",&atw,L"hello",
							"\"hello\"");

		avltree<const wchar_t *>	atwn;
		atwn.insert(L"42");
		testWideValue("avltree (const wchar_t * number)",&atwn,L"42",
								"42");

		for (uint16_t i=0;
				i<sizeof(wnumstrs)/sizeof(wnumstrs[0]); i++) {
			avltree<const wchar_t *>	atwns;
			atwns.insert(wnumstrs[i]);
			stringbuffer	title;
			title.append("avltree (const wchar_t * ");
			title.append(numstrs[i])->append(")");
			testWideValue(title.getString(),&atwns,
						wnumstrs[i],numjson[i]);
		}

		avltree<const wchar_t *>	atwe;
		atwe.insert(L"q\"b\\s\nl");
		testWideValue("avltree (const wchar_t * escaped)",&atwe,
						L"q\"b\\s\nl",ESCENC);

		wchar_t	wstr[]={L'h',L'i',0};
		avltree<wchar_t *>	atwm;
		atwm.insert(wstr);
		bytebuffer	wm;
		wideNode(&wm,0,"hi",2);
		testWideTree("avltree (wchar_t *)",&atwm,0,&wm);
		wideNode(&wm,3,"hi",2);
		testWideTree("avltree (wchar_t *)",&atwm,3,&wm);
		stringbuffer	wmj;
		wmj.append("[\"hi\"]");
		testWideTree("avltree (wchar_t *)",&atwm,1,&wmj);

		wchar_t	wzip[]={L'0',L'2',L'1',L'3',L'4',0};
		avltree<wchar_t *>	atwz;
		atwz.insert(wzip);
		stringbuffer	wzj;
		wzj.append("[\"02134\"]");
		testWideTree("avltree (wchar_t * 02134)",&atwz,1,&wzj);

		wchar_t	wneg[]={L'-',L'1',L'.',L'5',0};
		avltree<wchar_t *>	atwng;
		atwng.insert(wneg);
		stringbuffer	wngj;
		wngj.append("[-1.5]");
		testWideTree("avltree (wchar_t * -1.5)",&atwng,1,&wngj);

		avltree<wchar_t>	atwc;
		atwc.insert((wchar_t)0x263A);
		wchar_t	smile[]={(wchar_t)0x263A,0};

		avltree<wchar_t>	atwd;
		atwd.insert(L'7');
		bytebuffer	wd;
		wideNode(&wd,0,"7",1);
		testWideTree("avltree (wchar_t digit)",&atwd,0,&wd);
		wideNode(&wd,3,"7",1);
		testWideTree("avltree (wchar_t digit)",&atwd,3,&wd);
		stringbuffer	wdj;
		wdj.append("[7]");
		testWideTree("avltree (wchar_t digit)",&atwd,1,&wdj);

		// a null character is written as one null byte
		avltree<wchar_t>	atwz0;
		atwz0.insert(L'\0');
		bytebuffer	wz0;
		wideNode(&wz0,0,"",1);
		testWideTree("avltree (wchar_t null character)",&atwz0,0,&wz0);
		wideNode(&wz0,3,"",1);
		testWideTree("avltree (wchar_t null character)",&atwz0,3,&wz0);

		// null values
		avltree<const wchar_t *>	atwnull;
		atwnull.insert((const wchar_t *)NULL);
		testWideTree("avltree (const wchar_t * null)",&atwnull,0,NULL);
		testWideTree("avltree (const wchar_t * null)",&atwnull,3,NULL);
		bytebuffer	nj;
		nj.append("[null]");
		testWideTree("avltree (const wchar_t * null)",&atwnull,1,&nj);

		avltree<wchar_t *>	atwmnull;
		atwmnull.insert((wchar_t *)NULL);
		testWideTree("avltree (wchar_t * null)",&atwmnull,0,NULL);
		testWideTree("avltree (wchar_t * null)",&atwmnull,3,NULL);
		testWideTree("avltree (wchar_t * null)",&atwmnull,1,&nj);

		// wide keys are always written as strings
		dictionary<const wchar_t *,int32_t>	dw;
		dw.setValue(L"42",1);
		testWideJson("dictionary (const wchar_t * number key)",
							&dw,"{\"42\":1}");

		dictionary<const wchar_t *,int32_t>	dwk;
		dwk.setValue(L"ab",1);
		testWideJson("dictionary (const wchar_t * key)",
							&dwk,"{\"ab\":1}");

		dictionary<const wchar_t *,int32_t>	dwke;
		dwke.setValue(L"q\"b\\s\nl",1);
		testWideJson("dictionary (const wchar_t * escaped key)",
							&dwke,"{" ESCENC ":1}");

		dictionary<wchar_t *,int32_t>	dwmk;
		dwmk.setValue(wstr,1);
		testWideJson("dictionary (wchar_t * key)",&dwmk,"{\"hi\":1}");

		dictionary<wchar_t,int32_t>	dwck;
		dwck.setValue(L'a',1);
		testWideJson("dictionary (wchar_t key)",&dwck,"{\"a\":1}");

		dictionary<wchar_t,int32_t>	dwdk;
		dwdk.setValue(L'7',1);
		testWideJson("dictionary (wchar_t digit key)",&dwdk,"{\"7\":1}");

		// wide values
		dictionary<double,const wchar_t *>	dwv;
		dwv.setValue(1.5,L"ab");
		testWideJson("dictionary (const wchar_t * value)",
							&dwv,"{\"1.5\":\"ab\"}");

		dictionary<const wchar_t *,const wchar_t *>	dwnv;
		dwnv.setValue(L"a",L"12");
		testWideJson("dictionary (const wchar_t * number value)",
							&dwnv,"{\"a\":12}");

		dictionary<const wchar_t *,wchar_t *>	dwmv;
		dwmv.setValue(L"a",wstr);
		testWideJson("dictionary (wchar_t * value)",
							&dwmv,"{\"a\":\"hi\"}");

		dictionary<const wchar_t *,wchar_t>	dwcv;
		dwcv.setValue(L"a",L'x');
		testWideJson("dictionary (wchar_t value)",
							&dwcv,"{\"a\":\"x\"}");

		dictionary<const wchar_t *,wchar_t>	dwdv;
		dwdv.setValue(L"a",L'7');
		testWideJson("dictionary (wchar_t digit value)",
							&dwdv,"{\"a\":7}");

		linkedlist<const wchar_t *>	llw;
		llw.append(L"ab");
		llw.append(L"12");
		llw.append(L"-1.5");
		llw.append(L"007");
		llw.append(L"q\"b\\s\nl");
		testWideJson("linkedlist (const wchar_t *)",&llw,
				"[\"ab\",12,-1.5,\"007\"," ESCENC "]");

		linkedlist<wchar_t *>	llwm;
		llwm.append(wstr);
		llwm.append(wzip);
		llwm.append(wneg);
		testWideJson("linkedlist (wchar_t *)",&llwm,
						"[\"hi\",\"02134\",-1.5]");

		linkedlist<wchar_t>	llwc;
		llwc.append(L'x');
		llwc.append(L'7');
		testWideJson("linkedlist (wchar_t)",&llwc,"[\"x\",7]");

		// a character that the C locale cannot convert
		locale::setCType("C");
		char	*converted=NULL;
		if (charstring::printf(&converted,"%ls",smile)<0) {
			testWideError("avltree (wchar_t unconvertible)",
							&atwc,0,"<t v=\"");
			testWideError("avltree (wchar_t unconvertible)",
							&atwc,1,"[");
			testWideError("avltree (wchar_t unconvertible)",
							&atwc,3,"<t v=\"");
			linkedlist<const wchar_t *>	llwu;
			llwu.append(L"ab");
			llwu.append(smile);
			testWideError("linkedlist (const wchar_t * "
					"unconvertible)",&llwu,1,"[\"ab\",");
			dictionary<const wchar_t *,int32_t>	dwu;
			dwu.setValue(smile,1);
			testWideError("dictionary (const wchar_t * "
					"unconvertible key)",&dwu,1,"{");
		} else {
			stdoutput.printf("the C locale converts U+263A, "
					"unconvertible character skipped\n");
		}
		delete[] converted;

		// non-ascii keys and values in a UTF-8 locale
		if (setUtf8CType()) {
			linkedlist<const wchar_t *>	llw8;
			llw8.append(L"h\x00e9");
			testWideJson("linkedlist (const wchar_t * utf-8)",
						&llw8,"[\"h\xc3\xa9\"]");
			dictionary<const wchar_t *,int32_t>	dw8;
			dw8.setValue(L"h\x00e9",1);
			testWideJson("dictionary (const wchar_t * utf-8 key)",
						&dw8,"{\"h\xc3\xa9\":1}");
			testWideJson("avltree (wchar_t utf-8)",
						&atwc,"[\"\xe2\x98\xba\"]");
			bytebuffer	wc8;
			wideNode(&wc8,0,"\xe2\x98\xba",3);
			testWideTree("avltree (wchar_t utf-8)",&atwc,0,&wc8);
			wideNode(&wc8,3,"\xe2\x98\xba",3);
			testWideTree("avltree (wchar_t utf-8)",&atwc,3,&wc8);
		} else {
			stdoutput.printf("no UTF-8 locale available, "
					"utf-8 wide keys and values skipped\n");
		}
		locale::setCType("C");
	}

	stdoutput.printf("\n");

	return 0;
}
