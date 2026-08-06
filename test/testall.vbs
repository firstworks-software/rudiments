' run with cscript
if instr(lcase(WScript.FullName),"cscript")=0 Then
	WScript.Echo("Please run:  cscript /nologo testall.vbs")
	WScript.Quit
end if

tests=Array(_
	"aes128",_
	"character",_
	"charstring",_
	"commandline",_
	"csvdom",_
	"datetime",_
	"des",_
	"dh",_
	"dictionary",_
	"directory",_
	"domevents",_
	"dynamicarray",_
	"dynamiclib",_
	"environment",_
	"error",_
	"file",_
	"filesystem",_
	"groupentry",_
	"handoff",_
	"hostentry",_
	"iconvert",_
	"inidom",_
	"jsondom",_
	"linkedlist",_
	"listener",_
	"locale",_
	"logger",_
	"md5sum",_
	"memorymap",_
	"memorypool",_
	"mvcproperties",_
	"parameterstring",_
	"pbkdf2",_
	"permissions",_
	"process",_
	"propdom",_
	"protocolentry",_
	"randomnumber",_
	"regularexpression",_
	"resourcepool",_
	"scalar",_
	"semaphoreset",_
	"sensitivevalue",_
	"serviceentry",_
	"sha1",_
	"sharedmemory",_
	"signal",_
	"singlylinkedlist",_
	"snooze",_
	"staticarray",_
	"stringbuffer",_
	"sys",_
	"table",_
	"thread",_
	"threadmutex",_
	"ucs2character",_
	"ucs2charstring",_
	"ucs2stringbuffer",_
	"url",_
	"userentry",_
	"wcharacter",_
	"wcharstring",_
	"wstringbuffer",_
	"xmldom"_
)

' create shell object
set WshShell=WScript.CreateObject("WScript.Shell")

for each test in tests

	set cmd=WshShell.Exec(test)
	do until cmd.StdOut.AtEndOfStream
		WScript.Echo(cmd.StdOut.Readline())
	loop

	if cmd.ExitCode=1 then
		WScript.Quit
	end if
next
