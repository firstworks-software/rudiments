// Copyright (c) 2017  David Muse
// See the file COPYING for more information

#include <rudiments/datetime.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>

#define APPENDITERATIONS 1024*1024*2
#define PARSEITERATIONS 1024*1024*4
#define LENGTHITERATIONS 1024*1024*10

void displayTime(datetime *start, datetime *end) {
	uint32_t	sec=end->getEpoch()-start->getEpoch();
	int32_t		usec=end->getMicroseconds()-start->getMicroseconds();
	if (usec<0) {
		sec--;
		usec=usec+100000;
	}
	long double	totalsec=(long double)sec+
					(((long double)usec)/1000000.0);
	stdoutput.printf("%Lf\n",totalsec);
}

int main() {

	datetime	start;
	datetime	end;
	uint16_t	len;

	stdoutput.printf("append(int64_t)\n");
	start.getSystemDateAndTime();
	stringbuffer	str(1024,1024);
	for (uint64_t i=0; i<APPENDITERATIONS; i++) {
		str.append((int64_t)1234567890123456789);
		str.append((int64_t)123456789012345678);
		str.append((int64_t)12345678901234567);
		str.append((int64_t)1234567890123456);
		str.append((int64_t)123456789012345);
		str.append((int64_t)12345678901234);
		str.append((int64_t)1234567890123);
		str.append((int64_t)123456789012);
		str.append((int64_t)12345678901);
		str.append((int64_t)1234567890);
		str.append((int64_t)123456789);
		str.append((int64_t)12345678);
		str.append((int64_t)1234567);
		str.append((int64_t)123456);
		str.append((int64_t)12345);
		str.append((int64_t)1234);
		str.append((int64_t)123);
		str.append((int64_t)12);
		str.append((int64_t)1);
		str.append((int64_t)-1234567890123456789);
		str.append((int64_t)-123456789012345678);
		str.append((int64_t)-12345678901234567);
		str.append((int64_t)-1234567890123456);
		str.append((int64_t)-123456789012345);
		str.append((int64_t)-12345678901234);
		str.append((int64_t)-1234567890123);
		str.append((int64_t)-123456789012);
		str.append((int64_t)-12345678901);
		str.append((int64_t)-1234567890);
		str.append((int64_t)-123456789);
		str.append((int64_t)-12345678);
		str.append((int64_t)-1234567);
		str.append((int64_t)-123456);
		str.append((int64_t)-12345);
		str.append((int64_t)-1234);
		str.append((int64_t)-123);
		str.append((int64_t)-12);
		str.append((int64_t)-1);
		str.clear();
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("parseNumber(int64_t)\n");
	start.getSystemDateAndTime();
	for (uint64_t i=0; i<PARSEITERATIONS; i++) {
		delete[] charstring::parseNumber((int64_t)1234567890123456789);
		delete[] charstring::parseNumber((int64_t)123456789012345678);
		delete[] charstring::parseNumber((int64_t)12345678901234567);
		delete[] charstring::parseNumber((int64_t)1234567890123456);
		delete[] charstring::parseNumber((int64_t)123456789012345);
		delete[] charstring::parseNumber((int64_t)12345678901234);
		delete[] charstring::parseNumber((int64_t)1234567890123);
		delete[] charstring::parseNumber((int64_t)123456789012);
		delete[] charstring::parseNumber((int64_t)12345678901);
		delete[] charstring::parseNumber((int64_t)1234567890);
		delete[] charstring::parseNumber((int64_t)123456789);
		delete[] charstring::parseNumber((int64_t)12345678);
		delete[] charstring::parseNumber((int64_t)1234567);
		delete[] charstring::parseNumber((int64_t)123456);
		delete[] charstring::parseNumber((int64_t)12345);
		delete[] charstring::parseNumber((int64_t)1234);
		delete[] charstring::parseNumber((int64_t)123);
		delete[] charstring::parseNumber((int64_t)12);
		delete[] charstring::parseNumber((int64_t)1);
		delete[] charstring::parseNumber((int64_t)-1234567890123456789);
		delete[] charstring::parseNumber((int64_t)-123456789012345678);
		delete[] charstring::parseNumber((int64_t)-12345678901234567);
		delete[] charstring::parseNumber((int64_t)-1234567890123456);
		delete[] charstring::parseNumber((int64_t)-123456789012345);
		delete[] charstring::parseNumber((int64_t)-12345678901234);
		delete[] charstring::parseNumber((int64_t)-1234567890123);
		delete[] charstring::parseNumber((int64_t)-123456789012);
		delete[] charstring::parseNumber((int64_t)-12345678901);
		delete[] charstring::parseNumber((int64_t)-1234567890);
		delete[] charstring::parseNumber((int64_t)-123456789);
		delete[] charstring::parseNumber((int64_t)-12345678);
		delete[] charstring::parseNumber((int64_t)-1234567);
		delete[] charstring::parseNumber((int64_t)-123456);
		delete[] charstring::parseNumber((int64_t)-12345);
		delete[] charstring::parseNumber((int64_t)-1234);
		delete[] charstring::parseNumber((int64_t)-123);
		delete[] charstring::parseNumber((int64_t)-12);
		delete[] charstring::parseNumber((int64_t)-1);
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("integerLength(int64_t)\n");
	start.getSystemDateAndTime();
	for (uint64_t i=0; i<LENGTHITERATIONS; i++) {
		len=charstring::integerLength((int64_t)1234567890123456789);
		len=charstring::integerLength((int64_t)123456789012345678);
		len=charstring::integerLength((int64_t)12345678901234567);
		len=charstring::integerLength((int64_t)1234567890123456);
		len=charstring::integerLength((int64_t)123456789012345);
		len=charstring::integerLength((int64_t)12345678901234);
		len=charstring::integerLength((int64_t)1234567890123);
		len=charstring::integerLength((int64_t)123456789012);
		len=charstring::integerLength((int64_t)12345678901);
		len=charstring::integerLength((int64_t)1234567890);
		len=charstring::integerLength((int64_t)123456789);
		len=charstring::integerLength((int64_t)12345678);
		len=charstring::integerLength((int64_t)1234567);
		len=charstring::integerLength((int64_t)123456);
		len=charstring::integerLength((int64_t)12345);
		len=charstring::integerLength((int64_t)1234);
		len=charstring::integerLength((int64_t)123);
		len=charstring::integerLength((int64_t)12);
		len=charstring::integerLength((int64_t)1);
		len=charstring::integerLength((int64_t)-1234567890123456789);
		len=charstring::integerLength((int64_t)-123456789012345678);
		len=charstring::integerLength((int64_t)-12345678901234567);
		len=charstring::integerLength((int64_t)-1234567890123456);
		len=charstring::integerLength((int64_t)-123456789012345);
		len=charstring::integerLength((int64_t)-12345678901234);
		len=charstring::integerLength((int64_t)-1234567890123);
		len=charstring::integerLength((int64_t)-123456789012);
		len=charstring::integerLength((int64_t)-12345678901);
		len=charstring::integerLength((int64_t)-1234567890);
		len=charstring::integerLength((int64_t)-123456789);
		len=charstring::integerLength((int64_t)-12345678);
		len=charstring::integerLength((int64_t)-1234567);
		len=charstring::integerLength((int64_t)-123456);
		len=charstring::integerLength((int64_t)-12345);
		len=charstring::integerLength((int64_t)-1234);
		len=charstring::integerLength((int64_t)-123);
		len=charstring::integerLength((int64_t)-12);
		len=charstring::integerLength((int64_t)-1);
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("integerLength(int32_t)\n");
	start.getSystemDateAndTime();
	for (uint64_t i=0; i<LENGTHITERATIONS; i++) {
		len=charstring::integerLength((int32_t)1234567890);
		len=charstring::integerLength((int32_t)123456789);
		len=charstring::integerLength((int32_t)12345678);
		len=charstring::integerLength((int32_t)1234567);
		len=charstring::integerLength((int32_t)123456);
		len=charstring::integerLength((int32_t)12345);
		len=charstring::integerLength((int32_t)1234);
		len=charstring::integerLength((int32_t)123);
		len=charstring::integerLength((int32_t)12);
		len=charstring::integerLength((int32_t)1);
		len=charstring::integerLength((int32_t)-1234567890);
		len=charstring::integerLength((int32_t)-123456789);
		len=charstring::integerLength((int32_t)-12345678);
		len=charstring::integerLength((int32_t)-1234567);
		len=charstring::integerLength((int32_t)-123456);
		len=charstring::integerLength((int32_t)-12345);
		len=charstring::integerLength((int32_t)-1234);
		len=charstring::integerLength((int32_t)-123);
		len=charstring::integerLength((int32_t)-12);
		len=charstring::integerLength((int32_t)-1);
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("integerLength(int32_t)\n");
	start.getSystemDateAndTime();
	for (uint64_t i=0; i<LENGTHITERATIONS; i++) {
		len=charstring::integerLength((int16_t)12345);
		len=charstring::integerLength((int16_t)1234);
		len=charstring::integerLength((int16_t)123);
		len=charstring::integerLength((int16_t)12);
		len=charstring::integerLength((int16_t)1);
		len=charstring::integerLength((int16_t)-12345);
		len=charstring::integerLength((int16_t)-1234);
		len=charstring::integerLength((int16_t)-123);
		len=charstring::integerLength((int16_t)-12);
		len=charstring::integerLength((int16_t)-1);
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("%d\n",len);
}
