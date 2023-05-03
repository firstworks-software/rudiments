// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME parameters

#include <stencil/stencil.h>
#include <stencil/request.h>
#include <stencil/response.h>
#include <stencil/stencilmain.h>

static bool stencilMain(void *apistruct) {

	stencil		st(apistruct);
	request		req(&st);
	response	resp(&st);

	// send an http header
	resp.sendTextPlainHeader();

	// parameters...

	// display an individual parameter
	resp.write("variable1=");
	resp.write(req.parameter("variable1"));
	resp.write("<br><br>\n");

	// list all parameters
	resp.write("All Parameters:<br>\n");
	const char * const *vars=req.parameterVariables();
	const char * const *vals=req.parameterValues();
	for (uint64_t index=0; index<req.parameterCount(); index++) {
		resp.write("	");
		resp.write(vars[index]);
		resp.write("=");
		resp.write(vals[index]);
		resp.write("<br>\n");
	}
	resp.write("<br>\n");

	// display parameters as a get string
	stringbuffer	getstring;
	req.parametersAsGetString(&getstring,NULL);

	stringbuffer	getstringwithexceptions;
	const char * const gsexceptions[]={"variable1",NULL};
	req.parametersAsGetString(&getstringwithexceptions,gsexceptions);

	resp.write("All Parameters As A Get String: ")->write(getstring.getString())->write("<br><br>\n");

	resp.write("All Parameters (except variable1) As A Get String: ")->write(getstringwithexceptions.getString())->write("<br><br>\n");

	

	// display parameters as hidden variables
	stringbuffer	hiddenvars;
	req.parametersAsHiddenVariables(&hiddenvars,NULL);

	stringbuffer	hiddenvarswithexceptions;
	const char * const hvexceptions[]={"variable1",NULL};
	req.parametersAsHiddenVariables(&hiddenvarswithexceptions,hvexceptions);

	resp.write("All Parameters As Hidden Variables:<br>\n");
	resp.write(hiddenvars.getString())->write("<br><br>\n");

	resp.write("All Parameters (except variable1) As Hidden Variables:<br>\n");
	resp.write(hiddenvars.getString())->write("<br><br>\n");

	// add a parameter
	req.newParameter("newentry","newvalue");
	resp.write("New Parameter: ");
	resp.write(req.parameter("newentry"));
	resp.write("<br>\n");

	// change a parameter
	req.changeParameter("newentry","newervalue");
	resp.write("Changed Parameter: ");
	resp.write(req.parameter("newentry"));
	resp.write("<br>\n");
	
	return true;
}
