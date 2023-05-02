// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/wcharacter.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("wcharacter");

	bool	isalphanumeric[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,true,true,
		true,true,true,true,true,true,true,true,false,false,
		false,false,false,false,false,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,false,false,false,false,false,false,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,false,false,false,false,false
	};

	bool	isalphabetical[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,false,false,false,false,false,false,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,false,false,false,false,false
	};

	bool	islowercase[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,false,false,false,false,false
	};

	bool	isuppercase[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false
	};

	bool	ispunctuation[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,false,false,
		false,false,false,false,false,false,false,false,true,true,
		true,true,true,true,true,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,true,true,true,true,true,true,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,true,true,true,true,false
	};

	bool	isprintable[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,false
	};

	bool	isprintablenonspace[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,false
	};

	bool	iscontrolchar[]={
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,true
	};

	bool	isdigit[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,true,true,
		true,true,true,true,true,true,true,true,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false
	};

	bool	ishexdigit[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,true,true,
		true,true,true,true,true,true,true,true,false,false,
		false,false,false,false,false,true,true,true,true,true,
		true,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,true,true,true,
		true,true,true,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false
	};

	bool	isblank[]={
		false,false,false,false,false,false,false,false,false,true,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,true,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false
	};

	bool	iswhitespace[]={
		false,false,false,false,false,false,false,false,false,true,
		true,true,true,true,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,true,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false
	};

	bool	isascii[]={
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true
	};

	wchar_t	touppercase[]={
		0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16,17,18,19,
		20,21,22,23,24,25,26,27,28,29,
		30,31,32,33,34,35,36,37,38,39,
		40,41,42,43,44,45,46,47,48,49,
		50,51,52,53,54,55,56,57,58,59,
		60,61,62,63,64,65,66,67,68,69,
		70,71,72,73,74,75,76,77,78,79,
		80,81,82,83,84,85,86,87,88,89,
		90,91,92,93,94,95,96,L'A',L'B',L'C',
		L'D',L'E',L'F',L'G',L'H',L'I',L'J',L'K',L'L',L'M',
		L'N',L'O',L'P',L'Q',L'R',L'S',L'T',L'U',L'V',L'W',
		L'X',L'Y',L'Z',123,124,125,126,127
	};

	wchar_t	tolowercase[]={
		0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16,17,18,19,
		20,21,22,23,24,25,26,27,28,29,
		30,31,32,33,34,35,36,37,38,39,
		40,41,42,43,44,45,46,47,48,49,
		50,51,52,53,54,55,56,57,58,59,
		60,61,62,63,64,L'a',L'b',L'c',L'd',L'e',
		L'f',L'g',L'h',L'i',L'j',L'k',L'l',L'm',L'n',L'o',
		L'p',L'q',L'r',L's',L't',L'u',L'v',L'w',L'x',L'y',
		L'z',91,92,93,94,95,96,97,98,99,
		100,101,102,103,104,105,106,107,108,109,
		110,111,112,113,114,115,116,117,118,119,
		120,121,122,123,124,125,126,127
	};

	wchar_t	toascii[]={
		0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16,17,18,19,
		20,21,22,23,24,25,26,27,28,29,
		30,31,32,33,34,35,36,37,38,39,
		40,41,42,43,44,45,46,47,48,49,
		50,51,52,53,54,55,56,57,58,59,
		60,61,62,63,64,65,66,67,68,69,
		70,71,72,73,74,75,76,77,78,79,
		80,81,82,83,84,85,86,87,88,89,
		90,91,92,93,94,95,96,97,98,99,
		100,101,102,103,104,105,106,107,108,109,
		110,111,112,113,114,115,116,117,118,119,
		120,121,122,123,124,125,126,127
	};

	for (byte_t c=0; c<128; c++) {

		stdoutput.printf("\t0x%02x \"%c\":\n",c,c);
		test("\tisAlphanumeric",
			wcharacter::isAlphanumeric(c)==isalphanumeric[c]);
		test("\tisAlphabetical",
			wcharacter::isAlphabetical(c)==isalphabetical[c]);
		test("\tisLowerase",
			wcharacter::isLowerCase(c)==islowercase[c]);
		test("\tisUpperCase",
			wcharacter::isUpperCase(c)==isuppercase[c]);
		test("\tisPunctuation",
			wcharacter::isPunctuation(c)==ispunctuation[c]);
		test("\tisPrintable",
			wcharacter::isPrintable(c)==isprintable[c]);
		test("\tisPrintableNonSpace",
			wcharacter::isPrintableNonSpace(c)==
					isprintablenonspace[c]);
		test("\tisControlCharacter",
			wcharacter::isControlCharacter(c)==iscontrolchar[c]);
		test("\tisDigit",
			wcharacter::isDigit(c)==isdigit[c]);
		test("\tisHexDigit",
			wcharacter::isHexDigit(c)==ishexdigit[c]);
		test("\tisBlank",
			wcharacter::isBlank(c)==isblank[c]);
		test("\tisWhitespace",
			wcharacter::isWhitespace(c)==iswhitespace[c]);
		test("\tisAscii",
			wcharacter::isAscii(c)==isascii[c]);
		test("\tupper",
			wcharacter::upper(c)==touppercase[c]);
		test("\tlower",
			wcharacter::lower(c)==tolowercase[c]);
		test("\tconvertToAscii",
			wcharacter::convertToAscii(c)==toascii[c]);
		test("\tduplicate",
			wcharacter::duplicate((char)c)==(wchar_t)c);
	}

	return 0;
}
