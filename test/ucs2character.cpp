// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/ucs2character.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("ucs2character");

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

	char16_t	touppercase[]={
		(char16_t)0,(char16_t)1,(char16_t)2,(char16_t)3,(char16_t)4,
		(char16_t)5,(char16_t)6,(char16_t)7,(char16_t)8,(char16_t)9,
		(char16_t)10,(char16_t)11,(char16_t)12,(char16_t)13,(char16_t)14,
		(char16_t)15,(char16_t)16,(char16_t)17,(char16_t)18,(char16_t)19,
		(char16_t)20,(char16_t)21,(char16_t)22,(char16_t)23,(char16_t)24,
		(char16_t)25,(char16_t)26,(char16_t)27,(char16_t)28,(char16_t)29,
		(char16_t)30,(char16_t)31,(char16_t)32,(char16_t)33,(char16_t)34,
		(char16_t)35,(char16_t)36,(char16_t)37,(char16_t)38,(char16_t)39,
		(char16_t)40,(char16_t)41,(char16_t)42,(char16_t)43,(char16_t)44,
		(char16_t)45,(char16_t)46,(char16_t)47,(char16_t)48,(char16_t)49,
		(char16_t)50,(char16_t)51,(char16_t)52,(char16_t)53,(char16_t)54,
		(char16_t)55,(char16_t)56,(char16_t)57,(char16_t)58,(char16_t)59,
		(char16_t)60,(char16_t)61,(char16_t)62,(char16_t)63,(char16_t)64,
		(char16_t)65,(char16_t)66,(char16_t)67,(char16_t)68,(char16_t)69,
		(char16_t)70,(char16_t)71,(char16_t)72,(char16_t)73,(char16_t)74,
		(char16_t)75,(char16_t)76,(char16_t)77,(char16_t)78,(char16_t)79,
		(char16_t)80,(char16_t)81,(char16_t)82,(char16_t)83,(char16_t)84,
		(char16_t)85,(char16_t)86,(char16_t)87,(char16_t)88,(char16_t)89,
		(char16_t)90,(char16_t)91,(char16_t)92,(char16_t)93,(char16_t)94,
		(char16_t)95,(char16_t)96,(char16_t)'A',(char16_t)'B',(char16_t)'C',
		(char16_t)'D',(char16_t)'E',(char16_t)'F',(char16_t)'G',(char16_t)'H',
		(char16_t)'I',(char16_t)'J',(char16_t)'K',(char16_t)'L',(char16_t)'M',
		(char16_t)'N',(char16_t)'O',(char16_t)'P',(char16_t)'Q',(char16_t)'R',
		(char16_t)'S',(char16_t)'T',(char16_t)'U',(char16_t)'V',(char16_t)'W',
		(char16_t)'X',(char16_t)'Y',(char16_t)'Z',(char16_t)123,(char16_t)124,
		(char16_t)125,(char16_t)126,(char16_t)127
	};

	char16_t	tolowercase[]={
		(char16_t)0,(char16_t)1,(char16_t)2,(char16_t)3,(char16_t)4,(char16_t)5,
		(char16_t)6,(char16_t)7,(char16_t)8,(char16_t)9,(char16_t)10,(char16_t)11,
		(char16_t)12,(char16_t)13,(char16_t)14,(char16_t)15,(char16_t)16,
		(char16_t)17,(char16_t)18,(char16_t)19,(char16_t)20,(char16_t)21,
		(char16_t)22,(char16_t)23,(char16_t)24,(char16_t)25,(char16_t)26,
		(char16_t)27,(char16_t)28,(char16_t)29,(char16_t)30,(char16_t)31,
		(char16_t)32,(char16_t)33,(char16_t)34,(char16_t)35,(char16_t)36,
		(char16_t)37,(char16_t)38,(char16_t)39,(char16_t)40,(char16_t)41,
		(char16_t)42,(char16_t)43,(char16_t)44,(char16_t)45,(char16_t)46,
		(char16_t)47,(char16_t)48,(char16_t)49,(char16_t)50,(char16_t)51,
		(char16_t)52,(char16_t)53,(char16_t)54,(char16_t)55,(char16_t)56,
		(char16_t)57,(char16_t)58,(char16_t)59,(char16_t)60,(char16_t)61,
		(char16_t)62,(char16_t)63,(char16_t)64,(char16_t)'a',(char16_t)'b',
		(char16_t)'c',(char16_t)'d',(char16_t)'e',(char16_t)'f',(char16_t)'g',
		(char16_t)'h',(char16_t)'i',(char16_t)'j',(char16_t)'k',(char16_t)'l',
		(char16_t)'m',(char16_t)'n',(char16_t)'o',(char16_t)'p',(char16_t)'q',
		(char16_t)'r',(char16_t)'s',(char16_t)'t',(char16_t)'u',(char16_t)'v',
		(char16_t)'w',(char16_t)'x',(char16_t)'y',(char16_t)'z',(char16_t)91,
		(char16_t)92,(char16_t)93,(char16_t)94,(char16_t)95,(char16_t)96,
		(char16_t)97,(char16_t)98,(char16_t)99,(char16_t)100,(char16_t)101,
		(char16_t)102,(char16_t)103,(char16_t)104,(char16_t)105,(char16_t)106,
		(char16_t)107,(char16_t)108,(char16_t)109,(char16_t)110,(char16_t)111,
		(char16_t)112,(char16_t)113,(char16_t)114,(char16_t)115,(char16_t)116,
		(char16_t)117,(char16_t)118,(char16_t)119,(char16_t)120,(char16_t)121,
		(char16_t)122,(char16_t)123,(char16_t)124,(char16_t)125,(char16_t)126,
		(char16_t)127
	};

	char16_t	toascii[]={
		(char16_t)0,(char16_t)1,(char16_t)2,(char16_t)3,(char16_t)4,(char16_t)5,
		(char16_t)6,(char16_t)7,(char16_t)8,(char16_t)9,(char16_t)10,(char16_t)11,
		(char16_t)12,(char16_t)13,(char16_t)14,(char16_t)15,(char16_t)16,
		(char16_t)17,(char16_t)18,(char16_t)19,(char16_t)20,(char16_t)21,
		(char16_t)22,(char16_t)23,(char16_t)24,(char16_t)25,(char16_t)26,
		(char16_t)27,(char16_t)28,(char16_t)29,(char16_t)30,(char16_t)31,
		(char16_t)32,(char16_t)33,(char16_t)34,(char16_t)35,(char16_t)36,
		(char16_t)37,(char16_t)38,(char16_t)39,(char16_t)40,(char16_t)41,
		(char16_t)42,(char16_t)43,(char16_t)44,(char16_t)45,(char16_t)46,
		(char16_t)47,(char16_t)48,(char16_t)49,(char16_t)50,(char16_t)51,
		(char16_t)52,(char16_t)53,(char16_t)54,(char16_t)55,(char16_t)56,
		(char16_t)57,(char16_t)58,(char16_t)59,(char16_t)60,(char16_t)61,
		(char16_t)62,(char16_t)63,(char16_t)64,(char16_t)65,(char16_t)66,
		(char16_t)67,(char16_t)68,(char16_t)69,(char16_t)70,(char16_t)71,
		(char16_t)72,(char16_t)73,(char16_t)74,(char16_t)75,(char16_t)76,
		(char16_t)77,(char16_t)78,(char16_t)79,(char16_t)80,(char16_t)81,
		(char16_t)82,(char16_t)83,(char16_t)84,(char16_t)85,(char16_t)86,
		(char16_t)87,(char16_t)88,(char16_t)89,(char16_t)90,(char16_t)91,
		(char16_t)92,(char16_t)93,(char16_t)94,(char16_t)95,(char16_t)96,
		(char16_t)97,(char16_t)98,(char16_t)99,(char16_t)100,(char16_t)101,
		(char16_t)102,(char16_t)103,(char16_t)104,(char16_t)105,(char16_t)106,
		(char16_t)107,(char16_t)108,(char16_t)109,(char16_t)110,(char16_t)111,
		(char16_t)112,(char16_t)113,(char16_t)114,(char16_t)115,(char16_t)116,
		(char16_t)117,(char16_t)118,(char16_t)119,(char16_t)120,(char16_t)121,
		(char16_t)122,(char16_t)123,(char16_t)124,(char16_t)125,(char16_t)126,
		(char16_t)127
	};

	test("sizeof(char16_t)==2",sizeof(char16_t)==2);
	stdoutput.printf("\n");

	for (byte_t c=0; c<128; c++) {

		stdoutput.printf("\t0x%02x \"%c\":\n",c,c);
		test("\tisAlphanumeric",
			ucs2character::isAlphanumeric(c)==isalphanumeric[c]);
		test("\tisAlphabetical",
			ucs2character::isAlphabetical(c)==isalphabetical[c]);
		test("\tisLowerase",
			ucs2character::isLowerCase(c)==islowercase[c]);
		test("\tisUpperCase",
			ucs2character::isUpperCase(c)==isuppercase[c]);
		test("\tisPunctuation",
			ucs2character::isPunctuation(c)==ispunctuation[c]);
		test("\tisPrintable",
			ucs2character::isPrintable(c)==isprintable[c]);
		test("\tisPrintableNonSpace",
			ucs2character::isPrintableNonSpace(c)==
					isprintablenonspace[c]);
		test("\tisControlCharacter",
			ucs2character::isControlCharacter(c)==iscontrolchar[c]);
		test("\tisDigit",
			ucs2character::isDigit(c)==isdigit[c]);
		test("\tisHexDigit",
			ucs2character::isHexDigit(c)==ishexdigit[c]);
		test("\tisBlank",
			ucs2character::isBlank(c)==isblank[c]);
		test("\tisWhitespace",
			ucs2character::isWhitespace(c)==iswhitespace[c]);
		test("\tisAscii",
			ucs2character::isAscii(c)==isascii[c]);
		test("\ttoUpperCase",
			(char16_t)ucs2character::toUpperCase(c)==touppercase[c]);
		test("\ttoLowerCase",
			(char16_t)ucs2character::toLowerCase(c)==tolowercase[c]);
		test("\ttoAscii",
			(char16_t)ucs2character::toAscii(c)==toascii[c]);
		test("\tduplicate",
			ucs2character::duplicate((char)c)==(char16_t)c);
	}

	return 0;
}
