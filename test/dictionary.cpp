// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/dictionary.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// value that counts its creations and deletions
class countedvalue {
	public:
		countedvalue(int32_t value) : value(value) {
			created++;
		}
		countedvalue() : value(0) {
			created++;
		}
		countedvalue(const countedvalue &c) : value(c.value) {
			created++;
		}
		~countedvalue() {
			deleted++;
		}
		int32_t		value;
		static uint32_t	created;
		static uint32_t	deleted;
};

uint32_t	countedvalue::created=0;
uint32_t	countedvalue::deleted=0;

// 11172 dictionary - a nan key cannot be found
template <class floattype>
void testNanKey(const char *name) {

	stdoutput.printf("%s nan key:\n",name);
	volatile floattype	zero=0;
	floattype		nan=zero/zero;
	floattype		inf=1/zero;

	// nan key alone
	dictionary<floattype,const char *>	d;
	d.setValue(nan,"v");
	test("nan: setValue()/getValue()",
		!charstring::compare(d.getValue(nan),"v"));
	test("nan: getCount",d.getCount()==1);

	// replace the value
	d.setValue(nan,"w");
	test("nan: setValue() replaces",
		!charstring::compare(d.getValue(nan),"w"));
	test("nan: replace getCount",d.getCount()==1);
	test("nan: replace getKeys()->getCount",
		d.getKeys()->getCount()==1);

	// nan among ordinary and infinite keys
	d.setValue(inf,"i");
	d.setValue(-inf,"m");
	d.setValue((floattype)1.5,"x");
	d.setValue((floattype)0,"z");
	test("mixed: getCount",d.getCount()==5);
	test("mixed: getKeys()->getCount",d.getKeys()->getCount()==5);
	test("mixed: getValue(nan)",
		!charstring::compare(d.getValue(nan),"w"));
	test("mixed: getValue(inf)",
		!charstring::compare(d.getValue(inf),"i"));
	test("mixed: getValue(-inf)",
		!charstring::compare(d.getValue(-inf),"m"));
	test("mixed: getValue(1.5)",
		!charstring::compare(d.getValue((floattype)1.5),"x"));
	test("mixed: getValue(0)",
		!charstring::compare(d.getValue((floattype)0),"z"));

	// remove
	test("remove(nan)",d.remove(nan));
	test("remove(nan): getValue()",!d.getValue(nan));
	test("remove(nan): getCount",d.getCount()==4);
	test("remove(nan) again",!d.remove(nan));
	test("remove(nan): others kept",
		!charstring::compare(d.getValue(inf),"i") &&
		!charstring::compare(d.getValue(-inf),"m"));
	stdoutput.printf("\n");
}

int main(int argc, const char **argv) {

	header("dictionary");

	// string/string
	stdoutput.printf("string keys, string values:\n");
	dictionary< const char *, const char * >	strstrdict;

	// add values
	strstrdict.setValue("1","one");
	strstrdict.setValue("2","one");
	strstrdict.setValue("3","one");
	strstrdict.setValue("4","one");
	test("setValue()/getValue() 1",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("setValue()/getValue() 2",
		!charstring::compare(strstrdict.getValue("2"),"one"));
	test("setValue()/getValue() 3",
		!charstring::compare(strstrdict.getValue("3"),"one"));
	test("setValue()/getValue() 4",
		!charstring::compare(strstrdict.getValue("4"),"one"));
	test("getValue(): 5",!strstrdict.getValue("5"));

	// override values
	strstrdict.setValue("1","one");
	strstrdict.setValue("2","two");
	strstrdict.setValue("3","three");
	strstrdict.setValue("4","four");
	test("setValue()/getValue() 1,one (override)",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("setValue()/getValue() 2,two (override)",
		!charstring::compare(strstrdict.getValue("2"),"two"));
	test("setValue()/getValue() 3,three (override)",
		!charstring::compare(strstrdict.getValue("3"),"three"));
	test("setValue()/getValue() 4,four (override)",
		!charstring::compare(strstrdict.getValue("4"),"four"));
	test("getValue(): 5,NULL",!strstrdict.getValue("5"));

	// keys
	test("getCount",strstrdict.getCount()==4);
	test("getKeys()->getCount",
		strstrdict.getKeys()->getCount()==4);
	test("getValue(): 1,one",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("getValue(): 2,two",
		!charstring::compare(strstrdict.getValue("2"),"two"));
	test("getValue(): 3,three",
		!charstring::compare(strstrdict.getValue("3"),"three"));
	test("getValue(): 4,four",
		!charstring::compare(strstrdict.getValue("4"),"four"));
	test("getValue(): 5,NULL",!strstrdict.getValue("5"));

	// get value (reference)
	const char	*val=NULL;
	strstrdict.getValue("1",&val);
	test("getValue() by reference: 1,one",
		!charstring::compare(val,"one"));
	strstrdict.getValue("2",&val);
	test("getValue() by reference: 2,two",
		!charstring::compare(val,"two"));
	strstrdict.getValue("3",&val);
	test("getValue() by reference: 3,three",
		!charstring::compare(val,"three"));
	strstrdict.getValue("4",&val);
	test("getValue() by reference: 4,four",
		!charstring::compare(val,"four"));
	test("getValue() by reference: 5,NULL",!strstrdict.getValue("5"));

	// keys
	listnode< const char * > *strkey=strstrdict.getKeys()->getFirst();
	test("before start key",!strkey->getPrevious());
	test("key 1",!charstring::compare(strkey->getValue(),"1"));
	strkey=strkey->getNext();
	test("key 2",!charstring::compare(strkey->getValue(),"2"));
	strkey=strkey->getNext();
	test("key 3",!charstring::compare(strkey->getValue(),"3"));
	strkey=strkey->getNext();
	test("key 4",!charstring::compare(strkey->getValue(),"4"));
	test("after end key",!strkey->getNext());

	// remove
	strstrdict.remove("3");
	test("remove 3: getCount",strstrdict.getCount()==3);
	test("remove 3: getKeys()->getCount",
		strstrdict.getKeys()->getCount()==3);
	test("remove 3: getValue(): 1,one",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("remove 3: getValue(): 2,two",
		!charstring::compare(strstrdict.getValue("2"),"two"));
	test("remove 3: getValue(): 3,NULL",
		!strstrdict.getValue("3"));
	test("remove 3: getValue(): 4,four",
		!charstring::compare(strstrdict.getValue("4"),"four"));

	strstrdict.remove("2");
	test("remove 2: getCount",strstrdict.getCount()==2);
	test("remove 2: getKeys()->getCount",
		strstrdict.getKeys()->getCount()==2);
	test("remove 2: getValue(): 1,one",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("remove 2: getValue(): 2,NULL",
		!strstrdict.getValue("2"));
	test("remove 2: getValue(): 3,NULL",
		!strstrdict.getValue("3"));
	test("remove 2: getValue(): 4,four",
		!charstring::compare(strstrdict.getValue("4"),"four"));

	strstrdict.remove("1");
	test("remove 1: getCount",strstrdict.getCount()==1);
	test("remove 1: getKeys()->getCount",
		strstrdict.getKeys()->getCount()==1);
	test("remove 1: getValue(): 1,NULL",
		!strstrdict.getValue("1"));
	test("remove 1: getValue(): 2,NULL",
		!strstrdict.getValue("2"));
	test("remove 1: getValue(): 3,NULL",
		!strstrdict.getValue("3"));
	test("remove 1: getValue(): 4,four",
		!charstring::compare(strstrdict.getValue("4"),"four"));

	strstrdict.remove("4");
	test("remove 4: getCount",strstrdict.getCount()==0);
	test("remove 4: getKeys()->getCount",
		strstrdict.getKeys()->getCount()==0);
	test("remove 4: getValue(): 1,NULL",
		!strstrdict.getValue("1"));
	test("remove 4: getValue(): 2,NULL",
		!strstrdict.getValue("2"));
	test("remove 4: getValue(): 3,NULL",
		!strstrdict.getValue("3"));
	test("remove 4: getValue(): 4,NULL",
		!strstrdict.getValue("4"));

	// keys
	test("empty keys",!strstrdict.getKeys()->getFirst());

	// clear
	strstrdict.setValue("1","one");
	strstrdict.setValue("2","two");
	strstrdict.setValue("3","three");
	strstrdict.setValue("4","four");
	strstrdict.clear();
	test("clear: getCount",strstrdict.getCount()==0);
	test("clear: getKeys()->getCount",
		strstrdict.getKeys()->getCount()==0);

	// array
	const char	*keys[]={
		"1","2","3","4",NULL
	};
	const char	*values[]={
		"one","two","three","four",NULL
	};
	strstrdict.setValues(keys,values);
	test("array (null): getCount",strstrdict.getCount()==4);
	test("array (null): getKeys()->getCount",
		strstrdict.getKeys()->getCount()==4);
	test("array (null): setValue()/getValue() 1",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("array (null): setValue()/getValue() 2",
		!charstring::compare(strstrdict.getValue("2"),"two"));
	test("array (null): setValue()/getValue() 3",
		!charstring::compare(strstrdict.getValue("3"),"three"));
	test("array (null): setValue()/getValue() 4",
		!charstring::compare(strstrdict.getValue("4"),"four"));
	test("array (null): getValue(): 5",!strstrdict.getValue("5"));
	strstrdict.clear();
	strstrdict.setValues(keys,values,4);
	test("array (count): getCount",strstrdict.getCount()==4);
	test("array (count): getKeys()->getCount",
		strstrdict.getKeys()->getCount()==4);
	test("array (count): setValue()/getValue() 1",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("array (count): setValue()/getValue() 2",
		!charstring::compare(strstrdict.getValue("2"),"two"));
	test("array (count): setValue()/getValue() 3",
		!charstring::compare(strstrdict.getValue("3"),"three"));
	test("array (count): setValue()/getValue() 4",
		!charstring::compare(strstrdict.getValue("4"),"four"));
	test("array (count): getValue(): 5",!strstrdict.getValue("5"));

	// from another dictionary
	dictionary< const char *, const char * >	strstrdict1;
	strstrdict1.setValues(&strstrdict);
	test("dict: getCount",strstrdict.getCount()==4);
	test("dict: getKeys()->getCount",
		strstrdict.getKeys()->getCount()==4);
	test("dict: setValue()/getValue() 1",
		!charstring::compare(strstrdict.getValue("1"),"one"));
	test("dict: setValue()/getValue() 2",
		!charstring::compare(strstrdict.getValue("2"),"two"));
	test("dict: setValue()/getValue() 3",
		!charstring::compare(strstrdict.getValue("3"),"three"));
	test("dict: setValue()/getValue() 4",
		!charstring::compare(strstrdict.getValue("4"),"four"));
	test("dict: getValue(): 5",!strstrdict.getValue("5"));
	stdoutput.printf("\n");


	// int/string
	stdoutput.printf("int keys, string values:\n");
	dictionary< int64_t , const char * >	intstrdict;

	// add values
	intstrdict.setValue(1,"one");
	intstrdict.setValue(2,"one");
	intstrdict.setValue(3,"one");
	intstrdict.setValue(4,"one");
	test("setValue()/getValue() 1",
		!charstring::compare(intstrdict.getValue(1),"one"));
	test("setValue()/getValue() 2",
		!charstring::compare(intstrdict.getValue(2),"one"));
	test("setValue()/getValue() 3",
		!charstring::compare(intstrdict.getValue(3),"one"));
	test("setValue()/getValue() 4",
		!charstring::compare(intstrdict.getValue(4),"one"));
	test("getValue(): 5",!intstrdict.getValue(5));

	// override values
	intstrdict.setValue(1,"one");
	intstrdict.setValue(2,"two");
	intstrdict.setValue(3,"three");
	intstrdict.setValue(4,"four");
	test("setValue()/getValue() 1,one (override)",
		!charstring::compare(intstrdict.getValue(1),"one"));
	test("setValue()/getValue() 2,two (override)",
		!charstring::compare(intstrdict.getValue(2),"two"));
	test("setValue()/getValue() 3,three (override)",
		!charstring::compare(intstrdict.getValue(3),"three"));
	test("setValue()/getValue() 4,four (override)",
		!charstring::compare(intstrdict.getValue(4),"four"));
	test("getValue(): 5,NULL",!intstrdict.getValue(5));

	// keys
	test("getCount",intstrdict.getCount()==4);
	test("getKeys()->getCount",
		intstrdict.getKeys()->getCount()==4);
	test("getValue(): 1,one",
		!charstring::compare(intstrdict.getValue(1),"one"));
	test("getValue(): 2,two",
		!charstring::compare(intstrdict.getValue(2),"two"));
	test("getValue(): 3,three",
		!charstring::compare(intstrdict.getValue(3),"three"));
	test("getValue(): 4,four",
		!charstring::compare(intstrdict.getValue(4),"four"));
	test("getValue(): 5,NULL",!intstrdict.getValue(5));

	// get value (reference)
	intstrdict.getValue(1,&val);
	test("getValue() by reference: 1,one",
		!charstring::compare(val,"one"));
	intstrdict.getValue(2,&val);
	test("getValue() by reference: 2,two",
		!charstring::compare(val,"two"));
	intstrdict.getValue(3,&val);
	test("getValue() by reference: 3,three",
		!charstring::compare(val,"three"));
	intstrdict.getValue(4,&val);
	test("getValue() by reference: 4,four",
		!charstring::compare(val,"four"));
	test("getValue() by reference: 5,NULL",!intstrdict.getValue(5));

	// keys
	listnode< int64_t > *intkey=intstrdict.getKeys()->getFirst();
	test("before start key",!intkey->getPrevious());
	test("key 1",intkey->getValue()==1);
	intkey=intkey->getNext();
	test("key 2",intkey->getValue()==2);
	intkey=intkey->getNext();
	test("key 3",intkey->getValue()==3);
	intkey=intkey->getNext();
	test("key 4",intkey->getValue()==4);
	test("after end key",!intkey->getNext());

	// remove
	intstrdict.remove(3);
	test("remove 3: getCount",intstrdict.getCount()==3);
	test("remove 3: getKeys()->getCount",
		intstrdict.getKeys()->getCount()==3);
	test("remove 3: getValue(): 1,one",
		!charstring::compare(intstrdict.getValue(1),"one"));
	test("remove 3: getValue(): 2,two",
		!charstring::compare(intstrdict.getValue(2),"two"));
	test("remove 3: getValue(): 3,NULL",
		!intstrdict.getValue(3));
	test("remove 3: getValue(): 4,four",
		!charstring::compare(intstrdict.getValue(4),"four"));

	intstrdict.remove(2);
	test("remove 2: getCount",intstrdict.getCount()==2);
	test("remove 2: getKeys()->getCount",
		intstrdict.getKeys()->getCount()==2);
	test("remove 2: getValue(): 1,one",
		!charstring::compare(intstrdict.getValue(1),"one"));
	test("remove 2: getValue(): 2,NULL",
		!intstrdict.getValue(2));
	test("remove 2: getValue(): 3,NULL",
		!intstrdict.getValue(3));
	test("remove 2: getValue(): 4,four",
		!charstring::compare(intstrdict.getValue(4),"four"));

	intstrdict.remove(1);
	test("remove 1: getCount",intstrdict.getCount()==1);
	test("remove 1: getKeys()->getCount",
		intstrdict.getKeys()->getCount()==1);
	test("remove 1: getValue(): 1,NULL",
		!intstrdict.getValue(1));
	test("remove 1: getValue(): 2,NULL",
		!intstrdict.getValue(2));
	test("remove 1: getValue(): 3,NULL",
		!intstrdict.getValue(3));
	test("remove 1: getValue(): 4,four",
		!charstring::compare(intstrdict.getValue(4),"four"));

	intstrdict.remove(4);
	test("remove 4: getCount",intstrdict.getCount()==0);
	test("remove 4: getKeys()->getCount",
		intstrdict.getKeys()->getCount()==0);
	test("remove 4: getValue(): 1,NULL",
		!intstrdict.getValue(1));
	test("remove 4: getValue(): 2,NULL",
		!intstrdict.getValue(2));
	test("remove 4: getValue(): 3,NULL",
		!intstrdict.getValue(3));
	test("remove 4: getValue(): 4,NULL",
		!intstrdict.getValue(4));

	// keys
	test("empty keys",!intstrdict.getKeys()->getFirst());

	// clear
	intstrdict.setValue(1,"one");
	intstrdict.setValue(2,"two");
	intstrdict.setValue(3,"three");
	intstrdict.setValue(4,"four");
	intstrdict.clear();
	test("clear: getCount",intstrdict.getCount()==0);
	test("clear: getKeys()->getCount",
		intstrdict.getKeys()->getCount()==0);
	stdoutput.printf("\n");

	// remove and delete
	stdoutput.printf("remove/clear and delete:\n");
	dictionary<stringbuffer *, stringbuffer *>	strbstrbdict;
	strbstrbdict.setManageKeys(true);
	strbstrbdict.setManageValues(true);
	stringbuffer	*onekey=new stringbuffer();
	onekey->append("one");
	stringbuffer	*oneval=new stringbuffer();
	oneval->append("one");
	stringbuffer	*twokey=new stringbuffer();
	twokey->append("two");
	stringbuffer	*twoval=new stringbuffer();
	twoval->append("two");
	stringbuffer	*threekey=new stringbuffer();
	threekey->append("three");
	stringbuffer	*threeval=new stringbuffer();
	threeval->append("three");
	strbstrbdict.setValue(onekey,oneval);
	strbstrbdict.setValue(twokey,twoval);
	strbstrbdict.setValue(threekey,threeval);
	test("...and delete: getValue(): one,one",
			strbstrbdict.getValue(onekey)==oneval);
	test("...and delete: getValue(): two,two",
			strbstrbdict.getValue(twokey)==twoval);
	test("...and delete: getValue(): three,three",
			strbstrbdict.getValue(threekey)==threeval);
	test("...and delete: remove(): one",
			strbstrbdict.remove(onekey));
	test("...and delete: remove(): two",
			strbstrbdict.remove(twokey));
	test("...and delete: remove(): three",
			strbstrbdict.remove(threekey));
	onekey=new stringbuffer();
	onekey->append("one");
	oneval=new stringbuffer();
	oneval->append("one");
	twokey=new stringbuffer();
	twokey->append("two");
	twoval=new stringbuffer();
	twoval->append("two");
	threekey=new stringbuffer();
	threekey->append("three");
	threeval=new stringbuffer();
	threeval->append("three");
	strbstrbdict.setValue(onekey,oneval);
	strbstrbdict.setValue(twokey,twoval);
	strbstrbdict.setValue(threekey,threeval);
	strbstrbdict.clear();
	test("...and delete: clear(): ...",
				!strbstrbdict.getValue(oneval));
	stdoutput.printf("\n");

	// remove and array delete
	stdoutput.printf("remove/clear and array delete:\n");
	dictionary<char *, char *>	charchardict;
	charchardict.setManageArrayKeys(true);
	charchardict.setManageArrayValues(true);
	charchardict.setValue(charstring::duplicate("one"),
				charstring::duplicate("one"));
	charchardict.setValue(charstring::duplicate("two"),
				charstring::duplicate("two"));
	charchardict.setValue(charstring::duplicate("three"),
				charstring::duplicate("three"));
	test("...and array delete: getValue(): one,one",
		!charstring::compare(charchardict.getValue(
					(char *)"one"),(char *)"one"));
	test("...and array delete: getValue(): two,two",
		!charstring::compare(charchardict.getValue(
					(char *)"two"),(char *)"two"));
	test("...and array delete: getValue(): three,three",
		!charstring::compare(charchardict.getValue(
					(char *)"three"),(char *)"three"));
	test("...and array delete: remove(): one",
			charchardict.remove((char *)"one"));
	test("...and array delete: remove(): two",
			charchardict.remove((char *)"two"));
	test("...and array delete: remove(): three",
			charchardict.remove((char *)"three"));
	charchardict.setValue(charstring::duplicate("one"),
				charstring::duplicate("one"));
	charchardict.setValue(charstring::duplicate("two"),
				charstring::duplicate("two"));
	charchardict.setValue(charstring::duplicate("three"),
				charstring::duplicate("three"));
	charchardict.clear();
	test("...and array delete: clear(): ...",
				!charchardict.getValue((char *)"one"));
	stdoutput.printf("\n");

	// repeated keys with managed array keys
	stdoutput.printf("repeated keys, managed array keys:\n");
	dictionary<char *, char *>	rkdict;
	rkdict.setManageArrayKeys(true);

	// same key, different pointer
	char	*rkkey1=charstring::duplicate("one");
	rkdict.setValue(rkkey1,(char *)"first");
	rkdict.setValue(charstring::duplicate("one"),(char *)"second");
	test("repeat key: getCount",rkdict.getCount()==1);
	test("repeat key: getKeys()->getCount",
		rkdict.getKeys()->getCount()==1);
	test("repeat key: getValue()",
		!charstring::compare(rkdict.getValue((char *)"one"),"second"));
	test("repeat key: stored key kept",
		rkdict.getKeys()->getFirst()->getValue()==rkkey1);

	// same key, same pointer
	rkdict.setValue(rkkey1,(char *)"third");
	test("repeat pointer: getCount",rkdict.getCount()==1);
	test("repeat pointer: getValue()",
		!charstring::compare(rkdict.getValue((char *)"one"),"third"));
	test("repeat pointer: key still valid",
		!charstring::compare(rkkey1,"one"));
	rkdict.clear();

	// same key, different pointers, through arrays
	char	*rkkeys1[]={
		charstring::duplicate("one"),
		charstring::duplicate("two"),
		NULL
	};
	char	*rkvals1[]={
		(char *)"1",(char *)"2",NULL
	};
	char	*rkkeys2[]={
		charstring::duplicate("one"),
		charstring::duplicate("two"),
		NULL
	};
	char	*rkvals2[]={
		(char *)"11",(char *)"22",NULL
	};
	rkdict.setValues(rkkeys1,rkvals1);
	rkdict.setValues(rkkeys2,rkvals2);
	test("repeat array: getCount",rkdict.getCount()==2);
	test("repeat array: getKeys()->getCount",
		rkdict.getKeys()->getCount()==2);
	test("repeat array: getValue() one",
		!charstring::compare(rkdict.getValue((char *)"one"),"11"));
	test("repeat array: getValue() two",
		!charstring::compare(rkdict.getValue((char *)"two"),"22"));

	// same pointers through arrays
	rkdict.setValues(rkkeys1,rkvals2);
	test("repeat array pointers: getCount",rkdict.getCount()==2);
	test("repeat array pointers: getValue() one",
		!charstring::compare(rkdict.getValue((char *)"one"),"11"));
	test("repeat array pointers: keys still valid",
		!charstring::compare(rkkeys1[0],"one") &&
		!charstring::compare(rkkeys1[1],"two"));
	rkdict.clear();
	stdoutput.printf("\n");

	// copy and assignment
	for (uint16_t i=0; i<2; i++) {

		stdoutput.printf("copy/assignment%s...\n",(!i)?"":" (managed)");

		dictionary<uint16_t,char *>	cch1;
		cch1.setManageArrayValues(i);
		const char *values[]={
			"a","b","c","d","e","f","g","h","i","j","k","l","m",
			"n","o","p","q","r","s","t","u","v","w","x","y","z",
			NULL
		};
		for (uint16_t j=0; j<26; j++) {
			if (!i) {
				cch1.setValue(j,(char *)values[j]);
			} else {
				cch1.setValue(j,charstring::duplicate(
							(char *)values[j]));
			}
		}
	
		dictionary<uint16_t,char *>	cch2(cch1);
		for (uint16_t j=0; j<3; j++) {

			// 1st iteration is copy
			// 2nd is assignment
			// 3rd is assignment after clear
			if (j==2) {
				cch2.clear();
			}
			if (j) {
				cch2=cch1;
			}

			// verify flags
			test((!j)?"copy: manage keys":
					"assignment: manage keys",
						cch2.getManageKeys()==
						cch1.getManageKeys());
			test((!j)?"copy: manage array keys":
					"assignment: manage array keys",
						cch2.getManageArrayKeys()==
						cch1.getManageArrayKeys());
			test((!j)?"copy: manage values":
					"assignment: manage values",
						cch2.getManageValues()==
						cch1.getManageValues());
			test((!j)?"copy: manage array values":
					"assignment: manage array values",
						cch2.getManageArrayValues()==
						cch1.getManageArrayValues());

			// verify length
			test((!j)?"copy: length":"assignment: length",
						cch2.getCount()==
						cch1.getCount());

			// verify values
			bool	success=true;
			for (uint16_t k=0; k<26; k++) {
				if (charstring::compare(cch1.getValue(k),
							cch2.getValue(k))) {
					success=false;
					break;
				}
			}
			test((!j)?"copy: values":"assignment: values",success);
		}
	}
	stdoutput.printf("\n");

	// copy of an unmanaged const char * dictionary
	stdoutput.printf("unmanaged const char * copy:\n");
	const char	*ucv="parrot";
	dictionary< const char *, const char * >	ucd1;
	ucd1.setValue("bird",ucv);
	dictionary< const char *, const char * >	ucd2(ucd1);
	test("copy: value",ucd2.getValue("bird")==ucv &&
			!charstring::compare(ucd2.getValue("bird"),"parrot"));
	stdoutput.printf("\n");

	// setValues() from a managed dictionary
	for (uint16_t i=0; i<2; i++) {

		stdoutput.printf("setValues() from managed dictionary%s:\n",
					(!i)?"":" (insertion order)");

		dictionary<char *, char *>	*svsrc=
					new dictionary<char *, char *>();
		svsrc->setTrackInsertionOrder(i);
		svsrc->setManageArrayKeys(true);
		svsrc->setManageArrayValues(true);
		svsrc->setValue(charstring::duplicate("one"),
					charstring::duplicate("1"));
		svsrc->setValue(charstring::duplicate("two"),
					charstring::duplicate("2"));
		svsrc->setValue(charstring::duplicate("three"),
					charstring::duplicate("3"));

		dictionary<char *, char *>	*svdst=
					new dictionary<char *, char *>();
		svdst->setManageArrayKeys(true);
		svdst->setManageArrayValues(true);
		svdst->setValues(svsrc);

		test("setValues(dict): getCount",svdst->getCount()==3);
		test("setValues(dict): getValue() one",
			!charstring::compare(
				svdst->getValue((char *)"one"),"1"));
		test("setValues(dict): getValue() two",
			!charstring::compare(
				svdst->getValue((char *)"two"),"2"));
		test("setValues(dict): getValue() three",
			!charstring::compare(
				svdst->getValue((char *)"three"),"3"));

		// keys and values must be copies, not shared pointers
		bool	success=true;
		for (listnode<char *> *node=svsrc->getKeys()->getFirst();
						node; node=node->getNext()) {
			char	*key=node->getValue();
			for (listnode<char *> *dnode=
					svdst->getKeys()->getFirst();
					dnode; dnode=dnode->getNext()) {
				if (dnode->getValue()==key) {
					success=false;
				}
			}
			if (svdst->getValue(key)==svsrc->getValue(key)) {
				success=false;
			}
		}

		// delete both before testing, so a double free shows up
		delete svsrc;
		bool	survived=
			!charstring::compare(
				svdst->getValue((char *)"one"),"1") &&
			!charstring::compare(
				svdst->getValue((char *)"two"),"2") &&
			!charstring::compare(
				svdst->getValue((char *)"three"),"3");
		delete svdst;
		test("setValues(dict): keys and values copied",success);
		test("setValues(dict): values after source delete",survived);
		stdoutput.printf("\n");
	}

	testNanKey<float>("float");
	testNanKey<double>("double");

	// constructor with insertion order argument
	for (uint16_t i=0; i<2; i++) {

		stdoutput.printf("dictionary(bool)%s:\n",
					(i)?" (insertion order)":"");

		dictionary<const char *, const char *>	tod(i==1);
		test("getTrackInsertionOrder()",
			tod.getTrackInsertionOrder()==(i==1));
		tod.setValue("b","2");
		tod.setValue("a","1");
		tod.setValue("c","3");
		listnode<const char *>	*node=tod.getKeys()->getFirst();
		const char	*first=(i)?"b":"a";
		const char	*second=(i)?"a":"b";
		test("key 1",node &&
			!charstring::compare(node->getValue(),first));
		node=(node)?node->getNext():NULL;
		test("key 2",node &&
			!charstring::compare(node->getValue(),second));
		node=(node)?node->getNext():NULL;
		test("key 3",node &&
			!charstring::compare(node->getValue(),"c"));
		stdoutput.printf("\n");
	}

	// 10835 container setters should free the old value when manage
	// values is set
	stdoutput.printf("setValue() and the old value:\n");
	{
		typedef dictionary<const char *, countedvalue *>	cvdict;

		// unmanaged
		countedvalue	*u1=new countedvalue(1);
		countedvalue	*u2=new countedvalue(2);
		uint32_t	before=countedvalue::deleted;
		{
			cvdict	ud;
			ud.setValue("a",u1);
			ud.setValue("a",u2);
			test("unmanaged: old value not freed",
					countedvalue::deleted==before);
		}
		test("unmanaged: values not freed on delete",
					countedvalue::deleted==before);
		delete u1;
		delete u2;

		// new keys
		cvdict	md;
		md.setManageValues(true);
		before=countedvalue::deleted;
		md.setValue("a",new countedvalue(1));
		md.setValue("b",new countedvalue(2));
		test("managed, new key: nothing freed",
					countedvalue::deleted==before);

		// same pointer
		md.setValue("a",md.getValue("a"));
		test("managed, same pointer: nothing freed",
					countedvalue::deleted==before);
		test("managed, same pointer: value kept",
					md.getValue("a")->value==1);

		// existing key
		md.setValue("a",new countedvalue(3));
		test("managed, existing key: new value stored",
					md.getValue("a")->value==3 &&
					md.getValue("b")->value==2 &&
					md.getCount()==2);
		test("managed, existing key: old value freed",
					countedvalue::deleted==before+1);

		// array values
		cvdict	mad;
		mad.setManageArrayValues(true);
		mad.setValue("a",new countedvalue[2]);
		before=countedvalue::deleted;
		mad.setValue("a",mad.getValue("a"));
		test("managed array, same pointer: nothing freed",
					countedvalue::deleted==before);
		mad.setValue("a",new countedvalue[3]);
		test("managed array, existing key: old array freed",
					countedvalue::deleted==before+2);

		// setValues() from arrays, one existing key and one new key
		const char	*svtitles[]={
			"managed, setValues(keys,values)",
			"managed, setValues(const keys,const values)",
			"managed, setValues(keys,values,count)",
			"managed, setValues(const keys,const values,count)"
		};
		const char	*keys[]={"a","c",NULL};
		for (uint16_t i=0; i<4; i++) {
			cvdict	sd;
			sd.setManageValues(true);
			sd.setValue("a",new countedvalue(1));
			countedvalue	*values[]={
				new countedvalue(2),
				new countedvalue(3),
				NULL
			};
			before=countedvalue::deleted;
			switch (i) {
				case 0:
					sd.setValues(keys,values);
					break;
				case 1:
					sd.setValues((const char * const *)keys,
						(countedvalue * const *)values);
					break;
				case 2:
					sd.setValues(keys,values,2);
					break;
				default:
					sd.setValues((const char * const *)keys,
						(countedvalue * const *)values,
						2);
					break;
			}
			stringbuffer	title;
			title.append(svtitles[i]);
			title.append(": new values stored");
			test(title.getString(),
					sd.getValue("a")->value==2 &&
					sd.getValue("c")->value==3 &&
					sd.getCount()==2);
			title.clear();
			title.append(svtitles[i])->append(": old value freed");
			test(title.getString(),
					countedvalue::deleted==before+1);
		}

		// setValues() from an unmanaged dictionary
		countedvalue	*s1=new countedvalue(4);
		countedvalue	*s2=new countedvalue(5);
		{
			cvdict	src;
			src.setValue("a",s1);
			src.setValue("d",s2);
			cvdict	dst;
			dst.setManageValues(true);
			dst.setValue("a",new countedvalue(1));
			before=countedvalue::deleted;
			dst.setValues(&src);
			test("managed, setValues(dict): values copied",
					dst.getValue("a")!=s1 &&
					dst.getValue("a")->value==4 &&
					dst.getValue("d")!=s2 &&
					dst.getValue("d")->value==5 &&
					dst.getCount()==2);
			test("managed, setValues(dict): old value freed",
					countedvalue::deleted==before+1);
		}
		delete s1;
		delete s2;

		// setValues() from itself
		// A copy of each value and no copy at all are both correct,
		// so check only that no value leaks and no live value is freed.
		for (uint16_t i=0; i<2; i++) {
			cvdict	self;
			self.setTrackInsertionOrder(i==1);
			self.setManageValues(true);
			self.setValue("a",new countedvalue(1));
			self.setValue("b",new countedvalue(2));
			uint32_t	live=countedvalue::created-
						countedvalue::deleted;
			self.setValues(&self);
			test((!i)?"managed, setValues(self): values kept":
				"managed, setValues(self) "
				"(insertion order): values kept",
					self.getValue("a")->value==1 &&
					self.getValue("b")->value==2 &&
					self.getCount()==2);
			test((!i)?"managed, setValues(self): no leaked values":
				"managed, setValues(self) "
				"(insertion order): no leaked values",
					countedvalue::created-
					countedvalue::deleted==live);
		}

		// setValues() from itself, array values
		dictionary<const char *, char *>	sad;
		sad.setManageArrayValues(true);
		sad.setValue("a",charstring::duplicate("one"));
		sad.setValue("b",charstring::duplicate("two"));
		sad.setValues(&sad);
		test("managed array, setValues(self): values kept",
				!charstring::compare(sad.getValue("a"),"one") &&
				!charstring::compare(sad.getValue("b"),"two") &&
				sad.getCount()==2);

		// setValues() from itself, array values that can't be copied
		for (uint16_t i=0; i<2; i++) {
			cvdict	selfa;
			selfa.setTrackInsertionOrder(i==1);
			selfa.setManageArrayValues(true);
			countedvalue	*a=new countedvalue[2];
			countedvalue	*b=new countedvalue[2];
			selfa.setValue("a",a);
			selfa.setValue("b",b);
			before=countedvalue::deleted;
			selfa.setValues(&selfa);
			test((!i)?"managed array, setValues(self): "
					"uncopyable values kept":
				"managed array, setValues(self) "
				"(insertion order): uncopyable values kept",
					selfa.getValue("a")==a &&
					selfa.getValue("b")==b &&
					selfa.getCount()==2);
			test((!i)?"managed array, setValues(self): "
					"nothing freed":
				"managed array, setValues(self) "
				"(insertion order): nothing freed",
					countedvalue::deleted==before);
		}
	}
	test("setValue(): no leaked values",
			countedvalue::created==countedvalue::deleted);
	stdoutput.printf("\n");

	return 0;
}
