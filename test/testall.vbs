' run with cscript
if instr(lcase(WScript.FullName),"cscript")=0 Then
	WScript.Echo("Please run:  cscript /nologo testall.vbs")
	WScript.Quit
end if

tests=Array(
	"aes128",_
	"avltree",_
	"bytebuffer",_
	"bytestring",_
	"character",_
	"charstring",_
	"commandline",_
	"crypt",_
	"csvdom",_
	"datetime",_
	"dictionary",_
	"directory",_
	"dynamicarray",_
	"dynamiclib",_
	"environment",_
	"error",_
	"file",_
	"groupentry",_
	"handoff",_
	"hostentry",_
	"inidom",_
	"jsondom",_
	"linkedlist",_
	"listener",_
	"logger",_
	"md5sum",_
	"memorymap",_
	"memorypool",_
	"mvcproperties",_
	"parameterstring",_
	"permissions",_
	"process",_
	"propdom",_
	"protocolentry",_
	"randomnumber",_
	"regularexpression",_
	"semaphoreset",_
	"serviceentry",_
	"sharedmemory",_
	"signal",_
	"singlylinkedlist",_
	"snooze",_
	"staticarray",_
	"stringbuffer",_
	"sys",_
	"thread",_
	"threadmutex",_
	"url",_
	"userentry",_
	"xmldom"_
)

' create shell object
set WshShell=WScript.CreateObject("WScript.Shell")

for each test in tests

	WScript.Echo(test)

	'set cmd=WshShell.Exec(test)
	'do until cmd.StdOut.AtEndOfStream
		'WScript.Echo(cmd.StdOut.Readline())
	'loop

	'if cmd.ExitCode=1 then
		'WScript.Quit
	'end if
next
