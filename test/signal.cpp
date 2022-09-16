// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/signalclasses.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/directory.h>
#include <rudiments/process.h>
#include <rudiments/sys.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>
#include "test.cpp"

bool	gotsigterm=false;
void handleSigterm(int32_t sig) {
	//stdoutput.printf("got sigterm\n");
	gotsigterm=true;
}

#ifdef SIGFPE
bool	gotsigfpe=false;
void handleSigfpe(int32_t sig) {
	//stdoutput.printf("got sigfpe\n");
	gotsigfpe=true;
}
#endif

#ifdef SIGALRM
bool	gotsigalrm=false;
void handleSigalrm(int32_t sig) {
	//stdoutput.printf("got sigalrm\n");
	gotsigalrm=true;
}
#endif

int main(int argc, const char **argv) {

	if (argc==1) {

		header("signal");


		char	*osname=sys::getOperatingSystemName();

		// FIXME: it's not clear why this doesn't work on windows,
		// signals generally do work...
		// FIXME: this does actually work on syllable but there's
		// some problem with the timer and it takes forever
		if (!charstring::compare(osname,"Windows") ||
			!charstring::compare(osname,"syllable")) {
			stdoutput.printf("	not supported\n\n");
			delete[] osname;
			return 0;
		}


		// spawn a child to signal this process
		stringbuffer	cmd;
		char	*pwd=directory::getCurrentWorkingDirectory();
		cmd.append(pwd)->append("/signal");
		#ifdef _WIN32
			cmd.append(".exe");
		#endif
		delete[] pwd;
		char	*pidstr=charstring::parseNumber(
					(uint64_t)process::getProcessId());
		const char	*args1[]={"signal",pidstr,NULL};
		process::spawn(cmd.getString(),args1,true);
		delete[] pidstr;


		// this program will ignore all signals except
		// SIGTERM, SIGFPE, and SIGALRM
		signalset	ignoreset;
		ignoreset.addAllSignals();
		ignoreset.removeSignal(SIGTERM);
		#ifdef SIGFPE
		ignoreset.removeSignal(SIGFPE);
		#endif
		#ifdef SIGALRM
		ignoreset.removeSignal(SIGALRM);
		#endif
		signalmanager::ignoreSignals(&ignoreset);

		// when it gets a SIGFPE,
		// it will run the handleSigfpe() function
		#ifdef SIGFPE
		signalhandler	fpehandler;
		fpehandler.setHandler(handleSigfpe);
		fpehandler.handleSignal(SIGFPE);
		#endif

		// when it gets a SIGTERM,
		// it will run the handleSigterm() function
		signalhandler	termhandler;
		termhandler.setHandler(handleSigterm);
		termhandler.handleSignal(SIGTERM);

		// when it gets a SIGALRM,
		// it will run the handleSigalrm() function
		#ifdef SIGALRM
		signalhandler	alarmhandler;
		alarmhandler.setHandler(handleSigalrm);
		alarmhandler.handleSignal(SIGALRM);
		#endif

		// wait for signals
		//stdoutput.printf("waiting for sigterm\n");
		signalmanager::waitForSignals(&ignoreset);
		test("SIGTERM",gotsigterm);

		#ifdef SIGFPE
		//stdoutput.printf("waiting for sigfpe\n");
		signalmanager::waitForSignals(&ignoreset);
		test("SIGFPE",gotsigfpe);
		#endif

		#ifdef SIGALRM
		// set an alarm
		signalmanager::alarm(5);
		// snooze a sec, it appears that the actual alarm() system
		// call can be asynchronous, or something, and interrupt the
		// wait() below
		snooze::macrosnooze(1);
		//stdoutput.printf("waiting for sigalrm\n");
		signalmanager::waitForSignals(&ignoreset);
		test("SIGALRM",gotsigalrm);
		#endif

		delete[] osname;

		process::exit(0);

	} else {

		// get the parent pid from the command line
		pid_t	pid=charstring::toInteger(argv[1]);

		// send it a SIGTERM
		#ifdef SLOWSYSTEM
			snooze::macrosnooze(3);
		#else
			snooze::macrosnooze(1);
		#endif
		//stdoutput.printf("sending sigterm\n");
		process::sendSignal(pid,SIGTERM);

		// send it a SIGFPE
		#ifdef SIGFPE
		#ifdef SLOWSYSTEM
			snooze::macrosnooze(3);
		#else
			snooze::macrosnooze(1);
		#endif
		//stdoutput.printf("sending sigfpe\n");
		process::sendSignal(pid,SIGFPE);
		#endif
	}

	process::exit(0);
}
