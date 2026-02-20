#include <rudiments/gss.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// check if gss is supported
	if (!gss::isSupported()) {
		stdoutput.write("GSS is not supported\n");
		return 1;
	}
	stdoutput.write("GSS is supported\n\n");


	// get the available mechanisms
	gss	g;
	const char * const	*mechs=g.getAvailableMechanisms();
	if (mechs) {
		stdoutput.write("available mechanisms:\n");
		for (uint64_t i=0; mechs[i]; i++) {
			stdoutput.printf("  %s\n",mechs[i]);
		}
		stdoutput.write('\n');
	}


	// open a mechanism by OID string
	gssmechanism	mech;
	if (mech.open("{ 1 2 840 113554 1 2 2 }")) {
		stdoutput.printf("opened mechanism: %s\n\n",
						mech.getString());
	}


	// create credentials
	gsscredentials	cred;
	cred.addDesiredMechanism(&mech);
	cred.setDesiredLifetime(600);

	stdoutput.printf("desired lifetime:  %d seconds\n",
					cred.getDesiredLifetime());
	stdoutput.printf("desired mechanism count: %lld\n\n",
				(long long)cred.getDesiredMechanismCount());


	// create a gss context
	gsscontext	ctx;

	// configure the context
	ctx.setCredentials(&cred);
	ctx.setDesiredMechanism(&mech);
	ctx.setDesiredLifetime(600);
	ctx.setService("host");

	stdoutput.printf("service: %s\n",ctx.getService());
	stdoutput.printf("desired lifetime: %d seconds\n",
					ctx.getDesiredLifetime());


	// clean up
	mech.close();
	cred.close();
}
