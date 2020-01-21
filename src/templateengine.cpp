// Copyright (c) 1999-2018 David Muse
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
		const char	*qvarstart;
		uint16_t	qvarstartlen;
		const char	*qvarend;
		uint16_t	qvarendlen;
		const char	*dqvarstart;
		uint16_t	dqvarstartlen;
		const char	*dqvarend;
		uint16_t	dqvarendlen;
		char		esc;
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
		char		qset[3];
		char		dqset[3];
};

templateengine::templateengine() {
	pvt=new templateengineprivate;
	pvt->varstart="$(";
	pvt->varstartlen=2;
	pvt->varend=")";
	pvt->varendlen=1;
	pvt->qvarstart="$q(";
	pvt->qvarstartlen=3;
	pvt->qvarend=")";
	pvt->qvarendlen=1;
	pvt->dqvarstart="$dq(";
	pvt->dqvarstartlen=4;
	pvt->dqvarend=")";
	pvt->dqvarendlen=1;
	pvt->esc='\\';
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
	pvt->qset[0]='\'';
	pvt->qset[1]=pvt->esc;
	pvt->qset[2]='\0';
	pvt->dqset[0]='"';
	pvt->dqset[1]=pvt->esc;
	pvt->dqset[2]='\0';
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

void templateengine::setQuotedVariableStart(const char *delimiter) {
	pvt->qvarstart=delimiter;
	pvt->qvarstartlen=charstring::length(delimiter);
}

void templateengine::setQuotedVariableEnd(const char *delimiter) {
	pvt->qvarend=delimiter;
	pvt->qvarendlen=charstring::length(delimiter);
}

void templateengine::setDoubleQuotedVariableStart(const char *delimiter) {
	pvt->dqvarstart=delimiter;
	pvt->dqvarstartlen=charstring::length(delimiter);
}

void templateengine::setDoubleQuotedVariableEnd(const char *delimiter) {
	pvt->dqvarend=delimiter;
	pvt->dqvarendlen=charstring::length(delimiter);
}

void templateengine::setEscapeCharacter(char esc) {
	pvt->esc=esc;
	pvt->qset[1]=esc;
	pvt->dqset[1]=esc;
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

const char *templateengine::getQuotedVariableStart() {
	return pvt->qvarstart;
}

const char *templateengine::getQuotedVariableEnd() {
	return pvt->qvarend;
}

const char *templateengine::getDoubleQuotedVariableStart() {
	return pvt->dqvarstart;
}

const char *templateengine::getDoubleQuotedVariableEnd() {
	return pvt->dqvarend;
}

char templateengine::getEscapeCharacter() {
	return pvt->esc;
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
			output *out,
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
						out,filename,
						fileparsers[index].data);
		}
	}
	return false;
}

bool templateengine::parse(
			output *out,
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
		retval=parse(false,out,filecontents,
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
			output *out,
			const char *block,
			uint64_t blocklength,
			blockparser *blockparsers,
			dictionary< const char *, const char * > *vars) {
	return parse(false,out,block,blocklength,blockparsers,vars);
}

bool templateengine::parse(
			bool justvariables,
			output *out,
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

			replaceVariable(out,&buffer,vars,false,false);

		// handle quoted variable replacement
		} else if (!charstring::compare(buffer,
					pvt->qvarstart,
					pvt->qvarstartlen)) {

			replaceVariable(out,&buffer,vars,true,false);

		// handle double-quoted variable replacement
		} else if (!charstring::compare(buffer,
					pvt->dqvarstart,
					pvt->dqvarstartlen)) {

			replaceVariable(out,&buffer,vars,false,true);

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
				!parseBlock(out,
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
				!parse(out,filename.getString(),
						blockparsers,vars)) {
				return false;
			}

		// if not any of the above, just write out the character
		} else {
			out->write((char)*buffer);
			buffer++;
		}
	}

	return true;
}

void templateengine::replaceVariable(
			output *out, char **buffer,
			dictionary< const char *, const char * > *vars,
			bool escq, bool escdq) {

	// replace the variable with the corresponding value
	char	*start=*buffer+pvt->varstartlen;
	char	*end=charstring::findFirst(start,
				(escq)?pvt->qvarend:
				((escdq)?pvt->dqvarend:pvt->varend));
	if (end) {
		char		*var=charstring::duplicate(start,end-start);
		const char	*repl;
		if (vars->getValue(var,&repl)) {
			if (escq || escdq) {
				const char	*set=
						(escq)?pvt->qset:pvt->dqset;
				const char	*start=repl;
				for (;;) {
					repl=charstring::
						findFirstOfSetOrEnd(start,set);
					if (!*repl) {
						break;
					}
					out->write(start,repl-start);
					out->write(pvt->esc);
					out->write(*repl);
					start=repl+1;
				}
				out->write(start,repl-start);
			} else {
				out->write(repl);
			}
			*buffer=end+pvt->varendlen;
			delete[] var;
			return;
		}
		delete[] var;
	}

	// if not found, just print out the variable-start marker
	out->write(pvt->varstart);
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
			output *out,
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
				return sh->parser(sh->out,
							sh->blockname,
							block,blocklength,
							sh->data);
			}
		}
	}

	// if there was no block parser registered for this block,
	// just parse it using variables from the parent block
	return parse(false,out,block,blocklength,NULL,vars);
}
