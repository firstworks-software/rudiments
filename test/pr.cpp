// Copyright (c) David Muse
// See the COPYING file for more information

#define HTTP_MODULE_NAME parser
#define HTTP_MODULE_CGI

#include <rudiments/httpserverapi.h>
#include <rudiments/urlhttprequest.h>
#include <rudiments/httpresponse.h>
#include <rudiments/httpserverapimain.h>
#include <rudiments/templateengine.h>
#include <rudiments/file.h>

urlhttprequest	*req;
httpresponse	*resp;
templateengine	*te;

class mod1mod2 {
	public:
		static	bool	stHtml(output *container,
					const char *filename,
					void *data);
	class sthtmlparser {
		public:
			static	bool	block(output *container,
							const char *name,
							const char *block,
							uint64_t length,
							void *data);
			static	bool	replacements(output *container,
							const char *name,
							const char *block,
							uint64_t length,
							void *data);
	};
};

bool mod1mod2::stHtml(output *container,
			const char *filename,
			void *data) {

	resp->writeTextHtmlHeaderBlock();

	dictionary<const char *,const char *>	vars;
	vars.setValues(req->getAllVariables(),req->getAllValues());

	const char * const exceptions[]={"var1",NULL};
	stringbuffer	fegs;
	req->getParametersAsGetString(&fegs,exceptions);
	stringbuffer	fehv;
	req->getParametersAsHiddenVariables(&fehv,exceptions);

	vars.setValue("parametersAsHiddenVariables",fehv.getString());
	vars.setValue("parametersAsGetString",fegs.getString());

	blockparser	sh[]={
		{container,"block1",sthtmlparser::block,
						(void *)((uint64_t)1)},
		{container,"replacements",sthtmlparser::replacements,
						data},
		{NULL,NULL,NULL,NULL}
	};

	return te->parse(container,req->getPagePath(),sh,&vars);
}

bool mod1mod2::sthtmlparser::block(output *container,
					const char *name,
					const char *block,
					uint64_t length,
					void *data) {

	dictionary<const char *,const char *>	vars;
	vars.setValues(req->getAllVariables(),req->getAllValues());

	blockparser	sh[]={
		{container,"block2",sthtmlparser::block,
						(void *)((uint64_t)2)},
		{container,"block3",sthtmlparser::block,
						(void *)((uint64_t)3)},
		{container,"block4",sthtmlparser::block,
						(void *)((uint64_t)4)},
		{NULL,NULL,NULL,NULL}
	};

	for (uint64_t i=0; i<*((uint64_t *)data); i++) {
		if (!te->parse(container,block,length,sh,&vars)) {
			return false;
		}
	}
	return true;
}

bool mod1mod2::sthtmlparser::replacements(output *container,
						const char *name,
						const char *block,
						uint64_t length,
						void *data) {

	const char * const vararray1[]={"vararray1,1","vararray1,2",NULL};
	const char * const valarray1[]={"valarray1,1","valarray1,2",NULL};
	const char * const vararray2[]={"vararray2,1","vararray2,2",NULL};
	const char * const valarray2[]={"valarray2,1","valarray2,2",NULL};

	dictionary<const char *,const char *>	vars;
	vars.setValues(req->getAllVariables(),req->getAllValues());
	vars.setValues(vararray1,valarray1);
	vars.setValues(vararray2,valarray2);
	vars.setValue("var1","val1");
	vars.setValue("var2","val2");
	vars.setValue("var3","val3");
	vars.setValue("var4","val4");
	vars.setValue("quotedvar","\"value\" 'which' \"contains\" 'some' \"quoted\" 'stuff'");

	return te->parse(container,block,length,NULL,&vars);
}

static bool httpModuleInit(httpserverapi *sapi) {
	return true;
}

bool httpModuleMain(httpserverapi *sapi) {

	urlhttprequest	mreq(sapi);
	httpresponse	mresp(sapi);
	templateengine	mte;
	te=&mte;
	req=&mreq;
	resp=&mresp;

	struct fileparser	ph[]={
		{"$(modulePath)/pr.html",mod1mod2::stHtml,NULL},
		{NULL,NULL,NULL}
	};

	dictionary<const char *,const char *>	vars;
	vars.setValues(req->getAllVariables(),req->getAllValues());

	return te->parse(resp,req->getPagePath(),ph,&vars);
}

static bool httpModuleExit(httpserverapi *sapi) {
	return true;
}
