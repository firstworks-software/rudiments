// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/inisax.h>
#include <rudiments/character.h>
//#define DEBUG_MESSAGES
#include <rudiments/debugprint.h>

class inisaxprivate {
	friend class inisax;
	private:
		// reusing this over and over reduces heap fragmentation
		stringbuffer	_str;
		bool		_insection;
};

inisax::inisax() : sax() {
	pvt=new inisaxprivate;
	pvt->_insection=false;
}

inisax::inisax(inisax &i) : sax(i) {
	// FIXME: implement this
}

inisax &inisax::operator=(inisax &i) {
	if (this!=&i) {
		sax::operator=(i);
		// FIXME: implement this
	}
	return *this;
}

inisax::~inisax() {
	delete pvt;
}

bool inisax::whitespaceStart() {
	return true;
}

bool inisax::whitespace(const char *w) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	debugPrintf("%s",w);
#endif
	return true;
}

bool inisax::whitespaceEnd() {
	return true;
}

bool inisax::semicolonCommentStart() {
	// by default, just return success
	return true;
}

bool inisax::semicolonComment(const char *c) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	debugPrintf(";%s",c);
#endif
	return true;
}

bool inisax::semicolonCommentEnd() {
	// by default, just return success
	return true;
}

bool inisax::poundCommentStart() {
	// by default, just return success
	return true;
}

bool inisax::poundComment(const char *c) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	debugPrintf("#%s",c);
#endif
	return true;
}

bool inisax::poundCommentEnd() {
	// by default, just return success
	return true;
}

bool inisax::sectionStart() {
	// by default, just return success
	return true;
}

bool inisax::section(const char *s) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	debugPrintf("[%s]",s);
#endif
	return true;
}

bool inisax::sectionEnd() {
	// by default, just return success
	return true;
}

bool inisax::keyStart() {
	// by default, just return success
	return true;
}

bool inisax::key(const char *k) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	debugPrintf("%s=",k);
#endif
	return true;
}

bool inisax::keyEnd() {
	// by default, just return success
	return true;
}

bool inisax::valueStart() {
	// by default, just return success
	return true;
}

bool inisax::value(const char *v) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	debugPrintf("%s",v);
#endif
	return true;
}

bool inisax::valueEnd() {
	// by default, just return success
	return true;
}

bool inisax::parse() {

	pvt->_insection=false;

	// get first character, handle empty file
	char	ch=getCharacter();
	if (!ch) {
		return true;
	}

	for (;;) {
		if (character::isWhitespace(ch)) {
			if (!parseWhitespace(ch,&ch)) {
				return false;
			}
		}
		switch (ch) {
			case ';':
				if (!parseSemicolonComment(ch,&ch)) {
					return false;
				}
				break;
			case '#':
				if (!parsePoundComment(ch,&ch)) {
					return false;
				}
				break;
			case '[':
				if (!parseSection(&ch)) {
					return false;
				}
				break;
			case '\0':
				return (pvt->_insection)?sectionEnd():true;
			default:
				if (!parseKey(ch,&ch)) {
					return false;
				}
				break;
		}
	}
}

bool inisax::parseWhitespace(char current, char *next) {
	if (!whitespaceStart()) {
		return false;
	}
	pvt->_str.clear();
	pvt->_str.append(current);
	for (;;) {
		char	ch=getCharacter();
		if (character::isWhitespace(ch)) {
			pvt->_str.append(ch);
		} else {
			if (!whitespace(pvt->_str.getString())) {
				return false;
			}
			*next=ch;
			return whitespaceEnd();
		}
	}
}

bool inisax::parseSemicolonComment(char current, char *next) {
	if (!semicolonCommentStart()) {
		return false;
	}
	parseRestOfLine(next);
	return semicolonComment(pvt->_str.getString()) && semicolonCommentEnd();
}

void inisax::parseRestOfLine(char *next) {
	pvt->_str.clear();
	for (;;) {
		char	ch=getCharacter();
		if (ch=='\n' || ch=='\r' || ch=='\0') {
			*next=ch;
			return;
		} else {
			pvt->_str.append(ch);
		}
	}
}

bool inisax::parsePoundComment(char current, char *next) {
	if (!poundCommentStart()) {
		return false;
	}
	parseRestOfLine(next);
	return poundComment(pvt->_str.getString()) && poundCommentEnd();
}

bool inisax::parseSection(char *next) {
	if (pvt->_insection && !sectionEnd()) {
		return false;
	}
	pvt->_insection=true;
	if (!sectionStart()) {
		return false;
	}
	parseRestOfSectionName(next);
	return section(pvt->_str.getString());
}

void inisax::parseRestOfSectionName(char *next) {
	pvt->_str.clear();
	for (;;) {
		char	ch=getCharacter();
		if (ch==']') {
			ch=getCharacter();
			*next=ch;
			return;
		} else if (ch=='\n' || ch=='\r' || ch=='\0') {
			*next=ch;
			return;
		} else {
			pvt->_str.append(ch);
		}
	}
}

bool inisax::parseKey(char current, char *next) {
	if (!keyStart()) {
		return false;
	}
	pvt->_str.clear();
	pvt->_str.append(current);
	for (;;) {
		char	ch=getCharacter();
		if (ch=='=') {
			return key(pvt->_str.getString()) &&
						keyEnd() &&
						parseValue(next);
		} else if (ch=='\n' || ch=='\r' || ch=='\0') {
			*next=ch;
			return key(pvt->_str.getString()) &&
						keyEnd();
		} else {
			pvt->_str.append(ch);
		}
	}
}

bool inisax::parseValue(char *next) {
	if (!valueStart()) {
		return false;
	}
	parseRestOfLine(next);
	return value(pvt->_str.getString()) && valueEnd();
}
