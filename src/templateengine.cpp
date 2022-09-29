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
		const char	*_varstart;
		uint16_t	_varstartlen;
		const char	*_varend;
		uint16_t	_varendlen;
		const char	*_qvarstart;
		uint16_t	_qvarstartlen;
		const char	*_qvarend;
		uint16_t	_qvarendlen;
		const char	*_dqvarstart;
		uint16_t	_dqvarstartlen;
		const char	*_dqvarend;
		uint16_t	_dqvarendlen;
		char		_esc;
		const char	*_blockstartstart;
		uint16_t	_blockstartstartlen;
		const char	*_blockstartend;
		uint16_t	_blockstartendlen;
		const char	*_blockendstart;
		uint16_t	_blockendstartlen;
		const char	*_blockendend;
		uint16_t	_blockendendlen;
		const char	*_incstart;
		uint16_t	_incstartlen;
		const char	*_incend;
		uint16_t	_incendlen;
		char		_qset[3];
		char		_dqset[3];
		stringbuffer	_err;
};

templateengine::templateengine() : object() {
	pvt=new templateengineprivate;
	pvt->_varstart="$(";
	pvt->_varstartlen=2;
	pvt->_varend=")";
	pvt->_varendlen=1;
	pvt->_qvarstart="$q(";
	pvt->_qvarstartlen=3;
	pvt->_qvarend=")";
	pvt->_qvarendlen=1;
	pvt->_dqvarstart="$dq(";
	pvt->_dqvarstartlen=4;
	pvt->_dqvarend=")";
	pvt->_dqvarendlen=1;
	pvt->_esc='\\';
	pvt->_blockstartstart="<!-- start ";
	pvt->_blockstartstartlen=11;
	pvt->_blockstartend=" -->";
	pvt->_blockstartendlen=4;
	pvt->_blockendstart="<!-- end ";
	pvt->_blockendstartlen=9;
	pvt->_blockendend=" -->";
	pvt->_blockendendlen=4;
	pvt->_incstart="<!-- include ";
	pvt->_incstartlen=13;
	pvt->_incend=" -->";
	pvt->_incendlen=4;
	pvt->_qset[0]='\'';
	pvt->_qset[1]=pvt->_esc;
	pvt->_qset[2]='\0';
	pvt->_dqset[0]='"';
	pvt->_dqset[1]=pvt->_esc;
	pvt->_dqset[2]='\0';
}

templateengine::~templateengine() {
	delete pvt;
}

void templateengine::setVariableStart(const char *delimiter) {
	pvt->_varstart=delimiter;
	pvt->_varstartlen=charstring::length(delimiter);
}

void templateengine::setVariableEnd(const char *delimiter) {
	pvt->_varend=delimiter;
	pvt->_varendlen=charstring::length(delimiter);
}

void templateengine::setQuotedVariableStart(const char *delimiter) {
	pvt->_qvarstart=delimiter;
	pvt->_qvarstartlen=charstring::length(delimiter);
}

void templateengine::setQuotedVariableEnd(const char *delimiter) {
	pvt->_qvarend=delimiter;
	pvt->_qvarendlen=charstring::length(delimiter);
}

void templateengine::setDoubleQuotedVariableStart(const char *delimiter) {
	pvt->_dqvarstart=delimiter;
	pvt->_dqvarstartlen=charstring::length(delimiter);
}

void templateengine::setDoubleQuotedVariableEnd(const char *delimiter) {
	pvt->_dqvarend=delimiter;
	pvt->_dqvarendlen=charstring::length(delimiter);
}

void templateengine::setEscapeCharacter(char esc) {
	pvt->_esc=esc;
	pvt->_qset[1]=esc;
	pvt->_dqset[1]=esc;
}

void templateengine::setBlockStartStart(const char *delimiter) {
	pvt->_blockstartstart=delimiter;
	pvt->_blockstartstartlen=charstring::length(delimiter);
}

void templateengine::setBlockStartEnd(const char *delimiter) {
	pvt->_blockstartend=delimiter;
	pvt->_blockstartendlen=charstring::length(delimiter);
}

void templateengine::setBlockEndStart(const char *delimiter) {
	pvt->_blockendstart=delimiter;
	pvt->_blockendstartlen=charstring::length(delimiter);
}

void templateengine::setBlockEndEnd(const char *delimiter) {
	pvt->_blockendend=delimiter;
	pvt->_blockendendlen=charstring::length(delimiter);
}

void templateengine::setIncludeStart(const char *delimiter) {
	pvt->_incstart=delimiter;
	pvt->_incstartlen=charstring::length(delimiter);
}

void templateengine::setIncludeEnd(const char *delimiter) {
	pvt->_incend=delimiter;
	pvt->_incendlen=charstring::length(delimiter);
}

const char *templateengine::getVariableStart() {
	return pvt->_varstart;
}

const char *templateengine::getVariableEnd() {
	return pvt->_varend;
}

const char *templateengine::getQuotedVariableStart() {
	return pvt->_qvarstart;
}

const char *templateengine::getQuotedVariableEnd() {
	return pvt->_qvarend;
}

const char *templateengine::getDoubleQuotedVariableStart() {
	return pvt->_dqvarstart;
}

const char *templateengine::getDoubleQuotedVariableEnd() {
	return pvt->_dqvarend;
}

char templateengine::getEscapeCharacter() {
	return pvt->_esc;
}

const char *templateengine::getBlockStartStart() {
	return pvt->_blockstartstart;
}

const char *templateengine::getBlockStartEnd() {
	return pvt->_blockstartend;
}

const char *templateengine::getBlockEndStart() {
	return pvt->_blockendstart;
}

const char *templateengine::getBlockEndEnd() {
	return pvt->_blockendend;
}

const char *templateengine::getIncludeStart() {
	return pvt->_incstart;
}

const char *templateengine::getIncludeEnd() {
	return pvt->_incend;
}

bool templateengine::parse(
			output *out,
			const char *filename,
			fileparser *fileparsers,
			dictionary< const char *, const char * > *vars) {

	// clear any lingering error
	pvt->_err.clear();

	// for each fileparser...
	for (uint32_t index=0; fileparsers[index].parser; index++) {

		// perform replacements
		stringbuffer	file;
		if (!parse(true,&file,
				fileparsers[index].file,
				charstring::length(fileparsers[index].file),
				NULL,vars)) {
			if (!pvt->_err.getSize()) {
				pvt->_err.appendFormatted(
					"parse(%s) failed: "
					"filename replacements failed\n",
					filename);
			}
			return false;
		}

		// if the filename matches the fileparser
		// then use it to parse the file
		if (regularexpression::match(filename,file.getString())) {
			if ((fileparsers[index].parser)(
						out,filename,
						fileparsers[index].data)) {
				return true;
			} else {
				if (!pvt->_err.getSize()) {
					pvt->_err.appendFormatted(
						"parse(%s) failed: "
						"file handler returned false\n",
						filename);
				}
				return false;
			}
		}
	}
	if (!pvt->_err.getSize()) {
		pvt->_err.appendFormatted(
			"parse(%s) failed: no matching file parser\n",filename);
	}
	return false;
}

bool templateengine::parse(
			output *out,
			const char *filename,
			blockparser *blockparsers,
			dictionary< const char *, const char * > *vars) {

	// clear any lingering error
	pvt->_err.clear();

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

	} else {
		char	*err=error::getErrorString();
		if (!pvt->_err.getSize()) {
			pvt->_err.appendFormatted(
				"parse(%s) failed: %s\n",filename,err);
		}
		delete[] err;
	}
	return retval;
}

bool templateengine::parse(
			output *out,
			const char *block,
			uint64_t blocklength,
			blockparser *blockparsers,
			dictionary< const char *, const char * > *vars) {

	// clear any lingering error
	pvt->_err.clear();

	// parse the block
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
					pvt->_varstart,
					pvt->_varstartlen)) {

			replaceVariable(out,&buffer,vars,false,false);

		// handle quoted variable replacement
		} else if (!charstring::compare(buffer,
					pvt->_qvarstart,
					pvt->_qvarstartlen)) {

			replaceVariable(out,&buffer,vars,true,false);

		// handle double-quoted variable replacement
		} else if (!charstring::compare(buffer,
					pvt->_dqvarstart,
					pvt->_dqvarstartlen)) {

			replaceVariable(out,&buffer,vars,false,true);

		// handle a block
		} else if (!justvariables && 
				!charstring::compare(buffer,
						pvt->_blockstartstart,
						pvt->_blockstartstartlen)) {

			// get the block name
			stringbuffer	namestr;
			if (!getBlockName(&buffer,&namestr,vars)) {
				return false;
			}

			// get the length of the block and parse the block
			char		*blockbodyptr=buffer;
			uint64_t	blockbodylen;
			if (!getBlockLength(namestr.getString(),
						&buffer,&blockbodylen) ||
				!parseBlock(out,namestr.getString(),
						blockbodyptr,blockbodylen,
						blockparsers,vars)) {
				return false;
			}

		// handle an include
		} else if (!justvariables && 
				!charstring::compare(buffer,
						pvt->_incstart,
						pvt->_incstartlen)) {

			// get the filename and parse the file,
			// return false on failure
			stringbuffer	filename;
			if (!getIncludeFileName(&buffer,&filename,vars) ||
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
	char		*start;
	const char	*startpattern;
	uint16_t	startpatternlen;
	const char	*endpattern;
	uint16_t	endpatternlen;
	if (escq) {
		startpattern=pvt->_qvarstart;
		startpatternlen=pvt->_qvarstartlen;
		endpattern=pvt->_qvarend;
		endpatternlen=pvt->_qvarendlen;
		start=*buffer+pvt->_qvarstartlen;
	} else if (escdq) {
		startpattern=pvt->_dqvarstart;
		startpatternlen=pvt->_dqvarstartlen;
		endpattern=pvt->_dqvarend;
		endpatternlen=pvt->_dqvarendlen;
		start=*buffer+pvt->_dqvarstartlen;
	} else {
		startpattern=pvt->_varstart;
		startpatternlen=pvt->_varstartlen;
		endpattern=pvt->_varend;
		endpatternlen=pvt->_varendlen;
		start=*buffer+pvt->_varstartlen;
	}
	char	*end=charstring::findFirst(start,endpattern);
	if (vars && end) {
		char		*var=charstring::duplicate(start,end-start);
		const char	*repl;
		if (vars->getValue(var,&repl)) {
			if (escq || escdq) {
				const char	*set=
						(escq)?pvt->_qset:pvt->_dqset;
				const char	*start=repl;
				for (;;) {
					repl=charstring::
						findFirstOfSetOrEnd(start,set);
					if (!*repl) {
						break;
					}
					out->write(start,repl-start);
					out->write(pvt->_esc);
					out->write(*repl);
					start=repl+1;
				}
				out->write(start,repl-start);
			} else {
				out->write(repl);
			}
			*buffer=end+endpatternlen;
			delete[] var;
			return;
		}
		delete[] var;
	}

	// if not found, just print out the variable-start marker
	out->write(startpattern);
	(*buffer)+=startpatternlen;
}

bool templateengine::getBlockName(
			char **buffer,
			stringbuffer *blockname,
			dictionary< const char *, const char * > *vars) {

	// skip past the start of the block-start marker
	*buffer=*buffer+pvt->_blockstartstartlen;

	// get the block name and perform replacements on it
	if (!getName(buffer,blockname,vars,
				pvt->_blockstartend,
				pvt->_blockstartendlen)) {
		if (!pvt->_err.getSize()) {
			pvt->_err.appendFormatted(
				"getBlockName(%s) failed: getName() failed\n",
				blockname->getString());
		}
		return false;
	}

	// skip past the end of the block-start marker
	*buffer=*buffer+pvt->_blockstartendlen;
	return true;
}

bool templateengine::getBlockLength(const char *blockname,
					char **buffer,
					uint64_t *blocklength) {

	// get the length between the block-start and block-end markers
	uint16_t	depth=0;
	uint64_t	len=0;
	for (;;) {

		// look for nested blocks
		if (!charstring::compare(*buffer,
					pvt->_blockstartstart,
					pvt->_blockstartstartlen)) {

			// if a nested block-start is found then increment depth
			depth++;

		} else if (!charstring::compare(*buffer,
						pvt->_blockendstart,
						pvt->_blockendstartlen)) {

			if (depth==0) {

				// if this is not a nested block then skip to
				// the end of it and return success
				*buffer=charstring::findFirst(
						*buffer,pvt->_blockendend);
				if (*buffer) {
					*buffer=*buffer+pvt->_blockendendlen;
					*blocklength=len;
					return true;
				} else {
					// someone forgot the end of the
					// block-end marker then return failure
					if (!pvt->_err.getSize()) {
						pvt->_err.appendFormatted(
							"getBlockLength(%s) "
							"failed: no "
							"end-of-block marker\n",
							blockname);
					}
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
			if (!pvt->_err.getSize()) {
				pvt->_err.appendFormatted(
					"getBlockLength(%s) failed: "
					"no end-of-block marker\n",blockname);
			}
			return false;
		}

		// increment the length and keep going
		len++;
		(*buffer)++;
	}
}

bool templateengine::getIncludeFileName(
			char **buffer,
			stringbuffer *filename,
			dictionary< const char *, const char * > *vars) {

	// skip past the start of the include marker
	*buffer=*buffer+pvt->_incstartlen;

	// get the filename and perform replacements on it
	if (!getName(buffer,filename,vars,pvt->_incend,pvt->_incendlen)) {
		if (!pvt->_err.getSize()) {
			pvt->_err.appendFormatted(
				"getIncludeFileName(%s) failed: "
				"getName() failed\n",
				filename->getString());
		}
		return false;
	}

	// skip past the end of the include marker
	*buffer=*buffer+pvt->_incendlen;
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
			if (!pvt->_err.getSize()) {
				pvt->_err.append("getName() failed: "
						"no end-of-block marker\n");
			}
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
				if (sh->parser(sh->out,sh->blockname,
							block,blocklength,
							sh->data)) {
					return true;
				} else {
					if (!pvt->_err.getSize()) {
						pvt->_err.appendFormatted(
							"parse(%s) failed: "
							"block handler "
							"returned false\n",
							blockname);
					}
					return false;
				}
			}
		}
	}

	// if there was no block parser registered for this block,
	// just parse it using variables from the parent block
	return parse(false,out,block,blocklength,NULL,vars);
}

const char *templateengine::getError() {
	return pvt->_err.getString();
}
