// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SECURITYCONTEXT_H
#define RUDIMENTS_SECURITYCONTEXT_H

#include <rudiments/socketlayer.h>

/** The securitycontext class extends socketlayer, defining an interface for
 *  security contexts such as TLS and GSS. */
class RUDIMENTS_DLLSPEC securitycontext : public socketlayer {
};

#endif
