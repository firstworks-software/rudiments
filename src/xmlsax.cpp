// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/xmlsax.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

class xmlsaxprivate {
	friend class xmlsax;
	private:
		// reusing these over and over reduces heap fragmentation
		stringbuffer	_tagns;
		stringbuffer	_tagname;
		stringbuffer	_commenttext;
		stringbuffer	_cdatatext;
		stringbuffer	_attrdata;
		stringbuffer	_textdata;
		char		_entitybuffer[7];
};

xmlsax::xmlsax() : sax() {
	pvt=new xmlsaxprivate;
}

xmlsax::~xmlsax() {
	delete pvt;
}

bool xmlsax::tagStart(const char *ns, const char *name) {
	// by default, just return success
	return true;
}

bool xmlsax::attributeName(const char *name) {
	// by default, just return success
	return true;
}

bool xmlsax::attributeValue(const char *value) {
	// by default, just return success
	return true;
}

bool xmlsax::text(const char *string) {
	// by default, just return success
	return true;
}

bool xmlsax::tagEnd(const char *ns, const char *name) {
	// by default, just return success
	return true;
}

bool xmlsax::comment(const char *string) {
	// by default, just return success
	return true;
}

bool xmlsax::cdata(const char *string) {
	// by default, just return success
	return true;
}


bool xmlsax::parse() {

	char	ch;

	// skip whitespace/check for an empty document
	if (!(ch=skipWhitespace('\0'))) {
		return true;
	}

	// parse the document body
	for (;;) {

		// parse the tag
		if (!parseTag(ch,&ch)) {
			return false;
		} else if (!ch) {
			break;
		}

		// parse text until we find another tag
		// Strictly speaking, if parseText returns 0 then there was
		// trailing text after the last tag.  There is so commonly
		// trailing text though, that we'll allow it.
		if (!parseText(ch,&ch)) {
			break;
		}
	}

	// document parsed successfully
	return true;
}

bool xmlsax::parseTag(char current, char *next) {

	char	ch=current;

	// make sure there's a <, skip any whitespace after it
	if (ch!='<') {
		parseFailed("tag","missing <");
		return false;
	}
	if (!(ch=skipWhitespace(getCharacter()))) {
		parseFailed("tag","failed to parse whitespace after <");
			return false;
	}

	// is this a standalone tag or end-tag?
	int32_t	endtag=0;
	char	standalone='\0';
	if (ch=='!' || ch=='?') {
		standalone=ch;
	} else if (ch=='/') {
		endtag=1;
		if (!(ch=skipWhitespace(getCharacter()))) {
			parseFailed("tag","failed to parse whitespace after /");
			return false;
		}
	}

	// get the tag name
	pvt->_tagns.clear();
	pvt->_tagname.clear();
	if (!parseTagName(ch,&pvt->_tagns,&pvt->_tagname,&ch)) {
		return false;
	}

	// handle comments and cdata
	if (!charstring::compare(pvt->_tagname.getString(),"!--")) {
		if (!(ch=parseComment(ch))) {
			return false;
		}
		return (*next=getCharacter())!='\0';
	} else if (!charstring::compare(pvt->_tagname.getString(),"![CDATA[")) {
		if (!(ch=parseCData(ch))) {
			return false;
		}
		return (*next=getCharacter())!='\0';
	}

	if (endtag) {

		// skip whitespace after the tag name and look for >
		if (!(ch=skipWhitespace(ch))) {
			parseFailed("tag",
				"failed to parse whitespace before >");
			return false;
		}
		if (ch!='>') {
			parseFailed("tag","missing >");
			return false;
		}

	} else {

		// call the callback for tag start
		if (!tagStart((pvt->_tagns.getStringLength())?
					pvt->_tagns.getString():NULL,
				pvt->_tagname.getString())) {
			return false;
		}

		// parse the attributes
		for (;;) {

			// skip any whitespace before the attribute
			if (!(ch=skipWhitespace(ch))) {
				parseFailed("tag",
					"failed to parse whitespace "
					"before attribute");
				return false;
			}
	
			if (ch=='/') {
				// empty tag
				endtag=1;
				if (!(ch=skipWhitespace(getCharacter())) ||
								ch!='>') {
					parseFailed("tag",
						"failed to parse "
						"whitespace after /");
					return false;
				}
				break;
			} else if (ch=='?') {
				// ? standalone tag, make sure there's a >
				// immediately following the ?
				if (!(ch=getCharacter()) || ch!='>') {
					parseFailed("tag","missing >");
					return false;
				}
				break;
			} else if (ch=='>') {
				// normal tag
				break;
			} else {
				if (!(ch=parseAttribute(ch,standalone))) {
					return false;
				}
			}
		}
	}

	// if the tag was an empty or standalone tag,
	// call the callback for tag end
	if (endtag || standalone) {
		if (!tagEnd((pvt->_tagns.getStringLength())?
					pvt->_tagns.getString():NULL,
				pvt->_tagname.getString())) {
			return false;
		}
	}

	// return the first character after the closing >
	*next=getCharacter();
	return true;
}

bool xmlsax::parseTagName(char current, stringbuffer *ns,
				stringbuffer *name, char *next) {

	int32_t	bracketcount=0;
	bool	hascolon=false;

	// get characters and put them in the buffer
	char	ch=current;
	for (;;) {

		if (ch==':') {
			hascolon=true;
		}

		if (!ch) {

			// we should not run into a NULL or EOF here, if we
			// do then it's an error
			parseFailed("tag name","EOF encountered");
			return false;

		} else if (ch=='[') {

			name->append(ch);

			// CDATA tags will have 2 brackets (![CDATA[)
			// if we've found 2 brackets, we're done
			bracketcount++;
			if (bracketcount==2) {
				// return the character after
				// the end of the name
				if ((*next=getCharacter())!='\0') {
					return true;
				} else {
					parseFailed("tag name",
							"EOF encountered");
					return false;
				}
			}

		} else if (ch==' ' || ch=='	' ||
				ch=='\n' || ch=='\r' || ch=='/' || ch=='>') {

			// if we find whitespace, a / or a > then we're done
			// parsing the name

			// if the name contained a namespace then split it
			if (hascolon) {
				char	*n=name->detachString();
				char	*colon=charstring::findFirst(n,':');
				ns->append(n,colon-n);
				name->append(colon+1);
				delete[] n;
			}

			// return the character after the end of the name
			*next=ch;
			return true;
		} else {
			name->append(ch);
		}

		// look for comments
		if (name->getStringLength()==3 &&
			!charstring::compare(name->getString(),"!--")) {
			// return the character after the !--
			return (*next=getCharacter())!='\0';
		}

		// get the next character
		ch=getCharacter();
	}
}

char xmlsax::parseComment(char current) {

	// create a buffer to store the comment
	pvt->_commenttext.clear();
	char		ch=current;

	for (;;) {

		// handle potential terminators
		if (ch=='-') {
			if (!(ch=getCharacter())) {
				// FIXME: set error
				return '\0';
			} else if (ch=='-') {
				if (!(ch=getCharacter())) {
					return '\0';
				} else if (ch=='>') {
					// call the comment callback
					comment(pvt->_commenttext.getString());
					return ch;
				} else {
					pvt->_commenttext.append("--");
				}
			} else {
				pvt->_commenttext.append('-');
			}
		}

		pvt->_commenttext.append(ch);

		// get the next character
		if (!(ch=getCharacter())) {
			// FIXME: set error
			return '\0';
		}
	}
}

char xmlsax::parseCData(char current) {

	// create a buffer to store the comment
	pvt->_cdatatext.clear();
	char		ch=current;
	int32_t		nest=0;

	for (;;) {

		// handle potential terminators
		if (ch=='[') {
			nest++;
		} else if (ch==']') {
			if (nest==0) {
				if (!(ch=getCharacter())) {
					// FIXME: set error
					return '\0';
				} else if (ch==']') {
					// call the cdata callback
					cdata(pvt->_cdatatext.getString());
					break;
				} else {
					pvt->_cdatatext.append(']');
				}
			} else {
				nest--;
			}
		}

		pvt->_cdatatext.append(ch);

		// get the next character
		if (!(ch=getCharacter())) {
			// FIXME: set error
			return '\0';
		}
	}

	// skip whitespace, get the next character and return it,
	// it should be a >
	if (!(ch=skipWhitespace(getCharacter())) || ch!='>') {
		// FIXME: set error
		return '\0';
	}
	return ch;
}

char xmlsax::parseAttribute(char current, char standalone) {

	char		ch=current;
	pvt->_attrdata.clear();

	if (standalone!='!' ||
			(standalone=='!' &&
				ch!='"' && ch!='\'' && ch!='[' && ch!='(')) {

		// get the attribute name
		for (;;) {

			if (ch==' ' || ch=='	' || ch=='\n' || ch=='\r' ||
				(standalone && ch=='>')) {

				// if we got whitespace, skip past it
				if (!(ch=skipWhitespace(ch))) {
					parseFailed("attribute",
						"failed to parse whitespace "
						"after atribute name");
					return '\0';
				}
	
				if (standalone) {
					// for standalone tags, return an
					// attribute value,
					if (!attributeValue(
						pvt->_attrdata.getString())) {
						// FIXME: set error
						return '\0';
					}
					return ch;
				} else {
					// for non-standalone, make sure there's
					// an = after the whitespace
					if (ch!='=') {
						parseFailed("attribute",
								"missing =");
						return '\0';
					}
				}
	
			} else if (ch=='=') {
	
				// if we got an = then we've gotten the entire
				// name, terminate it and break out of the loop
				break;

			} else {
	
				// otherwise add the character
				// to the attribute name
				pvt->_attrdata.append(ch);
				if (!(ch=getCharacter())) {
					parseFailed("attribute name",
							"EOF encountered");
					return '\0';
				}
			}
		}

		// call the attribute name callback
		if (!attributeName(pvt->_attrdata.getString())) {
			parseFailed("attribute name","callback failed");
			return '\0';
		}

		// skip any whitespace after the =, then look for a " or ',
		// if we don't get one then that's an error
		if (!(ch=skipWhitespace(getCharacter()))) {
			parseFailed("attribute",
				"failed to parse whitespace after =");
			return '\0';
		}
		if (ch!='"' && ch!='\'') {
			parseFailed("attribute","missing quote");
			return '\0';
		}
	}

	// attribute values can be delimited by ' or "
	char	delimiter=ch;
	if (!(ch=getCharacter())) {
		parseFailed("attribute","EOF encountered");
		return '\0';
	}

	// get the attribute value
	pvt->_attrdata.clear();
	int32_t	nest=0;
	for (;;) {

		if (standalone=='!' &&
			((delimiter=='[' && ch==']') ||
			(delimiter=='(' && ch==')'))) {

			// handle nesting in internal subsets
			if (nest) {
				nest--;
				pvt->_attrdata.append(ch);
			} else {
				break;
			}

		} else if (ch==delimiter) {

			// handle nesting in internal subsets
			if (standalone=='!' && delimiter=='[') {
				pvt->_attrdata.append('[');
				nest++;
			} else if (standalone=='!' && delimiter=='(') {
				pvt->_attrdata.append('(');
				nest++;
			} else {
				// if we got a matching " or ' then we've
				// gotten the entire value, terminate it
				// and break out
				break;
			}

		} else {

			if (ch=='&') {

				// handle general entities
				int32_t	result=getGeneralEntity(delimiter);

				if (!result) {

					// if we hit the end, that's
					// an error
					parseFailed("attribute value",
							"EOF encountered");
					return '\0';

				} else if (result<0) {

					// if we hit the break character
					// (or an &) then write the
					// buffer to the value and loop
					// back without getting a new
					// character
					pvt->_attrdata.append(
						pvt->_entitybuffer);
					if (result==-1) {
						ch=delimiter;
					} else {
						ch='&';
					}
					continue;
				}

				// write the buffer to the value
				// and clean up
				pvt->_attrdata.append(pvt->_entitybuffer);

			} else {
				// add the character to the value
				pvt->_attrdata.append(ch);
			}
		}

		// get the next character
		if (!(ch=getCharacter())) {
			return '\0';
		}
	}

	// call the callback for attribute
	if (!attributeValue(pvt->_attrdata.getString())) {
		parseFailed("attribute value","callback failed");
		return '\0';
	}

	// return the first character after the attribute
	return getCharacter();
}

int32_t xmlsax::getGeneralEntity(char breakchar) {

	// create a buffer and set the first character to &
	pvt->_entitybuffer[0]='&';

	// get until a ; or the next 5 characters, whatever is smaller
	int32_t	i;
	for (i=1; i<6; i++) {

		pvt->_entitybuffer[i]=getCharacter();

		// jump out if we hit the end
		if (!pvt->_entitybuffer[i]) {
			pvt->_entitybuffer[i]='\0';
			return 0;
		}

		// if we find a break character, don't add it to the buffer,
		// just terminate and return the existing buffer
		if (pvt->_entitybuffer[i]==breakchar) {
			pvt->_entitybuffer[i]='\0';
			return -1;
		}

		// if we find a & then treat it similarly to a break character
		if (pvt->_entitybuffer[i]=='&') {
			pvt->_entitybuffer[i]='\0';
			return -2;
		}

		// if we find a ; then we're done
		if (pvt->_entitybuffer[i]==';') {
			pvt->_entitybuffer[i+1]='\0';
			break;
		}
	}

	// terminate the buffer if necessary
	if (i==6) {
		pvt->_entitybuffer[6]='\0';
	}

	// handle some predefined general entities
	if (!charstring::compare(pvt->_entitybuffer,"&amp;")) {
		pvt->_entitybuffer[0]='&';
		pvt->_entitybuffer[1]='\0';
	} else if (!charstring::compare(pvt->_entitybuffer,"&lt;")) {
		pvt->_entitybuffer[0]='<';
		pvt->_entitybuffer[1]='\0';
	} else if (!charstring::compare(pvt->_entitybuffer,"&gt;")) {
		pvt->_entitybuffer[0]='>';
		pvt->_entitybuffer[1]='\0';
	} else if (!charstring::compare(pvt->_entitybuffer,"&apos;")) {
		pvt->_entitybuffer[0]='\'';
		pvt->_entitybuffer[1]='\0';
	} else if (!charstring::compare(pvt->_entitybuffer,"&quot;")) {
		pvt->_entitybuffer[0]='"';
		pvt->_entitybuffer[1]='\0';
	} else {
		// handle numeric general entities
		if (pvt->_entitybuffer[1]=='#') {
			int64_t	number=charstring::convertToInteger(
							pvt->_entitybuffer+2);
			if (number>255) {
				number=255;
			}
			pvt->_entitybuffer[0]=static_cast<char>(number);
			pvt->_entitybuffer[1]='\0';
		}
	}

	return 1;
}

bool xmlsax::parseText(char current, char *next) {

	pvt->_textdata.clear();

	char	ch=current;
	for (;;) {

		if (!ch) {

			// we should not run into a NULL or EOF here, if we do
			// then return an error.
			*next='\0';
			return false;

		} else if (ch=='<') {

			// if we find an opening < then it should be a tag,
			// call the text callback (if we actually read any text)
			// and return the <
			if (pvt->_textdata.getStringLength()) {
				text(pvt->_textdata.getString());
			}
			*next=ch;
			return true;

		}


		if (ch=='&') {

			// handle general entities
			int32_t	result=getGeneralEntity('<');

			if (!result) {

				// if we hit the end, that's an error
				parseFailed("text","eof encountered");
				*next='\0';
				return false;

			} else if (result<0) {

				// if we hit the break character (or an &) then
				// write the buffer to the textdata and loop
				// back without getting a new character
				pvt->_textdata.append(pvt->_entitybuffer);
				if (result==-1) {
					ch='<';
				} else {
					ch='&';
				}
				continue;
			}

			// write the buffer to the textdata and clean up
			pvt->_textdata.append(pvt->_entitybuffer);

		} else {

			// if we haven't hit any exit conditions,
			// add the character to the buffer
			pvt->_textdata.append(ch);
		}

		// get the next character
		ch=getCharacter();
	}
}
