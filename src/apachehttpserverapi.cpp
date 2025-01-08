// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

//#define DEBUG_BUFFERING 1

#if defined(DEBUG_BUFFERING)
#ifdef _MSC_VER
	#define debugPrintf(ARGS,...) stdoutput.printf(ARGS,__VA_ARGS__);
#else
	#define debugPrintf(ARGS...) stdoutput.printf(ARGS);
#endif
#endif

#include <rudiments/private/config.h>
#include <rudiments/apachehttpserverapi.h>
#include <rudiments/bytestring.h>
#include <rudiments/character.h>
#include <rudiments/charstring.h>
#include <rudiments/wstringbuffer.h>
#include <rudiments/ucs2character.h>
#include <rudiments/ucs2stringbuffer.h>

#include <rudiments/private/apacheincludes.h>

#ifdef APACHE_2
	#include "ap_compat.h"
	#define array_header const apr_array_header_t
	#define table_entry apr_table_entry_t
	#define ap_table_set apr_table_set
	#define ap_table_get apr_table_get
	#define ap_table_elts apr_table_elts
#endif

#ifndef APACHE_2
	// apache 1.x uses table instead of apr_table_t, which
	// collides with our table class, so use some trickery
	// to work around that
	#define table apr_table_t
#endif
#include <util_script.h>
#ifndef APACHE_2
	#undef table
#endif

// this matches what is defined in apachemodule.h
struct apacheapistruct {
	void	*requestrec;
	void	*serverrec;
};

class apachehttpserverapiprivate {
	friend class apachehttpserverapi;
	private:
		void		*_apistruct;

		bool		_envdirty;
		uint64_t	_envcount;
		char		**_envvars;
		char		**_envvals;

		uint16_t	_crcount;

		#ifdef APACHE_2
			apr_bucket_brigade	*_brigade;
			apr_bucket		*_bucket;
			const char		*_buffer;
		#else
			char			_buffer[HUGE_STRING_LEN];
		#endif
		const char		*_buffertail;
		const char		*_bufferptr;
};

apachehttpserverapi::apachehttpserverapi(void *apistruct) : httpserverapi() {
	pvt=new apachehttpserverapiprivate;
	pvt->_apistruct=apistruct;
	pvt->_crcount=0;
	#ifdef APACHE_2
		pvt->_brigade=NULL;
		pvt->_bucket=NULL;
		pvt->_buffer=NULL;
		pvt->_buffertail=NULL;
		pvt->_bufferptr=NULL;
	#endif
}

apachehttpserverapi::~apachehttpserverapi() {
	#ifdef APACHE_2
		if (pvt->_brigade) {
			apr_brigade_destroy(pvt->_brigade);
		}
	#endif
}

const char *apachehttpserverapi::getType() {
	#ifdef APACHE_2
		return "apache2";
	#else
		return "apache1";
	#endif
}

ssize_t apachehttpserverapi::bufferedRead(void *buf, size_t count) {

	request_rec	*r=(request_rec *)
		((apacheapistruct *)pvt->_apistruct)->requestrec;
	if (!r) {
		return RESULT_ERROR;
	}

	#if defined(DEBUG_BUFFERING)
	debugPrintf("\nbufferedRead of %d bytes\n",(int)count);
	#endif

	// do an actual buffered read...
	byte_t	*data=(byte_t *)buf;
	size_t	bytesread=0;
	size_t	bytesunread=count;

	for (;;) {

		// copy out what we can from the buffer...
		size_t	bytesavailable=pvt->_buffertail-pvt->_bufferptr;
		if (bytesavailable) {

			#if defined(DEBUG_BUFFERING)
			debugPrintf("%d bytes in read buffer\n",
						(int)bytesavailable);
			#endif

			// calculate how many bytes to copy out
			size_t	bytestocopy=(bytesavailable<bytesunread)?
						bytesavailable:bytesunread;

			#if defined(DEBUG_BUFFERING)
			debugPrintf("copying %d bytes out of read buffer\n",
							(int)bytestocopy);
			#endif

			// copy out those bytes
			bytestring::copy(data,pvt->_bufferptr,bytestocopy);

			// advance various pointers
			data+=bytestocopy;
			bytesread+=bytestocopy;
			pvt->_bufferptr+=bytestocopy;
			bytesunread-=bytestocopy;

			// return if we've copied out enough
			// to satisfy the requst
			if (bytesread==count) {
				#if defined(DEBUG_BUFFERING)
				debugPrintf("yay, we're done reading\n");
				debugPrintf("returning %d bytes\n",bytesread);
				#endif
				return bytesread;
			}

			#if defined(DEBUG_BUFFERING)
			debugPrintf("need to read %d more bytes\n",
							(int)bytesunread);
			#endif
		}

		// if we've emptied the buffer, then fill it again...
		// FIXME: I think if we're here then we must have copied out
		// the entire buffer, and pvt->_bufferptr will always equal
		// pvt->_buffertail, so this test is redundant
		if (pvt->_bufferptr==pvt->_buffertail) {

			#if defined(DEBUG_BUFFERING)
			debugPrintf("attempting to fill read buffer...\n");
			#endif

			// reset buffer pointers
			pvt->_bufferptr=NULL;
			pvt->_buffertail=NULL;

			// attempt to fill the buffer...

#ifdef APACHE_2
			pvt->_buffer=NULL;

			// if we don't already have a brigade..
			if (!pvt->_brigade) {

				#if defined(DEBUG_BUFFERING)
				debugPrintf("creating brigade...\n");
				#endif

				// create a brigade
				pvt->_brigade=apr_brigade_create(r->pool,
						r->connection->bucket_alloc);
				if (!pvt->_brigade) {
					#if defined(DEBUG_BUFFERING)
					debugPrintf("apr_brigade_create"
								" failed\n");
					#endif
					return RESULT_ERROR;
				}
			}

			// if we don't already have a bucket...
			if (!pvt->_bucket) {

				#if defined(DEBUG_BUFFERING)
				debugPrintf("populating brigade...\n");
				#endif

				// populate the brigade with the buckets that
				// were produced by the input_filters
				if (ap_get_brigade(r->input_filters,
							pvt->_brigade,
							AP_MODE_READBYTES,
							APR_BLOCK_READ,
							HUGE_STRING_LEN!=
								APR_SUCCESS)) {
					#if defined(DEBUG_BUFFERING)
					debugPrintf("EOS "
						"(apr_get_brigade failed)\n");
					debugPrintf("returning %d bytes\n",
								bytesread);
					#endif
					return bytesread;
				}

				#if defined(DEBUG_BUFFERING)
				debugPrintf("getting first bucket...\n");
				#endif

				// get the first bucket of the brigade
				pvt->_bucket=APR_BRIGADE_FIRST(pvt->_brigade);

			} else 

			// if we do already have a bucket, then we must
			// have copied out all bytes from its buffer,
			// move on to the next bucket in the brigade...
			{

				// find a valid bucket...
				for (;;) {

					#if defined(DEBUG_BUFFERING)
					debugPrintf("getting next bucket...\n");
					#endif

					// get the next bucket
					pvt->_bucket=
						APR_BUCKET_NEXT(pvt->_bucket);

					// bail if we hit end of stream
					if (APR_BUCKET_IS_EOS(pvt->_bucket)) {
						apr_brigade_cleanup(
							pvt->_brigade);
						pvt->_bucket=NULL;
						#if defined(DEBUG_BUFFERING)
						debugPrintf("EOS bucket\n");
						debugPrintf("returning "
							"%d bytes\n",bytesread);
						#endif
						return bytesread;

					}

					// loop back and get the next bucket if
					// this bucket is a "flush bucket"
					if (APR_BUCKET_IS_FLUSH(pvt->_bucket)) {
						#if defined(DEBUG_BUFFERING)
						debugPrintf("FLUSH bucket\n");
						#endif
						continue;
					}

					// At this point, we either have a 
					// bucket with data in it, or we have
					// encountered the "sentinel bucket"
					// (end of the current brigade).
					// In either case, there is no need to
					// keep looping through buckets.
					break;
				}
			}

			// at this point, we should have a bucket

			// if the bucket is the "sentinal" then we need to loop
			// back and attempt to repopulate our brigade with
			// another set of buckets from the input_filters
			if (pvt->_bucket==APR_BRIGADE_SENTINEL(pvt->_brigade)) {
				#if defined(DEBUG_BUFFERING)
				debugPrintf("SENTINEL bucket\n");
				#endif
				pvt->_bucket=NULL;
				continue;
			}

			// get the buffer from the bucket
			#if defined(DEBUG_BUFFERING)
			debugPrintf("getting buffer from bucket...\n");
			#endif
			apr_size_t	len;
			if (apr_bucket_read(pvt->_bucket,
						&(pvt->_buffer),&len,
						APR_BLOCK_READ)!=APR_SUCCESS) {
				#if defined(DEBUG_BUFFERING)
				debugPrintf("error reading...\n");
				#endif
				apr_brigade_cleanup(pvt->_brigade);
				pvt->_bucket=NULL;
				return RESULT_ERROR;
			}

			// if we encountered an EOS...
			if (!len) {
				apr_brigade_cleanup(pvt->_brigade);
				pvt->_bucket=NULL;
				#if defined(DEBUG_BUFFERING)
				debugPrintf("EOS (0 bytes in buffer)\n");
				debugPrintf("returning %d bytes\n",bytesread);
				#endif
				return bytesread;
			}

#else

			// prepare to receive a block of data
			if (ap_setup_client_block(r,
					REQUEST_CHUNKED_ERROR)!=OK) {
				#if defined(DEBUG_BUFFERING)
				debugPrintf("ap_setup_client_block failed\n");
				#endif
				return RESULT_ERROR;
			}

			// should we be receiving a block of data?
			if (!ap_should_client_block(r)) {
				#if defined(DEBUG_BUFFERING)
				debugPrintf("EOS (ap_should_client_block"
								" failed)\n");
				debugPrintf("returning %d bytes\n",bytesread);
				#endif
				return bytesread;
			}

			// receive the block of data
			int	len=ap_get_client_block(r,pvt->_buffer,
							sizeof(pvt->_buffer));
			if (!len) {
				#if defined(DEBUG_BUFFERING)
				debugPrintf("EOS (0 bytes in buffer)\n");
				debugPrintf("returning %d bytes\n",bytesread);
				#endif
				return bytesread;
			}
#endif

			// all went well
			pvt->_bufferptr=pvt->_buffer;
			pvt->_buffertail=pvt->_buffer+len;

			#if defined(DEBUG_BUFFERING)
			debugPrintf("read %d bytes:\n",(int)len);
			debugPrintf("%.*s\n",len,pvt->_buffer);
			#endif
		}
	}
}

ssize_t apachehttpserverapi::bufferedRead(char *ch) {
	return RESULT_ERROR;
}

ssize_t apachehttpserverapi::read(byte_t *buffer, size_t size) {
	return bufferedRead(buffer,size);
}

ssize_t apachehttpserverapi::read(char *buffer, size_t length) {
	return bufferedRead(buffer,length);
}

ssize_t apachehttpserverapi::read(char *character) {
	return bufferedRead(character,sizeof(char));
}

ssize_t apachehttpserverapi::read(wchar_t *buffer, size_t length) {
	return bufferedRead(buffer,length);
}

ssize_t apachehttpserverapi::read(wchar_t *character) {
	return bufferedRead(character,sizeof(wchar_t));
}

ssize_t apachehttpserverapi::readUcs2(ucs2_t *buffer, size_t length) {
	return bufferedRead(buffer,length);
}

ssize_t apachehttpserverapi::readUcs2(ucs2_t *character) {
	return bufferedRead(character,sizeof(ucs2_t));
}

ssize_t apachehttpserverapi::read(int16_t *number) {
	ssize_t	retval=bufferedRead(number,sizeof(int16_t));
	*number=filedescriptor::convertNetToHost((uint16_t)*number);
	return retval;
}

ssize_t apachehttpserverapi::read(int32_t *number) {
	ssize_t	retval=bufferedRead(number,sizeof(int32_t));
	*number=filedescriptor::convertNetToHost((uint32_t)*number);
	return retval;
}

ssize_t apachehttpserverapi::read(int64_t *number) {
	ssize_t	retval=bufferedRead(number,sizeof(int64_t));
	*number=filedescriptor::convertNetToHost((uint64_t)*number);
	return retval;
}

ssize_t apachehttpserverapi::read(byte_t *character) {
	return bufferedRead(character,sizeof(byte_t));
}

ssize_t apachehttpserverapi::read(uint16_t *number) {
	ssize_t	retval=bufferedRead(number,sizeof(uint16_t));
	*number=filedescriptor::convertNetToHost(*number);
	return retval;
}

ssize_t apachehttpserverapi::read(uint32_t *number) {
	ssize_t	retval=bufferedRead(number,sizeof(uint32_t));
	*number=filedescriptor::convertNetToHost(*number);
	return retval;
}

ssize_t apachehttpserverapi::read(uint64_t *number) {
	ssize_t	retval=bufferedRead(number,sizeof(uint64_t));
	*number=filedescriptor::convertNetToHost(*number);
	return retval;
}

ssize_t apachehttpserverapi::read(float *number) {
	return bufferedRead(number,sizeof(float));
}

ssize_t apachehttpserverapi::read(double *number) {
	return bufferedRead(number,sizeof(double));
}

void apachehttpserverapi::initEnvironmentVariables() {

	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	if (!r) {
		return;
	}

	pvt->_envdirty=false;
	pvt->_envcount=0;
	pvt->_envvars=NULL;
	pvt->_envvals=NULL;
	ap_add_common_vars(r);
	ap_add_cgi_vars(r);
}

const char *apachehttpserverapi::getEnvironmentVariable(const char *name) {

	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	if (!r) {
		return NULL;
	}

	char	*val=(char *)ap_table_get(r->subprocess_env,name);
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

	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	if (!r) {
		return false;
	}

	pvt->_envdirty=true;
	ap_table_set(r->subprocess_env,name,value);
	return true;
}

void apachehttpserverapi::updateEnvironmentVariables() {

	// don't do anything unless an environment variable has been changed
	// or if they haven't been initialized at all
	if (!pvt->_envvars || !pvt->_envdirty) {
		return;
	}

	request_rec	*r=(request_rec *)
		((apacheapistruct *)pvt->_apistruct)->requestrec;
	if (!r) {
		return;
	}


	// delete old lists
	delete[] pvt->_envvars;
	delete[] pvt->_envvals;

	// update counter
	pvt->_envcount=ap_table_elts(r->subprocess_env)->nelts;

	// create new lists
	pvt->_envvars=new char *[pvt->_envcount+1];
	pvt->_envvals=new char *[pvt->_envcount+1];
	pvt->_envvars[pvt->_envcount]=NULL;
	pvt->_envvals[pvt->_envcount]=NULL;
	
	// insert variables into lists
	array_header	*arr=ap_table_elts(r->subprocess_env);
	table_entry	*list=(table_entry *)arr->elts;
	for (uint64_t index=0; index<pvt->_envcount; index++) {
		pvt->_envvars[index]=list[index].key;
		pvt->_envvals[index]=list[index].val;
	}

	pvt->_envdirty=false;
}

httpserverapi *apachehttpserverapi::writeStatusHeader(const char *string) {

	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	if (!r) {
		return this;
	}

	charstring::copy((char *)r->status_line,string);
	return this;
}

httpserverapi *apachehttpserverapi::writeHeader(const char *variable,
							const char *value) {

	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	if (!r) {
		return this;
	}

	if (!charstring::compare(variable,"Content-type")) {
		r->content_type=value;
	} else {
		ap_table_set(r->headers_out,variable,value);
	}
	return this;
}

httpserverapi *apachehttpserverapi::writeHeader(const char *string) {

	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	if (!r) {
		return this;
	}

	if (!charstring::compare(string,"\r\n")) {
		if (pvt->_crcount==1) {
			ap_send_http_header(r);
		} else {
			pvt->_crcount++;
		}
	}
	return this;
}

ssize_t	apachehttpserverapi::write(const byte_t *string, size_t size) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r)?ap_rwrite(string,size,r):0;
}

ssize_t	apachehttpserverapi::write(const char *string) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r && string)?ap_rputs(string,r):0;
}

ssize_t	apachehttpserverapi::write(const char *string, size_t size) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r)?ap_rwrite(string,size,r):0;
}

ssize_t	apachehttpserverapi::write(char ch) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r)?ap_rputc(ch,r):0;
}

ssize_t	apachehttpserverapi::write(const wchar_t *string) {
	// FIXME: This just converts to char * and writes.
	// Is there an ar_rputwc or something like that?
	return write(string,wcharstring::getLength(string));
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
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	// FIXME: This just converts to char and writes.
	// Is there an ar_rputwc or something like that?
	return (r)?ap_rputc(character::duplicate(ch,'?'),r):0;
}

ssize_t	apachehttpserverapi::writeUcs2(const ucs2_t *string) {
	// FIXME: This just converts to char * and writes.
	// Is there an ar_rputwc or something like that?
	return writeUcs2(string,ucs2charstring::getLength(string));
}

ssize_t	apachehttpserverapi::writeUcs2(const ucs2_t *string, size_t size) {
	// FIXME: This just converts to char * and writes.
	// Is there an ar_rputwc or something like that?
	char	*s=charstring::duplicateUcs2(string,size,'?');
	ssize_t	result=write(s,size);
	delete[] s;
	return result;
}

ssize_t	apachehttpserverapi::writeUcs2(ucs2_t ch) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	// FIXME: This just converts to char and writes.
	// Is there an ar_rputwc or something like that?
	return (r)?ap_rputc(character::duplicate(ch,'?'),r):0;
}

ssize_t	apachehttpserverapi::write(int16_t number) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r)?ap_rprintf(r,"%hd",number):0;
}

ssize_t	apachehttpserverapi::write(int32_t number) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r)?ap_rprintf(r,"%d",number):0;
}

ssize_t	apachehttpserverapi::write(int64_t number) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r)?ap_rprintf(r,"%lld",(long long)number):0;
}

ssize_t	apachehttpserverapi::write(byte_t ch) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r)?ap_rputc((char)ch,r):0;
}

ssize_t	apachehttpserverapi::write(uint16_t number) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r)?ap_rprintf(r,"%hd",number):0;
}

ssize_t	apachehttpserverapi::write(uint32_t number) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r)?ap_rprintf(r,"%d",number):0;
}

ssize_t	apachehttpserverapi::write(uint64_t number) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r)?ap_rprintf(r,"%lld",(long long)number):0;
}

ssize_t	apachehttpserverapi::write(float number) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r)?ap_rprintf(r,"%f",number):0;
}

ssize_t	apachehttpserverapi::write(double number) {
	request_rec	*r=(request_rec *)
			((apacheapistruct *)pvt->_apistruct)->requestrec;
	return (r)?ap_rprintf(r,"%f",number):0;
}

ssize_t apachehttpserverapi::printfDelegate(const char *format,
							va_list *argp) {
	bytebuffer	b;
	b.printf(format,argp);
	return write(b.getBuffer(),b.getSize());
}

ssize_t apachehttpserverapi::printfDelegate(const wchar_t *format,
							va_list *argp) {
	wstringbuffer	b;
	b.printf(format,argp);
	return write(b.getString(),b.getStringLength());
}

ssize_t apachehttpserverapi::printfUcs2Delegate(const ucs2_t *format,
							va_list *argp) {
	ucs2stringbuffer	b;
	b.printfUcs2(format,argp);
	return writeUcs2(b.getString(),b.getStringLength());
}
