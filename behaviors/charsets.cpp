#include <rudiments/wcharstring.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>

#include <wchar.h>
#include <stdio.h>
#include <locale.h>

#include "../test/test.cpp"

int myprintf(FILE *f, const wchar_t *format, ...) {
	va_list	args;
	va_start(args,format);
	size_t	result=vfwprintf(f,format,args);
	va_end(args);
	return result;
}

int main(int argc, char **argv) {

	// wchar_t size on this platform
	header("wchar_t size");
	stdoutput.printf("  sizeof(wchar_t): %d\n",sizeof(wchar_t));
	stdoutput.printf("\n");

	// define some locales
	const char *locales[]={
		"",
		"en_US.ISO-8859-15",
		"en_US.UTF-8",
		NULL
	};

	// define some text strings
	const wchar_t	*hellotxt=L"Hello!";
	const wchar_t	hellohex[]={
		0x00000048, 0x00000065, 0x0000006c, 0x0000006c, 
		0x0000006f, 0x00000021, 0x00000000
	};
	const wchar_t	*chairetetxt=L"Χαίρετε!";
	const wchar_t	chairetehex[]={
		0x000003a7, 0x000003b1, 0x000003af, 0x000003c1, 
		0x000003b5, 0x000003c4, 0x000003b5, 0x00000021, 
		0x00000000
	};
	const wchar_t	*privettxt=L"Привет!";
	const wchar_t	privethex[]={
		0x0000041f, 0x00000440, 0x00000438, 0x00000432,
		0x00000435, 0x00000442, 0x00000021, 0x00000000
	};
	const wchar_t	*texts[]={
		hellotxt,
		hellohex,
		chairetetxt,
		chairetehex,
		privettxt,
		privethex,
		NULL
	};
	const char	*descriptions[]={
		"\"Hello!\" in english (UCS-2/4 encoded text)",
		"\"Hello!\" in english (UCS-2/4 hex)",
		"\"Chairete!\" in greek (UCS-2/4 encoded text)",
		"\"Chairete!\" in greek (UCS-2/4 hex)",
		"\"Privet!\" in cyrillic (UCS-2/4 encoded text)",
		"\"Privet!\" in cyrillic (UCS-2/4 hex)",
		NULL
	};

	// iterate over locales
	for (const char **locale=locales; *locale; locale++) {

		// set locale
		if (!charstring::isNullOrEmpty(*locale)) {
			setlocale(LC_CTYPE,*locale);
			header(*locale);
		} else {
			header("initial locale");
		}

		// display locale
		stdoutput.printf("  requested locale: %s\n",*locale);
		stdoutput.printf("  actual locale:    %s\n",
						setlocale(LC_CTYPE,NULL));
		stdoutput.printf("\n");


		// display characters 32-255
		stdoutput.printf("  characters:\n");
		for (uint16_t i=32; i<256; i++) {
			stdoutput.printf("%c  ",i);
			if (!((i-31)%25)) {
				stdoutput.printf("\n");
			}
		}
		stdoutput.printf("\n\n");


		// iterate over text strings
		uint16_t	counter=0;
		const wchar_t	**text=texts;
		const char	**description=descriptions;
		while (*text && *description) {

			// display description of the text
			stdoutput.printf("%s\n",*description);

#if 0
			// display the text as hex
			stdoutput.printf("  hex:\n");
			uint16_t	hexcounter=0;
			uint16_t	nullcounter=0;
			const unsigned char	*t=(const unsigned char *)*text;
			for (;;) {
				stdoutput.printf("%02x ",*t);
				if (!*t) {
					nullcounter++;
					if (nullcounter==sizeof(wchar_t)) {
						break;
					}
				}
				hexcounter++;
				if (!(hexcounter%sizeof(wchar_t))) {
					stdoutput.printf(" ");
					nullcounter=0;
				}
				if (!(hexcounter%16)) {
					stdoutput.printf("\n");
				}
				t++;
			}
			stdoutput.printf("\n");

			// display the text as text, 3 different ways
			stdoutput.printf("  write:     ");
			stdoutput.write(*text);
			stdoutput.printf("\n");
			stdoutput.printf("  vfprintf:  ");
			stdoutput.printf("%ls",*text);
			stdoutput.printf("\n");
			stdoutput.printf("  vfwprintf: ");
			stdoutput.printf(L"%ls",*text);
			stdoutput.printf("\n");
			stdoutput.printf("\n");
#else
			printf("  printf:    ");
			printf("%ls",*text);
			printf("\n");
			printf("  wprintf:   ");
			wprintf(L"%ls",*text);
			printf("\n");
			printf("  fwprintf:   ");
			fwprintf(stdout,L"%ls",*text);
			printf("\n");
			printf("  vfwprintf:   ");
			myprintf(stdout,L"%ls",*text);
			printf("\n");
#endif

			// move on
			counter++;
			text++;
			description++;
		}
	}
}
