// Copyright (c) David Muse
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

static int32_t hexValue(char ch) {
	if (ch>='0' && ch<='9') {
		return ch-'0';
	} else if (ch>='a' && ch<='f') {
		return ch-'a'+10;
	} else if (ch>='A' && ch<='F') {
		return ch-'A'+10;
	}
	return -1;
}

static void appendUtf8(stringbuffer *str, uint32_t cp) {
	if (cp<0x80) {
		str->append((char)cp);
	} else if (cp<0x800) {
		str->append((char)(0xC0|(cp>>6)));
		str->append((char)(0x80|(cp&0x3F)));
	} else if (cp<0x10000) {
		str->append((char)(0xE0|(cp>>12)));
		str->append((char)(0x80|((cp>>6)&0x3F)));
		str->append((char)(0x80|(cp&0x3F)));
	} else {
		str->append((char)(0xF0|(cp>>18)));
		str->append((char)(0x80|((cp>>12)&0x3F)));
		str->append((char)(0x80|((cp>>6)&0x3F)));
		str->append((char)(0x80|(cp&0x3F)));
	}
}

bool jsonsax::parseHex4(uint32_t *value) {
	uint32_t	v=0;
	for (uint8_t i=0; i<4; i++) {
		int32_t	h=hexValue(getCharacter());
		if (h<0) {
			parseFailed("str","invalid \\u escape");
			return false;
		}
		v=(v<<4)|h;
	}
	*value=v;
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
			} else if (ch=='\\' || ch=='\"' || ch=='/') {
				str->append(ch);
			} else if (ch=='u') {
				uint32_t	cp;
				if (!parseHex4(&cp)) {
					return false;
				}
				if (cp>=0xD800 && cp<=0xDBFF) {

					// high surrogate, needs a low one
					uint32_t	low;
					if (getCharacter()!='\\' ||
						getCharacter()!='u' ||
						!parseHex4(&low) ||
						low<0xDC00 || low>0xDFFF) {
						parseFailed("str",
							"invalid surrogate");
						return false;
					}
					cp=0x10000+
						((cp-0xD800)<<10)+
						(low-0xDC00);
				} else if (cp>=0xDC00 && cp<=0xDFFF) {
					parseFailed("str","invalid surrogate");
					return false;
				}
				if (cp) {
					appendUtf8(str,cp);
				} else {

					// a C string can't hold a null, so keep
					// the escape as text, which the writer
					// emits for a null
					str->append("\\u0000");
				}
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
	} else if (ch=='-' || character::isDigit(ch)) {
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

	// optional sign
	if (ch=='-') {
		str->append(ch);
		ch=getCharacter();
	}

	// integer part: a single 0 or a nonzero digit followed by digits
	if (ch=='0') {
		str->append(ch);
		ch=getCharacter();
	} else if (character::isDigit(ch)) {
		do {
			str->append(ch);
			ch=getCharacter();
		} while (character::isDigit(ch));
	} else {
		parseFailed("number","missing digit");
		return false;
	}

	// optional fraction
	if (ch=='.') {
		str->append(ch);
		ch=getCharacter();
		if (!character::isDigit(ch)) {
			parseFailed("number","missing digit after .");
			return false;
		}
		do {
			str->append(ch);
			ch=getCharacter();
		} while (character::isDigit(ch));
	}

	// optional exponent
	if (ch=='e' || ch=='E') {
		str->append(ch);
		ch=getCharacter();
		if (ch=='+' || ch=='-') {
			str->append(ch);
			ch=getCharacter();
		}
		if (!character::isDigit(ch)) {
			parseFailed("number","missing exponent digit");
			return false;
		}
		do {
			str->append(ch);
			ch=getCharacter();
		} while (character::isDigit(ch));
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
