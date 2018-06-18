// Copyright (c) 2018 David Muse
// See the COPYING file for more information

#include <rudiments/csvsax.h>
#include <rudiments/stdio.h>

enum csvstate {
	HEADER_START=0,
	COLUMN_START,
	COLUMN,
	COLUMN_END,
	HEADER_END,
	BODY_START,
	ROW_START,
	FIELD_START,
	FIELD,
	FIELD_END,
	ROW_END,
	BODY_END
};

class csvsaxprivate {
	friend class csvsax;
	private:
		csvstate	_state;
		char		_quote;
		char		_escape;
		char		_delimiter;
};

csvsax::csvsax() : sax() {
	pvt=new csvsaxprivate;
}

csvsax::~csvsax() {
	delete pvt;
}

void csvsax::reset() {
	pvt->_state=HEADER_START;
	pvt->_quote='"';
	pvt->_escape='\\';
	pvt->_delimiter=',';
	sax::reset();
}

void csvsax::setQuote(char quote) {
	pvt->_quote=quote;
}

char csvsax::getQuote() {
	return pvt->_quote;
}

void csvsax::setDelimiter(char delimiter) {
	pvt->_delimiter=delimiter;
}

char csvsax::getDelimiter() {
	return pvt->_delimiter;
}

void csvsax::setEscape(char escape) {
	pvt->_escape=escape;
}

char csvsax::getEscape() {
	return pvt->_escape;
}

bool csvsax::headerStart() {
	// by default, just return success
stdoutput.printf("headerStart {\n");
	return true;
}

bool csvsax::column(const char *name) {
	// by default, just return success
stdoutput.printf("    column: \"%s\"\n",name);
	return true;
}

bool csvsax::headerEnd() {
	// by default, just return success
stdoutput.printf("}\n");
	return true;
}

bool csvsax::bodyStart() {
	// by default, just return success
stdoutput.printf("bodyStart {\n");
	return true;
}

bool csvsax::rowStart() {
	// by default, just return success
stdoutput.printf("    rowStart {\n");
	return true;
}

bool csvsax::field(const char *value) {
	// by default, just return success
stdoutput.printf("        field: \"%s\"\n",value);
	return true;
}

bool csvsax::rowEnd() {
	// by default, just return success
stdoutput.printf("    }\n");
	return true;
}

bool csvsax::bodyEnd() {
	// by default, just return success
stdoutput.printf("}\n");
	return true;
}

bool csvsax::parse() {

	stringbuffer	current;
	bool		quoted=false;
	bool		ignore=false;

	for (;;) {

		// get a character
		char	ch=getCharacter();

		// at the very beginning of the file, skip leading \n or \r
		// and return an error if the file/string was empty
		if (pvt->_state==HEADER_START) {
			if (ch=='\n' || ch=='\r') {
				continue;
			} else if (ch=='\0') {
				return false;
			}
		}

		// handle end of file/string
		if (ch=='\0') {
			break;
		}

		// deal with escape characters
		bool	wasescaped=(ch==pvt->_escape);
		if (wasescaped) {
			ch=getCharacter();
		}

		// handle various states
		if (pvt->_state==HEADER_START) {
			headerStart();
			pvt->_state=COLUMN_START;
			quoted=false;
		}
		if (pvt->_state==COLUMN_START) {
			quoted=(ch==pvt->_quote);
			pvt->_state=COLUMN;
			if (quoted) {
				continue;
			}
		}
		if (pvt->_state==COLUMN) {
			if (!wasescaped) {
				if (quoted && ch==pvt->_quote) {
					ignore=true;
					continue;
				} else if (ch==pvt->_delimiter) {
					pvt->_state=COLUMN_END;
					column(current.getString());
					current.clear();
					ignore=false;
					pvt->_state=COLUMN_START;
					continue;
				} else if (ch=='\n') {
					pvt->_state=COLUMN_END;
					column(current.getString());
					current.clear();
					ignore=false;
					pvt->_state=HEADER_END;
					headerEnd();
					continue;
				}
			}
			if (!ignore) {
				current.append(ch);
			}
			continue;
		}
		if (pvt->_state==HEADER_END) {
			pvt->_state=BODY_START;
			bodyStart();
			pvt->_state=ROW_START;
		}
		if (pvt->_state==ROW_START) {
			if (ch=='\r') {
				continue;
			}
			rowStart();
			pvt->_state=FIELD_START;
		}
		if (pvt->_state==FIELD_START) {
			quoted=(ch==pvt->_quote);
			pvt->_state=FIELD;
			if (quoted) {
				continue;
			}
		}
		if (pvt->_state==FIELD) {
			if (!wasescaped) {
				if (quoted && ch==pvt->_quote) {
					ignore=true;
					continue;
				} else if (ch==pvt->_delimiter) {
					pvt->_state=FIELD_END;
					field(current.getString());
					current.clear();
					ignore=false;
					pvt->_state=FIELD_START;
					continue;
				} else if (ch=='\n') {
					pvt->_state=FIELD_END;
					field(current.getString());
					current.clear();
					ignore=false;
					pvt->_state=ROW_END;
					rowEnd();
					pvt->_state=ROW_START;
					continue;
				}
			}
			if (!ignore) {
				current.append(ch);
			}
			continue;
		}
	}

	// document parsed successfully
	pvt->_state=BODY_END;
	bodyEnd();
	return true;
}
