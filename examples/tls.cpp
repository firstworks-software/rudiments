#include <rudiments/tls.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// check if tls is supported
	if (!tls::isSupported()) {
		stdoutput.write("TLS is not supported\n");
		return 1;
	}
	stdoutput.write("TLS is supported\n\n");


	// create a tls context
	tlscontext	ctx;

	// configure the tls context
	ctx.setProtocolVersion("TLS1.2");
	ctx.setValidatePeer(true);
	ctx.setValidationDepth(9);
	ctx.setCertificateAuthority("/etc/pki/tls/certs");

	stdoutput.printf("protocol:     %s\n",ctx.getProtocolVersion());
	stdoutput.printf("validate:     %s\n",
				(ctx.getValidatePeer())?"yes":"no");
	stdoutput.printf("depth:        %d\n",ctx.getValidationDepth());
	stdoutput.printf("ca:           %s\n\n",
				ctx.getCertificateAuthority());


	// connect to a server using tls
	inetsocketclient	client;
	client.setHost("www.google.com");
	client.setPort(443);
	client.setTimeoutSeconds(5);
	client.setTimeoutMicroseconds(0);
	client.setRetryWait(1);
	client.setTries(1);

	if (client.connect()!=RESULT_SUCCESS) {
		stdoutput.write("failed to connect\n");
		return 1;
	}

	// associate the tls context with the socket
	ctx.setFileDescriptor(&client);

	// perform the tls handshake
	if (ctx.connect()) {
		stdoutput.write("TLS handshake succeeded\n");

		// get the peer certificate
		tlscertificate	*cert=ctx.getPeerCertificate();
		if (cert) {
			stdoutput.printf("subject:      %s\n",
						cert->getSubject());
			stdoutput.printf("issuer:       %s\n",
						cert->getIssuer());
			stdoutput.printf("common name:  %s\n",
						cert->getCommonName());
			delete cert;
		}

		ctx.close();
	} else {
		stdoutput.printf("TLS handshake failed: %s\n",
						ctx.getErrorString());
	}

	client.close();
}
