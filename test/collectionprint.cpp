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

template <class collectiontype>
void testFailWrite(const char *name, collectiontype *c) {

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
			cleanresult==(ssize_t)clean.getStringLength() &&
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
	avltree<int32_t>	eati32;
	const char * const	eexpected[]={
		"<t/>\n","[]","[\n]\n","<t/>\n","<t/>\n"
	};
	testTreeFailWrite("avltree (empty)",&eati32,eexpected);

	stdoutput.printf("\n");

	return 0;
}
