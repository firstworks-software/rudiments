// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define HTTP_MODULE_NAME cgitest
#define HTTP_MODULE_CGI

#include <rudiments/httpserverapi.h>
#include <rudiments/stencilhttprequest.h>
#include <rudiments/httpresponse.h>
#include <rudiments/httpserverapimain.h>
#include <rudiments/stdio.h>

static bool httpModuleMain(httpserverapi *sapi) {

	stencilhttprequest	req(sapi);
	httpresponse		resp(sapi);


	// convenience methods

	// send an http header
	resp.textPlain();
	req.dumpVariables();



	// file ouptut
	stdoutput.printf("File Output:\n");
	file	f;
	f.open("../../ChangeLog",O_RDONLY);
	resp.write(&f);



	// parameter methods

	// get a specific parameter
	stdoutput.printf("\n");
	stdoutput.printf("Specific Parameter:\n");
	stdoutput.printf("	var1=%s\n",req.getParameter("var1"));
	stdoutput.printf("	var1=");
	resp.write(req.getParameter("var1"));
	stdoutput.printf("\n\n");

	// all parameters as a get string:
	const char	*exceptions[]={"var1",NULL};
	stringbuffer	*container=new stringbuffer();

	req.getParametersAsGetString(container,NULL);
	stdoutput.printf("Get string: \n%s\n\n",container->getString());
	delete container;

	container=new stringbuffer();
	req.getParametersAsGetString(container,exceptions);
	stdoutput.printf("Get string (without var1): \n%s\n\n",container->getString());
	delete container;

	// all parameters as hidden variables
	container=new stringbuffer();
	req.getParametersAsHiddenVariables(container,NULL);
	stdoutput.printf("Hidden vars: \n%s\n\n",container->getString());
	delete container;

	container=new stringbuffer();
	req.getParametersAsHiddenVariables(container,exceptions);
	stdoutput.printf("Hidden vars (without var1): \n%s\n\n",container->getString());
	delete container;

	// list all parameters
	stdoutput.printf("All Parameters: %d\n",req.getParameterCount());
	const char * const *vars=req.getParameterVariables();
	const char * const *vals=req.getParameterValues();
	int	index=0;
	while (vars[index]) {
		stdoutput.printf("	%s=%s\n",vars[index],vals[index]);
		index++;
	}
	stdoutput.printf("\n");

	// new parameter
	req.setParameter("newentry","newvalue");
	stdoutput.printf("New Parameter: %s\n\n",req.getParameter("newentry"));
	req.setParameter("newentry","newervalue");
	stdoutput.printf("Changed Parameter: %s\n\n",req.getParameter("newentry"));



	// cookie entry methods

	// get a specific cookie entry
	stdoutput.printf("Specific Cookie:\n");
	stdoutput.printf("	cookie=%s\n",req.getCookie("cookievar1"));
	stdoutput.printf("	cookie=");
	resp.write(req.getCookie("cookievar1"));
	stdoutput.printf("\n\n");

	// list all cookie entries
	stdoutput.printf("All Cookies: %d\n",req.getCookieCount());
	vars=req.getCookieVariables();
	vals=req.getCookieValues();
	index=0;
	while (vars[index]) {
		stdoutput.printf("	%s=%s\n",vars[index],vals[index]);
		index++;
	}
	stdoutput.printf("\n");

	// new cookie entry
	req.setCookie("newentry","newvalue");
	stdoutput.printf("New Cookie: %s\n\n",req.getCookie("newentry"));
	req.setCookie("newentry","newervalue");
	stdoutput.printf("Changed Cookie: %s\n\n",req.getCookie("newentry"));



	// file entry methods

	// get a specific file entry
	stdoutput.printf("Specific File Parameter:\n");
	stdoutput.printf("	     file=%s\n",req.getFileParameterFilename("file"));
	stdoutput.printf("	temp file=%s\n",req.getFileParameterTempFilename("file"));
	stdoutput.printf("	mime type=%s\n\n",req.getFileParameterMimeType("file"));

	stdoutput.printf("Count of File Parameters: %d\n",req.getFileCount());

	// new file entry
	req.setFileParameter("newfile","filename","tempfilename","text/html");
	stdoutput.printf("	     file=%s\n",req.getFileParameterFilename("newfile"));
	stdoutput.printf("	temp file=%s\n",req.getFileParameterTempFilename("newfile"));
	stdoutput.printf("	mime type=%s\n",req.getFileParameterMimeType("newfile"));
	stdoutput.printf("Count of File Parameters: %d\n",req.getFileCount());
	stdoutput.printf("\n");

	return true;
}
