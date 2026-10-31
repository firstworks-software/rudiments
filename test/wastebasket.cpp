// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/wastebasket.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

class counted : public object {
	public:
			counted() { alive++; }
			~counted() { alive--; }
		static uint32_t	alive;
};

uint32_t	counted::alive=0;

int main() {

	header("wastebasket");

	wastebasket	w;

	// detach from an empty wastebasket
	stdoutput.printf("detach from empty...\n");
	char	*str=charstring::duplicate("hello");
	w.detach(str);
	w.detach((object *)NULL);
	w.empty();
	test("char * alive",!charstring::compare(str,"hello"));
	delete[] str;
	stdoutput.printf("\n");

	// attach, detach, empty
	stdoutput.printf("attach, detach, empty...\n");
	str=charstring::duplicate("hello");
	w.attach(str);
	w.detach(str);
	w.empty();
	test("char * alive",!charstring::compare(str,"hello"));
	delete[] str;

	int32_t	*i32=new int32_t[4];
	i32[0]=32;
	w.attach(i32);
	w.detach(i32);
	w.empty();
	test("int32_t * alive",i32[0]==32);
	delete[] i32;

	byte_t	*b=new byte_t[4];
	b[0]=8;
	w.attach(b);
	w.detach(b);
	w.empty();
	test("byte_t * alive",b[0]==8);
	delete[] b;

	int16_t	*i16=new int16_t[4];
	i16[0]=-16;
	w.attach(i16);
	w.detach(i16);
	w.empty();
	test("int16_t * alive",i16[0]==-16);
	delete[] i16;

	uint32_t	*u32=new uint32_t[4];
	u32[0]=32;
	w.attach(u32);
	w.detach(u32);
	w.empty();
	test("uint32_t * alive",u32[0]==32);
	delete[] u32;

	uint64_t	*u64=new uint64_t[4];
	u64[0]=64;
	w.attach(u64);
	w.detach(u64);
	w.empty();
	test("uint64_t * alive",u64[0]==64);
	delete[] u64;

	float	*fl=new float[4];
	fl[0]=1.5;
	w.attach(fl);
	w.detach(fl);
	w.empty();
	test("float * alive",fl[0]==1.5);
	delete[] fl;

	double	*db=new double[4];
	db[0]=2.5;
	w.attach(db);
	w.detach(db);
	w.empty();
	test("double * alive",db[0]==2.5);
	delete[] db;

	long double	*ld=new long double[4];
	ld[0]=3.5;
	w.attach(ld);
	w.detach(ld);
	w.empty();
	test("long double * alive",ld[0]==3.5);
	delete[] ld;

	counted	*o=new counted;
	w.attach(o);
	w.detach(o);
	w.empty();
	test("object * alive",counted::alive==1);
	delete o;
	test("object * deleted",counted::alive==0);
	stdoutput.printf("\n");

	// detach a value that was never attached
	stdoutput.printf("detach unattached...\n");
	counted	*o1=new counted;
	counted	*o2=new counted;
	w.attach(o1);
	w.detach(o2);
	w.empty();
	test("object * attached deleted",counted::alive==1);
	delete o2;
	test("object * unattached deleted",counted::alive==0);

	double	*d=new double[2];
	w.attach(d);
	double	*dother=new double[2];
	dother[0]=2.5;
	w.detach(dother);
	float	*f=new float[2];
	f[0]=1.5;
	w.detach(f);
	w.empty();
	test("double * and float * unattached",dother[0]==2.5 && f[0]==1.5);
	delete[] dother;
	delete[] f;
	stdoutput.printf("\n");

	// detach in the middle
	stdoutput.printf("detach middle...\n");
	counted	*first=new counted;
	counted	*middle=new counted;
	counted	*last=new counted;
	w.attach(first);
	w.attach(middle);
	w.attach(last);
	w.detach(middle);
	w.empty();
	test("first and last deleted",counted::alive==1);
	delete middle;
	test("middle deleted",counted::alive==0);

	uint16_t	*u1=new uint16_t[2];
	uint16_t	*u2=new uint16_t[2];
	uint16_t	*u3=new uint16_t[2];
	u2[0]=16;
	w.attach(u1);
	w.attach(u2);
	w.attach(u3);
	w.detach(u2);
	w.empty();
	test("uint16_t * middle alive",u2[0]==16);
	delete[] u2;
	stdoutput.printf("\n");

	// detach the first
	stdoutput.printf("detach first...\n");
	first=new counted;
	middle=new counted;
	last=new counted;
	w.attach(first);
	w.attach(middle);
	w.attach(last);
	w.detach(first);
	w.empty();
	test("middle and last deleted",counted::alive==1);
	delete first;
	test("first deleted",counted::alive==0);
	stdoutput.printf("\n");

	// detach the last
	stdoutput.printf("detach last...\n");
	first=new counted;
	middle=new counted;
	last=new counted;
	w.attach(first);
	w.attach(middle);
	w.attach(last);
	w.detach(last);
	w.empty();
	test("first and middle deleted",counted::alive==1);
	delete last;
	test("last deleted",counted::alive==0);
	stdoutput.printf("\n");

	// attach, detach, attach again, empty
	stdoutput.printf("re-attach...\n");
	o=new counted;
	w.attach(o);
	w.detach(o);
	w.attach(o);
	w.empty();
	test("object * deleted",counted::alive==0);

	wchar_t	*wstr=new wchar_t[4];
	w.attach(wstr);
	w.detach(wstr);
	w.attach(wstr);
	w.empty();
	test("wchar_t * deleted",true);
	stdoutput.printf("\n");

	// different types are kept apart
	stdoutput.printf("types kept apart...\n");
	o=new counted;
	w.attach(o);
	w.detach((char *)o);
	w.detach((object **)o);
	w.empty();
	test("object * not detached by char *",counted::alive==0);

	str=new char[sizeof(counted)];
	w.attach(str);
	w.detach((object *)str);
	w.detach((byte_t *)str);
	w.empty();
	test("char * not detached by object *",true);

	object	**oa=new object *[2];
	w.attach(oa);
	w.detach((object *)oa);
	w.detach(oa);
	w.empty();
	test("object ** detached",true);
	delete[] oa;
	stdoutput.printf("\n");

	// attached twice, detached once
	stdoutput.printf("attached twice...\n");
	o=new counted;
	w.attach(o);
	w.attach(o);
	w.detach(o);
	w.empty();
	test("object * deleted once",counted::alive==0);

	int64_t	*i64=new int64_t[2];
	w.attach(i64);
	w.attach(i64);
	w.detach(i64);
	w.empty();
	test("int64_t * deleted once",true);
	stdoutput.printf("\n");

	return 0;
}
