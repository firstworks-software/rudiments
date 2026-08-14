#include <rudiments/arc4.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// check whether arc4 is supported
	if (!arc4().isSupported()) {
		stdoutput.write("arc4 is not supported\n");
		return 1;
	}


	// two instances constructed from the same key, one for each
	// direction of a duplex stream
	const byte_t	*key=(const byte_t *)"a shared key";
	size_t		keysize=charstring::getLength((const char *)key);
	arc4		writecipher(key,keysize);
	arc4		readcipher(key,keysize);


	// encrypt some outgoing data
	const char	*outgoing="hello world";
	size_t		outgoingsize=charstring::getLength(outgoing);
	byte_t		*outbuffer=new byte_t[outgoingsize];
	bytestring::copy(outbuffer,outgoing,outgoingsize);
	writecipher.crypt(outbuffer,outgoingsize);


	// decrypt some incoming data, encrypted separately with a cipher
	// constructed from the same key
	arc4		peerwritecipher(key,keysize);
	const char	*incoming="goodbye world";
	size_t		incomingsize=charstring::getLength(incoming);
	byte_t		*inbuffer=new byte_t[incomingsize];
	bytestring::copy(inbuffer,incoming,incomingsize);
	peerwritecipher.crypt(inbuffer,incomingsize);
	readcipher.crypt(inbuffer,incomingsize);


	// crypt() may be called repeatedly, buffer by buffer, and each
	// instance's keystream just continues from where it left off,
	// independently of the other instance's
	stdoutput.printf("decrypted: %.*s\n",(int)incomingsize,
						(const char *)inbuffer);

	delete[] outbuffer;
	delete[] inbuffer;

	return 0;
}
