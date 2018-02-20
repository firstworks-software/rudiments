// Copyright (c) 1999-2017 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/templateengine.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

#include <rudiments/memorymap.h>

class templateengineprivate {
	friend class templateengine;
	private:
		const char	*varstart;
		uint16_t	varstartlen;
		const char	*varend;
		uint16_t	varendlen;
		const char	*blockstartstart;
		uint16_t	blockstartstartlen;
		const char	*blockstartend;
		uint16_t	blockstartendlen;
		const char	*blockendstart;
		uint16_t	blockendstartlen;
		const char	*blockendend;
		uint16_t	blockendendlen;
		const char	*incstart;
		uint16_t	incstartlen;
		const char	*incend;
		uint16_t	incendlen;
};

templateengine::templateengine() {
	pvt=new templateengineprivate;
	pvt->varstart="$(";
	pvt->varstartlen=2;
	pvt->varend=")";
	pvt->varendlen=1;
	pvt->blockstartstart="<!-- start ";
	pvt->blockstartstartlen=11;
	pvt->blockstartend=" -->";
	pvt->blockstartendlen=4;
	pvt->blockendstart="<!-- end ";
	pvt->blockendstartlen=9;
	pvt->blockendend=" -->";
	pvt->blockendendlen=4;
	pvt->incstart="<!-- include ";
	pvt->incstartlen=13;
	pvt->incend=" -->";
	pvt->incendlen=4;
}

templateengine::~templateengine() {
	delete pvt;
}

void templateengine::setVariableStart(const char *delimiter) {
	pvt->varstart=delimiter;
	pvt->varstartlen=charstring::length(delimiter);
}

void templateengine::setVariableEnd(const char *delimiter) {
	pvt->varend=delimiter;
	pvt->varendlen=charstring::length(delimiter);
}

void templateengine::setBlockStartStart(const char *delimiter) {
	pvt->blockstartstart=delimiter;
	pvt->blockstartstartlen=charstring::length(delimiter);
}

void templateengine::setBlockStartEnd(const char *delimiter) {
	pvt->blockstartend=delimiter;
	pvt->blockstartendlen=charstring::length(delimiter);
}

void templateengine::setBlockEndStart(const char *delimiter) {
	pvt->blockendstart=delimiter;
	pvt->blockendstartlen=charstring::length(delimiter);
}

void templateengine::setBlockEndEnd(const char *delimiter) {
	pvt->blockendend=delimiter;
	pvt->blockendendlen=charstring::length(delimiter);
}

void templateengine::setIncludeStart(const char *delimiter) {
	pvt->incstart=delimiter;
	pvt->incstartlen=charstring::length(delimiter);
}

void templateengine::setIncludeEnd(const char *delimiter) {
	pvt->incend=delimiter;
	pvt->incendlen=charstring::length(delimiter);
}

const char *templateengine::getVariableStart() {
	return pvt->varstart;
}

const char *templateengine::getVariableEnd() {
	return pvt->varend;
}

const char *templateengine::getBlockStartStart() {
	return pvt->blockstartstart;
}

const char *templateengine::getBlockStartEnd() {
	return pvt->blockstartend;
}

const char *templateengine::getBlockEndStart() {
	return pvt->blockendstart;
}

const char *templateengine::getBlockEndEnd() {
	return pvt->blockendend;
}

const char *templateengine::getIncludeStart() {
	return pvt->incstart;
}

const char *templateengine::getIncludeEnd() {
	return pvt->incend;
}

bool templateengine::parse(
			stringbuffer *output,
			const char *filename,
			fileparser *fileparsers,
			dictionary< const char *, const char * > *vars) {

	// for each fileparser...
	for (uint32_t index=0; fileparsers[index].parser; index++) {

		// perform replacements
		stringbuffer	file;
		if (!parse(true,&file,
				fileparsers[index].file,
				charstring::length(fileparsers[index].file),
				NULL,vars)) {
			return false;
		}

		// if the filename matches the fileparser
		// then use it to parse the file
		if (regularexpression::match(filename,file.getString())) {
			return (fileparsers[index].parser)(
						output,filename,
						fileparsers[index].data);
		}
	}
	return false;
}

bool templateengine::parse(
			stringbuffer *output,
			const char *filename,
			blockparser *blockparsers,
			dictionary< const char *, const char * > *vars) {

	// initialize the return value
	bool	retval=false;

	// open the file
	file	templatefile;
	if (templatefile.open(filename,O_RDONLY)) {

		// get the contents of the file
		char		*filecontents;
		uint64_t	filelength=templatefile.getSize();
		bool		deleteflag=false;
		memorymap	mm;
		if (mm.attach(templatefile.getFileDescriptor(),
					0,filelength,
					PROT_READ,MAP_PRIVATE)) {
			filecontents=(char *)mm.getData();
		} else {
			filecontents=templatefile.getContents();
			deleteflag=true;
		}
		
		// parse the contents of the file
		retval=parse(false,output,filecontents,
					filelength,blockparsers,vars);

		// clean up
		templatefile.close();
		if (deleteflag) {
			delete[] filecontents;
		} else {
			mm.detach();
		}

	}
	return retval;
}

bool templateengine::parse(
			stringbuffer *output,
			const char *block,
			uint64_t blocklength,
			blockparser *blockparsers,
			dictionary< const char *, const char * > *vars) {
	return parse(false,output,block,blocklength,blockparsers,vars);
}

bool templateengine::parse(
			bool justvariables,
			stringbuffer *output,
			const char *block,
			uint64_t blocklength,
			blockparser *blockparsers,
			dictionary< const char *, const char * > *vars) {

	// handle null block
	if (!block || !blocklength) {
		return true;
	}

	// use a character pointer to iterate through the file contents
	char	*buffer=(char *)block;
	char	*endptr=(char *)block+blocklength;
	while (buffer<endptr) {

		// handle a variable replacement
		if (!charstring::compare(buffer,
					pvt->varstart,
					pvt->varstartlen)) {

			replaceVariable(output,&buffer,vars);

		// handle a block
		} else if (!justvariables && 
				!charstring::compare(buffer,
						pvt->blockstartstart,
						pvt->blockstartstartlen)) {

			// get the block name
			stringbuffer	namestr;
			if (!getBlockName(&buffer,&namestr,vars)) {
				return false;
			}

			// get the length of the block and parse the block
			char		*blockbodyptr=buffer;
			uint64_t	blockbodylen;
			if (!getBlockLength(&buffer,&blockbodylen) ||
				!parseBlock(output,
						namestr.getString(),
						blockbodyptr,blockbodylen,
						blockparsers,vars)) {
				return false;
			}

		// handle an include
		} else if (!justvariables && 
				!charstring::compare(buffer,
						pvt->incstart,
						pvt->incstartlen)) {

			// get the filename and parse the file,
			// return false on failure
			stringbuffer	filename;
			if (!getIncludeFilename(&buffer,&filename,vars) ||
				!parse(output,filename.getString(),
						blockparsers,vars)) {
				return false;
			}

		// if not any of the above, just write out the character
		} else {
			output->write((char)*buffer);
			buffer++;
		}
	}

	return true;
}

void templateengine::replaceVariable(
			stringbuffer *output, char **buffer,
			dictionary< const char *, const char * > *vars) {

	// replace the variable with the corresponding value
	char	*start=*buffer+pvt->varstartlen;
	char	*end=charstring::findFirst(start,pvt->varend);
	if (end) {
		char		*var=charstring::duplicate(start,end-start);
		const char	*repl;
		if (vars->getValue(var,&repl)) {
			output->write(repl);
			*buffer=end+pvt->varendlen;
			delete[] var;
			return;
		}
		delete[] var;
	}

	// if not found, just print out the variable-start marker
	output->write(pvt->varstart);
	(*buffer)+=pvt->varstartlen;
}

bool templateengine::getBlockName(
			char **buffer,
			stringbuffer *blockname,
			dictionary< const char *, const char * > *vars) {

	// skip past the start of the block-start marker
	*buffer=*buffer+pvt->blockstartstartlen;

	// get the block name and perform replacements on it
	if (!getName(buffer,blockname,vars,
				pvt->blockstartend,
				pvt->blockstartendlen)) {
		return false;
	}

	// skip past the end of the block-start marker
	*buffer=*buffer+pvt->blockstartendlen;
	return true;
}

bool templateengine::getBlockLength(char **buffer, uint64_t *blocklength) {

	// get the length between the block-start and block-end markers
	uint16_t	depth=0;
	uint64_t	len=0;
	for (;;) {

		// look for nested blocks
		if (!charstring::compare(*buffer,
					pvt->blockstartstart,
					pvt->blockstartstartlen)) {

			// if a nested block-start is found then increment depth
			depth++;

		} else if (!charstring::compare(*buffer,
						pvt->blockendstart,
						pvt->blockendstartlen)) {

			if (depth==0) {

				// if this is not a nested block then skip to
				// the end of it and return success
				*buffer=charstring::findFirst(
						*buffer,pvt->blockendend);
				if (*buffer) {
					*buffer=*buffer+pvt->blockendendlen;
					*blocklength=len;
					return true;
				} else {
					// someone forgot the end of the
					// block-end marker then return failure
					return false;
				}

			} else {

				// if depth is >0 then just decrement it
				depth--;
			}
		}

		// if we hit the end of the buffer then somebody forgot the
		// end-block marker, so return false
		if (!**buffer) {
			return false;
		}

		// increment the length and keep going
		len++;
		(*buffer)++;
	}
}

bool templateengine::getIncludeFilename(
			char **buffer,
			stringbuffer *filename,
			dictionary< const char *, const char * > *vars) {

	// skip past the start of the include marker
	*buffer=*buffer+pvt->incstartlen;

	// get the filename and perform replacements on it
	if (!getName(buffer,filename,vars,pvt->incend,pvt->incendlen)) {
		return false;
	}

	// skip past the end of the include marker
	*buffer=*buffer+pvt->incendlen;
	return true;
}

bool templateengine::getName(
			char **buffer,
			stringbuffer *name,
			dictionary< const char *, const char * > *vars,
			const char *end,
			uint16_t endlen) {

	char		*tempname=*buffer;
	uint64_t	tempnamelen=0;
	for (;;) {
		// if we hit the end of the buffer, someone
		// forgot the end of the block-end marker,  return false
		if (!*(*buffer+endlen)) {
			return false;
		}

		// as long as the next characters aren't the end of the
		// block-end marker, then copy the characters into the name
		if (charstring::compare(*buffer,end,endlen)) {
			tempnamelen++;
			(*buffer)++;
		} else {
			break;
		}
	}

	return parse(true,name,tempname,tempnamelen,NULL,vars);
}

bool templateengine::parseBlock(
			stringbuffer *output,
			const char *blockname,
			const char *block,
			uint64_t blocklength,
			blockparser *blockparsers,
			dictionary< const char *, const char * > *vars) {

	// look through the list of block parsers, call the appropriate one
	if (blockparsers) {
		for (uint64_t index=0; blockparsers[index].parser; index++) {
			blockparser	*sh=&blockparsers[index];
			if (!charstring::compare(blockname,sh->blockname)) {
				return sh->parser(sh->output,
							sh->blockname,
							block,blocklength,
							sh->data);
			}
		}
	}

	// if there was no block parser registered for this block,
	// just parse it using variables from the parent block
	return parse(false,output,block,blocklength,NULL,vars);
}
