' run with cscript
if instr(lcase(WScript.FullName),"cscript")=0 Then
	WScript.Echo("Please run:  cscript /nologo testall.vbs")
	WScript.Quit
end if

tests=Array(_
	"aes128",_
	"aes192",_
	"aes256",_
	"arc4",_
	"avltree",_
	"bignumber",_
	"bytebuffer",_
	"bytestring",_
	"character",_
	"charstring",_
	"collectionprint",_
	"commandline",_
	"csvdom",_
	"csvsax",_
	"datetime",_
	"des",_
	"dh",_
	"dictionary",_
	"directory",_
	"domevents",_
	"domnode",_
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
	"md5",_
	"memorymap",_
	"memorypool",_
	"mvcproperties",_
	"mvcresult",_
	"parameterstring",_
	"pbkdf2",_
	"permissions",_
	"process",_
	"propdom",_
	"protocolentry",_
	"prng",_
	"csprng",_
	"regularexpression",_
	"resourcepool",_
	"scalar",_
	"semaphoreset",_
	"sensitivevalue",_
	"serviceentry",_
	"sha1",_
	"sha256",_
	"sha512",_
	"sharedmemory",_
	"signal",_
	"singledes",_
	"singlylinkedlist",_
	"snooze",_
	"staticarray",_
	"stringbuffer",_
	"sys",_
	"table",_
	"thread",_
	"threadmutex",_
	"tripledes",_
	"ucs2character",_
	"ucs2charstring",_
	"ucs2stringbuffer",_
	"url",_
	"urlhttprequest",_
	"userentry",_
	"wastebasket",_
	"wcharacter",_
	"wcharstring",_
	"wstringbuffer",_
	"xmldom"_
)

' create shell object
set WshShell=WScript.CreateObject("WScript.Shell")

' put ..\src on the path, for librudiments.dll
set fso=WScript.CreateObject("Scripting.FileSystemObject")
srcdir=fso.GetParentFolderName(fso.GetParentFolderName(_
				WScript.ScriptFullName))&"\src"
set env=WshShell.Environment("PROCESS")
env("PATH")=srcdir&";"&env("PATH")

for each test in tests

	' Stderr is merged into stdout so that a full stderr pipe cannot
	' hang the test while the script reads stdout.
	set cmd=WshShell.Exec("cmd /c "&test&" 2>&1")
	' close stdin so a test that reads it sees end of file
	cmd.StdIn.Close
	do until cmd.StdOut.AtEndOfStream
		WScript.Echo(cmd.StdOut.Readline())
	loop
	do while cmd.Status=0
		WScript.Sleep 10
	loop

	' stop on any failure, including a test that failed to start
	if cmd.ExitCode<>0 then
		WScript.Echo("FAILED: "&test&" exit "&cmd.ExitCode)
		WScript.Quit 1
	end if
next
