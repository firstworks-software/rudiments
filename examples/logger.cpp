#include <rudiments/logger.h>
#include <rudiments/permissions.h>

int main(int argc, const char **argv) {

	// create log destinations
	stdoutdestination	sod;
	filedestination		fd;
	fd.open("test.log",permissions::parsePermString("rw-rw-r--"));


	// create a logger and add both destinations
	logger	log;
	log.addLogDestination(&sod);
	log.addLogDestination(&fd);


	// set the logging level
	log.setLogLevel(1);


	// get a log header
	char	*header=logger::getLogHeader("myprogram");


	// write some structured log entries
	log.start(1,header,0,"starting up");
	log.write(1,header,1,"initializing: %s","done");
	log.write(1,header,1,"connecting to: %s:%d","localhost",9000);
	log.end(1,header,0);
	delete[] header;


	// write a simple string to the log
	log.write("simple log message\n");


	// remove a log destination
	log.removeLogDestination(&fd);
	fd.close();


	// this will only go to stdout now
	header=logger::getLogHeader("myprogram");
	log.write(1,header,0,"stdout only message");
	delete[] header;
}
