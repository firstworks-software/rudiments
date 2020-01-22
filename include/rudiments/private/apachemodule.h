// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_APACHEMODULE_H
#define RUDIMENTS_APACHEMODULE_H

#ifndef HTTP_MODULE_NAME
	#error HTTP_MODULE_NAME not defined
#endif

#define XHANDLER_DO_FUNCTION(NM) HANDLER_DO_FUNCTION(NM)
#define HANDLER_DO_FUNCTION(NM) do_##NM##_handler

#define XHANDLER_FUNCTION(NM) HANDLER_FUNCTION(NM)
#define HANDLER_FUNCTION(NM) NM##_handler

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

static bool httpModuleMain(httpserverapi *sapi);

extern "C" {

static int XHANDLER_FUNCTION(HTTP_MODULE_NAME)(request_rec *r);

#ifdef APACHE_2

	static int XHANDLER_DO_FUNCTION(HTTP_MODULE_NAME)(request_rec *r) {

		if (charstring::compare(r->handler,
					XMOD_NAME(HTTP_MODULE_NAME))) {
			return DECLINED;
		}

		return XHANDLER_FUNCTION(HTTP_MODULE_NAME)(r);
	}

	static void register_hooks(apr_pool_t *pool) {
		ap_hook_handler(XHANDLER_DO_FUNCTION(HTTP_MODULE_NAME),
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

#else

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
		NULL,		/* child_init                          */
		NULL,		/* child_exit                          */
		NULL		/* [#0] post read-request              */
		#ifdef EAPI
		,NULL,		/* EAPI: add_module                    */
		NULL,		/* EAPI: remove_module                 */
		NULL,		/* EAPI: rewrite_command               */
		NULL		/* EAPI: new_connection                */
		#endif
	};

#endif

static int XHANDLER_FUNCTION(HTTP_MODULE_NAME)(request_rec *r) {
	apachehttpserverapi	sapi((void *)r);
	return (httpModuleMain(&sapi))?OK:DECLINED;
}

}

#endif
