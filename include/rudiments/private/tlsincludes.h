// Copyright (c) David Muse
// See the COPYING file for more information.

#include <rudiments/object.h>

#include <rudiments/securitycontext.h>
#include <rudiments/filedescriptor.h>
#include <rudiments/datetime.h>
#include <rudiments/threadmutex.h>

class tlscontext;
class tlscontextprivate;
class tlscertificate;
class tlscertificateprivate;

class filedescriptor;

// openssl's BIO, forward-declared so the public header doesn't
// have to include openssl
struct bio_st;
