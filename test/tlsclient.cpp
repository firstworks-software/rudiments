// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
#include <rudiments/tls.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

static void usage() {
	stdoutput.printf("tlsclient "
			"[-host host] [-port port] "
			"[-version version] [-cert cert] [-ciphers ciphers] "
			"[-validate (yes|no)] [-depth depth] [-ca ca] "
			"[-commonname name] "
			"[-ccount count] [-mcount count] [-dcount count]\n");
}

int main(int argc, const char **argv) {

	// process the command line
	commandline	cmdl(argc,argv);
	if (cmdl.isFound("help")) {
		usage();
		return 0;
	}
	const char	*host="127.0.0.1";
	if (cmdl.isFound("host")) {
		host=cmdl.getValue("host");
	}
	uint16_t	port=9000;
	if (cmdl.isFound("port")) {
		port=charstring::convertToUnsignedInteger(cmdl.getValue("port"));
	}
	const char	*version=NULL;
	if (cmdl.isFound("version")) {
		version=cmdl.getValue("version");
	}
	const char	*cert=NULL;
	if (cmdl.isFound("cert")) {
		cert=cmdl.getValue("cert");
	}
	const char	*ciphers=NULL;
	if (cmdl.isFound("ciphers")) {
		ciphers=cmdl.getValue("ciphers");
	}
	bool	validate=true;
	if (cmdl.isFound("validate")) {
		validate=charstring::compare(cmdl.getValue("validate"),"no");
	}
	uint16_t	depth=9;
	if (cmdl.isFound("depth")) {
		depth=charstring::convertToUnsignedInteger(cmdl.getValue("depth"));
	}
	const char	*ca=NULL;
	if (cmdl.isFound("ca")) {
		ca=cmdl.getValue("ca");
	}
	const char	*commonname="server.localdomain";
	if (cmdl.isFound("commonname")) {
		commonname=cmdl.getValue("commonname");
	}
	const char	*msg="hello";
	if (cmdl.isFound("message")) {
		msg=cmdl.getValue("message");
	}
	int64_t	ccount=charstring::convertToInteger(cmdl.getValue("ccount"));
	if (ccount<0) {
		usage();
		return 1;
	} else if (!ccount) {
		ccount=1;
	}
	int64_t	mcount=charstring::convertToInteger(cmdl.getValue("mcount"));
	if (mcount<0) {
		usage();
		return 1;
	} else if (!mcount) {
		mcount=1;
	}
	int64_t	dcount=charstring::convertToInteger(cmdl.getValue("dcount"));
	if (dcount<0) {
		usage();
		return 1;
	} else if (!dcount) {
		dcount=1;
	}

	// duplicate the message the
	// specified number of times...
	bytebuffer	msgbuf;
	for (int64_t i=0; i<dcount; i++) {
		msgbuf.append(msg)->append(' ');
	}

{
	// configure the security context
	tlscontext	ctx;
	ctx.setProtocolVersion(version);
	ctx.setCertificateChainFile(cert);
	ctx.setPrivateKeyPassword("password");
	ctx.setCiphers(ciphers);
	ctx.setValidatePeer(validate);
	ctx.setValidationDepth(depth);
	ctx.setCertificateAuthority(ca);

	// create an inet socket client
	inetsocketclient	fd;
	fd.setHost(host);
	fd.setPort(port);
	fd.setWriteBufferSize(65536);
	fd.setReadBufferSize(65536);

	// attach the security context
	fd.setSocketLayer(&ctx);

	// loop, having sessions with the server
	for (int64_t i=0; i<ccount; i++) {

		// connect 
		if (fd.connect()!=RESULT_SUCCESS) {
			if (error::getErrorNumber()) {
				stdoutput.printf("connect failed (1): %s\n",
							error::getErrorString());
			} else {
				stdoutput.printf("connect failed (2): %s\n",
							ctx.getErrorString());
			}
			continue;
		}

		// make sure the server sent a certificate
		if (validate) {
			tlscertificate	*pcert=ctx.getPeerCertificate();
			if (!pcert) {
				stdoutput.printf(
					"peer sent no certificate\n%s\n",
					ctx.getErrorString());
				fd.close();
				delete pcert;
				continue;
			}

			// Make sure the commonname in the certificate
			// is the one we expect it to be.
			const char	*cn=pcert->getCommonName();
			if (charstring::compareIgnoringCase(cn,commonname)) {
				stdoutput.printf("%s!=%s\n",cn,commonname);
				fd.close();
				delete pcert;
				continue;
			}

			stdoutput.printf("server certificate {\n");
			stdoutput.printf("  version: %d\n",
					pcert->getVersion());
			stdoutput.printf("  serial number: %lld\n",
					pcert->getSerialNumber());
			stdoutput.printf("  signature algorithm: %s\n",
					pcert->getSignatureAlgorithm());
			stdoutput.printf("  issuer: %s\n",
					pcert->getIssuer());
			stdoutput.printf("  valid-from: %s\n",
					pcert->getValidFrom()->getString());
			stdoutput.printf("  valid-to: %s\n",
					pcert->getValidTo()->getString());
			stdoutput.printf("  subject: %s\n",
					pcert->getSubject());
			stdoutput.printf("  public key algorithm: %s\n",
					pcert->getPublicKeyAlgorithm());
			stdoutput.printf("  public key: ");
			stdoutput.safePrint(pcert->getPublicKey(),
					(pcert->getPublicKeyByteSize()<5)?
					pcert->getPublicKeyByteSize():5);
			stdoutput.printf("...\n");
			stdoutput.printf("  public key size: %lld\n",
					pcert->getPublicKeyByteSize());
			stdoutput.printf("  public key bits: %lld\n",
					pcert->getPublicKeyBitSize());
			stdoutput.printf("  common name: %s\n",
					pcert->getCommonName());
			stdoutput.printf("  subject alternate names:\n");
			for (listnode< char * > *node=
				pcert->getSubjectAlternateNames()->getFirst();
				node; node=node->getNext()) {
				stdoutput.printf("    %s\n",node->getValue());
			}
			stdoutput.printf("}\n");

			delete pcert;
		}

		stdoutput.printf("serverSession {\n");

		// write the message to the server,
		// the specified number of times
		for (int64_t j=0; j<mcount; j++) {

			// write size
			ssize_t	sizewritten=fd.write((uint64_t)
						msgbuf.getSize());
			if (sizewritten<=0) {
				if (sizewritten==0) {
					stdoutput.printf(
						"  write() size failed (0): "
						"eof\n");
					break;
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  write() size failed (1): "
						"%s\n",
						error::getErrorString());
					break;
				} else {
					stdoutput.printf(
						"  write() size failed (2): "
						"%s\n",
						ctx.getErrorString());
					break;
				}
			} else if (sizewritten!=sizeof(uint64_t)) {
				stdoutput.printf(
					"  write() size failed (3): %s\n",
					ctx.getErrorString());
				break;
			}

			// write message
			sizewritten=fd.write(msgbuf.getBuffer(),
						msgbuf.getSize());
			if (sizewritten<=0) {
				if (sizewritten==0) {
					stdoutput.printf(
						"  write() msg failed (0): "
						"eof\n");
					break;
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  write() msg failed (1): "
						"%s\n",
						error::getErrorString());
					break;
				} else {
					stdoutput.printf(
						"  write() msg failed (2): "
						"%s\n",
						ctx.getErrorString());
					break;
				}
			} else if (sizewritten!=(ssize_t)msgbuf.getSize()) {
				stdoutput.printf(
					"  write() msg failed (3): %s\n",
					ctx.getErrorString());
				break;
			}

			// flush write buffer
			if (!fd.flushWriteBuffer(-1,-1)) {
				stdoutput.printf("flushWriteBuffer() failed\n");
				break;
			}

			stdoutput.printf("\n  Sent message... "
					"(size=%d):\n  ",msgbuf.getSize());
			stdoutput.safePrint(msgbuf.getBuffer(),
				(msgbuf.getSize()<=80)?msgbuf.getSize():80);
			if (msgbuf.getSize()>80) {
				stdoutput.write("...");
			}
			stdoutput.write('\n');
			stdoutput.printf("\n  Receiving response...");

			// read size
			uint64_t	msgsize;
			ssize_t	sizeread=fd.read(&msgsize);
			if (sizeread<=0) {
				if (sizeread==0) {
					stdoutput.printf(
						"  read() size failed (0): "
						"eof\n");
					break;
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  read() size failed (1): "
						"%s\n",
						error::getErrorString());
					break;
				} else {
					stdoutput.printf(
						"  read() size failed (2): "
						"%s\n",
						ctx.getErrorString());
					break;
				}
			} else if (sizeread!=sizeof(uint64_t)) {
				stdoutput.printf(
					"  read() size failed (3): %s\n",
					ctx.getErrorString());
				break;
			}

			// read message
			byte_t	*msg=new byte_t[msgsize];
			sizeread=fd.read(msg,msgsize);
			if (sizeread<=0) {
				if (sizeread==0) {
					stdoutput.printf(
						"  read() msg failed (0): "
						"eof\n");
					delete[] msg;
					break;
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  read() msg failed (1): "
						"%s\n",
						error::getErrorString());
					delete[] msg;
					break;
				} else {
					stdoutput.printf(
						"  read() msg failed (2): "
						"%s\n",
						ctx.getErrorString());
					delete[] msg;
					break;
				}
			} else if (sizeread!=(ssize_t)msgsize) {
				stdoutput.printf(
					"  read() msg failed (3): %s\n",
					ctx.getErrorString());
				delete[] msg;
				break;
			}

			stdoutput.printf("  success\n");

			delete[] msg;
		}

		stdoutput.printf("}\n");

		// close the connection to the server
		fd.close();
	}
}

	return 0;
}
