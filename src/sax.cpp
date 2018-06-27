// Copyright (c) 2018 David Muse
// See the COPYING file for more information

#include <rudiments/sax.h>
#include <rudiments/file.h>
#include <rudiments/memorymap.h>
#include <rudiments/charstring.h>
#include <rudiments/filesystem.h>
#include <rudiments/character.h>
#include <rudiments/url.h>
#include <rudiments/sys.h>

class saxprivate {
	friend class sax;
	private:
		const char	*_string;
		const char	*_ptr;
		const char	*_endptr;
		file		*_fl;
		bool		_mmapped;
		off64_t		_filesize;
		off64_t		_optblocksize;
		memorymap	_mm;
		off64_t		_fileoffset;
		uint32_t	_line;
		stringbuffer	_err;
};

sax::sax() {
	pvt=new saxprivate;
	reset();
}

sax::~sax() {
	close();
	delete pvt;
}

void sax::reset() {
	pvt->_string=NULL;
	pvt->_ptr=NULL;
	pvt->_endptr=NULL;
	pvt->_fl=NULL;
	pvt->_filesize=0;
	pvt->_fileoffset=0;
	pvt->_mmapped=false;
	pvt->_line=1;
}

bool sax::parseFile(const char *filename) {

	// reset string/line
	reset();

	// close any previously opened files, open the file, parse it, close
	// it again
	close();

	// skip leading whitespace
	while (*filename && character::isWhitespace(*filename)) {
		filename++;
	}

	// parse the file...
	if (!charstring::compare(filename,"file://",7)) {
		return parseLocalFile(filename+7);
	} else if (charstring::contains(filename,"://")) {
		return parseRemoteFile(filename);
	} else {
		return parseLocalFile(filename);
	}
}

bool sax::parseLocalFile(const char *filename) {

	pvt->_fl=new file();

	// open and parse the file...
	bool retval=pvt->_fl->open(filename,O_RDONLY);
	if (retval) {

		// Set the read buffer size...

		// Get the optimum transfer size for the filesystem
		// the file is found on, if possible.
		filesystem	fs;
		if (fs.open(filename)) {
			pvt->_optblocksize=fs.getOptimumTransferBlockSize();
			if (!pvt->_optblocksize) {
				pvt->_optblocksize=1024;
			}
		}

		// If we're memory mapping, since we'll use this for the
		// offsets as well, then we must use an even multiple of
		// the allocation granularity.  Use the allocation granularity
		// unless the transfer size is an even multiple of it.
		off64_t	allocgran=sys::getAllocationGranularity();
		if (allocgran>pvt->_optblocksize ||
			pvt->_optblocksize%allocgran) {
			pvt->_optblocksize=allocgran;
		}

		// get the file size
		pvt->_filesize=pvt->_fl->getSize();

		// optimize...
		pvt->_fl->setReadBufferSize(pvt->_optblocksize);
		pvt->_fl->sequentialAccess(0,pvt->_filesize);
		pvt->_fl->onlyOnce(0,pvt->_filesize);

		// Try to memorymap the file.  If it fails, that's ok, pvt->_ptr
		// will be set to NULL from the previous call to reset() and
		// will cause getCharacter() to read from the file rather than
		// the map when parse() calls it.
		pvt->_fileoffset=0;
		pvt->_mmapped=true;
		mapFile();

		// parse the file
		retval=parse();

		// unmap the file, if necessary
		if (pvt->_ptr) {
			pvt->_mm.detach();
		}
	}

	// close and return
	close();
	return retval;
}

bool sax::parseRemoteFile(const char *filename) {

	pvt->_fl=new url();

	// open and parse the file
	bool retval=(pvt->_fl->open(filename,O_RDONLY) && parse());

	// close and return
	close();
	return retval;
}

bool sax::parseString(const char *string) {

	// close any previously opened files
	close();

	// reset fd/line
	reset();

	// set string pointers
	pvt->_ptr=pvt->_string=string;
	pvt->_endptr=pvt->_string+charstring::length(string);
	pvt->_mmapped=false;

	return parse();
}

void sax::close() {

	// close any previously opened files
	delete pvt->_fl;

	// reset string/fd/line
	reset();
}

char sax::skipWhitespace(char current) {

	char	ch=current;
	int32_t	first=1;

	for (;;) {

		// the first time, just process the current character
		if (!first) {
			ch=getCharacter();
		}

		// if we find a non-whitespace character, we need to break out
		// unless it's a NULL on the first time
		if (ch!=' ' && ch!='	' && ch!='\n' && ch!='\r') {
			if (!ch) {
				if (!first) {
					break;
				}
			} else {
				break;
			}
		}

		// indicate that it's no longer the first time
		if (first) {
			first=0;
		}
	}

	// return the first character after the whitespace
	return ch;
}

char sax::getCharacter() {

	// get a character from the string or file, whichever is appropriate,
	// if the character is an EOF, return a NULL
	char	ch;
	if (pvt->_string) {
		// If you've come here chasing valgrind errors...
		// ptr may be set to the return value of mmap() which is
		// neither on the stack nor in the heap.  There's no actual
		// error here, valgrind just doesn't know about variables that
		// aren't on the stack or in the heap and it thinks it's
		// uninitialized.
		if (pvt->_ptr==pvt->_endptr) {
			// if we're not parsing a memory-mapped file, we're done
			// if we're parsing a memory-mapped file,
			// we need to try to re-map it, if we can't we're done
			if (!pvt->_mmapped || !mapFile()) {
				return '\0';
			}
		}
		ch=*(pvt->_ptr);
		(pvt->_ptr)++;
		if (pvt->_mmapped) {
			pvt->_fileoffset++;
		}
	} else {
		if (pvt->_fl->read(&ch)!=sizeof(char)) {
			return '\0';
		}
	}
	if (ch=='\n') {
		(pvt->_line)++;
	}
	return ch;
}

const char *sax::getError() {
	return pvt->_err.getString();
}


bool sax::mapFile() {

	if (!memorymap::supported()) {
		return false;
	}
	if (pvt->_fileoffset) {
		pvt->_mm.detach();
	}

	off64_t	len=pvt->_filesize-pvt->_fileoffset;
	if (len>pvt->_optblocksize) {
		len=pvt->_optblocksize;
	}
	if (!len) {
		return false;
	}

	if (pvt->_mm.attach(pvt->_fl->getFileDescriptor(),
				pvt->_fileoffset,len,PROT_READ,MAP_PRIVATE)) {
		pvt->_string=static_cast<char *>(pvt->_mm.getData());
		pvt->_ptr=pvt->_string;	
		pvt->_endptr=pvt->_ptr+len;
		return true;
	}
	return false;
}

void sax::parseFailed(const char *thing) {
        pvt->_err.clear();
        pvt->_err.append("error: parse ");
	pvt->_err.append(thing);
	pvt->_err.append(" failed at line ");
        pvt->_err.append(pvt->_line);
}
