// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/environment.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/process.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#ifdef RUDIMENTS_HAVE_ENVIRON
	#ifdef RUDIMENTS_HAVE_UNISTD_H
		#include <unistd.h>
	#endif
#else
	#ifdef RUDIMENTS_HAVE_NSGETENVIRON
		#include <crt_externs.h>
		#define environ (*_NSGetEnviron())
	#else
		extern	char	**environ;
	#endif
#endif

// LAME: not in the class
static threadmutex	*_envmutex;

#if defined(RUDIMENTS_HAVE_PUTENV) || defined(RUDIMENTS_HAVE__PUTENV)

// When using putenv(), things are a lot more complicated...
//
// You have to build a "key=value" type string and pass that to putenv().
// All putenv() does with that is set a pointer to it in the global environ
// variable.  So, the string you built has to persist until the program
// exits or until you unsetenv() it.
//
// As such, we need a static _envstrings dictionary to keep track of
// key -> "key=value" pairs.
//
// The "key=value" string must also be allocated using malloc() rather than
// new.  I think.  Sadly, I don't have a record of which platform cares about
// this, or why, but some platform does.
//
// So, we can't just call _envstrings->setManageArrayValues(true) and let it
// automatically clean up.  Rather we have to manually clean up before calling
// _envstrings->setValue() or _envstrings->remove(), and register an atExit()
// method to clean up the values before _envstrings gets deleted.
//
// Further, we can't trust that the compiler will be compliant and call
// atExit() methods after calling static destructors, so _envstrings has to be
// manually allocated and then manually deleted during the atExit() method.
//
// We also can't trust that a program which calls setValue() will do so with a
// string literal, or some other pointer to a persistent string.  It may have
// passed in a local variable that will disappear soon.
//
// Additionally, on some platforms (fedora 34, at least), the text segment of
// the process appers to be deallocated (or some other bad thing) by the time
// the atExit() methods run, so even if the program did pass in a string
// literal, we don't appear to have access to it when the atExit() method is
// run, leading to crash-loops on exit and hung processes.
//
// So we need to make a copy of the key.  Fortunately we can use
// _envstrings->setManageArrayKeys(true) and let it automatically clean up
// these copies.
//
// setenv() does all of that for us, so ideally, we'd prefer setenv() to
// putenv(), but it turns out there are platforms who's setenv()
// implementations leak memory if you call it over and over, and their putenv()
// implementations don't.  Sadly, again, I don't have a record of which ones.
//
// It's a mess.

// LAME: not in the class
static dictionary<char *, char *>	*_envstrings;

void environment::init() {
	_envstrings=new dictionary<char *, char *>;
	_envstrings->setManageArrayKeys(true);

	// On windows, the entire rudiments dll appears to have been unloaded
	// before atexit functions are called, and it leads to all kinds of
	// chaos.  Ideally this should be called when the dll is unloaded but
	// that's tricky to do too.  For now it's just disabled.
	#ifndef _WIN32
		process::registerExitHandler((void (*)(void))environment::exit);
	#endif
}

void environment::exit() {
	for (listnode<char *> *node=_envstrings->getKeys()->getFirst();
						node; node=node->getNext()) {
		free((void *)_envstrings->getValue(node->getValue()));
	}
	delete _envstrings;
}

bool environment::setValue(const char *variable, const char *value) {
	bool	retval=false;
	if (_envmutex && !_envmutex->lock()) {
		return retval;
	}
	if (!_envstrings) {
		init();
	}
	size_t	pestrlen=charstring::getLength(variable)+
				charstring::getLength(value)+2;
	char	*pestr=(char *)malloc(pestrlen*sizeof(char));
	charstring::copy(pestr,variable);
	charstring::append(pestr,"=");
	charstring::append(pestr,value);
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE__PUTENV)
			result=_putenv(pestr);
		#elif defined(RUDIMENTS_HAVE_PUTENV)
			result=putenv(pestr);
		#else
			#error no putenv or anything like it
		#endif
	} while (result && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	if (!result) {
		char	*oldpestr=NULL;
		if (_envstrings->getValue((char *)variable,&oldpestr)) {
			free((void *)oldpestr);
			_envstrings->setValue((char *)variable,pestr);
		} else {
			_envstrings->setValue(
				charstring::duplicate(variable),pestr);
		}
		retval=true;
	} else {
		free((void *)pestr);
		retval=false;
	}
	if (_envmutex) {
		_envmutex->unlock();
	}
	return retval;
}
#else
void environment::exit() {
	// do nothing
}

void environment::init() {
	// do nothing
}
#endif

#ifdef RUDIMENTS_HAVE__DUPENV_S
	static	char	*envval=NULL;
#endif

const char *environment::getValue(const char *variable) {
	char	*retval=NULL;
	if (_envmutex && !_envmutex->lock()) {
		return retval;
	}
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE__DUPENV_S)
			// FIXME: _dupenv_s is meant to be thread-safe and this
			// usage certainly isn't.  This just emulates the
			// behavior of getenv and suppresses compiler warnings.
			free((void *)envval);
			envval=NULL;
			size_t	len;
			if (!_dupenv_s(&envval,&len,variable)) {
				retval=envval;
			}
		#elif defined(RUDIMENTS_HAVE_GETENV)
			retval=getenv(variable);
		#else
			#error no getenv or anything like it
		#endif
	} while (error::getErrorNumber()==EINTR && !process::getShutDownFlag());
	if (_envmutex) {
		_envmutex->unlock();
	}
	return retval;
}

#if defined(RUDIMENTS_HAVE_SETENV) && \
	!defined(RUDIMENTS_HAVE_PUTENV) && \
	!defined(RUDIMENTS_HAVE__PUTENV)
bool environment::setValue(const char *variable, const char *value) {
	bool	retval=false;
	if (_envmutex && !_envmutex->lock()) {
		return retval;
	}
	error::clearError();
	do {
		retval=!setenv(variable,value,1);
	} while (!retval && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	if (_envmutex) {
		_envmutex->unlock();
	}
	return retval;
}
#endif

bool environment::remove(const char *variable) {
#ifdef RUDIMENTS_HAVE_UNSETENV
	bool	retval=false;
	if (_envmutex && !_envmutex->lock()) {
		return retval;
	}
	#if defined(RUDIMENTS_HAVE_PUTENV) || defined(RUDIMENTS_HAVE__PUTENV)
		if (!_envstrings) {
			init();
		}
	#endif
	unsetenv(variable);
	#if defined(RUDIMENTS_HAVE_PUTENV) || defined(RUDIMENTS_HAVE__PUTENV)
		char *pestr;
		if (_envstrings->getValue((char *)variable,&pestr)) {
			free((void *)pestr);
			_envstrings->remove((char *)variable);
		}
	#endif
	retval=true;
	if (_envmutex) {
		_envmutex->unlock();
	}
	return retval;
#else
	// I know this isn't the same as calling unsetenv, but as far as I
	// know, it's all that can be done.
	if (!getValue(variable)) {
		return true;
	}
	return setValue(variable,"");
#endif
}

const char * const *environment::getVariables() {
	return environ;
}

bool environment::clear() {
#ifdef RUDIMENTS_HAVE_CLEARENV
	return !clearenv();
#else
	environ[0]=NULL;
	return true;
#endif
}

void environment::setMutex(threadmutex *mtx) {
	_envmutex=mtx;
}
