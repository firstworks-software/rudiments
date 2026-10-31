// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/mvc.h>
#include <rudiments/linkedlist.h>
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

	return 0;
}
