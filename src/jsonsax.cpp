// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/jsonsax.h>
#include <rudiments/character.h>
//#define DEBUG_MESSAGES
#include <rudiments/debugprint.h>

class jsonsaxprivate {
	friend class jsonsax;
	private:
		// reusing this over and over reduces heap fragmentation
		stringbuffer	_str;

		#ifdef DEBUG_MESSAGES
		uint16_t	_indent;
		#endif
};

#ifdef DEBUG_MESSAGES
void indent(uint16_t ind) {
	for (uint16_t i=0; i<ind; i++) {
		stdoutput.write(' ');
	}
}
#endif

jsonsax::jsonsax() : sax() {
	pvt=new jsonsaxprivate;
	#ifdef DEBUG_MESSAGES
	pvt->_indent=0;
	#endif
}

jsonsax::jsonsax(jsonsax &j) : sax(j) {
	// FIXME: implement this
}

jsonsax &jsonsax::operator=(jsonsax &j) {
	if (this!=&j) {
		sax::operator=(j);
		// FIXME: implement this
	}
	return *this;
}

jsonsax::~jsonsax() {
	delete pvt;
}

bool jsonsax::objectStart() {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	if (pvt->_indent) {
		debugPrintf("\n");
	}
	indent(pvt->_indent);
	debugPrintf("objectStart {\n");
	pvt->_indent+=2;
#endif
	return true;
}

bool jsonsax::memberStart() {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("memberStart {\n");
	pvt->_indent+=2;
#endif
	return true;
}

bool jsonsax::memberName(const char *name) {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("\"%s\" : ",name);
	pvt->_indent+=2;
#endif
	return true;
}

bool jsonsax::valueStart() {
	// by default, just return success
	return true;
}

bool jsonsax::stringValue(const char *value) {
	// by default, just return success
	debugPrintf("\"%s\"",value);
	return true;
}

bool jsonsax::numberValue(const char *value) {
	// by default, just return success
	debugPrintf("%s",value);
	return true;
}

bool jsonsax::trueValue() {
	// by default, just return success
	debugPrintf("true");
	return true;
}

bool jsonsax::falseValue() {
	// by default, just return success
	debugPrintf("false");
	return true;
}

bool jsonsax::nullValue() {
	// by default, just return success
	debugPrintf("null");
	return true;
}

bool jsonsax::arrayStart() {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("[");
	pvt->_indent+=2;
#endif
	return true;
}

bool jsonsax::arrayEnd() {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("]\n");
#endif
	return true;
}

bool jsonsax::valueEnd() {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	debugPrintf("\n");
#endif
	return true;
}

bool jsonsax::memberEnd() {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("}\n");
#endif
	return true;
}

bool jsonsax::objectEnd() {
	// by default, just return success
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("}\n");
#endif
	return true;
}

bool jsonsax::parse() {

	char	ch;

	// skip whitespace/check for an empty document
	if (!(ch=skipWhitespace('\0'))) {
		return true;
	}

	// parse the document body
	if (ch=='[') {
		if (!parseArray(ch,&ch)) {
			return false;
		}
	} else {
		if (!parseObject(ch,&ch)) {
			return false;
		}
	}

	// make sure we're at the end of the input
	if (ch!='\0') {
		// FIXME: set error...
		return false;
	}
	return true;
}

bool jsonsax::parseObject(char current, char *next) {

	char	ch=current;

	// make sure there's a {, skip any whitespace after it
	if (ch!='{') {
		parseFailed("object","missing {");
		return false;
	}
	if (!(ch=skipWhitespace(getCharacter()))) {
		parseFailed("object","failed to parse whitespace after {");
		return false;
	}

	if (!objectStart()) {
		return false;
	}

	// parse members
	for (;;) {

		// handle empty objects or trailing commas
		if (ch=='}') {
			break;
		}

		if (!memberStart()) {
			return false;
		}

		// parse name
		pvt->_str.clear();
		if (!parseStr(&pvt->_str,ch,&ch)) {
			return false;
		}

		if (!memberName(pvt->_str.getString())) {
			return false;
		}

		// make sure there's a colon
		if (ch!=':') {
			parseFailed("object","missing :");
			return false;
		}

		// skip whitespace after the colon
		ch=skipWhitespace(getCharacter());

		// parse value
		if (!parseValue(ch,&ch)) {
			return false;
		}

		if (!memberEnd()) {
			return false;
		}

		// keep going if we find a comma, otherwise we're done
		if (ch==',') {
			ch=skipWhitespace(getCharacter());
		} else {
			break;
		}
	}

	// make sure there's a }
	if (ch!='}') {
		parseFailed("object","missing }");
		return false;
	}

	if (!objectEnd()) {
		return false;
	}

	// skip whitespace after the closing } and return the next character
	*next=skipWhitespace(getCharacter());
	return true;
}

bool jsonsax::parseStr(stringbuffer *str, char current, char *next) {

	char	ch=current;

	// make sure there's a "
	if (ch!='"') {
		parseFailed("str","missing \"");
		return false;
	}

	// get the string into the stringbuffer
	for (;;) {
		ch=getCharacter();
		if (ch=='\\') {
			ch=getCharacter();
			if (ch=='b') {
				str->append('\b');
			} else if (ch=='f') {
				str->append('\f');
			} else if (ch=='n') {
				str->append('\n');
			} else if (ch=='r') {
				str->append('\r');
			} else if (ch=='t') {
				str->append('\t');
			} else if (ch=='\\' || ch=='\"') {
				str->append(ch);
			} else {
				str->append('\\');
				str->append(ch);
			}
		} else if (ch=='"') {
			break;
		} else if (ch=='\0') {
			parseFailed("str","null character encountered");
			return false;
		} else {
			str->append(ch);
		}
	}

	// skip whitespace after the closing " and return the next character
	*next=skipWhitespace(getCharacter());
	return true;
}

bool jsonsax::parseValue(char current, char *next) {

	if (!valueStart()) {
		return false;
	}

	char	ch=current;
	bool	success=false;

	if (ch=='"') {
		pvt->_str.clear();
		if ((success=parseStr(&pvt->_str,ch,next))) {
			if (!stringValue(pvt->_str.getString())) {
				return false;
			}
		}
	} else if (ch=='{') {
		success=parseObject(ch,next);
	} else if (ch=='[') {
		success=parseArray(ch,next);
	} else if (character::isDigit(ch) || character::inSet(ch,"-+,")) {
		pvt->_str.clear();
		if ((success=parseNumber(&pvt->_str,ch,next))) {
			if (!numberValue(pvt->_str.getString())) {
				return false;
			}
		}
	} else if (ch=='t') {
		if ((success=parseLiteral("true",current,next))) {
			if (!trueValue()) {
				return false;
			}
		}
	} else if (ch=='f') {
		if ((success=parseLiteral("false",current,next))) {
			if (!falseValue()) {
				return false;
			}
		}
	} else if (ch=='n') {
		if ((success=parseLiteral("null",current,next))) {
			if (!nullValue()) {
				return false;
			}
		}
	}

	if (success) {
		return valueEnd();
	}

	parseFailed("value","");
	return false;
}

bool jsonsax::parseArray(char current, char *next) {

	char	ch=current;

	// make sure there's a [, skip any whitespace after it
	if (ch!='[') {
		parseFailed("array","missing [");
		return false;
	}
	if (!(ch=skipWhitespace(getCharacter()))) {
		parseFailed("array","failed to parse whitespace after [");
		return false;
	}

	if (!arrayStart()) {
		return false;
	}

	// parse values
	for (;;) {

		// handle empty arrays or trailing commas
		if (ch==']') {
			break;
		}

		if (!parseValue(ch,&ch)) {
			return false;
		}

		// keep going if we find a comma, otherwise we're done
		if (ch==',') {
			ch=skipWhitespace(getCharacter());
		} else {
			break;
		}
	}

	// make sure there's a ]
	if (ch!=']') {
		parseFailed("array","missing ]");
		return false;
	}

	if (!arrayEnd()) {
		return false;
	}

	// skip whitespace after the closing ] and return the next character
	*next=skipWhitespace(getCharacter());
	return true;
}

bool jsonsax::parseNumber(stringbuffer *str, char current, char *next) {

	char	ch=current;

	bool	gotdecimal=false;

	// handle signs
	if (ch=='-') {
		str->append(ch);
		ch=getCharacter();
	} else if (ch=='+') {
		ch=getCharacter();
	}

	// prepend a zero if the number starts with a decimal
	if (ch=='.') {
		gotdecimal=true;
		str->append("0.");
		ch=getCharacter();
	}

	// get the number into the stringbuffer
	for (;;) {
		if (character::isDigit(ch) || (ch=='.' && !gotdecimal)) {
			str->append(ch);
		} else {
			break;
		}
		ch=getCharacter();
	}

	// skip whitespace after and return the next character
	*next=skipWhitespace(ch);
	return true;
}

bool jsonsax::parseLiteral(const char *literal, char current, char *next) {

	char		ch=current;
	const char	*ptr=literal;
	while (ch && *ptr) {
		if (ch!=*ptr) {
			parseFailed("literal","");
			return false;
		}
 		ch=getCharacter();
		ptr++;
	}

	// skip whitespace after and return the next character
	*next=skipWhitespace(ch);
	return true;
}
