// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/sax.h>
#include <rudiments/file.h>
#include <rudiments/memorymap.h>
#include <rudiments/charstring.h>
#include <rudiments/filesystem.h>
#include <rudiments/character.h>
#include <rudiments/url.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>

class saxprivate {
	friend class sax;
	private:
		const char	*_string;
		const char	*_ptr;
		const char	*_startptr;
		const char	*_endptr;
		file		*_fl;
		off64_t		_eof;
		bool		_mmapped;
		off64_t		_filesize;
		off64_t		_optblocksize;
		memorymap	_mm;
		off64_t		_offset;
		uint32_t	_line;
		stringbuffer	_err;
		uint64_t	_ignoreheaderlines;
		uint64_t	_ignorefooterlines;
};

sax::sax() : object() {
	pvt=new saxprivate;
	pvt->_ignoreheaderlines=0;
	pvt->_ignorefooterlines=0;
	reset();
}

sax::~sax() {
	close();
	delete pvt;
}

void sax::reset() {
	pvt->_string=NULL;
	pvt->_ptr=NULL;
	pvt->_startptr=NULL;
	pvt->_endptr=NULL;
	pvt->_fl=NULL;
	pvt->_eof=0;
	pvt->_filesize=0;
	pvt->_offset=0;
	pvt->_mmapped=false;
	pvt->_line=1;
}

void sax::setIgnoreHeaderLines(uint64_t lines) {
	pvt->_ignoreheaderlines=lines;
}

uint64_t sax::getIgnoreHeaderLines() {
	return pvt->_ignoreheaderlines;
}

void sax::setIgnoreFooterLines(uint64_t lines) {
	pvt->_ignorefooterlines=lines;
}

uint64_t sax::getIgnoreFooterLines() {
	return pvt->_ignorefooterlines;
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
		pvt->_optblocksize=1024;
		filesystem	fs;
		if (fs.open(filename)) {
			pvt->_optblocksize=fs.getOptimumTransferBlockSize();
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
		pvt->_offset=0;
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
	pvt->_string=string;
	pvt->_ptr=string;
	pvt->_startptr=string;
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
	return getCharacter(true);
}

char sax::getCharacter(bool processignores) {

	// if we're at the very beginning, then ignore footer and header lines
	if (processignores && !pvt->_offset) {
		ignoreFooterLines();
		ignoreHeaderLines();
	}

	// bail if we've hit the designated eof
	if (pvt->_eof && pvt->_offset==pvt->_eof) {
		return '\0';
	}

	// get a character from the string or file, whichever is appropriate,
	// bail and return a NULL character if we hit the end of string or file
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
			// we need to try to re-map it, if we can't, we're done
			if (!pvt->_mmapped || !mapFile()) {
				return '\0';
			}
		}
		ch=*(pvt->_ptr);
		(pvt->_ptr)++;
	} else {
		if (pvt->_fl->read(&ch)!=sizeof(char)) {
			return '\0';
		}
	}
	(pvt->_offset)++;
	if (ch=='\n') {
		(pvt->_line)++;
	}
	return ch;
}

void sax::ignoreHeaderLines() {
	uint64_t	skipped=0;
	char		ch;
	while (skipped<pvt->_ignoreheaderlines) {
		ch=getCharacter(false);
		if (ch=='\0') {
			return;
		}
		if (ch=='\n') {
			skipped++;
		}
	}
}

char sax::getCharacterBackwards() {

	if (pvt->_offset==-1) {
		return '\0';
	}

	// NOTE: It's possible for offset to go negative in the final branch
	// below.  This method handles that case, but no other code does.
	// Currently, no code other than ignoreFooterLines() calls
	// getCharacterBackwards() and it ultimately makes sure that offset is
	// positive or 0, but be aware of this possibility if any other code is
	// updated to use this method.

	// get a character from the string or file, whichever is appropriate,
	// bail and return a NULL character if we run off of the beginning of
	// the string or file
	char	ch;
	if (pvt->_string) {
		// If you've come here chasing valgrind errors...
		// ptr may be set to the return value of mmap() which is
		// neither on the stack nor in the heap.  There's no actual
		// error here, valgrind just doesn't know about variables that
		// aren't on the stack or in the heap and it thinks it's
		// uninitialized.
		if (pvt->_ptr==NULL) {
			// if we're not parsing a memory-mapped file, we're done
			// if we're parsing a memory-mapped file,
			// we need to try to re-map it, if we can't, we're done
			if (!pvt->_mmapped || !mapFile()) {
				return '\0';
			}
			pvt->_ptr=pvt->_endptr-1;
		}
		ch=*(pvt->_ptr);
		if (pvt->_ptr==pvt->_startptr) {
			pvt->_ptr=NULL;
		} else {
			(pvt->_ptr)--;
		}
		if (!pvt->_offset) {
			pvt->_ptr=NULL;
		} else {
			(pvt->_offset)--;
		}
	} else {
		pvt->_fl->setPositionRelativeToBeginning(pvt->_offset);
		if (pvt->_fl->read(&ch)!=sizeof(char)) {
			return '\0';
		}
		(pvt->_offset)--;
	}
	return ch;
}

void sax::ignoreFooterLines() {

	// bail if we're not ignoring any footer lines
	if (!pvt->_ignorefooterlines) {
		return;
	}

	// go to the very last character
	if (pvt->_mmapped) {
		pvt->_offset=pvt->_fl->getSize()-1;
		if (!mapFile()) {
			// FIXME: do something...
		}
		pvt->_ptr=pvt->_endptr-1;
	} else if (pvt->_string) {
		pvt->_offset=pvt->_endptr-pvt->_string-1;
		pvt->_ptr=pvt->_endptr-1;
	} else {
		// ugly hack...
		// setPosition*() don't work with buffered reads, so disable
		// buffering for now
		pvt->_fl->setReadBufferSize(0);
		pvt->_offset=pvt->_fl->getSize()-1;
		pvt->_fl->setPositionRelativeToEnd(-1);
	}

	// work backwards, finding carriage returns
	bool		first=true;
	uint64_t	skipped=0;
	char		ch;
	while (skipped<pvt->_ignorefooterlines) {
		ch=getCharacterBackwards();
		if (ch=='\0') {
			return;
		}
		if (first) {
			// if the last character in the file was a
			// carriage return the don't count that one
			first=false;
		} else {
			if (ch=='\n') {
				skipped++;
			}
		}
	}

	// set the end-of-file to 1 char after the
	// final carriage return that we found
	pvt->_eof=pvt->_offset+2;

	// go to the beginning...
	pvt->_offset=0;
	if (pvt->_mmapped) {
		if (!mapFile()) {
			// FIXME: do something...
		}
	} else if (pvt->_string) {
		pvt->_ptr=pvt->_string;
	} else {
		pvt->_fl->setPositionRelativeToBeginning(0);
		// ugly hack...
		// setPosition*() don't work with buffered reads, so we
		// disabled it earliser.  Re-enable it now.
		pvt->_fl->setReadBufferSize(pvt->_optblocksize);
	}
}

const char *sax::getError() {
	return pvt->_err.getString();
}

bool sax::mapFile() {

	pvt->_mmapped=false;

	// map the block that contains the offset,
	// which may not begin at the offset

	if (!memorymap::supported()) {
		return false;
	}
	if (pvt->_offset) {
		pvt->_mm.detach();
	}

	// bail if offset is past the end of the file, otherwise, if the offset
	// is contained in what would be the last block of the file if the file
	// were long enough, then the calculation below will return the last
	// block 
	if (pvt->_offset>=pvt->_filesize) {
		return false;
	}

	off64_t	startofblock=(pvt->_offset/pvt->_optblocksize)*
						pvt->_optblocksize;

	off64_t	len=pvt->_filesize-startofblock;
	if (len>pvt->_optblocksize) {
		len=pvt->_optblocksize;
	}
	if (!len) {
		return false;
	}

	if (pvt->_mm.attach(pvt->_fl->getFileDescriptor(),
				startofblock,len,PROT_READ,MAP_PRIVATE)) {
		pvt->_string=static_cast<char *>(pvt->_mm.getData());
		pvt->_ptr=pvt->_string;	
		pvt->_startptr=pvt->_string;
		pvt->_endptr=pvt->_ptr+len;
		pvt->_mmapped=true;
		return true;
	}
	return false;
}

void sax::parseFailed(const char *thing, const char *why) {
        pvt->_err.clear();
        pvt->_err.append("parse ")->append(thing);
	pvt->_err.append(" failed at line ")->append(pvt->_line);
	if (!charstring::isNullOrEmpty(why)) {
		pvt->_err.append(": ")->append(why);
	}
}
