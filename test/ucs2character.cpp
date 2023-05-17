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

	ucs2_t	touppercase[]={
		(ucs2_t)0,(ucs2_t)1,(ucs2_t)2,(ucs2_t)3,(ucs2_t)4,
		(ucs2_t)5,(ucs2_t)6,(ucs2_t)7,(ucs2_t)8,(ucs2_t)9,
		(ucs2_t)10,(ucs2_t)11,(ucs2_t)12,(ucs2_t)13,(ucs2_t)14,
		(ucs2_t)15,(ucs2_t)16,(ucs2_t)17,(ucs2_t)18,(ucs2_t)19,
		(ucs2_t)20,(ucs2_t)21,(ucs2_t)22,(ucs2_t)23,(ucs2_t)24,
		(ucs2_t)25,(ucs2_t)26,(ucs2_t)27,(ucs2_t)28,(ucs2_t)29,
		(ucs2_t)30,(ucs2_t)31,(ucs2_t)32,(ucs2_t)33,(ucs2_t)34,
		(ucs2_t)35,(ucs2_t)36,(ucs2_t)37,(ucs2_t)38,(ucs2_t)39,
		(ucs2_t)40,(ucs2_t)41,(ucs2_t)42,(ucs2_t)43,(ucs2_t)44,
		(ucs2_t)45,(ucs2_t)46,(ucs2_t)47,(ucs2_t)48,(ucs2_t)49,
		(ucs2_t)50,(ucs2_t)51,(ucs2_t)52,(ucs2_t)53,(ucs2_t)54,
		(ucs2_t)55,(ucs2_t)56,(ucs2_t)57,(ucs2_t)58,(ucs2_t)59,
		(ucs2_t)60,(ucs2_t)61,(ucs2_t)62,(ucs2_t)63,(ucs2_t)64,
		(ucs2_t)65,(ucs2_t)66,(ucs2_t)67,(ucs2_t)68,(ucs2_t)69,
		(ucs2_t)70,(ucs2_t)71,(ucs2_t)72,(ucs2_t)73,(ucs2_t)74,
		(ucs2_t)75,(ucs2_t)76,(ucs2_t)77,(ucs2_t)78,(ucs2_t)79,
		(ucs2_t)80,(ucs2_t)81,(ucs2_t)82,(ucs2_t)83,(ucs2_t)84,
		(ucs2_t)85,(ucs2_t)86,(ucs2_t)87,(ucs2_t)88,(ucs2_t)89,
		(ucs2_t)90,(ucs2_t)91,(ucs2_t)92,(ucs2_t)93,(ucs2_t)94,
		(ucs2_t)95,(ucs2_t)96,(ucs2_t)'A',(ucs2_t)'B',(ucs2_t)'C',
		(ucs2_t)'D',(ucs2_t)'E',(ucs2_t)'F',(ucs2_t)'G',(ucs2_t)'H',
		(ucs2_t)'I',(ucs2_t)'J',(ucs2_t)'K',(ucs2_t)'L',(ucs2_t)'M',
		(ucs2_t)'N',(ucs2_t)'O',(ucs2_t)'P',(ucs2_t)'Q',(ucs2_t)'R',
		(ucs2_t)'S',(ucs2_t)'T',(ucs2_t)'U',(ucs2_t)'V',(ucs2_t)'W',
		(ucs2_t)'X',(ucs2_t)'Y',(ucs2_t)'Z',(ucs2_t)123,(ucs2_t)124,
		(ucs2_t)125,(ucs2_t)126,(ucs2_t)127
	};

	ucs2_t	tolowercase[]={
		(ucs2_t)0,(ucs2_t)1,(ucs2_t)2,(ucs2_t)3,(ucs2_t)4,(ucs2_t)5,
		(ucs2_t)6,(ucs2_t)7,(ucs2_t)8,(ucs2_t)9,(ucs2_t)10,(ucs2_t)11,
		(ucs2_t)12,(ucs2_t)13,(ucs2_t)14,(ucs2_t)15,(ucs2_t)16,
		(ucs2_t)17,(ucs2_t)18,(ucs2_t)19,(ucs2_t)20,(ucs2_t)21,
		(ucs2_t)22,(ucs2_t)23,(ucs2_t)24,(ucs2_t)25,(ucs2_t)26,
		(ucs2_t)27,(ucs2_t)28,(ucs2_t)29,(ucs2_t)30,(ucs2_t)31,
		(ucs2_t)32,(ucs2_t)33,(ucs2_t)34,(ucs2_t)35,(ucs2_t)36,
		(ucs2_t)37,(ucs2_t)38,(ucs2_t)39,(ucs2_t)40,(ucs2_t)41,
		(ucs2_t)42,(ucs2_t)43,(ucs2_t)44,(ucs2_t)45,(ucs2_t)46,
		(ucs2_t)47,(ucs2_t)48,(ucs2_t)49,(ucs2_t)50,(ucs2_t)51,
		(ucs2_t)52,(ucs2_t)53,(ucs2_t)54,(ucs2_t)55,(ucs2_t)56,
		(ucs2_t)57,(ucs2_t)58,(ucs2_t)59,(ucs2_t)60,(ucs2_t)61,
		(ucs2_t)62,(ucs2_t)63,(ucs2_t)64,(ucs2_t)'a',(ucs2_t)'b',
		(ucs2_t)'c',(ucs2_t)'d',(ucs2_t)'e',(ucs2_t)'f',(ucs2_t)'g',
		(ucs2_t)'h',(ucs2_t)'i',(ucs2_t)'j',(ucs2_t)'k',(ucs2_t)'l',
		(ucs2_t)'m',(ucs2_t)'n',(ucs2_t)'o',(ucs2_t)'p',(ucs2_t)'q',
		(ucs2_t)'r',(ucs2_t)'s',(ucs2_t)'t',(ucs2_t)'u',(ucs2_t)'v',
		(ucs2_t)'w',(ucs2_t)'x',(ucs2_t)'y',(ucs2_t)'z',(ucs2_t)91,
		(ucs2_t)92,(ucs2_t)93,(ucs2_t)94,(ucs2_t)95,(ucs2_t)96,
		(ucs2_t)97,(ucs2_t)98,(ucs2_t)99,(ucs2_t)100,(ucs2_t)101,
		(ucs2_t)102,(ucs2_t)103,(ucs2_t)104,(ucs2_t)105,(ucs2_t)106,
		(ucs2_t)107,(ucs2_t)108,(ucs2_t)109,(ucs2_t)110,(ucs2_t)111,
		(ucs2_t)112,(ucs2_t)113,(ucs2_t)114,(ucs2_t)115,(ucs2_t)116,
		(ucs2_t)117,(ucs2_t)118,(ucs2_t)119,(ucs2_t)120,(ucs2_t)121,
		(ucs2_t)122,(ucs2_t)123,(ucs2_t)124,(ucs2_t)125,(ucs2_t)126,
		(ucs2_t)127
	};

	ucs2_t	toascii[]={
		(ucs2_t)0,(ucs2_t)1,(ucs2_t)2,(ucs2_t)3,(ucs2_t)4,(ucs2_t)5,
		(ucs2_t)6,(ucs2_t)7,(ucs2_t)8,(ucs2_t)9,(ucs2_t)10,(ucs2_t)11,
		(ucs2_t)12,(ucs2_t)13,(ucs2_t)14,(ucs2_t)15,(ucs2_t)16,
		(ucs2_t)17,(ucs2_t)18,(ucs2_t)19,(ucs2_t)20,(ucs2_t)21,
		(ucs2_t)22,(ucs2_t)23,(ucs2_t)24,(ucs2_t)25,(ucs2_t)26,
		(ucs2_t)27,(ucs2_t)28,(ucs2_t)29,(ucs2_t)30,(ucs2_t)31,
		(ucs2_t)32,(ucs2_t)33,(ucs2_t)34,(ucs2_t)35,(ucs2_t)36,
		(ucs2_t)37,(ucs2_t)38,(ucs2_t)39,(ucs2_t)40,(ucs2_t)41,
		(ucs2_t)42,(ucs2_t)43,(ucs2_t)44,(ucs2_t)45,(ucs2_t)46,
		(ucs2_t)47,(ucs2_t)48,(ucs2_t)49,(ucs2_t)50,(ucs2_t)51,
		(ucs2_t)52,(ucs2_t)53,(ucs2_t)54,(ucs2_t)55,(ucs2_t)56,
		(ucs2_t)57,(ucs2_t)58,(ucs2_t)59,(ucs2_t)60,(ucs2_t)61,
		(ucs2_t)62,(ucs2_t)63,(ucs2_t)64,(ucs2_t)65,(ucs2_t)66,
		(ucs2_t)67,(ucs2_t)68,(ucs2_t)69,(ucs2_t)70,(ucs2_t)71,
		(ucs2_t)72,(ucs2_t)73,(ucs2_t)74,(ucs2_t)75,(ucs2_t)76,
		(ucs2_t)77,(ucs2_t)78,(ucs2_t)79,(ucs2_t)80,(ucs2_t)81,
		(ucs2_t)82,(ucs2_t)83,(ucs2_t)84,(ucs2_t)85,(ucs2_t)86,
		(ucs2_t)87,(ucs2_t)88,(ucs2_t)89,(ucs2_t)90,(ucs2_t)91,
		(ucs2_t)92,(ucs2_t)93,(ucs2_t)94,(ucs2_t)95,(ucs2_t)96,
		(ucs2_t)97,(ucs2_t)98,(ucs2_t)99,(ucs2_t)100,(ucs2_t)101,
		(ucs2_t)102,(ucs2_t)103,(ucs2_t)104,(ucs2_t)105,(ucs2_t)106,
		(ucs2_t)107,(ucs2_t)108,(ucs2_t)109,(ucs2_t)110,(ucs2_t)111,
		(ucs2_t)112,(ucs2_t)113,(ucs2_t)114,(ucs2_t)115,(ucs2_t)116,
		(ucs2_t)117,(ucs2_t)118,(ucs2_t)119,(ucs2_t)120,(ucs2_t)121,
		(ucs2_t)122,(ucs2_t)123,(ucs2_t)124,(ucs2_t)125,(ucs2_t)126,
		(ucs2_t)127
	};

	test("sizeof(ucs2_t)==2",sizeof(ucs2_t)==2);
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
		test("\tupper",
			(ucs2_t)ucs2character::upper(c)==touppercase[c]);
		test("\tlower",
			(ucs2_t)ucs2character::lower(c)==tolowercase[c]);
		test("\tconvertToAscii",
			(ucs2_t)ucs2character::convertToAscii(c)==toascii[c]);
		test("\tduplicate (char)",
			ucs2character::duplicate((char)c)==(ucs2_t)c);
	}

	return 0;
}
