// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_CGIMODULE_H
#define RUDIMENTS_CGIMODULE_H

#include <rudiments/cgihttpserverapi.h>
#include <rudiments/process.h>

static bool httpModuleMain(httpserverapi *sapi);

int main(int argc, char **argv) {
	cgihttpserverapi	sapi;
	process::exit(httpModuleMain(&sapi)?0:1);
}

#endif
