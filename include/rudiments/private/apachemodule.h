// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_APACHEMODULE_H
#define RUDIMENTS_APACHEMODULE_H

#ifndef HTTP_MODULE_NAME
	#error HTTP_MODULE_NAME not defined
#endif

#define XDO_CHILD_INIT_FUNCTION(NM) DO_CHILD_INIT_FUNCTION(NM)
#define DO_CHILD_INIT_FUNCTION(NM) do_##NM##_init
#define XCHILD_INIT_FUNCTION(NM) CHILD_INIT_FUNCTION(NM)
#define CHILD_INIT_FUNCTION(NM) NM##_init

#define XDO_HANDLER_FUNCTION(NM) DO_HANDLER_FUNCTION(NM)
#define DO_HANDLER_FUNCTION(NM) do_##NM##_handler
#define XHANDLER_FUNCTION(NM) HANDLER_FUNCTION(NM)
#define HANDLER_FUNCTION(NM) NM##_handler

#define XDO_CHILD_EXIT_FUNCTION(NM) DO_CHILD_EXIT_FUNCTION(NM)
#define DO_CHILD_EXIT_FUNCTION(NM) do_##NM##_exit
#define XCHILD_EXIT_FUNCTION(NM) CHILD_EXIT_FUNCTION(NM)
#define CHILD_EXIT_FUNCTION(NM) NM##_exit

#define XMODULE_STRUCT(NM) MODULE_STRUCT(NM)
#define MODULE_STRUCT(NM) NM##_module

#define XXFILE_NAME(NM) XFILE_NAME(NM)
#define XFILE_NAME(NM) FILE_NAME(mod_##NM)
#define FILE_NAME(NM) #NM".c"

#define XMOD_NAME(NM) MOD_NAME(NM)
#define MOD_NAME(NM) #NM

#include <rudiments/apachehttpserverapi.h>
#include <rudiments/private/apacheincludes.h>

#ifdef APACHE_2
	#include <rudiments/charstring.h>
#endif

struct apacheapistruct {
	void	*requestrec;
	void	*serverrec;
};

static bool httpModuleInit(httpserverapi *sapi);
static bool httpModuleExit(httpserverapi *sapi);
static bool httpModuleMain(httpserverapi *sapi);

extern "C" {

static apr_status_t XHANDLER_FUNCTION(HTTP_MODULE_NAME)(request_rec *r);

#ifdef APACHE_2

	static void XCHILD_INIT_FUNCTION(HTTP_MODULE_NAME)(apr_pool_t *p,
								server_rec *s);
	static apr_status_t XCHILD_EXIT_FUNCTION(HTTP_MODULE_NAME)(void *);

	static void XDO_CHILD_INIT_FUNCTION(HTTP_MODULE_NAME)(apr_pool_t *p,
								server_rec *s) {
		XCHILD_INIT_FUNCTION(HTTP_MODULE_NAME)(p,s);
		apr_pool_cleanup_register(p,s,
				XCHILD_EXIT_FUNCTION(HTTP_MODULE_NAME),
				XCHILD_EXIT_FUNCTION(HTTP_MODULE_NAME));
	}

	static int XDO_HANDLER_FUNCTION(HTTP_MODULE_NAME)(request_rec *r) {
		if (charstring::compare(r->handler,
					XMOD_NAME(HTTP_MODULE_NAME))) {
			return DECLINED;
		}
		return XHANDLER_FUNCTION(HTTP_MODULE_NAME)(r);
	}

	static void register_hooks(apr_pool_t *pool) {
		ap_hook_child_init(XDO_CHILD_INIT_FUNCTION(HTTP_MODULE_NAME),
				NULL,NULL,APR_HOOK_FIRST);
		ap_hook_handler(XDO_HANDLER_FUNCTION(HTTP_MODULE_NAME),
				NULL,NULL,APR_HOOK_FIRST);
	}

	module AP_MODULE_DECLARE_DATA XMODULE_STRUCT(HTTP_MODULE_NAME) = {
		MODULE_MAGIC_NUMBER_MAJOR,
		MODULE_MAGIC_NUMBER_MINOR,
		-1,
		XXFILE_NAME(HTTP_MODULE_NAME),
		NULL,
		NULL,
		MODULE_MAGIC_COOKIE,
		NULL,		/* module initializer                  */
		NULL,		/* create per-dir    config structures */
		NULL,		/* merge  per-dir    config structures */
		NULL,		/* create per-server config structures */
		NULL,		/* merge  per-server config structures */
		NULL,		/* command handlers                    */
		register_hooks	/* register hooks                      */
	};

	static void XCHILD_INIT_FUNCTION(HTTP_MODULE_NAME)(apr_pool_t *p,
								server_rec *s) {
		apacheapistruct	aas;
		aas.requestrec=NULL;
		aas.serverrec=s;

		apachehttpserverapi	sapi((void *)&aas);
		httpModuleInit(&sapi);
	}

	static apr_status_t XCHILD_EXIT_FUNCTION(HTTP_MODULE_NAME)(void *v) {

		apacheapistruct	aas;
		aas.requestrec=NULL;
		aas.serverrec=(server_rec *)v;

		apachehttpserverapi	sapi((void *)&aas);
		return (httpModuleExit(&sapi))?OK:DECLINED;
	}

#else

	static void XCHILD_INIT_FUNCTION(HTTP_MODULE_NAME)(
						server_rec *s, pool *p);
	static apr_status_t XCHILD_EXIT_FUNCTION(HTTP_MODULE_NAME)(
						server_rec *s, pool *p);

	static const handler_rec handler_struct[] = {
		{ XMOD_NAME(HTTP_MODULE_NAME),
			XHANDLER_FUNCTION(HTTP_MODULE_NAME) },
		{ NULL, NULL }
	};

	#ifdef MODULE_MAGIC_COOKIE
		#define GW_MODULE_MAGIC_COOKIE MODULE_MAGIC_COOKIE,
	#else
		#define GW_MODULE_MAGIC_COOKIE
	#endif

	module MODULE_VAR_EXPORT XMODULE_STRUCT(HTTP_MODULE_NAME) = {
		MODULE_MAGIC_NUMBER_MAJOR,
		MODULE_MAGIC_NUMBER_MINOR,
		-1,
		XXFILE_NAME(HTTP_MODULE_NAME),
		NULL,
		NULL,
		GW_MODULE_MAGIC_COOKIE
		NULL,		/* module initializer                  */
		NULL,		/* create per-dir    config structures */
		NULL,		/* merge  per-dir    config structures */
		NULL,		/* create per-server config structures */
		NULL,		/* merge  per-server config structures */
		NULL,		/* table of config file commands       */
		handler_struct,	/* [#8] MIME-typed-dispatched handlers */
		NULL,		/* [#1] URI to filename translation    */
		NULL,		/* [#4] validate user id from request  */
		NULL,		/* [#5] check if the user is ok _here_ */
		NULL,		/* [#3] check access by host address   */
		NULL,		/* [#6] determine MIME type            */
		NULL,		/* [#7] pre-run fixups                 */
		NULL,		/* [#9] log a transaction              */
		NULL,		/* [#2] header parser                  */
		XCHILD_INIT_FUNCTION(HTTP_MODULE_NAME),	/* child_init  */
		XCHILD_EXIT_FUNCTION(HTTP_MODULE_NAME),	/* child_exit  */
		NULL		/* [#0] post read-request              */
		#ifdef EAPI
		,NULL,		/* EAPI: add_module                    */
		NULL,		/* EAPI: remove_module                 */
		NULL,		/* EAPI: rewrite_command               */
		NULL		/* EAPI: new_connection                */
		#endif
	};

	static void XCHILD_INIT_FUNCTION(HTTP_MODULE_NAME)(
						server_rec *s, pool *p) {
		apacheapistruct	aas;
		aas.requestrec=NULL;
		aas.serverrec=s;

		apachehttpserverapi	sapi((void *)&aas);
		httpModuleInit(&sapi);
	}

	static void XCHILD_EXIT_FUNCTION(HTTP_MODULE_NAME)(
						server_rec *s, pool *p) {

		apacheapistruct	aas;
		aas.requestrec=NULL;
		aas.serverrec=s;

		apachehttpserverapi	sapi((void *)&aas);
		httpModuleExit(&sapi);
	}
#endif

static apr_status_t XHANDLER_FUNCTION(HTTP_MODULE_NAME)(request_rec *r) {

	apacheapistruct	aas;
	aas.requestrec=r;
	aas.serverrec=r->server;

	apachehttpserverapi	sapi((void *)&aas);
	return (httpModuleMain(&sapi))?OK:DECLINED;
}

}

#endif
