// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/mvc.h>
#include <rudiments/linkedlist.h>
#include <rudiments/avltree.h>
#include <rudiments/table.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>
#include "test.cpp"

class countedlist : public linkedlist<int32_t> {
	public:
			countedlist(int32_t value) {
				append(value);
				alive++;
			}
			~countedlist() { alive--; }
		static uint32_t	alive;
};

uint32_t	countedlist::alive=0;

static uint64_t keyCount(mvcresult *m) {
	return m->getKeys()->getCount();
}

static bool hasKey(mvcresult *m, const char *key) {
	for (listnode<char *> *node=m->getKeys()->getFirst();
					node; node=node->getNext()) {
		if (!charstring::compare(node->getValue(),key)) {
			return true;
		}
	}
	return false;
}

static int32_t firstValue(collection *c) {
	return ((linkedlist<int32_t> *)c)->getFirst()->getValue();
}

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

const char *resultmethods[]={
	"writeJson","writeJson indent","write"
};

static ssize_t writeResult(mvcresult *m, uint16_t method, output *out) {
	switch (method) {
		case 0:
			return m->writeJson(out,false);
		case 1:
			return m->writeJson(out,true);
		default:
			return m->write(out);
	}
}

// "expected" is the output of each of resultmethods, or NULL to skip checking
// it, either as a whole or one entry at a time
static void testResultFailWrite(const char *name, mvcresult *m,
					const char * const *expected) {

	// hard error codes, 0 for a short write
	const ssize_t	codes[]={RESULT_ERROR,RESULT_TIMEOUT,RESULT_ABORT,0};

	for (uint16_t i=0; i<3; i++) {

		stringbuffer	prefix;
		prefix.append(name)->append(": ")->append(resultmethods[i]);

		// count the writes of a clean run
		failbuffer	clean(0,false);
		ssize_t	cleanresult=writeResult(m,i,&clean);
		size_t	total=clean.writes;
		stringbuffer	title;
		title.append(prefix.getString())->append(": clean run");
		test(title.getString(),
			cleanresult==(ssize_t)clean.getStringLength() &&
			total>0 &&
			(!expected || !expected[i] ||
			!charstring::compare(clean.getString(),expected[i])));

		// fail or shorten each write
		for (uint16_t c=0; c<4; c++) {
			bool	shortwrite=!codes[c];
			for (size_t failat=1; failat<=total; failat++) {
				failbuffer	fb(failat,shortwrite,
						(shortwrite)?RESULT_ERROR:codes[c]);
				ssize_t	result=writeResult(m,i,&fb);
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

				// A zero-byte write, such as an empty
				// status message, can't be made short, so
				// the run must complete.
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

int main() {

	header("mvcresult");

	// remove a key set with setData()
	stdoutput.printf("removeData() after setData()...\n");
	mvcresult	*m=new mvcresult;
	countedlist	*a=new countedlist(1);
	countedlist	*b=new countedlist(2);
	m->setData("a",a);
	m->setData("b",b);
	test("count before",keyCount(m)==2);
	m->removeData("a");
	test("data NULL",m->getData("a")==NULL);
	test("key removed",!hasKey(m,"a"));
	test("count after",keyCount(m)==1);
	test("other data intact",m->getData("b")==b);
	test("other key intact",hasKey(m,"b"));
	delete m;
	test("set data alive",countedlist::alive==2);
	delete a;
	delete b;
	test("set data deleted",countedlist::alive==0);
	stdoutput.printf("\n");

	// remove a key set with attachData()
	stdoutput.printf("removeData() after attachData()...\n");
	m=new mvcresult;
	a=new countedlist(1);
	b=new countedlist(2);
	m->attachData("a",a);
	m->attachData("b",b);
	m->removeData("a");
	test("data NULL",m->getData("a")==NULL);
	test("key removed",!hasKey(m,"a"));
	test("count after",keyCount(m)==1);
	test("attached data alive",countedlist::alive==2);
	test("attached data usable",firstValue(a)==1);
	test("other data intact",firstValue(m->getData("b"))==2);
	delete m;
	test("attached data deleted",countedlist::alive==0);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clearData()...\n");
	m=new mvcresult;
	a=new countedlist(1);
	b=new countedlist(2);
	countedlist	*c=new countedlist(3);
	m->attachData("a",a);
	m->attachData("b",b);
	m->setData("c",c);
	m->clearData();
	test("no keys",keyCount(m)==0);
	test("data NULL",m->getData("a")==NULL &&
				m->getData("b")==NULL &&
				m->getData("c")==NULL);
	test("data alive",countedlist::alive==3);
	test("attached data usable",firstValue(a)==1 && firstValue(b)==2);
	m->clearData();
	test("clear empty",keyCount(m)==0);
	delete m;
	test("attached data deleted",countedlist::alive==1);
	delete c;
	test("set data deleted",countedlist::alive==0);
	stdoutput.printf("\n");

	// detach
	stdoutput.printf("detachData()...\n");
	m=new mvcresult;
	a=new countedlist(1);
	b=new countedlist(2);
	m->attachData("a",a);
	m->attachData("b",b);
	collection	*d=m->detachData("a");
	test("detached data returned",d==a);
	test("data NULL",m->getData("a")==NULL);
	test("key removed",!hasKey(m,"a"));
	test("count after",keyCount(m)==1);
	test("detach again",m->detachData("a")==NULL);
	m->getWastebasket()->empty();
	test("wastebasket released data",countedlist::alive==1);
	test("detached data usable",firstValue(d)==1);
	delete d;
	test("detached data deleted",countedlist::alive==0);
	delete m;
	test("no double delete",countedlist::alive==0);

	m=new mvcresult;
	a=new countedlist(1);
	m->setData("a",a);
	m->getWastebasket()->attach(a);
	d=m->detachData("a");
	test("set data detached",d==a);
	delete m;
	test("wastebasket attachment detached",countedlist::alive==1);
	delete d;
	test("set data deleted",countedlist::alive==0);
	stdoutput.printf("\n");

	// keys that do not exist
	stdoutput.printf("nonexistent keys...\n");
	m=new mvcresult;
	a=new countedlist(1);
	m->attachData("a",a);
	m->removeData("x");
	test("removeData()",keyCount(m)==1 && m->getData("a")==a);
	test("detachData()",m->detachData("x")==NULL);
	test("count after",keyCount(m)==1);
	delete m;
	test("attached data deleted",countedlist::alive==0);
	stdoutput.printf("\n");

	// remove and add again
	stdoutput.printf("remove and add again...\n");
	m=new mvcresult;
	a=new countedlist(1);
	b=new countedlist(2);
	c=new countedlist(3);
	m->attachData("a",a);
	m->removeData("a");
	test("count after remove",keyCount(m)==0);
	m->setData("a",b);
	test("setData() again",m->getData("a")==b);
	test("count after setData()",keyCount(m)==1);
	m->removeData("a");
	test("count after remove",keyCount(m)==0);
	m->attachData("a",c);
	test("attachData() again",m->getData("a")==c);
	test("count after attachData()",keyCount(m)==1);
	test("key listed",hasKey(m,"a"));
	delete m;
	test("attached data deleted",countedlist::alive==1);
	delete b;
	test("set data deleted",countedlist::alive==0);
	stdoutput.printf("\n");

	// replace attached data
	stdoutput.printf("replace attached data...\n");
	m=new mvcresult;
	a=new countedlist(1);
	b=new countedlist(2);
	m->attachData("a",a);
	m->setData("a",b);
	test("replaced",m->getData("a")==b);
	test("count",keyCount(m)==1);
	test("replaced data alive",countedlist::alive==2);
	delete m;
	test("replaced data deleted",countedlist::alive==1);
	delete b;
	test("set data deleted",countedlist::alive==0);
	stdoutput.printf("\n");

	// replace attached data, then detach
	stdoutput.printf("attachData(), setData(), detachData()...\n");
	m=new mvcresult;
	a=new countedlist(1);
	b=new countedlist(2);
	m->attachData("a",a);
	m->setData("a",b);
	d=m->detachData("a");
	test("detached data returned",d==b);
	test("data NULL",m->getData("a")==NULL);
	test("count",keyCount(m)==0);
	test("data alive",countedlist::alive==2);
	test("attached data usable",firstValue(a)==1);
	delete m;
	test("attached data deleted",countedlist::alive==1);
	test("detached data usable",firstValue(d)==2);
	delete d;
	test("detached data deleted",countedlist::alive==0);
	stdoutput.printf("\n");

	// keys taken from getKeys()
	stdoutput.printf("keys from getKeys()...\n");
	m=new mvcresult;
	a=new countedlist(1);
	b=new countedlist(2);
	m->attachData("a",a);
	m->attachData("b",b);
	m->removeData(m->getKeys()->getFirst()->getValue());
	test("removeData()",keyCount(m)==1 && m->getData("a")==NULL &&
					m->getData("b")==b);
	d=m->detachData(m->getKeys()->getFirst()->getValue());
	test("detachData()",d==b && keyCount(m)==0);
	delete d;
	test("detached data deleted",countedlist::alive==1);
	delete m;
	test("attached data deleted",countedlist::alive==0);

	m=new mvcresult;
	a=new countedlist(1);
	b=new countedlist(2);
	c=new countedlist(3);
	m->setData("a",a);
	m->setData(m->getKeys()->getFirst()->getValue(),b);
	test("setData()",keyCount(m)==1 && hasKey(m,"a") &&
					m->getData("a")==b);
	m->attachData(m->getKeys()->getFirst()->getValue(),c);
	test("attachData()",keyCount(m)==1 && hasKey(m,"a") &&
					m->getData("a")==c);
	delete m;
	test("attached data deleted",countedlist::alive==2);
	delete a;
	delete b;
	test("set data deleted",countedlist::alive==0);
	stdoutput.printf("\n");

	// json output
	stdoutput.printf("writeJson()...\n");
	m=new mvcresult;
	m->setSuccess();
	a=new countedlist(1);
	b=new countedlist(2);
	m->attachData("a",a);
	m->attachData("b",b);
	m->removeData("a");
	stringbuffer	sb;
	m->writeJson(&sb,false);
	test("removed key absent",
			!charstring::contains(sb.getString(),"\"a\""));
	test("other key present",charstring::contains(sb.getString(),"\"b\""));
	m->setData("a",a);
	sb.clear();
	m->writeJson(&sb,false);
	test("re-added key present",
			charstring::contains(sb.getString(),"\"a\""));
	delete m;
	test("attached data deleted",countedlist::alive==0);
	stdoutput.printf("\n");

	// write errors and short writes
	stdoutput.printf("write errors...\n");

	linkedlist<int32_t>	list;
	list.append(1);
	list.append(2);

	table<const char *>	tbl;
	tbl.setColumnName(0,"x");
	tbl.setColumnName(1,"y");
	tbl.setValue(0,0,"a");
	tbl.setValue(0,1,"b");

	avltree<int32_t>	tree;
	tree.insert(1);
	tree.insert(2);
	tree.insert(3);

	mvcresult	ra;
	ra.setSuccess();
	const char * const	aexpected[]={
		"{\"s\":true,\"c\":0,\"m\":\"\"}",
		"{\n\"s\": true,\n\"c\": 0,\n\"m\": \"\"\n}\n",
		"{\n\"s\": true,\n\"c\": 0,\n\"m\": \"\"\n}\n"
	};
	testResultFailWrite("success",&ra,aexpected);

	mvcresult	ra3;
	ra3.setSuccess(200,"ok");
	const char * const	a3expected[]={
		"{\"s\":true,\"c\":200,\"m\":\"ok\"}",
		"{\n\"s\": true,\n\"c\": 200,\n\"m\": \"ok\"\n}\n",
		"{\n\"s\": true,\n\"c\": 200,\n\"m\": \"ok\"\n}\n"
	};
	testResultFailWrite("success with message",&ra3,a3expected);

	mvcresult	rb4;
	rb4.setSuccess();
	rb4.setData("list",&list);
	const char * const	b4expected[]={
		"{\"s\":true,\"c\":0,\"m\":\"\",\"list\":[1,2]}",NULL,NULL
	};
	testResultFailWrite("list",&rb4,b4expected);

	mvcresult	rlt;
	rlt.setSuccess();
	rlt.setData("list",&list);
	rlt.setData("table",&tbl);
	const char * const	ltexpected[]={
		"{\"s\":true,\"c\":0,\"m\":\"\",\"list\":[1,2],"
		"\"table\":{\"c\":[\"x\",\"y\"],\"r\":[[\"a\",\"b\"]]}}",
		NULL,NULL
	};
	testResultFailWrite("list and table",&rlt,ltexpected);

	mvcresult	rltr;
	rltr.setSuccess();
	rltr.setData("list",&list);
	rltr.setData("tree",&tree);
	const char * const	ltrexpected[]={
		"{\"s\":true,\"c\":0,\"m\":\"\",\"list\":[1,2],"
		"\"tree\":[2,[1],[3]]}",
		NULL,NULL
	};
	testResultFailWrite("list and tree",&rltr,ltrexpected);

	mvcresult	rall;
	rall.setSuccess();
	rall.setData("list",&list);
	rall.setData("table",&tbl);
	rall.setData("tree",&tree);
	const char * const	allexpected[]={
		"{\"s\":true,\"c\":0,\"m\":\"\",\"list\":[1,2],"
		"\"table\":{\"c\":[\"x\",\"y\"],\"r\":[[\"a\",\"b\"]]},"
		"\"tree\":[2,[1],[3]]}",
		NULL,NULL
	};
	testResultFailWrite("list, table, and tree",&rall,allexpected);

	mvcresult	rn;
	rn.setSuccess();
	rn.setData("n",NULL);
	const char * const	nexpected[]={
		"{\"s\":true,\"c\":0,\"m\":\"\",\"n\":null}",NULL,NULL
	};
	testResultFailWrite("null",&rn,nexpected);

	mvcresult	rln;
	rln.setSuccess();
	rln.setData("list",&list);
	rln.setData("n",NULL);
	const char * const	lnexpected[]={
		"{\"s\":true,\"c\":0,\"m\":\"\",\"list\":[1,2],\"n\":null}",
		NULL,NULL
	};
	testResultFailWrite("list and null",&rln,lnexpected);

	mvcresult	rd;
	rd.setFailed(7,"bad");
	rd.setData("list",&list);
	const char * const	dexpected[]={
		"{\"s\":false,\"c\":7,\"m\":\"bad\"}",
		"{\n\"s\": false,\n\"c\": 7,\n\"m\": \"bad\"\n}\n",
		"{\n\"s\": false,\n\"c\": 7,\n\"m\": \"bad\"\n}\n"
	};
	testResultFailWrite("failed with list",&rd,dexpected);

	mvcresult	rf;
	rf.setFailed();
	const char * const	fexpected[]={
		"{\"s\":false,\"c\":0,\"m\":\"\"}",
		"{\n\"s\": false,\n\"c\": 0,\n\"m\": \"\"\n}\n",
		"{\n\"s\": false,\n\"c\": 0,\n\"m\": \"\"\n}\n"
	};
	testResultFailWrite("failed",&rf,fexpected);

	stdoutput.printf("\n");

	return 0;
}
