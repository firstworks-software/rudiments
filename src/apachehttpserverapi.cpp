// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/apachehttpserverapi.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
#include <rudiments/wstringbuffer.h>

#include <rudiments/private/apacheincludes.h>

#ifdef APACHE_2
	#include "ap_compat.h"
	#define array_header const apr_array_header_t
	#define table_entry apr_table_entry_t
	#define ap_table_set apr_table_set
	#define ap_table_get apr_table_get
	#define ap_table_elts apr_table_elts
#endif

#include <util_script.h>

class apachehttpserverapiprivate {
	friend class apachehttpserverapi;
	private:
		void		*_apistruct;

		bool		_envdirty;
		uint64_t	_envcount;
		char		**_envvars;
		char		**_envvals;

		uint16_t	_crcount;

		const unsigned char	*_stdinptr;
		size_t			_stdinpos;
		bytebuffer		*_standardin;
};

apachehttpserverapi::apachehttpserverapi(void *apistruct) : httpserverapi() {
	pvt=new apachehttpserverapiprivate;
	pvt->_apistruct=apistruct;
	pvt->_crcount=0;
	pvt->_stdinptr=NULL;
	pvt->_standardin=new bytebuffer();
}

apachehttpserverapi::~apachehttpserverapi() {
	delete pvt->_standardin;
}

bool apachehttpserverapi::getCharacter(char *ch) {

	// FIXME: this is sort of lame, it should be possible to do this
	// without first buffering the entire string
	if (!pvt->_stdinptr) {

		request_rec		*r=(request_rec *)pvt->_apistruct;
		apr_bucket_brigade	*bb=apr_brigade_create(r->pool,
						r->connection->bucket_alloc);
		bool	done=false;
		while (!done) {

			if (ap_get_brigade(r->input_filters,bb,
						AP_MODE_READBYTES,
						APR_BLOCK_READ,
						HUGE_STRING_LEN)!=APR_SUCCESS) {
				break;
			}
 
			for (apr_bucket *bucket=APR_BRIGADE_FIRST(bb);
					bucket!=APR_BRIGADE_SENTINEL(bb);
					bucket=APR_BUCKET_NEXT(bucket)) {

				if (APR_BUCKET_IS_EOS(bucket)) {
					done=true;
					break;
				}

				if (APR_BUCKET_IS_FLUSH(bucket)) {
					continue;
				}

				const char	*data;
				apr_size_t	len;
				apr_bucket_read(bucket,&data,&len,
							APR_BLOCK_READ);

				pvt->_standardin->append(data,
					static_cast<size_t>(len));
			}

			apr_brigade_cleanup(bb);
		}

		pvt->_stdinptr=pvt->_standardin->getBuffer();
		pvt->_stdinpos=0;
	}
	if (pvt->_stdinpos<pvt->_standardin->getSize()) {
		*ch=static_cast<char>(*pvt->_stdinptr);
		pvt->_stdinptr++;
		pvt->_stdinpos++;
		return true;
	}
	return false;
}

void apachehttpserverapi::initEnvironmentVariables() {
	pvt->_envdirty=false;
	pvt->_envcount=0;
	pvt->_envvars=NULL;
	pvt->_envvals=NULL;
	ap_add_common_vars((request_rec *)pvt->_apistruct);
	ap_add_cgi_vars((request_rec *)pvt->_apistruct);
}

const char *apachehttpserverapi::getEnvironmentVariable(const char *name) {
	char	*val=(char *)ap_table_get(
			((request_rec *)pvt->_apistruct)->subprocess_env,
			name);
	if (val) {
		return val;
	}
	return NULL;
}

uint64_t apachehttpserverapi::getEnvironmentVariableCount() {
	updateEnvironmentVariables();
	return pvt->_envcount;
}

const char * const *apachehttpserverapi::getEnvironmentVariables() {
	updateEnvironmentVariables();
	return pvt->_envvars;
}

const char * const *apachehttpserverapi::getEnvironmentValues() {
	updateEnvironmentVariables();
	return pvt->_envvals;
}

bool apachehttpserverapi::setEnvironmentVariable(const char *name,
							const char *value) {
	pvt->_envdirty=true;
	ap_table_set(((request_rec *)pvt->_apistruct)->
				subprocess_env,name,value);
	return true;
}

void apachehttpserverapi::updateEnvironmentVariables() {

	// don't do anything unless an environment variable has been changed
	// or if they haven't been initialized at all
	if (!pvt->_envvars || pvt->_envdirty) {

		// delete old lists
		delete[] pvt->_envvars;
		delete[] pvt->_envvals;

		// update counter
		pvt->_envcount=ap_table_elts(((request_rec *)pvt->_apistruct)->
							subprocess_env)->nelts;

		// create new lists
		pvt->_envvars=new char *[pvt->_envcount+1];
		pvt->_envvals=new char *[pvt->_envcount+1];
		pvt->_envvars[pvt->_envcount]=NULL;
		pvt->_envvals[pvt->_envcount]=NULL;
		
		// insert variables into lists
		array_header	*arr=ap_table_elts(
					((request_rec *)pvt->_apistruct)->
							subprocess_env);
		table_entry	*list=(table_entry *)arr->elts;
		for (uint64_t index=0; index<pvt->_envcount; index++) {
			pvt->_envvars[index]=list[index].key;
			pvt->_envvals[index]=list[index].val;
		}

		pvt->_envdirty=false;
	}
}

httpserverapi *apachehttpserverapi::status(const char *string) {
	charstring::copy((char *)
			((request_rec *)pvt->_apistruct)->status_line,string);
	return this;
}

httpserverapi *apachehttpserverapi::header(const char *variable,
							const char *value) {
	if (!charstring::compare(variable,"Content-type")) {
		((request_rec *)pvt->_apistruct)->content_type=value;
	} else {
		ap_table_set(((request_rec *)pvt->_apistruct)->headers_out,
				variable,value);
	}
	return this;
}

httpserverapi *apachehttpserverapi::header(const char *string) {
	if (!charstring::compare(string,"\r\n")) {
		if (pvt->_crcount==1) {
			ap_send_http_header((request_rec *)pvt->_apistruct);
		} else {
			pvt->_crcount++;
		}
	}
	return this;
}

ssize_t	apachehttpserverapi::write(const unsigned char *string, size_t size) {
	int	count=0;
	for (uint64_t index=0; index<size; index++) {
		int	result=ap_rputc(string[index],
					(request_rec *)pvt->_apistruct);
		if (result>=0) {
			count+=result;
		} else {
			return result;
		}
	}
	return count;
}

ssize_t	apachehttpserverapi::write(const char *string) {
	return ap_rputs(string,(request_rec *)pvt->_apistruct);
}

ssize_t	apachehttpserverapi::write(const char *string, size_t size) {
	int	count=0;
	for (uint64_t index=0; index<size; index++) {
		int	result=ap_rputc(string[index],
					(request_rec *)pvt->_apistruct);
		if (result>=0) {
			count+=result;
		} else {
			return result;
		}
	}
	return count;
}

ssize_t	apachehttpserverapi::write(char ch) {
	return ap_rputc(ch,(request_rec *)pvt->_apistruct);
}

ssize_t	apachehttpserverapi::write(const wchar_t *string) {
	// FIXME: This just converts to char * and writes.
	// Is there an ar_rputwc or something like that?
	return write(string,wcharstring::length(string));
}

ssize_t	apachehttpserverapi::write(const wchar_t *string, size_t size) {
	// FIXME: This just converts to char * and writes.
	// Is there an ar_rputwc or something like that?
	char	*s=charstring::duplicate(string,size,'?');
	ssize_t	result=write(s,size);
	delete[] s;
	return result;
}

ssize_t	apachehttpserverapi::write(wchar_t ch) {
	// FIXME: This just converts to char and writes.
	// Is there an ar_rputwc or something like that?
	return ap_rputc(character::duplicate(ch,'?'),
				(request_rec *)pvt->_apistruct);
}

ssize_t	apachehttpserverapi::write(int16_t number) {
	char	buffer[7];
	charstring::printf(buffer,sizeof(buffer),"%hd",number);
	return write(buffer);
}

ssize_t	apachehttpserverapi::write(int32_t number) {
	char	buffer[12];
	charstring::printf(buffer,sizeof(buffer),"%d",number);
	return write(buffer);
}

ssize_t	apachehttpserverapi::write(int64_t number) {
	char	buffer[22];
	charstring::printf(buffer,sizeof(buffer),"%lld",number);
	return write(buffer);
}

ssize_t	apachehttpserverapi::write(unsigned char ch) {
	return ap_rputc((char)ch,(request_rec *)pvt->_apistruct);
}

ssize_t	apachehttpserverapi::write(uint16_t number) {
	char	buffer[7];
	charstring::printf(buffer,sizeof(buffer),"%hd",number);
	return write(buffer);
}

ssize_t	apachehttpserverapi::write(uint32_t number) {
	char	buffer[12];
	charstring::printf(buffer,sizeof(buffer),"%d",number);
	return write(buffer);
}

ssize_t	apachehttpserverapi::write(uint64_t number) {
	char	buffer[22];
	charstring::printf(buffer,sizeof(buffer),"%lld",number);
	return write(buffer);
}

ssize_t	apachehttpserverapi::write(float number) {
	char	buffer[22];
	charstring::printf(buffer,sizeof(buffer),"%f",number);
	return write(buffer);
}

ssize_t	apachehttpserverapi::write(double number) {
	char	buffer[22];
	charstring::printf(buffer,sizeof(buffer),"%f",number);
	return write(buffer);
}

ssize_t apachehttpserverapi::printf(const char *format, ...) {
	va_list	argp;
	va_start(argp,format);
	size_t	result=printf(format,&argp);
	va_end(argp);
	return result;
}

ssize_t apachehttpserverapi::printf(const char *format, va_list *argp) {
	bytebuffer	b;
	b.printf(format,argp);
	return write(b.getBuffer(),b.getSize());
}

ssize_t apachehttpserverapi::printf(const wchar_t *format, ...) {
	va_list	argp;
	va_start(argp,format);
	size_t	result=printf(format,&argp);
	va_end(argp);
	return result;
}

ssize_t apachehttpserverapi::printf(const wchar_t *format, va_list *argp) {
	wstringbuffer	b;
	b.printf(format,argp);
	return write(b.getString(),b.getStringLength());
}
