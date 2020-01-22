// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/cgihttpserverapi.h>
#include <rudiments/charstring.h>
#include <rudiments/directory.h>
#include <rudiments/environment.h>
#include <rudiments/stdio.h>
#include <rudiments/signalclasses.h>
#include <rudiments/process.h>

cgihttpserverapi::cgihttpserverapi(void *apistruct) :
					httpserverapi(apistruct) {
	this->apistruct=apistruct;
}

cgihttpserverapi::~cgihttpserverapi() {
}

void *cgihttpserverapi::getApiStruct() {
	return apistruct;
}

bool cgihttpserverapi::getCharacter(char *ch) {
	return (stdinput.read(ch)==sizeof(char));
}

void cgihttpserverapi::initEnvironmentVariables() {
	envdirty=false;
	envcount=0;
	envvars=NULL;
	envvals=NULL;

	// some web servers don't set DOCUMENT_ROOT, so we'll attempt to
	// derive it from the current working directory and SCRIPT_NAME,
	// then set it ourselves
	const char	*docroot=environment::getValue("DOCUMENT_ROOT");
	const char	*scriptname=environment::getValue("SCRIPT_NAME");
	if (!docroot && scriptname) {

		// get the current directory
		char	*docroot=directory::getCurrentWorkingDirectory();
		if (docroot) {

			// find the last dot in the scriptname
			const char	*ptr=scriptname+
					charstring::length(scriptname)-1;
			while (*ptr!='.' && ptr>scriptname) {
				ptr--;
			}
		
			// find the slash before the last dot in the scriptname
			while (*ptr!='/' && ptr>scriptname) {
				ptr--;
			}

			// if there are a group of slashes, skip back to just
			// before the first slash
			while (*ptr=='/' && ptr>scriptname) {
				ptr--;
			}

			if (ptr>scriptname) {

				// skip forward so ptr is on the slash
				ptr++;

				// copy the scriptname from the
				// beginning to the slash
				size_t	len=ptr-scriptname;
				char	*scriptdir=new char[len+1];
				charstring::copy(scriptdir,scriptname,len);
				scriptdir[len]='\0';

				// figure out where scriptdir and docroot
				// overlap and terminate docroot there
				char	*end=charstring::findFirst(docroot,
								scriptdir);
				if (end) {
					(*end)='\0';
				}
			}

			// set the DOCUMENT_ROOT environment variable
			environment::setValue("DOCUMENT_ROOT",docroot);

			// clean up
			delete[] docroot;
		}
	}
}

const char *cgihttpserverapi::getEnvironmentVariable(const char *name) {
	return environment::getValue(name);
}

uint64_t cgihttpserverapi::getEnvironmentVariableCount() {
	updateEnvironmentVariables();
	return envcount;
}

const char * const *cgihttpserverapi::getEnvironmentVariables() {
	updateEnvironmentVariables();
	return envvars;
}

const char * const *cgihttpserverapi::getEnvironmentValues() {
	updateEnvironmentVariables();
	return envvals;
}

bool cgihttpserverapi::setEnvironmentVariable(const char *name,
						const char *value) {
	envdirty=true;
	return environment::setValue(name,value);
}

void cgihttpserverapi::updateEnvironmentVariables() {

	// don't do anything unless an environment variable has been changed
	// or if they haven't been initialized at all
	if (envvars && envvals && !envdirty) {
		return;
	}

	// delete old lists
	if (envcount) {
		for (uint64_t index=0; index<envcount; index++) {
			delete[] envvars[index];
			delete[] envvals[index];
		}
	}
	delete[] envvars;
	delete[] envvals;

	// update counter
	for (envcount=0; environment::variables()[envcount]; envcount++) {}

	// create new lists
	envvars=new char *[envcount+1];
	envvals=new char *[envcount+1];
	envvars[envcount]=NULL;
	envvals[envcount]=NULL;
		
	// insert variables into lists
	for (uint64_t index=0; index<envcount; index++) {

		const char	*equal=charstring::
				findFirst(environment::variables()[index],'=');
		if (equal) {
			size_t	bytes=equal-
					environment::variables()[index];
			envvars[index]=new char[bytes+1];
			envvars[index][bytes]='\0';
			charstring::copy(envvars[index],
					environment::variables()[index],
					bytes);
			envvals[index]=charstring::duplicate(equal+1);
		} else {
			envvars[index]=NULL;
			envvals[index]=NULL;
		}
	}

	envdirty=false;
}

httpserverapi *cgihttpserverapi::status(const char *string) {
	stdoutput.write(string,charstring::length(string));
	stdoutput.write("\n",1);
	return this;
}

httpserverapi *cgihttpserverapi::header(const char *variable,
						const char *value) {
	stdoutput.write(variable,charstring::length(variable));
	stdoutput.write(": ",2);
	stdoutput.write(value,charstring::length(value));
	stdoutput.write("\r\n",2);
	return this;
}

httpserverapi *cgihttpserverapi::header(const char *string) {
	stdoutput.write(string,charstring::length(string));
	return this;
}

ssize_t	cgihttpserverapi::write(const unsigned char *string, size_t size) {
	return stdoutput.write(string,size);
}

ssize_t	cgihttpserverapi::write(const char *string) {
	return stdoutput.write(string);
}

ssize_t	cgihttpserverapi::write(const char *string, size_t size) {
	return stdoutput.write(string,size);
}

ssize_t	cgihttpserverapi::write(char character) {
	return stdoutput.write(character);
}

ssize_t	cgihttpserverapi::write(int16_t number) {
	return stdoutput.printf("%hd",number);
}

ssize_t	cgihttpserverapi::write(int32_t number) {
	return stdoutput.printf("%d",number);
}

ssize_t	cgihttpserverapi::write(int64_t number) {
	return stdoutput.printf("%lld",number);
}

ssize_t	cgihttpserverapi::write(unsigned char character) {
	return stdoutput.write(character);
}

ssize_t	cgihttpserverapi::write(uint16_t number) {
	return stdoutput.printf("%hd",number);
}

ssize_t	cgihttpserverapi::write(uint32_t number) {
	return stdoutput.printf("%d",number);
}

ssize_t	cgihttpserverapi::write(uint64_t number) {
	return stdoutput.printf("%lld",number);
}

ssize_t	cgihttpserverapi::write(float number) {
	return stdoutput.printf("%f",number);
}

ssize_t	cgihttpserverapi::write(double number) {
	return stdoutput.printf("%f",number);
}
