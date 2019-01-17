// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/websocket.h>
#include <rudiments/dictionary.h>
#include <rudiments/bytestring.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/sha1.h>

// FIXME do this some other way...
#define MAXBYTES 1024*1024*512

class websocketprivate {
	friend class websocket;
	private:
		bool		_isclient;
		filedescriptor	*_fd;
		uint64_t	_bufferpos;
		uint64_t	_buffersize;
		unsigned char	*_buffer;

		dictionary<char *,char *>	_headers;
};

websocket::websocket() : securitycontext() {
	pvt=new websocketprivate;
	pvt->_isclient=false;
	pvt->_fd=NULL;
	pvt->_bufferpos=0;
	pvt->_buffersize=0;
	pvt->_buffer=NULL;
}

websocket::~websocket() {
	delete[] pvt->_buffer;
	pvt->_headers.clearAndArrayDelete();
	delete pvt;
}

void websocket::setFileDescriptor(filedescriptor *fd) {
	pvt->_fd=fd;
}

filedescriptor *websocket::getFileDescriptor() {
	return pvt->_fd;
}

bool websocket::connect() {

	// we're a client
	pvt->_isclient=true;

	return false;
}

bool websocket::accept() {

	// init everything here because of goto's
	bool		retval=true;
	char		*request=NULL;
	ssize_t		result=0;
	char		*hdrs=NULL;
	char		**headers=NULL;
	uint64_t	headercount=0;
	char		*key=NULL;
	bytebuffer	fullkey;
	sha1		s;
	const unsigned char	*hashedkey=NULL;
	uint64_t		hashedkeysize=0;
	char		*encodedkey=NULL;
	stringbuffer	response;

	// temporarily disable the security context so
	// local reads don't use websocket::read();
	if (pvt->_fd->getSecurityContext()!=this) {
stdoutput.printf("accept fail 0\n");
		retval=false;
		goto end;
	}
	pvt->_fd->setSecurityContext(NULL);

	// read the request
	result=pvt->_fd->read(&request,"\r\n",MAXBYTES);
	if (result==RESULT_ERROR) {
stdoutput.printf("accept fail 1\n");
		// FIXME: set error or something...
		retval=false;
		goto end;
	} else if (result==RESULT_TIMEOUT) {
stdoutput.printf("accept fail 2\n");
		// FIXME: set error or something...
		retval=false;
		goto end;
	} else if (result==RESULT_MAX) {
stdoutput.printf("accept fail 3\n");
		// FIXME: set error or something...
		retval=false;
		goto end;
	}

	// validate the request
	// FIXME: support other http protocols...
	if (charstring::compare(request,"GET / HTTP/1.1\r\n")) {
stdoutput.printf("accept fail 4\n");
		// FIXME: set error or something...
		retval=false;
		goto end;
	}

	// read headers
	result=pvt->_fd->read(&hdrs,"\r\n\r\n",MAXBYTES);
	if (result==RESULT_ERROR) {
stdoutput.printf("accept fail 5\n");
		// FIXME: set error or something...
		retval=false;
		goto end;
	} else if (result==RESULT_TIMEOUT) {
stdoutput.printf("accept fail 6\n");
		// FIXME: set error or something...
		retval=false;
		goto end;
	} else if (result==RESULT_MAX) {
stdoutput.printf("accept fail 7\n");
		// FIXME: set error or something...
		retval=false;
		goto end;
	}

	// split headers into dictionary
	charstring::split(hdrs,"\r\n",true,&headers,&headercount);
	for (uint64_t i=0; i<headercount; i++) {
		char		**parts=NULL;
		uint64_t	partcount=0;
		charstring::split(headers[i],": ",true,&parts,&partcount);
		if (partcount>=2) {
			pvt->_headers.setValue(parts[0],parts[1]);
		}
		delete[] headers[i];
	}
	delete[] headers;

	// FIXME: get/eval Sec-WebSocket-Version

	// get Sec-WebSocket-Key
	key=pvt->_headers.getValue((char *)"Sec-WebSocket-Version");
	if (!key) {
stdoutput.printf("accept fail 8\n");
		// FIXME: set error or something...
		retval=false;
		goto end;
	}
	charstring::leftTrim(key);

	// hash key
	fullkey.append(key);
	fullkey.append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	s.append(fullkey.getBuffer(),fullkey.getSize());
	hashedkey=s.getHash();
	hashedkeysize=s.getHashLength();

	// encode response key
	encodedkey=charstring::base64Encode(hashedkey,hashedkeysize);

	// build response headers
	response.append("HTTP/1.1 101 Switching Protocols\r\n");
	response.append("Upgrade: websocket\r\n");
	response.append("Connection: Upgrade\r\n");
	response.append("Sec-WebSocket-Accept: ");
	response.append(encodedkey);
	response.append("\r\n");
	response.append("Sec-WebSocket-Protocol: chat\r\n");
	response.append("\r\n\r\n");

	// send response headers
	result=pvt->_fd->write(response.getString(),response.getSize());
	if (result!=(ssize_t)response.getSize()) {
		if (result==RESULT_ERROR) {
stdoutput.printf("accept fail 9\n");
			// FIXME: set error or something...
		} else if (result==RESULT_TIMEOUT) {
stdoutput.printf("accept fail 10\n");
			// FIXME: set error or something...
		} else if (result==RESULT_MAX) {
stdoutput.printf("accept fail 11\n");
			// FIXME: set error or something...
		} else {
stdoutput.printf("accept fail 12\n");
			// FIXME: set error or something...
		}
		retval=false;
		goto end;
	}

end:
	// reset security context
	pvt->_fd->setSecurityContext(this);

	// we're not a client
	pvt->_isclient=false;

	// return result
	return retval;
}

ssize_t websocket::read(void *buf, ssize_t count) {

	// copy out any existing buffered data
	ssize_t	bytescopiedout=copyOut(buf,count);
	if (bytescopiedout==count) {
		return count;
	}

	// decrement count by whatever we got from the buffer
	count-=bytescopiedout;

	// get more...

	// init everything here because of goto's
	ssize_t		retval=0;
	unsigned char	firstbyte=0;
	/*unsigned char	flags=0;
	unsigned char	opcode=0;
	unsigned char	fin=0;
	unsigned char	rsv1=0;
	unsigned char	rsv2=0;
	unsigned char	rsv3=0;*/
	unsigned char	mask=0;
	unsigned char	payloadlen1=0;
	uint16_t	payloadlen2=0;
	uint64_t	payloadlen3=0;
	uint64_t	payloadlen=0;
	uint32_t	maskingkey=0;

	// temporarily disable the security context so
	// local reads don't use websocket::read();
	if (pvt->_fd->getSecurityContext()!=this) {
stdoutput.printf("read fail 0\n");
		retval=RESULT_ERROR;
		goto end;
	}
	pvt->_fd->setSecurityContext(NULL);


	// read header...

	// read first byte
	retval=pvt->_fd->read(&firstbyte);
	if (retval!=sizeof(firstbyte)) {
stdoutput.printf("read fail 1\n");
		goto end;
	}

	// first byte is 4 flags and 4-bit opcode
	// FIXME: do something with these...
	/*flags=firstbyte>>4;
	opcode=firstbyte&0x0f;*/

	// parse flags
	// FIXME: do something with these...
	/*fin=flags&0x08;
	rsv1=flags&0x04;
	rsv2=flags&0x02;
	rsv3=flags&0x01;*/

	// read payload length
	retval=pvt->_fd->read(&payloadlen1);
	if (retval!=sizeof(payloadlen1)) {
stdoutput.printf("read fail 2\n");
		goto end;
	}

	// first bit is the mask, next 7 are actual payload length
	mask=payloadlen1&0x80;
	payloadlen=payloadlen&0x7f;

	// if this is a client then mask must be 1
	// if this is a server then mask must be 0
	if ((!pvt->_isclient && mask) || (pvt->_isclient && !mask)) {
		// FIXME: close the connection if this is wrong
stdoutput.printf("read fail 2.5\n");
		retval=RESULT_ERROR;
		goto end;
	}

	// read extended payload length
	if (payloadlen1==126) {
		retval=pvt->_fd->read(&payloadlen2);
		if (retval!=sizeof(payloadlen2)) {
stdoutput.printf("read fail 3\n");
			goto end;
		}
		pvt->_buffersize=payloadlen2;
	} else if (payloadlen1==127) {
		retval=pvt->_fd->read(&payloadlen3);
		if (retval!=sizeof(payloadlen3)) {
stdoutput.printf("read fail 4\n");
			goto end;
		}
		pvt->_buffersize=payloadlen3;
	} else {
		pvt->_buffersize=payloadlen1;
	}

	// read masking key
	if (mask) {
		retval=pvt->_fd->read(&maskingkey);
		if (retval!=sizeof(maskingkey)) {
stdoutput.printf("read fail 5\n");
			goto end;
		}
	}


	// read payload
	delete[] pvt->_buffer;
	pvt->_buffer=new unsigned char[pvt->_buffersize];
	retval=pvt->_fd->read(pvt->_buffer,pvt->_buffersize);
	if (retval!=(ssize_t)pvt->_buffersize) {
stdoutput.printf("read fail 6\n");
		goto end;
	}


	// copy out what we can
	retval=copyOut(buf,count);

end:
	// reset security context
	pvt->_fd->setSecurityContext(this);

	// return result
	return retval;
}

ssize_t websocket::copyOut(void *buf, ssize_t count) {

	ssize_t	bytestocopy=count;
	if (bytestocopy>(ssize_t)(pvt->_buffersize-pvt->_bufferpos)) {
		bytestocopy=pvt->_buffersize-pvt->_bufferpos;
	}

	bytestring::copy(buf,pvt->_buffer+pvt->_bufferpos,bytestocopy);
	pvt->_bufferpos+=bytestocopy;
	return bytestocopy;
}

ssize_t websocket::write(const void *buf, ssize_t count) {

	// init everything here because of goto's
	ssize_t		retval=0;
	unsigned char	firstbyte=0;
	unsigned char	flags=0;
	unsigned char	opcode=0;
	unsigned char	fin=0;
	unsigned char	rsv1=0;
	unsigned char	rsv2=0;
	unsigned char	rsv3=0;
	unsigned char	mask=0;
	unsigned char	payloadlen1=0;
	uint16_t	payloadlen2=0;
	uint64_t	payloadlen3=0;
	uint32_t	maskingkey=0;

	// temporarily disable the security context so
	// local writes don't use websocket::write();
	if (pvt->_fd->getSecurityContext()!=this) {
stdoutput.printf("write fail 0\n");
		retval=RESULT_ERROR;
		goto end;
	}
	pvt->_fd->setSecurityContext(NULL);


	// write header...

	// first byte is 4 flags and 4-bit opcode
	fin=1;
	flags=(fin<<3)|(rsv1<<2)|(rsv2<<1)|rsv3;
	firstbyte=(flags<<4)|opcode;

	// write first byte
	retval=pvt->_fd->write(firstbyte);
	if (retval!=sizeof(firstbyte)) {
stdoutput.printf("write fail 1\n");
		goto end;
	}

	// set mask
	mask=(pvt->_isclient)?1:0;

	// set payload length parts
	if (count<126) {
		payloadlen1=count;
	} else if (count<65536) {
		payloadlen1=126;
		payloadlen2=count;
	} else {
		payloadlen1=127;
		payloadlen3=count;
	}
	payloadlen1=payloadlen1|(mask<<7);

	// write payload length
	retval=pvt->_fd->write(payloadlen1);
	if (retval!=sizeof(payloadlen1)) {
stdoutput.printf("write fail 2\n");
		goto end;
	}

	// write extended payload length
	if (payloadlen1==126) {
		retval=pvt->_fd->write(payloadlen2);
		if (retval!=sizeof(payloadlen2)) {
stdoutput.printf("write fail 3\n");
			goto end;
		}
	} else if (payloadlen1==127) {
		retval=pvt->_fd->write(payloadlen3);
		if (retval!=sizeof(payloadlen3)) {
stdoutput.printf("write fail 4\n");
			goto end;
		}
	}

	// write masking key
	if (mask) {
		// FIXME: set masking key to something...
		retval=pvt->_fd->write(maskingkey);
		if (retval!=sizeof(maskingkey)) {
stdoutput.printf("write fail 5\n");
			goto end;
		}
	}


	// write payload
	retval=pvt->_fd->write(buf,count);
	if (retval!=count) {
stdoutput.printf("write fail 6\n");
		goto end;
	}

end:
	// reset security context
	pvt->_fd->setSecurityContext(this);

	// return result
	return retval;
}

ssize_t websocket::pending() {
	return (pvt->_buffersize-pvt->_bufferpos);
}

bool websocket::close() {
	return true;
}

ssize_t websocket::getSizeMax() {
	return 0;
}
