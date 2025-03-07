// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/dictionary.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>
#include "test.cpp"

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

	return 0;
}
