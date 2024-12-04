' command line arguments
OPTCPPFLAGS="/O2"
DEBUGCPPFLAGS="/MD"
DEBUGLDFLAGS=""
hexversion=""

for i=0 to WScript.Arguments.Count-1

	arg=Wscript.Arguments.Item(i)

	if arg="--enable-small-code" then
		OPTCPPFLAGS="/O1"
	elseif arg="--enable-debug" then
		OPTCPPFLAGS=""
		DEBUGCPPFLAGS="/Zi /MDd /D _DEBUG"
		DEBUGLDFLAGS="/debug"
	elseif mid(arg,1,23)="--with-windows-version=" then
		hexversion=mid(arg,24)
	end if
next


' source, object and header files
srcs="winsock.cpp "+_
	"object.cpp "+_
	"hash.cpp "+_
	"sha1.cpp "+_
	"sha256.cpp "+_
	"md5.cpp "+_
	"encryption.cpp "+_
	"aes128.cpp "+_
	"output.cpp "+_
	"input.cpp "+_
	"wastebasket.cpp "+_
	"sensitivevalue.cpp "+_
	"character.cpp "+_
	"wcharacter.cpp "+_
	"ucs2character.cpp "+_
	"utf8character.cpp "+_
	"utf16character.cpp "+_
	"charstring.cpp "+_
	"wcharstring.cpp "+_
	"ucs2charstring.cpp "+_
	"utf8charstring.cpp "+_
	"utf16charstring.cpp "+_
	"iconvert.cpp "+_
	"chat.cpp "+_
	"client.cpp "+_
	"commandline.cpp "+_
	"des.cpp "+_
	"datetime.cpp "+_
	"device.cpp "+_
	"directory.cpp "+_
	"dynamiclib.cpp "+_
	"environment.cpp "+_
	"error.cpp "+_
	"file.cpp "+_
	"url.cpp "+_
	"filedescriptor.cpp "+_
	"stdio.cpp "+_
	"gss.cpp "+_
	"tls.cpp "+_
	"websocket.cpp "+_
	"filesystem.cpp "+_
	"groupentry.cpp "+_
	"hostentry.cpp "+_
	"inetsocketclient.cpp "+_
	"inetsocketserver.cpp "+_
	"listener.cpp "+_
	"logger.cpp "+_
	"memorymap.cpp "+_
	"memorypool.cpp "+_
	"thread.cpp "+_
	"threadmutex.cpp "+_
	"parameterstring.cpp "+_
	"userentry.cpp "+_
	"permissions.cpp "+_
	"process.cpp "+_
	"protocolentry.cpp "+_
	"randomnumber.cpp "+_
	"bytestring.cpp "+_
	"regex.cpp "+_
	"regularexpression.cpp "+_
	"semaphoreset.cpp "+_
	"server.cpp "+_
	"serviceentry.cpp "+_
	"sharedmemory.cpp "+_
	"signalclasses.cpp "+_
	"snooze.cpp "+_
	"sys.cpp "+_
	"unixsocketclient.cpp "+_
	"unixsocketserver.cpp "+_
	"bytebuffer.cpp "+_
	"dom.cpp "+_
	"domnode.cpp "+_
	"xmldom.cpp "+_
	"sax.cpp "+_
	"xmlsax.cpp "+_
	"domevents.cpp "+_
	"codetree.cpp "+_
	"compiler.cpp "+_
	"inetsocketutil.cpp "+_
	"unixsocketutil.cpp "+_
	"socketserver.cpp "+_
	"socketclient.cpp "+_
	"prompt.cpp "+_
	"templateengine.cpp "+_
	"csvsax.cpp "+_
	"csvdom.cpp "+_
	"jsonsax.cpp "+_
	"jsondom.cpp "+_
	"inisax.cpp "+_
	"inidom.cpp "+_
	"propsax.cpp "+_
	"propdom.cpp "+_
	"httprequest.cpp "+_
	"urlhttprequest.cpp "+_
	"httpresponse.cpp "+_
	"cgihttpserverapi.cpp "+_
	"mvctier.cpp "+_
	"mvcview.cpp "+_
	"mvcresult.cpp "+_
	"mvcproperties.cpp "+_
	"httpbasicsecurity.cpp "+_
	"cronschedule.cpp "+_
	"locale.cpp "

lobjs="winsock.obj "+_
	"object.obj "+_
	"hash.obj "+_
	"sha1.obj "+_
	"sha256.obj "+_
	"md5.obj "+_
	"encryption.obj "+_
	"aes128.obj "+_
	"output.obj "+_
	"input.obj "+_
	"wastebasket.obj "+_
	"sensitivevalue.obj "+_
	"character.obj "+_
	"wcharacter.obj "+_
	"ucs2character.obj "+_
	"utf8character.obj "+_
	"utf16character.obj "+_
	"charstring.obj "+_
	"wcharstring.obj "+_
	"ucs2charstring.obj "+_
	"utf8charstring.obj "+_
	"utf16charstring.obj "+_
	"iconvert.obj "+_
	"chat.obj "+_
	"client.obj "+_
	"commandline.obj "+_
	"des.obj "+_
	"datetime.obj "+_
	"device.obj "+_
	"directory.obj "+_
	"dynamiclib.obj "+_
	"environment.obj "+_
	"error.obj "+_
	"file.obj "+_
	"url.obj "+_
	"filedescriptor.obj "+_
	"stdio.obj "+_
	"gss.obj "+_
	"tls.obj "+_
	"websocket.obj "+_
	"filesystem.obj "+_
	"groupentry.obj "+_
	"hostentry.obj "+_
	"inetsocketclient.obj "+_
	"inetsocketserver.obj "+_
	"listener.obj "+_
	"logger.obj "+_
	"memorymap.obj "+_
	"memorypool.obj "+_
	"thread.obj "+_
	"threadmutex.obj "+_
	"parameterstring.obj "+_
	"userentry.obj "+_
	"permissions.obj "+_
	"process.obj "+_
	"protocolentry.obj "+_
	"randomnumber.obj "+_
	"bytestring.obj "+_
	"regex.obj "+_
	"regularexpression.obj "+_
	"semaphoreset.obj "+_
	"server.obj "+_
	"serviceentry.obj "+_
	"sharedmemory.obj "+_
	"signalclasses.obj "+_
	"snooze.obj "+_
	"sys.obj "+_
	"unixsocketclient.obj "+_
	"unixsocketserver.obj "+_
	"bytebuffer.obj "+_
	"dom.obj "+_
	"domnode.obj "+_
	"xmldom.obj "+_
	"sax.obj "+_
	"xmlsax.obj "+_
	"domevents.obj "+_
	"codetree.obj "+_
	"compiler.obj "+_
	"inetsocketutil.obj "+_
	"unixsocketutil.obj "+_
	"socketserver.obj "+_
	"socketclient.obj "+_
	"prompt.obj "+_
	"templateengine.obj "+_
	"csvsax.obj "+_
	"csvdom.obj "+_
	"jsonsax.obj "+_
	"jsondom.obj "+_
	"inisax.obj "+_
	"inidom.obj "+_
	"propsax.obj "+_
	"propdom.obj "+_
	"httprequest.obj "+_
	"urlhttprequest.obj "+_
	"httpresponse.obj "+_
	"cgihttpserverapi.obj "+_
	"mvctier.obj "+_
	"mvcview.obj "+_
	"mvcresult.obj "+_
	"mvcproperties.obj "+_
	"httpbasicsecurity.obj "+_
	"cronschedule.obj "+_
	"locale.obj "

headers0="object.h "+_
	"collection.h "+_
	"comparator.h "+_
	"nodecollection.h "+_
	"listcollection.h "+_
	"treecollection.h "+_
	"tablecollection.h "+_
	"scalarcollection.h "+_
	"arraycollection.h "+_
	"dictionarycollection.h "+_
	"resultcodes.h "+_
	"debugprint.h"
headers1="table.h "+_
	"linkedlist.h "+_
	"singlylinkedlist.h "+_
	"avltree.h "+_
	"scalar.h "+_
	"character.h "+_
	"wcharacter.h "+_
	"ucs2character.h "+_
	"utf8character.h "+_
	"utf16character.h "+_
	"charstring.h "+_
	"wcharstring.h "+_
	"ucs2charstring.h "+_
	"utf8charstring.h "+_
	"utf16charstring.h "+_
	"iconvert.h "+_
	"chat.h "+_
	"client.h "+_
	"commandline.h "+_
	"des.h "+_
	"datetime.h "+_
	"device.h "+_
	"directory.h "+_
	"dynamiclib.h "+_
	"environment.h "+_
	"error.h "+_
	"file.h "+_
	"url.h "+_
	"filedescriptor.h "+_
	"stdio.h "+_
	"filesystem.h "+_
	"groupentry.h "+_
	"hostentry.h "+_
	"inetsocketclient.h "+_
	"inetsocketserver.h "+_
	"listener.h "+_
	"logger.h "+_
	"memorymap.h "+_
	"memorypool.h "+_
	"thread.h "+_
	"threadmutex.h"
headers2="dictionary.h "+_
	"hash.h "+_
	"sha1.h "+_
	"sha256.h "+_
	"md5.h "+_
	"encryption.h "+_
	"aes128.h "+_
	"parameterstring.h "+_
	"userentry.h "+_
	"permissions.h "+_
	"process.h "+_
	"protocolentry.h "+_
	"randomnumber.h "+_
	"bytestring.h "+_
	"regularexpression.h "+_
	"semaphoreset.h "+_
	"server.h "+_
	"serviceentry.h "+_
	"sharedmemory.h "+_
	"signalclasses.h "+_
	"snooze.h "+_
	"stringbuffer.h "+_
	"wstringbuffer.h "+_
	"sys.h"
headers3="dynamicarray.h "+_
	"staticarray.h "+_
	"output.h "+_
	"input.h "+_
	"wastebasket.h "+_
	"sensitivevalue.h "+_
	"socketlayer.h "+_
	"securitycontext.h "+_
	"gss.h "+_
	"tls.h "+_
	"websocket.h "+_
	"unixsocketclient.h "+_
	"unixsocketserver.h "+_
	"bytebuffer.h "+_
	"dom.h "+_
	"domnode.h "+_
	"xmldom.h "+_
	"sax.h "+_
	"xmlsax.h "+_
	"domevents.h "+_
	"codetree.h "+_
	"compiler.h "+_
	"inetsocketutil.h "+_
	"unixsocketutil.h "+_
	"socketserver.h "+_
	"socketclient.h "+_
	"prompt.h "+_
	"templateengine.h "+_
	"csvsax.h "+_
	"csvdom.h "+_
	"jsonsax.h "+_
	"jsondom.h "+_
	"inisax.h "+_
	"inidom.h "+_
	"propsax.h "+_
	"propdom.h "+_
	"httprequest.h "+_
	"urlhttprequest.h "+_
	"httpresponse.h "+_
	"apachehttpserverapi.h "+_
	"cgihttpserverapi.h "+_
	"httpserverapi.h "+_
	"httpserverapimain.h "+_
	"mvc.h "+_
	"cronschedule.h "+_
	"resourcepool.h "+_
	"locale.h "

privateheaders0="config.h "+_
	"debugprint.h "+_
	"objectincludes.h "+_
	"object.h "+_
	"collection.h "+_
	"collectioninlines.h "+_
	"comparator.h "+_
	"comparatorinlines.h "+_
	"listnode.h "+_
	"listcollectioninlines.h "+_
	"treenode.h "+_
	"treecollection.h "+_
	"treecollectioninlines.h "+_
	"arraycollectioninlines.h "+_
	"dictionarycollectioninlines.h "+_
	"scalarcollectioninlines.h"
privateheaders1="inttypes.h "+_
	"new.h "+_
	"characterincludes.h "+_
	"wcharacterincludes.h "+_
	"ucs2characterincludes.h "+_
	"utf8characterincludes.h "+_
	"utf16characterincludes.h "+_
	"charstring.h "+_
	"charstringincludes.h "+_
	"wcharstring.h "+_
	"wcharstringincludes.h "+_
	"ucs2charstring.h "+_
	"ucs2charstringincludes.h "+_
	"utf8charstring.h "+_
	"utf8charstringincludes.h "+_
	"utf16charstring.h "+_
	"utf16charstringincludes.h "+_
	"iconvert.h "+_
	"iconvertincludes.h "+_
	"chat.h "+_
	"chatincludes.h "+_
	"client.h "+_
	"clientincludes.h "+_
	"commandline.h "+_
	"commandlineincludes.h "+_
	"des.h "+_
	"desincludes.h "+_
	"datetime.h "+_
	"datetimeincludes.h "+_
	"device.h "+_
	"deviceincludes.h "+_
	"directory.h "+_
	"directoryincludes.h"
privateheaders2="winsock.h "+_
	"dll.h "+_
	"dynamiclib.h "+_
	"dynamiclibincludes.h "+_
	"environment.h "+_
	"environmentincludes.h "+_
	"errorincludes.h "+_
	"file.h "+_
	"fileincludes.h "+_
	"url.h "+_
	"urlincludes.h "+_
	"filedescriptor.h "+_
	"filedescriptorincludes.h "+_
	"filedescriptorinlines.h "+_
	"stdio.h "+_
	"filesystem.h "+_
	"filesystemincludes.h "+_
	"groupentry.h "+_
	"groupentryincludes.h "+_
	"hostentry.h "+_
	"hostentryincludes.h "+_
	"inetsocketclient.h "+_
	"inetsocketclientincludes.h "+_
	"inetsocketserver.h "+_
	"inetsocketserverincludes.h"
privateheaders3="tablecollection.h "+_
	"tablecollectioninlines.h "+_
	"table.h "+_
	"tableincludes.h "+_
	"tableinlines.h "+_
	"linkedlist.h "+_
	"linkedlistincludes.h "+_
	"linkedlistinlines.h "+_
	"linkedlistnode.h "+_
	"nodeinlines.h "+_
	"avltree.h "+_
	"avltreeincludes.h "+_
	"avltreeinlines.h "+_
	"avltreenode.h "+_
	"scalar.h "+_
	"scalarincludes.h "+_
	"scalarinlines.h "+_
	"listener.h "+_
	"listenerincludes.h "+_
	"filedestination.h "+_
	"logdestination.h "+_
	"logger.h "+_
	"stderrdestination.h "+_
	"stdoutdestination.h "+_
	"syslogdestination.h "+_
	"loggerincludes.h "+_
	"memorymap.h "+_
	"memorymapincludes.h "+_
	"memorypool.h "+_
	"memorypoolincludes.h "+_
	"thread.h "+_
	"threadincludes.h "+_
	"threadmutex.h "+_
	"threadmutexincludes.h"
privateheaders4="singlylinkedlist.h "+_
	"singlylinkedlistincludes.h "+_
	"singlylinkedlistinlines.h "+_
	"singlylinkedlistnode.h "+_
	"parameterstring.h "+_
	"parameterstringincludes.h "+_
	"userentry.h "+_
	"userentryincludes.h "+_
	"permissions.h "+_
	"permissionsincludes.h "+_
	"process.h "+_
	"processincludes.h "+_
	"protocolentry.h "+_
	"protocolentryincludes.h "+_
	"randomnumber.h "+_
	"randomnumberincludes.h "+_
	"bytestringincludes.h "+_
	"regularexpression.h "+_
	"regularexpressionincludes.h "+_
	"semaphoreset.h "+_
	"semaphoresetincludes.h"
privateheaders5="dictionary.h "+_
	"dictionaryincludes.h "+_
	"dictionaryinlines.h "+_
	"hash.h "+_
	"hashincludes.h "+_
	"encryption.h "+_
	"encryptionincludes.h "+_
	"sha1.h "+_
	"sha1includes.h "+_
	"sha256.h "+_
	"sha256includes.h "+_
	"md5.h "+_
	"md5includes.h "+_
	"aes128.h "+_
	"aes128includes.h "+_
	"server.h "+_
	"serverincludes.h "+_
	"serviceentry.h "+_
	"serviceentryincludes.h "+_
	"sharedmemory.h "+_
	"sharedmemoryincludes.h "+_
	"signalhandler.h "+_
	"signalset.h "+_
	"signalclassesincludes.h "+_
	"snooze.h "+_
	"snoozeincludes.h "+_
	"stringbufferincludes.h "+_
	"stringbufferinlines.h "+_
	"wstringbufferincludes.h "+_
	"wstringbufferinlines.h "+_
	"sys.h "+_
	"sysincludes.h "+_
	"sysinlines.h"
privateheaders6="dynamicarray.h "+_
	"dynamicarrayincludes.h "+_
	"dynamicarrayinlines.h "+_
	"unixsocketclient.h "+_
	"unixsocketclientincludes.h "+_
	"unixsocketserver.h "+_
	"unixsocketserverincludes.h "+_
	"bytebuffer.h "+_
	"bytebufferincludes.h "+_
	"dom.h "+_
	"domincludes.h "+_
	"domnode.h "+_
	"domnodeincludes.h "+_
	"xmldom.h "+_
	"xmldomincludes.h "+_
	"sax.h "+_
	"saxincludes.h "+_
	"xmlsax.h "+_
	"xmlsaxincludes.h "+_
	"domevents.h "+_
	"domeventsincludes.h "+_
	"codetree.h "+_
	"codetreeincludes.h "+_
	"codetreegrammar.h "+_
	"compiler.h "+_
	"compilerincludes.h "+_
	"inetsocketutil.h "+_
	"inetsocketutilincludes.h "+_
	"unixsocketutil.h "+_
	"unixsocketutilincludes.h "+_
	"socketserver.h "+_
	"socketserverincludes.h "+_
	"socketclient.h "+_
	"socketclientincludes.h "+_
	"prompt.h "+_
	"promptincludes.h"
privateheaders7="staticarray.h "+_
	"staticarrayinlines.h "+_
	"outputincludes.h "+_
	"output.h "+_
	"outputinlines.h "+_
	"input.h "+_
	"inputincludes.h "+_
	"inputinlines.h "+_
	"wastebasketincludes.h "+_
	"wastebasket.h "+_
	"sensitivevalueincludes.h "+_
	"sensitivevalue.h "+_
	"socketlayerincludes.h "+_
	"socketlayer.h "+_
	"gss.h "+_
	"gssmechanism.h "+_
	"gsscredentials.h "+_
	"gsscontext.h "+_
	"gssincludes.h "+_
	"tls.h "+_
	"tlscontext.h "+_
	"tlscertificate.h "+_
	"tlsincludes.h "+_
	"websocket.h "+_
	"websocketincludes.h "+_
	"templateengine.h "+_
	"templateengineincludes.h "+_
	"csvsax.h "+_
	"csvsaxincludes.h "+_
	"csvdom.h "+_
	"csvdomincludes.h "+_
	"jsonsax.h "+_
	"jsonsaxincludes.h "+_
	"jsondom.h "+_
	"jsondomincludes.h "+_
	"inisax.h "+_
	"inisaxincludes.h "+_
	"inidom.h "+_
	"inidomincludes.h "+_
	"propsax.h "+_
	"propsaxincludes.h "+_
	"propdom.h "+_
	"propdomincludes.h "+_
	"httprequest.h "+_
	"httprequestincludes.h "+_
	"urlhttprequest.h "+_
	"urlhttprequestincludes.h "+_
	"httpresponse.h "+_
	"httpresponseincludes.h "+_
	"apachehttpserverapi.h "+_
	"apachehttpserverapiincludes.h "+_
	"apacheincludes.h "+_
	"apachemodule.h "+_
	"cgihttpserverapi.h "+_
	"cgihttpserverapiincludes.h "+_
	"cgimodule.h "+_
	"httpserverapi.h "+_
	"httpserverapiincludes.h "+_
	"mvcincludes.h "+_
	"mvcsecurity.h "+_
	"mvcproperties.h "+_
	"mvcresult.h "+_
	"mvctier.h "+_
	"mvccontroller.h "+_
	"mvcservice.h "+_
	"mvcdao.h "+_
	"mvcview.h "+_
	"mvccrudinlines.h "+_
	"httpbasicsecurity.h "+_
	"cronscheduleincludes.h "+_
	"cronschedule.h "+_
	"resourcepool.h "+_
	"resourcepoolincludes.h "+_
	"resourcepoolinlines.h "+_
	"localeincludes.h "+_
	"locale.h "

' version
RUDIMENTS_VERSION="2.0.1"

' paths
pfix="C:\\Program Files\\Firstworks"
prefix=pfix
exec_prefix=pfix
bindir=pfix+"\\bin"
includedir=pfix+"\\include"
libdir=pfix+"\\lib"
mandir=pfix+"\\share\\man"
datadir=pfix+"\\share"
localstatedir=pfix+"\\var"
tmpdir=localstatedir+"\\run\\rudiments"

' libraries
SOCKETLIBS=""
PTHREADLIB=""
SSLLIBS=""
SERVERPEM=""
GSSINCLUDES=""
GSSLIBS="secur32.lib crypt32.lib" 
PCRELIBS=""
'LIBCURLINCLUDES="/I""C:\Program Files\cURL\include"""
'LIBCURLLIBS="/LIBPATH:""C:\Program Files\cURL\lib"" libcurl.lib"
LIBCURLINCLUDES=""
LIBCURLLIBS=""
NANOSLEEPLIB=""
CLOCKNANOSLEEPLIB=""
CRYPTLIB=""
INETATONLIB=""
NETAPI32LIB="ws2_32.lib netapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib uuid.lib dbghelp.lib psapi.lib"
BELIB=""
GNULIB=""
CRTLIB=""
DLLIB=""
MASLIB=""
GETPGIDLIB=""

' extension
EXE=".exe"

' create file system object
set fso=CreateObject("Scripting.FileSystemObject")


' get top_builddir
top_builddir=chr(34) & fso.GetAbsolutePathName(".") & chr(34)


' determine VC++ version and architecture
set WshShell=WScript.CreateObject("WScript.Shell")
set cmd=WshShell.exec("cl")
stdout=cmd.StdOut.ReadAll()
stderr=cmd.StdErr.ReadLine()
parts=split(stderr)
arch=parts(ubound(parts))
if arch="80x86" then
	arch="x86"
end if
version=parts(ubound(parts)-2)
parts=split(version,".")
version=parts(0)

' determine VC++ architecture
USE_32BIT_TIME_T=""
if arch="x86" then
	USE_32BIT_TIME_T="/D _USE_32BIT_TIME_T"
end if



' determine OS Version number
if len(hexversion)=0 then
	set cmd=WshShell.exec("%comspec% /c ver")
	stdout=cmd.StdOut.ReadAll()
	stderr=cmd.StdErr.ReadLine()
	if instr(stdout,"Windows NT Version 4.0")>0 then
		hexversion="0x0400"
	else
		parts0=split(stdout,"[")
		parts1=split(parts0(1)," ")
		parts2=split(parts1(1),"]")
		parts3=split(parts2(0),".")
		if parts3(1)="00" then
			parts3(1)="0"
		end if
		hexversion="0x0"&parts3(0)&"0"&parts3(1)
	end if
end if

' in general, we need to set WIN32WINNT to the hexversion
WINVER=""
WIN32WINDOWS=""
WIN32WINNT=hexversion

' but, for OS'es older than WinXP we have to do some special things...

' for Win2k and WinNT4, set WINVER also
if hexversion="0x0500" or hexversion="0x0400" then
	WINVER=hexversion

' for WinME, set WIN32WINDOWS and unset WIN32WINNT
elseif hexversion="0x0490" then
	WIN32WINDOWS=hexversion
	WIN32WINNT=""

' for Win98, set WIN32WINDOWS and WINVER and unset WIN32WINNT
elseif hexversion="0x0410" then
	WIN32WINDOWS=hexversion
	WINVER=hexversion
	WIN32WINNT=""

' for Win95, set WINVER and unset WIN32WINNT
elseif hexversion="0x0400" then
	WINVER=hexversion
	WIN32WINNT=""

' FIXME: not sure about WinNT3X, Win3X or below
end if

' add /D and macro name
if WINVER<>"" then
	WINVER="/D WINVER="&WINVER
end if
if WIN32WINDOWS<>"" then
	WIN32WINDOWS="/D _WIN32_WINDOWS="&WIN32WINDOWS
end if
if WIN32WINNT<>"" then
	WIN32WINNT="/D _WIN32_WINNT="&WIN32WINNT
end if



' determine config.h template...

' default to VS2017
configwindowsh="include\\rudiments\\private\\config_vs2017.h"


' VS2015
if version=19 then
	configwindowsh="include\\rudiments\\private\\config_vs2015.h"

' VS2013
elseif version=18 then
	configwindowsh="include\\rudiments\\private\\config_vs2013.h"

' VS2010 and VS2012
elseif version<=17 and version>15 then
	configwindowsh="include\\rudiments\\private\\config_vs2010.h"

' VS2005 and VS2008
elseif version<=15 and version>13 then
	configwindowsh="include\\rudiments\\private\\config_vs2005.h"

' VC6, VS2002 and VS2003 (.NET)
elseif version=13 or version=12 then
	configwindowsh="include\\rudiments\\private\\config_vs6.h"

' VS5 and lower
elseif version<=11 then
	configwindowsh="include\\rudiments\\private\\config_vs5.h"
end if



' determine SDK headers and libs... (FIXME: make this configurable)

' VS2002, VS2003 and VS2008 and up come with a platform SDK
SDKINCLUDES=""
SDKLIBS=""

' VS2005 doesn't come with an SDK and there are several that are compatible
if version=14 then

	' older SDK's have various issues

	' 5.2.3700.0 - Microsoft Platform SDK February 2003
	'SDKINCLUDES="/I""C:\Program Files\Microsoft SDK\include"""
	'SDKLIBS="/LIBPATH:""C:\Program Files\Microsoft SDK\Lib"""

	' 5.2.3790.1830.15 - Windows Server 2003 SP1 Platform SDK
	SDKINCLUDES="/I""C:\Program Files\Microsoft Platform SDK\Include"""
	SDKLIBS="/LIBPATH:""C:\Program Files\Microsoft Platform SDK\Lib"""

	' 5.2.3790.2075.51 - Windows Server 2003 R2 Platform SDK
	'SDKINCLUDES="/I""C:\Program Files\Microsoft Platform SDK for Windows Server 2003 R2\Include"""
	'SDKLIBS="/LIBPATH:""C:\Program Files\Microsoft Platform SDK for Windows Server 2003 R2\Lib"""

	' 6.0A (comes with VC2008)
	'SDKINCLUDES="/I""C:\Program Files\Microsoft SDKs\Windows\v6.0A\Include"""
	'SDKLIBS="/LIBPATH:""C:\Program Files\Microsoft SDKs\Windows\v6.0A\Lib"""

	' not sure about newer SDK's

' VS6 doesn't come with a platform SDK
elseif version=12 then

	' older SDK's might work too

	' 5.2.3700.0 - Microsoft Platform SDK February 2003
	SDKINCLUDES="/I""C:\Program Files\Microsoft SDK\include"""
	SDKLIBS="/LIBPATH:""C:\Program Files\Microsoft SDK\Lib"""

	' not sure about newer SDK's

' VS5 and lower don't come with a platform SDK
elseif version<=11 then

	' older SDK's might work too

	' 5.1.2600.2180 - Microsoft Platform SDK for Windows XP SP2
	' (this doesn't actually work)
	'SDKINCLUDES="/I""C:\Program Files\Microsoft Platform SDK for Windows XP SP2\Include"""
	'SDKLIBS="/LIBPATH:""C:\Program Files\Microsoft Platform SDK for Windows XP SP2\Lib"""

	' newer SDK's give link errors

end if



' input and output files
infiles=Array(_
	"config_windows.mk",_
	configwindowsh,_
	"src\\Makefile.in",_
	"include\\Makefile.in",_
	"bin\\rudiments-config.in",_
	"rudiments.pc.in",_
	"msvc\\setupx64\\setupx64.vdproj.in",_
	"msvc\\setupx86\\setupx86.vdproj.in"_
	)
outfiles=Array(_
	"config.mk",_
	"include\\rudiments\\private\\config.h",_
	"src\\Makefile",_
	"include\\Makefile",_
	"bin\\rudiments-config",_
	"rudiments.pc",_
	"msvc\\setupx64\\setupx64.vdproj",_
	"msvc\\setupx86\\setupx86.vdproj"_
	)


' create output files
for i=lbound(infiles) to ubound(infiles)

	' read input file
	set infile=fso.OpenTextFile(infiles(i))
	content=infile.ReadAll()

	' source, object and header files
	content=replace(content,"@SRCS@",srcs,1,-1,0)
	content=replace(content,"@LOBJS@",lobjs,1,-1,0)
	content=replace(content,"@HEADERS0@",headers0,1,-1,0)
	content=replace(content,"@HEADERS1@",headers1,1,-1,0)
	content=replace(content,"@HEADERS2@",headers2,1,-1,0)
	content=replace(content,"@HEADERS3@",headers3,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS0@",privateheaders0,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS1@",privateheaders1,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS2@",privateheaders2,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS3@",privateheaders3,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS4@",privateheaders4,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS5@",privateheaders5,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS6@",privateheaders6,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS7@",privateheaders7,1,-1,0)

	' version
	content=replace(content,"@RUDIMENTS_VERSION@",RUDIMENTS_VERSION,1,-1,0)

	' paths
	content=replace(content,"@prefix@",prefix,1,-1,0)
	content=replace(content,"@exec_prefix@",exec_prefix,1,-1,0)
	content=replace(content,"@bindir@",bindir,1,-1,0)
	content=replace(content,"@includedir@",includedir,1,-1,0)
	content=replace(content,"@libdir@",libdir,1,-1,0)
	content=replace(content,"@mandir@",mandir,1,-1,0)
	content=replace(content,"@datadir@",datadir,1,-1,0)
	content=replace(content,"@localstatedir@",localstatedir,1,-1,0)
	content=replace(content,"@tmpdir@",tmpdir,1,-1,0)

	' flags
	content=replace(content,"@OPTCPPFLAGS@",OPTCPPFLAGS,1,-1,0)
	content=replace(content,"@DEBUGCPPFLAGS@",DEBUGCPPFLAGS,1,-1,0)
	content=replace(content,"@DEBUGLDFLAGS@",DEBUGLDFLAGS,1,-1,0)
	content=replace(content,"@_USE_32BIT_TIME_T@",USE_32BIT_TIME_T,1,-1,0)
	content=replace(content,"@SDKINCLUDES@",SDKINCLUDES,1,-1,0)
	content=replace(content,"@WINVER@",WINVER,1,-1,0)
	content=replace(content,"@WIN32WINDOWS@",WIN32WINDOWS,1,-1,0)
	content=replace(content,"@WIN32WINNT@",WIN32WINNT,1,-1,0)

	' libraries
	content=replace(content,"@SOCKETLIBS@",SOCKETLIBS,1,-1,0)
	content=replace(content,"@PTHREADLIB@",PTHREADLIB,1,-1,0)
	content=replace(content,"@SSLLIBS@",SSLLIBS,1,-1,0)
	content=replace(content,"@SERVERPEM@",SERVERPEM,1,-1,0)
	content=replace(content,"@GSSINCLUDES@",GSSINCLUDES,1,-1,0)
	content=replace(content,"@GSSLIBS@",GSSLIBS,1,-1,0)
	content=replace(content,"@PCRELIBS@",PCRELIBS,1,-1,0)
	content=replace(content,"@LIBCURLINCLUDES@",LIBCURLINCLUDES,1,-1,0)
	content=replace(content,"@LIBCURLLIBS@",LIBCURLLIBS,1,-1,0)
	content=replace(content,"@NANOSLEEPLIB@",NANOSLEEPLIB,1,-1,0)
	content=replace(content,"@CLOCKNANOSLEEPLIB@",CLOCKNANOSLEEPLIB,1,-1,0)
	content=replace(content,"@CRYPTLIB@",CRYPTLIB,1,-1,0)
	content=replace(content,"@INETATONLIB@",INETATONLIB,1,-1,0)
	content=replace(content,"@NETAPI32LIB@",NETAPI32LIB,1,-1,0)
	content=replace(content,"@BELIB@",BELIB,1,-1,0)
	content=replace(content,"@GNULIB@",GNULIB,1,-1,0)
	content=replace(content,"@CRTLIB@",CRTLIB,1,-1,0)
	content=replace(content,"@DLLIB@",DLLIB,1,-1,0)
	content=replace(content,"@MASLIB@",MASLIB,1,-1,0)
	content=replace(content,"@GETPGIDLIB@",GETPGIDLIB,1,-1,0)
	content=replace(content,"@SDKLIBS@",SDKLIBS,1,-1,0)
	content=replace(content,"@APACHEINCLUDES@","",1,-1,0)
	content=replace(content,"@APACHESRCS@","",1,-1,0)
	content=replace(content,"@APACHELOBJS@","",1,-1,0)
	content=replace(content,"@LIBRUDIMENTSAPACHE@","",1,-1,0)
	content=replace(content,"@INSTALLLIBAPACHE@","",1,-1,0)
	content=replace(content,"@MAKELIBRUDIMENTSAPACHE@","",1,-1,0)

	' extension
	content=replace(content,"@EXE@",EXE,1,-1,0)


	' top_builddir
	content=replace(content,"@top_builddir@",top_builddir,1,-1,0)

	' write output file
	set outfile=fso.OpenTextFile(outfiles(i),2,true)
	call outfile.Write(content)
	call outfile.Close()
next
