// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/dynamiclib.h>
#include <rudiments/charstring.h>
#include <rudiments/process.h>
#include <rudiments/error.h>

#if defined(RUDIMENTS_HAVE_DLFCN_H)
	#include <dlfcn.h>
#elif defined(RUDIMENTS_HAVE_MACH_O_DYLD_H)
	#include <rudiments/environment.h>
	#include <rudiments/stringbuffer.h>
	#include <rudiments/singlylinkedlist.h>
	#include <rudiments/file.h>
	#include <mach-o/dyld.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif

#include <stdio.h>

class dynamiclibprivate {
	friend class dynamiclib;
	private:
		#if defined(RUDIMENTS_HAVE_DLOPEN)
			void	*_handle;
		#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
			HMODULE	_handle;
		#elif defined(RUDIMENTS_HAVE_NSLINKMODULE)
			NSObjectFileImage	_nsofi;
			bool			_isbundle;
			void			*_handle;
			const char		*_error;
			bool			_linkerror;
		#endif
};

// LAME: not in the class
static	threadmutex	*_errormutex=NULL;

dynamiclib::dynamiclib() : object() {
	pvt=new dynamiclibprivate;
	#if defined(RUDIMENTS_HAVE_DLOPEN) || \
		defined(RUDIMENTS_HAVE_LOADLIBRARYEX) || \
		defined(RUDIMENTS_HAVE_NSLINKMODULE)
		pvt->_handle=NULL;
	#endif
	#if !defined(RUDIMENTS_HAVE_DLOPEN) && \
		defined(RUDIMENTS_HAVE_NSLINKMODULE)
		pvt->_nsofi=NULL;
		pvt->_isbundle=false;
		pvt->_error=NULL;
		pvt->_linkerror=false;
	#endif
}

dynamiclib::~dynamiclib() {

	// see NOTE in ~threadmutex()

	if (!pvt) {
		return;
	}

	close();

	dynamiclibprivate	*tmppvt=pvt;
	pvt=NULL;
	delete tmppvt;
}

bool dynamiclib::open(const char *library, bool loaddependencies, bool global) {
	#if defined(RUDIMENTS_HAVE_DLOPEN)

		int32_t	flag=(loaddependencies)?RTLD_NOW:RTLD_LAZY;
		#ifdef RTLD_GLOBAL
		if (global) {
			flag|=RTLD_GLOBAL;
		}
		#endif
		error::clearError();
		do {
			dlerror();
			pvt->_handle=dlopen(library,flag);
		} while (!pvt->_handle && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return (pvt->_handle!=NULL);

	#elif defined(RUDIMENTS_HAVE_NSLINKMODULE)

		// clear any previous error
		error::clearError();
		pvt->_error=NULL;
		pvt->_linkerror=false;

		// reset nsofi and isbundle
		pvt->_nsofi=NULL;
		pvt->_isbundle=false;

		// If the library contains a slash then assume that it's a
		// full or relative path.  Otherwise assume it's just a file
		// name that we need to search for in the usual places.
		singlylinkedlist<char *>	paths;
		if (charstring::contains(library,'/')) {

			// stub out the paths list
			paths.append("");

		} else {

			paths.setManageArrayValues(true);

			// build list of paths to search,
			// including DYLD_LIBRARY_PATH, LD_LIBRARY_PATH,
			// /lib, and /usr/lib
			char				**list;
			uint64_t			listlength;
			const char *envs[]={
				"DYLD_LIBRARY_PATH",
				"LD_LIBRARY_PATH",
				NULL
			};
			for (const char **env=envs; *env; env++) {
				const char	*val=
						environment::getValue(*env);
				charstring::split(val,":",true,
							&list,&listlength);
				for (uint64_t i=0; i<listlength; i++) {
					if (!charstring::isNullOrEmpty(
								list[i])) {
						paths.append(list[i]);
					}
				}
				delete[] list;
			}
			paths.append(charstring::duplicate("/lib"));
			paths.append(charstring::duplicate("/usr/lib"));
		}
		
		// search for the file...
		// assume that the file doesn't exist
		// then, for each search path...
		//	* if the file exists but there is some permissions
		//		error or file format error, then that becomes
		//		the current result and we keep looking
		// 	* if the file exists and is a good file, then that
		// 		becomes	the current result and we stop looking
		// 	* if we don't find the file in subsequent paths then
		// 		that doesn't override the current result
		// 	* but, if we don't find the file in any search path,
		// 		and didn't run into any other errors, then
		// 		the final result is file-not-found
		stringbuffer			fullpath;
		NSObjectFileImageReturnCode	result=NSObjectFileImageFailure;
		for (listnode<char *> *node=paths.getFirst();
						node; node=node->getNext()) {

			// get the path
			const char	*path=node->getValue();

			// build the fully qualified path name of the library
			if (path[0]) {
				fullpath.append(path)->append('/');
			}
			fullpath.append(library);

			// fully resolve any symbolic links
			const char	*fp=NULL;
			char		*f=NULL;
			for (;;) {
				fp=fullpath.getString();
				f=file::resolveSymbolicLink(fp);
				if (!f) {
					break;
				} else {
					fullpath.clear();
					if (!charstring::contains(f,'/')) {
						fullpath.append(path);
						fullpath.append('/');
					}
					fullpath.append(f);
					delete[] f;
				}
			}

			// create the object file image
			NSObjectFileImageReturnCode	res=
					NSCreateObjectFileImageFromFile(fp,
								&pvt->_nsofi);

			// if we got this specific error then try to add the
			// file as a library directly, this appears to be
			// necessary if the file is a dylib, rather than a
			// bundle, and works for dylibs with flat namespaces
			if (res==NSObjectFileImageInappropriateFile) {

				pvt->_handle=(void *)NSAddLibrary(fp);

				#ifdef NSADDIMAGE_OPTION_RETURN_ON_ERROR
				// if that failed, then try to add it as an
				// image, this appears to be necessary for
				// dylibs with two-level namespaces
				if (!pvt->_handle) {
					pvt->_handle=(void *)NSAddImage(fp,
					NSADDIMAGE_OPTION_RETURN_ON_ERROR);
				}
				#endif

				if (pvt->_handle) {
					return true;
				} else {
					break;
				}

			} else if (res==NSObjectFileImageSuccess) {
				result=res;
				break;
			} else if (res!=NSObjectFileImageFailure) {
				result=res;
			}

			// clean up
			fullpath.clear();
		}

		// set the error appropriately (if one occurred)
		switch (result) {
			case NSObjectFileImageSuccess:
				break;
			case NSObjectFileImageFailure:
				pvt->_error="No such file or directory";
				return false;
			case NSObjectFileImageAccess:
				pvt->_error="Permission denied";
				return false;
			default:
				pvt->_error="Exec format error";
				return false;
		}

		// determine link options
		unsigned long	options=NSLINKMODULE_OPTION_RETURN_ON_ERROR;
		if (loaddependencies) {
			options|=NSLINKMODULE_OPTION_BINDNOW;
		}
		// FIXME: if global is not specified then include
		// NSLINKMODULE_OPTION_PRIVATE?

		// link the module
		pvt->_handle=NSLinkModule(pvt->_nsofi,library,options);
		if (!pvt->_handle) {

			pvt->_linkerror=true;

			// destroy the object file image
			NSDestroyObjectFileImage(pvt->_nsofi);
			return false;
		}
		pvt->_isbundle=true;
		return true;

	#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)

		pvt->_handle=LoadLibraryEx(library,NULL,
			(loaddependencies)?0:DONT_RESOLVE_DLL_REFERENCES);
		return (pvt->_handle)?true:false;

	#else

		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool dynamiclib::close() {
	#if defined(RUDIMENTS_HAVE_DLOPEN) || \
		defined(RUDIMENTS_HAVE_LOADLIBRARYEX) || \
		defined(RUDIMENTS_HAVE_NSLINKMODULE)
		if (!pvt->_handle) {
			return true;
		}
	#endif
	bool	retval=false;
	#if defined(RUDIMENTS_HAVE_DLOPEN)
		int32_t	result;
		error::clearError();
		do {
			dlerror();
			result=dlclose(pvt->_handle);
			// dlclose() should return 0 on success or -1 on error.
			// Some platforms (MacOS X 10.3) also return 1 on
			// success.  So, we'll catch -1 below, rather than 0.
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		retval=(result!=-1);
	#elif defined(RUDIMENTS_HAVE_NSLINKMODULE)

		// clear any previous error
		error::clearError();
		pvt->_error=NULL;
		pvt->_linkerror=false;

		if (pvt->_nsofi) {

			// unlink the module
			retval=(NSUnLinkModule(pvt->_handle,
					NSUNLINKMODULE_OPTION_NONE)==TRUE);

			// destroy the object file image
			NSDestroyObjectFileImage(pvt->_nsofi);

		} else {
			retval=true;
		}

	#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
		retval=(FreeLibrary(pvt->_handle)==TRUE);
	#else
		RUDIMENTS_SET_ENOSYS
	#endif
	#if defined(RUDIMENTS_HAVE_DLOPEN) || \
		defined(RUDIMENTS_HAVE_LOADLIBRARYEX) || \
		defined(RUDIMENTS_HAVE_NSLINKMODULE)
		pvt->_handle=NULL;
	#endif
	return retval;
}

void *dynamiclib::getSymbol(const char *symbol) {
	#if defined(RUDIMENTS_HAVE_DLOPEN)
		void	*symhandle;
		error::clearError();
		do {
			dlerror();
			// What's this char * cast all about?
			// Really old versions of dlsym (linux libc5) define
			// dlsym with char * argument rather than const char *.
			// This works with old and new implementations.
			symhandle=dlsym(pvt->_handle,(char *)symbol);
		} while (!symhandle && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return (pvt->_handle)?symhandle:NULL;
	#elif defined(RUDIMENTS_HAVE_NSLINKMODULE)

		// clear any previous error
		error::clearError();
		pvt->_error=NULL;
		pvt->_linkerror=false;

		// prepend a _ to the symbol name
		stringbuffer	temp;
		temp.append('_')->append(symbol);
		symbol=temp.getString();

		NSSymbol	sym=NULL;
		if (pvt->_isbundle) {

			// if we loaded a bundle, then get the symbol this way
			sym=NSLookupSymbolInModule(pvt->_handle,symbol);
			if (!sym) {
				pvt->_error="Undefined symbol";
				return NULL;
			}

		} else {

			// if we loaded a dylib, then get the symbol this way
			if (!NSIsSymbolNameDefined(symbol)) {
				pvt->_error="Undefined symbol";
				return NULL;
			}

			// bind the symbol
			sym=NSLookupAndBindSymbol(symbol);
			if (!sym) {
				pvt->_error="Undefined symbol";
				return NULL;
			}
		}

		// get the address of the symbol
		void	*address=NSAddressOfSymbol(sym);
		if (!address) {
			pvt->_error="Bad address";
		}
		return address;

	#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
		return (void *)GetProcAddress(pvt->_handle,symbol);
	#else
		RUDIMENTS_SET_ENOSYS
		return NULL;
	#endif
}

char *dynamiclib::getError() {
	#if defined(RUDIMENTS_HAVE_DLOPEN)
		if (_errormutex && !_errormutex->lock()) {
			return NULL;
		}
		const char	*err;
		error::clearError();
		do {
			err=dlerror();
		} while (!err && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		char		*retval=NULL;
		if (err) {
			retval=charstring::duplicate(err);
		}
		if (_errormutex) {
			_errormutex->unlock();
		}
		return retval;
	#elif defined(RUDIMENTS_HAVE_NSLINKMODULE)
		char	*retval=NULL;
		if (pvt->_error) {
			retval=charstring::duplicate(pvt->_error);
		} else if (pvt->_linkerror) {
			if (_errormutex && !_errormutex->lock()) {
				return NULL;
			}
			NSLinkEditErrors	c;
			int			errornumber;
			const char		*filename;
			const char		*errorstring;
			NSLinkEditError(&c,&errornumber,&filename,&errorstring);
			retval=charstring::duplicate(errorstring);
			if (_errormutex) {
				_errormutex->unlock();
			}
		}
		return retval;
	#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
		char	*buffer;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|
				FORMAT_MESSAGE_IGNORE_INSERTS|
				FORMAT_MESSAGE_MAX_WIDTH_MASK|
				FORMAT_MESSAGE_ALLOCATE_BUFFER,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
				(char *)&buffer,
				0,
				NULL);

		// copy the buffer and return that so the message can be
		// freed using delete[] rather than having to use LocalFree
		char	*retval=NULL;
		if (charstring::compare(buffer,
				"The operation completed successfully. ")) {
			retval=charstring::duplicate(buffer);
		}
		LocalFree(buffer);
		return retval;
	#else
		RUDIMENTS_SET_ENOSYS
		return NULL;
	#endif
}

void dynamiclib::setErrorMutex(threadmutex *mtx) {
	_errormutex=mtx;
}
