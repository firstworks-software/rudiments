// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/input.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/bytestring.h>

input::input() {
}

input::~input() {
}

ssize_t input::read(char **buffer, const char *terminator,
				size_t maxbytes, char escapechar,
				int32_t sec, int32_t usec) {

	// initialize the return buffer
	if (buffer) {
		*buffer=NULL;
	}

	// initialize a temp buffer
	stringbuffer	temp;

	// initialize termination detector
	int32_t	termlen=charstring::length(terminator);
	char	*term=new char[termlen];
	bytestring::zero(term,termlen);

	// initialize some variables
	ssize_t	sizeread;
	char	charbuffer;
	bool	escaped=false;
	bool	copytobuffer;
	bool	copytoterm;
	ssize_t	retval=RESULT_SUCCESS;

	// loop, getting 1 character at a time
	for (;;) {

		// read from the file descriptor
		sizeread=read(&charbuffer,sec,usec);
		if (sizeread<=0) {
			retval=sizeread;
			break;
		}

		// handle escaping
		if (escaped) {
			copytobuffer=true;
			copytoterm=false;
			escaped=false;
		} else {
			escaped=((escapechar!='\0')?
					(charbuffer==escapechar):false);
			copytobuffer=!escaped;
			copytoterm=!escaped;
		}

		// copy to return buffer
		if (copytobuffer && buffer) {
			temp.append(charbuffer);
		}

		if (copytoterm) {

			// update terminator detector
			for (int32_t i=0; i<termlen-1; i++) {
				term[i]=term[i+1];
			}
			term[termlen-1]=charbuffer;

			// check for termination
			if (!charstring::compare(term,terminator,termlen)) {
				break;
			}

		} else {

			// clear terminator
			bytestring::zero(term,termlen);
		}

		// max-bytes-read condition
		if (maxbytes && temp.getSize()>maxbytes) {
			retval=RESULT_MAX;
			break;
		}
	}

	if (retval>=RESULT_SUCCESS) {

		// get the size to return
		retval=temp.getSize();

		// set the return buffer
		if (buffer) {
			*buffer=temp.detachString();
		}
	}

	// clean up
	delete[] term;

	return retval;
}

ssize_t input::read(byte_t *string, size_t size, int32_t sec, int32_t usec) {
	return read(string,size);
}

ssize_t input::read(char *string, size_t size, int32_t sec, int32_t usec) {
	return read(string,size);
}

ssize_t input::read(char *character, int32_t sec, int32_t usec) {
	return read(character);
}

ssize_t input::read(wchar_t *string, size_t size, int32_t sec, int32_t usec) {
	return read(string,size);
}

ssize_t input::read(wchar_t *character, int32_t sec, int32_t usec) {
	return read(character);
}

ssize_t input::read(ucs2_t *string, size_t size, int32_t sec, int32_t usec) {
	return read(string,size);
}

ssize_t input::read(ucs2_t *character, int32_t sec, int32_t usec) {
	return read(character);
}

ssize_t input::read(int16_t *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(int32_t *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(int64_t *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(byte_t *character, int32_t sec, int32_t usec) {
	return read(character);
}

ssize_t input::read(uint16_t *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(uint32_t *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(uint64_t *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(float *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(double *number, int32_t sec, int32_t usec) {
	return read(number);
}
