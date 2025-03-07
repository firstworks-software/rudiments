// Copyright (c) David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_CGIMODULE_H
#define RUDIMENTS_CGIMODULE_H

#include <rudiments/cgihttpserverapi.h>
#include <rudiments/process.h>

static bool httpModuleInit(httpserverapi *sapi);
static bool httpModuleMain(httpserverapi *sapi);
static bool httpModuleExit(httpserverapi *sapi);

int main(int argc, char **argv) {
	cgihttpserverapi	sapi;
	process::exit((httpModuleInit(&sapi) &&
			httpModuleMain(&sapi) &&
			httpModuleExit(&sapi))?0:1);
}

#endif
