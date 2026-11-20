// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/staticarray.h>
#include <rudiments/dynamicarray.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

struct test {
	char		str[100];
	uint32_t	uintval;
};

class testclass {
	public:
		testclass();
		uint32_t	getValue();
		void		setValue(uint32_t value);
	private:
		uint32_t	value;
};

testclass::testclass() {
	value=1;
}

uint32_t testclass::getValue() {
	return value;
}

void testclass::setValue(uint32_t value) {
	this->value=value;
}

#define COUNT 4096

// an element with every byte set to "value"
template< class valuetype >
static valuetype filled(byte_t value) {
	uint64_t	v=0;
	for (uint8_t i=0; i<sizeof(valuetype); i++) {
		v=(v<<8)|value;
	}
	return (valuetype)v;
}

// true if the elements of a 4 element array match
template< class valuetype >
static bool matches(staticarray<valuetype,4> &sa,
			valuetype v0, valuetype v1,
			valuetype v2, valuetype v3) {
	return (sa[0]==v0 && sa[1]==v1 && sa[2]==v2 && sa[3]==v3);
}

// set() and zero() with and without a start, on a 4 element array
template< class valuetype >
static void testSetZero() {

	stdoutput.printf("set and zero, %d byte elements...\n",
						(int)sizeof(valuetype));

	valuetype	ff=filled<valuetype>(0xff);
	valuetype	ab=filled<valuetype>(0xab);
	valuetype	x10=filled<valuetype>(0x10);
	valuetype	z=0;

	staticarray<valuetype,4>	sa;

	// fill
	sa.set(0xff);
	test("set(value), fill is exact",matches(sa,ff,ff,ff,ff));

	// start and count that are not 0
	sa.zero(2,2);
	test("zero(2,2)",matches(sa,ff,ff,z,z));
	sa.set(0xff);
	sa.set(0xab,1,2);
	test("set(0xab,1,2)",matches(sa,ff,ab,ab,ff));
	sa.set(0xff);
	sa.set(0x10,3,1);
	test("set(0x10,3,1)",matches(sa,ff,ff,ff,x10));

	// count only
	sa.set(0xff);
	sa.zero(1);
	test("zero(1)",matches(sa,z,ff,ff,ff));
	sa.set(0xff);
	sa.set(0xab,2);
	test("set(0xab,2)",matches(sa,ab,ab,ff,ff));

	// all elements
	sa.set(0xff);
	sa.set(0xab);
	test("set(0xab)",matches(sa,ab,ab,ab,ab));
	sa.zero();
	test("zero()",matches(sa,z,z,z,z));

	// start of 0
	sa.set(0xff);
	sa.set(0xab,0,3);
	test("set(0xab,0,3)",matches(sa,ab,ab,ab,ff));
	sa.set(0xff);
	sa.zero(0,3);
	test("zero(0,3)",matches(sa,z,z,z,ff));

	// count of 0
	sa.set(0xff);
	sa.set(0xab,2,0);
	test("set(0xab,2,0)",matches(sa,ff,ff,ff,ff));
	sa.set(0xab,4,0);
	test("set(0xab,4,0)",matches(sa,ff,ff,ff,ff));
	sa.zero(1,0);
	test("zero(1,0)",matches(sa,ff,ff,ff,ff));
	sa.zero(0);
	test("zero(0)",matches(sa,ff,ff,ff,ff));

	stdoutput.printf("\n");
}


int main(int argc, const char **argv) {

	header("staticarray");

	// Create a staticarray
	staticarray<struct test,COUNT>	sa1;
	stdoutput.printf("create <test>...\n");
	test("create, getCount()",sa1.getCount()==COUNT);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("set values...\n");
	uint32_t	count=COUNT;
	uint32_t	i;
	for (i=0; i<count; i++) {
		char	*val=charstring::parseNumber(i);
		charstring::copy(sa1[i].str,val);
		delete[] val;
		sa1[i].uintval=i;
	}
	test("get values, getCount()",(uint32_t)sa1.getCount()==count);
	bool	success=true;
	for (i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(sa1[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (i=0; success && i<count; i++) {
		success=(sa1[i].uintval==i);
	}
	test("get values, int",success);
	for (i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(sa1[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (i=count; success && i>0; i--) {
		success=(sa1[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// copy using =
	stdoutput.printf("copy (using =)...\n");
	staticarray<struct test,COUNT>	sa1copy1;
	sa1copy1=sa1;
	test("get values, getCount()",(uint32_t)sa1copy1.getCount()==count);
	success=true;
	for (i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(sa1copy1[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (i=0; success && i<count; i++) {
		success=(sa1copy1[i].uintval==i);
	}
	test("get values, int",success);
	for (i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(sa1copy1[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (i=count; success && i>0; i--) {
		success=(sa1copy1[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// copy using constructor
	stdoutput.printf("copy (using constructor)...\n");
	staticarray<struct test,COUNT>	sa1copy2(sa1);
	test("get values, getCount()",(uint32_t)sa1copy2.getCount()==count);
	success=true;
	for (i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(sa1copy2[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (i=0; success && i<count; i++) {
		success=(sa1copy2[i].uintval==i);
	}
	test("get values, int",success);
	for (i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(sa1copy2[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (i=count; success && i>0; i--) {
		success=(sa1copy2[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	sa1.clear();
	test("clear, getCount()",sa1.getCount()==COUNT);
	stdoutput.printf("\n");



	// create
	staticarray<testclass,COUNT>	sa2;
	stdoutput.printf("create <testclass>...\n");
	test("create, getCount()",sa2.getCount()==COUNT);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=COUNT;
	success=true;
	for (i=0; i<count; i++) {
		success=(sa2[i].getValue()==1);
	}
	test("get values, default",success);
	test("get values, getCount()",(uint32_t)sa2.getCount()==count);
	for (i=0; success && i<count; i++) {
		sa2[i].setValue(2);
	}
	for (i=0; success && i<count; i++) {
		success=(sa2[i].getValue()==2);
	}
	test("get values, forwards",success);
	for (i=count; success && i>0; i--) {
		success=(sa2[i-1].getValue()==2);
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	sa2.clear();
	test("clear, getCount()",sa2.getCount()==COUNT);
	stdoutput.printf("\n");



	// create
	staticarray<testclass *,COUNT>	sa3;
	stdoutput.printf("create <testclass *>...\n");
	test("create, getCount()",sa3.getCount()==COUNT);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=COUNT;
	success=true;
	for (i=0; i<count; i++) {
		sa3[i]=new testclass();
	}
	for (i=0; success && i<count; i++) {
		success=(sa3[i]->getValue()==1);
	}
	test("get values, default",success);
	for (i=0; i<count; i++) {
		sa3[i]->setValue(2);
	}
	for (i=0; success && i<count; i++) {
		success=(sa3[i]->getValue()==2);
	}
	test("get values, forwards",success);
	for (i=count; success && i>0; i--) {
		success=(sa3[i-1]->getValue()==2);
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	for (i=0; i<count; i++) {
		delete sa3[i];
	}
	sa3.clear();
	test("clear, getCount()",sa3.getCount()==COUNT);
	stdoutput.printf("\n");



	// create
	staticarray< staticarray< uint32_t, COUNT > ,COUNT>	sa4;
	stdoutput.printf("create nested...\n");
	test("create, getCount()",sa4.getCount()==COUNT);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=COUNT;
	success=true;
	for (i=0; i<10; i++) {
		for (uint32_t j=0; j<count; j++) {
			sa4[i][j]=i*count+j;
		}
	}
	for (i=0; success && i<10; i++) {
		for (uint32_t j=0; success && j<count; j++) {
			success=(sa4[i][j]==i*count+j);
		}
	}
	test("get values, forwards",success);
	for (i=10; success && i>0; i--) {
		for (uint32_t j=count; success && j>0; j--) {
			success=(sa4[i-1][j-1]==(i-1)*count+(j-1));
		}
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	sa4.clear();
	test("clear, getCount()",sa4.getCount()==COUNT);
	stdoutput.printf("\n");



	// create, zero, and clear (successful if it doesn't crash)
	stdoutput.printf("create, zero and clear...\n");
	staticarray<struct test *, 1024>	sa5;
	sa5.setManageValues(true);
	sa5.zero();
	sa5.clear();
	test("create, zero, clear",true);
	stdoutput.printf("\n");



	// set and zero
	testSetZero<uint8_t>();
	testSetZero<uint32_t>();
	testSetZero<uint64_t>();



	// copy and assignment
	for (uint16_t i=0; i<2; i++) {

		stdoutput.printf("copy/assignment%s...\n",(!i)?"":" (managed)");

		staticarray<char *,26>	cch1;
		cch1.setManageArrayValues(i);
		const char *values[]={
			"a","b","c","d","e","f","g","h","i","j","k","l","m",
			"n","o","p","q","r","s","t","u","v","w","x","y","z",
			NULL
		};
		uint16_t	k=0;
		for (const char * const *v=values; *v; v++) {
			if (!i) {
				cch1[k]=(char *)*v;
			} else {
				cch1[k]=charstring::duplicate((char *)*v);
			}
			k++;
		}
	
		// copy/assign to another staticarray...
		staticarray<char *,26>	cch2(cch1);
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
			for (k=0; k<26; k++) {
				if (charstring::compare(cch1[k],cch2[k])) {
					success=false;
					break;
				}
			}
			test((!j)?"copy: values":"assignment: values",success);
		}
	
		// copy/assign to a dynamicarray...
		dynamicarray<char *>	cch3(cch1);
		for (uint16_t j=0; j<3; j++) {

			// 1st iteration is copy
			// 2nd is assignment
			// 3rd is assignment after clear
			if (j==2) {
				cch3.clear();
			}
			if (j) {
				cch3=cch1;
			}

			// verify flags
			test((!j)?"copy: manage values":
					"assignment: manage values",
						cch3.getManageValues()==
						cch1.getManageValues());
			test((!j)?"copy: manage array values":
					"assignment: manage array values",
						cch3.getManageArrayValues()==
						cch1.getManageArrayValues());

			// verify length
			test((!j)?"copy: length":"assignment: length",
						cch3.getCount()==
						cch1.getCount());

			// verify values
			bool	success=true;
			for (k=0; k<26; k++) {
				if (charstring::compare(cch1[k],cch3[k])) {
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
