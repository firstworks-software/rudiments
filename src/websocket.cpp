// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/websocket.h>
#include <rudiments/regularexpression.h>
#include <rudiments/bytestring.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/sha1.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_HAVE_LIMITS_H
	#include <limits.h>
#endif

// if SSIZE_MAX is undefined, choose a good safe value
// that should even work on 16-bit systems
#ifndef SSIZE_MAX
	#define SSIZE_MAX 16383
#endif

//#define DEBUG_CONNECT 1
//#define DEBUG_ACCEPT 1
//#define DEBUG_READ 1
//#define DEBUG_WRITE 1

// FIXME do this some other way...
#define MAXBYTES 1024*1024*512

#define OPCODE_CONT_FRAME	0x00
#define OPCODE_TEXT_FRAME	0x01
#define OPCODE_BINARY_FRAME	0x02
#define OPCODE_CONNECTION_CLOSE	0x08
#define OPCODE_PING		0x09
#define OPCODE_PONG		0x0A

class websocketprivate {
	friend class websocket;
	private:
		bool		_isclient;

		filedescriptor	*_fd;

		char		*_key;
		char		*_protocol;
		char		*_ext;

		unsigned char	*_buffer;
		uint64_t	_buffersize;
		uint64_t	_bufferpos;

		unsigned char	*_pingbuffer;
		uint64_t	_pingbuffersize;
};

websocket::websocket() : securitycontext() {
	pvt=new websocketprivate;
	pvt->_isclient=false;
	pvt->_fd=NULL;
	pvt->_key=NULL;
	pvt->_protocol=NULL;
	pvt->_ext=NULL;
	pvt->_buffer=NULL;
	pvt->_buffersize=0;
	pvt->_bufferpos=0;
	pvt->_pingbuffer=NULL;
	pvt->_pingbuffersize=0;
}

websocket::~websocket() {
	delete[] pvt->_key;
	delete[] pvt->_protocol;
	delete[] pvt->_ext;
	delete[] pvt->_buffer;
	delete[] pvt->_pingbuffer;
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

	// temporarily disable the security context so
	// local reads don't use websocket::read();
	if (pvt->_fd->getSecurityContext()!=this) {
		// FIXME: set error
		return false;
	}
	pvt->_fd->setSecurityContext(NULL);

	// accept...
	bool	retval=acceptInternal();

	// reset security context
	pvt->_fd->setSecurityContext(this);

	return retval;
}

bool websocket::acceptInternal() {

	#ifdef DEBUG_ACCEPT
		stdoutput.write("websocket::accept() {\n");
	#endif

	// we're not a client
	pvt->_isclient=false;

	// reinit some buffers
	delete[] pvt->_key;
	delete[] pvt->_protocol;
	delete[] pvt->_ext;
	pvt->_key=NULL;
	pvt->_protocol=NULL;
	pvt->_ext=NULL;

	// read the request
	char	*request=NULL;
	ssize_t	result=pvt->_fd->read(&request,"\r\n",MAXBYTES);
	if (result<0) {
		delete[] request;
		if (result==RESULT_ERROR) {
			#ifdef DEBUG_ACCEPT
				stdoutput.write("	request error\n}\n");
			#endif
			// FIXME: set error
			return false;
		} else if (result==RESULT_TIMEOUT) {
			#ifdef DEBUG_ACCEPT
				stdoutput.write("	request timeout\n}\n");
			#endif
			// FIXME: set error
			return false;
		} else if (result==RESULT_MAX) {
			#ifdef DEBUG_ACCEPT
				stdoutput.write("	request max\n}\n");
			#endif
			// FIXME: set error
			return false;
		}
	}
	#ifdef DEBUG_ACCEPT
		stdoutput.printf("	request:\n%s",request);
	#endif

	// validate the request
	// FIXME: support higher http versions...
	if (!regularexpression::match(
			request,"^GET /.* HTTP/[1-9].[1-9]\r\n")) {
		#ifdef DEBUG_ACCEPT
			stdoutput.printf("	invalid request:\n%s\n}\n",
								request);
		#endif
		// FIXME: set error
		delete[] request;
		return false;
	}
	delete[] request;

	// read headers
	char	*hdrs=NULL;
	result=pvt->_fd->read(&hdrs,"\r\n\r\n",MAXBYTES);
	if (result<0) {
		delete[] hdrs;
		if (result==RESULT_ERROR) {
			#ifdef DEBUG_ACCEPT
				stdoutput.write("	header error\n}\n");
			#endif
			// FIXME: set error
			return false;
		} else if (result==RESULT_TIMEOUT) {
			#ifdef DEBUG_ACCEPT
				stdoutput.write("	header timeout\n}\n");
			#endif
			// FIXME: set error
			return false;
		} else if (result==RESULT_MAX) {
			#ifdef DEBUG_ACCEPT
				stdoutput.write("	header max\n}\n");
			#endif
			// FIXME: set error
			return false;
		}
	}

	// split and evaluate headers
	char		**headers=NULL;
	uint64_t	headercount=0;
	bool		gothost=false;
	bool		gotupgradewebsocket=false;
	bool		gotconnectionupgrade=false;
	bool		gotversion=false;
	charstring::split(hdrs,"\r\n",true,&headers,&headercount);
	#ifdef DEBUG_ACCEPT
		stdoutput.write("\n	headers:\n");
	#endif
	for (uint64_t i=0; i<headercount; i++) {

		// split the header
		char		**parts=NULL;
		uint64_t	partcount=0;
		charstring::split(headers[i],": ",true,&parts,&partcount);

		// ignore invalid headers...
		if (partcount!=2) {
			for (uint64_t j=0; j<partcount; j++) {
				delete[] parts[j];
			}
			delete[] parts;
			delete[] headers[i];
			continue;
		}

		// trim header parts
		charstring::bothTrim(parts[0]);
		charstring::bothTrim(parts[1]);
		#ifdef DEBUG_ACCEPT
			stdoutput.printf("	\"%s\": \"%s\"\n",
						parts[0],parts[1]);
		#endif

		// look for Host: ...
		if (!charstring::compare(parts[0],"Host")) {
			gothost=true;
		} else

		// look for Upgrade: websocket
		if (!charstring::compare(parts[0],"Upgrade") &&
			charstring::contains(parts[1],"websocket")) {
			gotupgradewebsocket=true;
		} else

		// look for Connection: Upgrade
		if (!charstring::compare(parts[0],"Connection") &&
			charstring::contains(parts[1],"Upgrade")) {
			gotconnectionupgrade=true;
		} else

		// get the Sec-WebSocket-Key
		if (!charstring::compare(parts[0],"Sec-WebSocket-Key")) {
			pvt->_key=parts[1];
			parts[1]=NULL;
		} else

		// get the Sec-WebSocket-Version
		if (!charstring::compare(parts[0],"Sec-WebSocket-Version") &&
					!charstring::compare(parts[1],"13")) {
			gotversion=true;
		} else

		// get the Sec-WebSocket-Protocol
		if (!charstring::compare(parts[0],"Sec-WebSocket-Protocol")) {
			// FIXME: multiple protocol headers can be specified
			pvt->_protocol=parts[1];
			parts[1]=NULL;
		}

		// get the Sec-WebSocket-Extensions
		if (!charstring::compare(parts[0],"Sec-WebSocket-Extensions")) {
			// FIXME: multiple extension headers can be specified
			pvt->_ext=parts[1];
			parts[1]=NULL;
		}

		// clean up
		delete[] parts[0];
		delete[] parts[1];
		delete[] parts;
		delete[] headers[i];
	}
	delete[] headers;
	delete[] hdrs;

	// bail for various reasons
	if (!gothost) {
		#ifdef DEBUG_ACCEPT
			stdoutput.write("	no Host: ...\n}\n");
		#endif
		// FIXME: set error
		return false;
	}
	if (!gotupgradewebsocket) {
		#ifdef DEBUG_ACCEPT
			stdoutput.write("	no Upgrade: websocket\n}\n");
		#endif
		// FIXME: set error
		return false;
	}
	if (!gotconnectionupgrade) {
		#ifdef DEBUG_ACCEPT
			stdoutput.write("	no Connection: Upgrade\n}\n");
		#endif
		// FIXME: set error
		return false;
	}
	if (!pvt->_key) {
		#ifdef DEBUG_ACCEPT
			stdoutput.write("	no Sec-WebSocket-Key\n}\n");
		#endif
		// FIXME: set error
		return false;
	} else {
		unsigned char	*decodedkey;
		uint64_t	decodedkeylength;
		charstring::base64Decode(pvt->_key,
					charstring::length(pvt->_key),
					&decodedkey,
					&decodedkeylength);
		delete[] decodedkey;
		if (decodedkeylength!=16) {
			#ifdef DEBUG_ACCEPT
			stdoutput.write("	decoded Sec-WebSocket-Key "
							"not 16 bytes\n}\n");
			#endif
			// FIXME: set error
			return false;
		}
	}
	if (!gotversion) {
		#ifdef DEBUG_ACCEPT
			stdoutput.write("	"
					"no Sec-WebSocket-Version: 13\n}\n");
		#endif
		// FIXME: set error
		return false;
	}
	if (pvt->_ext) {
		// FIXME: negotiate extensions
		// for now, we don't support any extensions
		delete[] pvt->_ext;
		pvt->_ext=NULL;
	}

	// build response key
	bytebuffer	fullkey;
	fullkey.append(pvt->_key);
	fullkey.append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	sha1		s;
	s.append(fullkey.getBuffer(),fullkey.getSize());
	char	*encodedkey=charstring::base64Encode(s.getHash(),
							s.getHashSize());

	// build response
	stringbuffer	response;
	response.append("HTTP/1.1 101 Switching Protocols\r\n");

	// base headers...
	response.append("Upgrade: websocket\r\n");
	response.append("Connection: Upgrade\r\n");

	// response key
	response.append("Sec-WebSocket-Accept: ");
	response.append(encodedkey);
	response.append("\r\n");

	//  protocol
	if (pvt->_protocol) {
		response.append("Sec-WebSocket-Protocol: ");
		response.append(pvt->_protocol);
		response.append("\r\n");
	}

	// ext
	if (pvt->_ext) {
		response.append("Sec-WebSocket-Extensions: ");
		response.append(pvt->_ext);
		response.append("\r\n");
	}

	// terminator
	response.append("\r\n");
	#ifdef DEBUG_ACCEPT
		stdoutput.printf("\n	response:\n%s",response.getString());
	#endif
	delete[] encodedkey;

	// send response headers
	result=pvt->_fd->write(response.getString(),response.getSize());
	if (result!=(ssize_t)response.getSize()) {
		if (result==RESULT_ERROR) {
			#ifdef DEBUG_ACCEPT
				stdoutput.write("	response error\n}\n");
			#endif
			// FIXME: set error
			return false;
		} else if (result==RESULT_TIMEOUT) {
			#ifdef DEBUG_ACCEPT
				stdoutput.write("	response timeout\n}\n");
			#endif
			// FIXME: set error
			return false;
		} else if (result==RESULT_MAX) {
			#ifdef DEBUG_ACCEPT
				stdoutput.write("	response max\n}\n");
			#endif
			// FIXME: set error
			return false;
		} else {
			#ifdef DEBUG_ACCEPT
				stdoutput.write("	"
						"response short write\n}\n");
			#endif
			// FIXME: set error
			return false;
		}
	}

	#ifdef DEBUG_ACCEPT
		stdoutput.write("}\n");
	#endif

	return true;
}

ssize_t websocket::read(void *buf, ssize_t size) {

	// copy out any existing buffered data
	ssize_t	bytescopiedout=copyOut(buf,size);
	if (bytescopiedout==size) {
		return size;
	}

	// decrement size by whatever we got from the buffer
	size-=bytescopiedout;

	// temporarily disable the security context so
	// local reads don't use websocket::read();
	if (pvt->_fd->getSecurityContext()!=this) {
		// FIXME: set error
		return RESULT_ERROR;
	}
	pvt->_fd->setSecurityContext(NULL);

	// read...
	ssize_t	retval=readInternal(buf,size);

	// reset security context
	pvt->_fd->setSecurityContext(this);

	return retval;
}

ssize_t websocket::readInternal(void *buf, ssize_t size) {

	#ifdef DEBUG_READ
		stdoutput.write("websocket::read() {\n");
	#endif

	bool	loop=true;
	while (loop) {

		// read header...

		// read first byte
		unsigned char	firstbyte=0;
		if (pvt->_fd->read(&firstbyte)!=sizeof(firstbyte)) {
			#ifdef DEBUG_READ
				stdoutput.write("	firstbyte error\n}\n");
			#endif
			// FIXME: set error
			return RESULT_ERROR;
		}

		// first byte is 4 flags and 4-bit opcode
		// FIXME: do something with these...
		unsigned char	flags=firstbyte>>4;
		unsigned char	opcode=firstbyte&0x0f;

		// parse flags
		// FIXME: do something with these...
		unsigned char	fin=(flags&0x08)>>3;
		unsigned char	rsv1=(flags&0x04)>>2;
		unsigned char	rsv2=(flags&0x02)>>1;
		unsigned char	rsv3=flags&0x01;
		#ifdef DEBUG_READ
			stdoutput.write("	firstbyte: ");
			stdoutput.printBits(firstbyte);
			stdoutput.write("\n");
			stdoutput.printf("	fin: %d\n",fin);
			stdoutput.printf("	rsv1: %d\n",rsv1);
			stdoutput.printf("	rsv2: %d\n",rsv2);
			stdoutput.printf("	rsv3: %d\n",rsv3);
			stdoutput.printf("	opcode: 0x%02x\n",opcode);
		#endif

		if (fin) {
			// FIXME: ...
		}
		if (rsv1) {
			// FIXME: ...
		}
		if (rsv2) {
			// FIXME: ...
		}
		if (rsv3) {
			// FIXME: ...
		}

		// read payload length
		unsigned char	payloadlen1=0;
		uint16_t	payloadlen2=0;
		uint64_t	payloadlen3=0;
		if (pvt->_fd->read(&payloadlen1)!=sizeof(payloadlen1)) {
			#ifdef DEBUG_READ
				stdoutput.write("	"
						"payload length 1 error\n}\n");
			#endif
			// FIXME: set error
			return RESULT_ERROR;
		}

		// first bit is the mask, next 7 are actual payload length
		unsigned char	mask=(payloadlen1&0x80)>>7;
		payloadlen1=payloadlen1&0x7f;

		#ifdef DEBUG_READ
			stdoutput.printf("	mask: %d\n",mask);
		#endif

		// if this is a client then mask must be 0
		// if this is a server then mask must be 1
		if ((pvt->_isclient && mask) || (!pvt->_isclient && !mask)) {
			#ifdef DEBUG_READ
				stdoutput.printf("	"
						"wrong mask: %d\n}\n",mask);
			#endif
			// FIXME: set error
			// FIXME: close the connection
			return RESULT_ERROR;
		}

		// read extended payload length
		if (payloadlen1==126) {
			if (pvt->_fd->read(&payloadlen2)!=sizeof(payloadlen2)) {
				#ifdef DEBUG_READ
					stdoutput.write("	"
						"payload length 2 error:\n}\n");
				#endif
				// FIXME: set error
				return RESULT_ERROR;
			}
			pvt->_buffersize=payloadlen2;
		} else if (payloadlen1==127) {
			if (pvt->_fd->read(&payloadlen3)!=sizeof(payloadlen3)) {
				#ifdef DEBUG_READ
					stdoutput.write("	"
						"payload length 3 error\n}\n");
				#endif
				// FIXME: set error
				return RESULT_ERROR;
			}
			pvt->_buffersize=payloadlen3;
		} else {
			pvt->_buffersize=payloadlen1;
		}

		#ifdef DEBUG_READ
			stdoutput.printf("	payload length 1: %d\n",
								payloadlen1);
			stdoutput.printf("	payload length 2: %d\n",
								payloadlen2);
			stdoutput.printf("	payload length 3: %d\n",
								payloadlen3);
		#endif

		// read masking key
		unsigned char	maskingkey[4];
		if (mask) {
			if (pvt->_fd->read(maskingkey,sizeof(maskingkey))!=
							sizeof(maskingkey)) {
				#ifdef DEBUG_READ
					stdoutput.write("	"
						"masking key error\n}\n");
				#endif
				// FIXME: set error
				return RESULT_ERROR;
			}
		}

		#ifdef DEBUG_READ
			stdoutput.write("	masking key: ");
			stdoutput.safePrint(maskingkey,sizeof(maskingkey));
			stdoutput.write("\n");
		#endif


		// read payload
		delete[] pvt->_buffer;
		pvt->_buffer=new unsigned char[pvt->_buffersize];
		if (pvt->_fd->read(pvt->_buffer,pvt->_buffersize)!=
						(ssize_t)pvt->_buffersize) {
			#ifdef DEBUG_READ
				stdoutput.write("	payload error\n}\n");
			#endif
			// FIXME: set error
			return RESULT_ERROR;
		}

		// unmask payload (if necessary)
		if (mask) {
			for (uint64_t i=0; i<pvt->_buffersize; i++) {
				pvt->_buffer[i]=pvt->_buffer[i]^maskingkey[i%4];
			}
		}

		#ifdef DEBUG_READ
			stdoutput.write("	payload:\n");
			stdoutput.safePrint(pvt->_buffer,pvt->_buffersize);
			stdoutput.write("\n");
		#endif

		switch (opcode) {
			case OPCODE_PING:
				if (!pong()) {
					// FIXME: set error
					return RESULT_ERROR;
				}
				break;
			case OPCODE_PONG:
				if (!validatePong()) {
					// FIXME: set error
					return RESULT_ERROR;
				}
				break;
			case OPCODE_CONNECTION_CLOSE:
				// FIXME: close the connection
				loop=false;
				break;
			case OPCODE_CONT_FRAME:
				loop=false;
				break;
			case OPCODE_TEXT_FRAME:
				loop=false;
				break;
			case OPCODE_BINARY_FRAME:
				loop=false;
				break;
			default:
				// FIXME: set error
				return RESULT_ERROR;
		}
	}

	#ifdef DEBUG_READ
		stdoutput.write("}\n");
	#endif

	// copy out what we can
	return copyOut(buf,size);
}

ssize_t websocket::copyOut(void *buf, ssize_t size) {

	ssize_t	bytestocopy=size;
	if (bytestocopy>(ssize_t)(pvt->_buffersize-pvt->_bufferpos)) {
		bytestocopy=pvt->_buffersize-pvt->_bufferpos;
	}

	bytestring::copy(buf,pvt->_buffer+pvt->_bufferpos,bytestocopy);
	pvt->_bufferpos+=bytestocopy;

	if (pvt->_bufferpos==pvt->_buffersize) {
		pvt->_bufferpos=0;
		pvt->_buffersize=0;
	}
	return bytestocopy;
}

ssize_t websocket::write(const void *buf, ssize_t size) {
	// FIXME: Select text/binary somehow...
	return write(buf,size,OPCODE_TEXT_FRAME);
}

ssize_t websocket::write(const void *buf, ssize_t size,
						unsigned char opcode) {

	// temporarily disable the security context so
	// local writes don't use websocket::write();
	if (pvt->_fd->getSecurityContext()!=this) {
		// FIXME: set error
		return RESULT_ERROR;
	}
	pvt->_fd->setSecurityContext(NULL);

	// write...
	ssize_t	retval=writeInternal(buf,size,opcode);

	// reset security context
	pvt->_fd->setSecurityContext(this);

	return retval;
}

ssize_t websocket::writeInternal(const void *buf, ssize_t size,
							unsigned char opcode) {

	#ifdef DEBUG_WRITE
		stdoutput.write("websocket::write() {\n");
	#endif

	// write header...

	// first byte is 4 flags and 4-bit opcode
	unsigned char	fin=1;
	unsigned char	rsv1=0;
	unsigned char	rsv2=0;
	unsigned char	rsv3=0;
	unsigned char	flags=(fin<<3)|(rsv1<<2)|(rsv2<<1)|rsv3;
	unsigned char	firstbyte=(flags<<4)|opcode;

	#ifdef DEBUG_WRITE
		stdoutput.write("	firstbyte: ");
		stdoutput.printBits(firstbyte);
		stdoutput.write("\n");
		stdoutput.printf("	fin: %d\n",fin);
		stdoutput.printf("	rsv1: %d\n",rsv1);
		stdoutput.printf("	rsv2: %d\n",rsv2);
		stdoutput.printf("	rsv3: %d\n",rsv3);
		stdoutput.printf("	opcode: 0x%02x\n",opcode);
	#endif

	// write first byte
	if (pvt->_fd->write(firstbyte)!=sizeof(firstbyte)) {
		#ifdef DEBUG_WRITE
			stdoutput.write("	firstbyte error\n}\n");
		#endif
		// FIXME: set error
		return RESULT_ERROR;
	}

	// set mask
	unsigned char	mask=(pvt->_isclient)?1:0;

	#ifdef DEBUG_WRITE
		stdoutput.printf("	mask: %d\n",mask);
	#endif

	// set payload length parts
	unsigned char	payloadlen1=0;
	uint16_t	payloadlen2=0;
	uint64_t	payloadlen3=0;
	if (size<126) {
		payloadlen1=size;
	} else if (size<65536) {
		payloadlen1=126;
		payloadlen2=size;
	} else {
		payloadlen1=127;
		payloadlen3=size;
	}
	#ifdef DEBUG_WRITE
		stdoutput.printf("	payload length 1: %d\n",payloadlen1);
		stdoutput.printf("	payload length 2: %d\n",payloadlen2);
		stdoutput.printf("	payload length 3: %d\n",payloadlen3);
	#endif

	// add mask
	payloadlen1=payloadlen1|(mask<<7);

	#ifdef DEBUG_WRITE
		stdoutput.write("	payload length 1 (with mask): ");
		stdoutput.printBits(payloadlen1);
		stdoutput.write("\n");
	#endif

	// write payload length
	if (pvt->_fd->write(payloadlen1)!=sizeof(payloadlen1)) {
		#ifdef DEBUG_WRITE
			stdoutput.write("	payload length 1 error\n}\n");
		#endif
		// FIXME: set error
		return RESULT_ERROR;
	}

	// write extended payload length
	if (payloadlen1==126) {
		if (pvt->_fd->write(payloadlen2)!=sizeof(payloadlen2)) {
			#ifdef DEBUG_WRITE
				stdoutput.write("	"
						"payload length 2 error\n}\n");
			#endif
			// FIXME: set error
			return RESULT_ERROR;
		}
	} else if (payloadlen1==127) {
		if (pvt->_fd->write(payloadlen3)!=sizeof(payloadlen3)) {
			#ifdef DEBUG_WRITE
				stdoutput.write("	"
						"payload length 3 error\n}\n");
			#endif
			// FIXME: set error
			return RESULT_ERROR;
		}
	}

	// handle payload masking
	unsigned char	*payload=(unsigned char *)buf;
	if (mask) {

		// generate masking key
		unsigned char	maskingkey[4];
		// FIXME: set masking key to random bits
		bytestring::zero(maskingkey,sizeof(maskingkey));
		#ifdef DEBUG_WRITE
			stdoutput.write("	masking key: ");
			stdoutput.safePrint(maskingkey,sizeof(maskingkey));
			stdoutput.write("\n");
		#endif

		// write masking key
		if (pvt->_fd->write(maskingkey)!=sizeof(maskingkey)) {
			#ifdef DEBUG_WRITE
				stdoutput.write("	"
						"masking key error\n}\n");
			#endif
			// FIXME: set error
			return RESULT_ERROR;
		}

		// mask payload
		payload=(unsigned char *)bytestring::duplicate(buf,size);
		for (ssize_t i=0; i<size; i++) {
			payload[i]=payload[i]^maskingkey[i%4];
		}
	}

	#ifdef DEBUG_READ
		stdoutput.write("	payload:\n");
		stdoutput.safePrint((const unsigned char *)buf,size);
		stdoutput.write("\n");
	#endif

	// write payload
	ssize_t	retval=pvt->_fd->write(payload,size);
	if (mask) {
		delete[] payload;
	}
	if (retval!=size) {
		#ifdef DEBUG_WRITE
			stdoutput.write("	payload error\n}\n");
		#endif
		// FIXME: set error
		return RESULT_ERROR;
	}

	#ifdef DEBUG_WRITE
		stdoutput.write("}\n");
	#endif

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
	return SSIZE_MAX;
}

bool websocket::ping(const unsigned char *buf, ssize_t size) {
	delete[] pvt->_pingbuffer;
	pvt->_pingbuffer=(unsigned char *)bytestring::duplicate(buf,size);
	pvt->_pingbuffersize=size;
	return false;
}

bool websocket::pong() {
	return (write(pvt->_buffer,
			pvt->_buffersize,
			OPCODE_PING)==(ssize_t)pvt->_buffersize);
}

bool websocket::validatePong() {
	return (pvt->_buffersize==pvt->_pingbuffersize &&
			bytestring::compare(pvt->_buffer,
						pvt->_pingbuffer,
						pvt->_pingbuffersize));
}
