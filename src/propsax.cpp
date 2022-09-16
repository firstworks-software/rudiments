// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/propsax.h>
#include <rudiments/character.h>
//#define DEBUG_MESSAGES
#include <rudiments/debugprint.h>

class propsaxprivate {
	friend class propsax;
	private:
		// reusing this over and over reduces heap fragmentation
		stringbuffer	_str;
		stringbuffer	_eq;
};

propsax::propsax() : sax() {
	pvt=new propsaxprivate;
}

propsax::~propsax() {
	delete pvt;
}

bool propsax::whitespaceStart() {
	return true;
}

bool propsax::whitespace(const char *w) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	debugPrintf("%s",w);
#endif
	return true;
}

bool propsax::whitespaceEnd() {
	return true;
}

bool propsax::exclamationCommentStart() {
	// by default, just return success
	return true;
}

bool propsax::exclamationComment(const char *c) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	debugPrintf("!%s",c);
#endif
	return true;
}

bool propsax::exclamationCommentEnd() {
	// by default, just return success
	return true;
}

bool propsax::poundCommentStart() {
	// by default, just return success
	return true;
}

bool propsax::poundComment(const char *c) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	debugPrintf("#%s",c);
#endif
	return true;
}

bool propsax::poundCommentEnd() {
	// by default, just return success
	return true;
}

bool propsax::keyStart() {
	// by default, just return success
	return true;
}

bool propsax::key(const char *k) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	debugPrintf("%s=",k);
#endif
	return true;
}

bool propsax::keyEnd() {
	// by default, just return success
	return true;
}

bool propsax::equals(const char *e) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	debugPrintf("%s",e);
#endif
	return true;
}

bool propsax::valueStart() {
	// by default, just return success
	return true;
}

bool propsax::value(const char *v) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	debugPrintf("%s",v);
#endif
	return true;
}

bool propsax::valueEnd() {
	// by default, just return success
	return true;
}

bool propsax::parse() {

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
			case '!':
				if (!parseExclamationComment(ch,&ch)) {
					return false;
				}
				break;
			case '#':
				if (!parsePoundComment(ch,&ch)) {
					return false;
				}
				break;
			case '\0':
				return true;
			default:
				if (!parseKey(ch,&ch)) {
					return false;
				}
				break;
		}
	}
}

bool propsax::parseWhitespace(char current, char *next) {
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

bool propsax::parseExclamationComment(char current, char *next) {
	if (!exclamationCommentStart()) {
		return false;
	}
	parseRestOfLine('\0',next,false);
	return exclamationComment(pvt->_str.getString()) &&
					exclamationCommentEnd();
}

void propsax::parseRestOfLine(char ch, char *next, bool invalue) {
	pvt->_str.clear();
	bool	first=true;
	for (;;) {
		if (first) {
			if (ch=='\0') {
				ch=getCharacter();
			}
			first=false;
		} else {
			ch=getCharacter();
		}
		if (ch=='\n' || ch=='\r' || ch=='\0') {
			*next=ch;
			return;
		} else {
			if (invalue && ch=='\\') {
				ch=getCharacter();
				if (ch=='\0') {
					*next=ch;
					return;
				}
			}
			pvt->_str.append(ch);
		}
	}
}

bool propsax::parsePoundComment(char current, char *next) {
	if (!poundCommentStart()) {
		return false;
	}
	parseRestOfLine('\0',next,false);
	return poundComment(pvt->_str.getString()) && poundCommentEnd();
}

bool propsax::parseKey(char current, char *next) {
	if (!keyStart()) {
		return false;
	}
	pvt->_str.clear();
	pvt->_str.append(current);
	for (;;) {
		char	ch=getCharacter();

		// FIXME: this code feels like brute-force
		if (ch==' ') {
			ch=getCharacter();
			if (ch=='=') {
				ch=getCharacter();
				if (ch=='\0' || ch=='\n' || ch=='\r') {
					*next=ch;
					return key(pvt->_str.getString()) &&
								keyEnd() &&
								equals(" =");
				} else if (ch==' ') {
					ch=getCharacter();
					if (ch=='\0' || ch=='\n' || ch=='\r') {
						*next=ch;
						return
						key(pvt->_str.getString()) &&
						keyEnd() &&
						equals(" = ");
					} else {
						return
						key(pvt->_str.getString()) &&
						keyEnd() &&
						equals(" = ") &&
						parseValue(ch,next);
					}
				} else {
					return key(pvt->_str.getString()) &&
							keyEnd() &&
							equals(" =") &&
							parseValue(ch,next);
				}
			} else if (ch==':') {
				ch=getCharacter();
				if (ch=='\0' || ch=='\n' || ch=='\r') {
					*next=ch;
					return key(pvt->_str.getString()) &&
								keyEnd() &&
								equals(" :");
				} else if (ch==' ') {
					ch=getCharacter();
					if (ch=='\0' || ch=='\n' || ch=='\r') {
						*next=ch;
						return
						key(pvt->_str.getString()) &&
						keyEnd() &&
						equals(" : ");
					} else {
						return
						key(pvt->_str.getString()) &&
						keyEnd() &&
						equals(" : ") &&
						parseValue(ch,next);
					}
				} else {
					return key(pvt->_str.getString()) &&
							keyEnd() &&
							equals(" :") &&
							parseValue(ch,next);
				}
			} else {
				return key(pvt->_str.getString()) &&
							keyEnd() &&
							equals(" ") &&
							parseValue(ch,next);
			}
		} else if (ch=='=') {
			return key(pvt->_str.getString()) &&
						keyEnd() &&
						equals("=") &&
						parseValue('\0',next);
		} else if (ch==':') {
			return key(pvt->_str.getString()) &&
						keyEnd() &&
						equals(":") &&
						parseValue('\0',next);
		} else if (ch=='\0' || ch=='\n' || ch=='\r') {
			*next=ch;
			return key(pvt->_str.getString()) &&
						keyEnd();
		} else {
			if (ch=='\\') {
				ch=getCharacter();
				if (ch=='\0') {
					*next=ch;
					return key(pvt->_str.getString()) &&
								keyEnd();
				}
			}
			pvt->_str.append(ch);
		}
	}
}

bool propsax::parseValue(char ch, char *next) {
	if (!valueStart()) {
		return false;
	}
	parseRestOfLine(ch,next,true);
	return value(pvt->_str.getString()) && valueEnd();
}
