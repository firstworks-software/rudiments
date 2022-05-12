// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/csvsax.h>
//#define DEBUG_MESSAGES
#include <rudiments/debugprint.h>

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
		csvstate		_state;
		char			_quote;
		char			_delimiter;
};

csvsax::csvsax() : sax() {
	pvt=new csvsaxprivate;
	pvt->_quote='"';
	pvt->_delimiter=',';
	reset();
}

csvsax::~csvsax() {
	delete pvt;
}

void csvsax::reset() {
	pvt->_state=HEADER_START;
	sax::reset();
}

void csvsax::setQuote(char quote) {
	pvt->_quote=quote;
}

char csvsax::getQuote() const {
	return pvt->_quote;
}

void csvsax::setDelimiter(char delimiter) {
	pvt->_delimiter=delimiter;
}

char csvsax::getDelimiter() const {
	return pvt->_delimiter;
}

bool csvsax::headerStart() {
	// by default, just return success
	debugPrintf("headerStart {\n");
	return true;
}

bool csvsax::column(const char *name, bool quoted) {
	// by default, just return success
	debugPrintf("    column: \"%s\" (%squoted)\n",name,(quoted)?"":"not ");
	return true;
}

bool csvsax::headerEnd() {
	// by default, just return success
	debugPrintf("}\n");
	return true;
}

bool csvsax::bodyStart() {
	// by default, just return success
	debugPrintf("bodyStart {\n");
	return true;
}

bool csvsax::rowStart() {
	// by default, just return success
	debugPrintf("    rowStart {\n");
	return true;
}

bool csvsax::field(const char *value, bool quoted) {
	// by default, just return success
	debugPrintf("        field: \"%s\" (%squoted)\n",
					value,(quoted)?"":"not ");
	return true;
}

bool csvsax::rowEnd() {
	// by default, just return success
	debugPrintf("    }\n");
	return true;
}

bool csvsax::bodyEnd() {
	// by default, just return success
	debugPrintf("}\n");
	return true;
}

bool csvsax::parse() {

	stringbuffer	current;
	bool		quoted=false;
	bool		inquotes=false;
	bool		ignore=false;
	bool		keepchar=false;
	char		ch='\0';

	for (;;) {

		// get a character
		if (!keepchar) {
			ch=getCharacter();
		} else {
			keepchar=false;
		}

		// at the very beginning of the file, skip leading \n or \r
		if (pvt->_state==HEADER_START) {
			if (ch=='\n' || ch=='\r') {
				continue;
			} else if (ch=='\0') {
				// if the file was empty then create an empty
				// header and empty body
				headerStart();
				pvt->_state=HEADER_END;
				headerEnd();
				pvt->_state=BODY_START;
				bodyStart();
				pvt->_state=BODY_END;
				bodyEnd();
				return true;
			}
		}

		// handle end of file/string
		if (ch=='\0') {
			break;
		}

		// handle various states
		if (pvt->_state==HEADER_START) {
			headerStart();
			pvt->_state=COLUMN_START;
		}
		if (pvt->_state==COLUMN_START) {
			quoted=(ch==pvt->_quote);
			pvt->_state=COLUMN;
			if (quoted) {
				inquotes=true;
				continue;
			}
		}
		if (pvt->_state==COLUMN) {
			if (inquotes) {
				if (ch==pvt->_quote) {
					ch=getCharacter();
					if (ch!=pvt->_quote) {
						inquotes=false;
						keepchar=true;
						ignore=true;
						continue;
					}
				}
			} else {
				if (ch==pvt->_delimiter) {
					pvt->_state=COLUMN_END;
					column(current.getString(),quoted);
					current.clear();
					ignore=false;
					pvt->_state=COLUMN_START;
					continue;
				} else if (ch=='\r' || ch=='\n') {
					pvt->_state=COLUMN_END;
					column(current.getString(),quoted);
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
			if (ch=='\r' || ch=='\n') {
				continue;
			}
			rowStart();
			pvt->_state=FIELD_START;
		}
		if (pvt->_state==FIELD_START) {
			quoted=(ch==pvt->_quote);
			pvt->_state=FIELD;
			if (quoted) {
				inquotes=true;
				continue;
			}
		}
		if (pvt->_state==FIELD) {
			if (inquotes) {
				if (ch==pvt->_quote) {
					ch=getCharacter();
					if (ch!=pvt->_quote) {
						inquotes=false;
						keepchar=true;
						ignore=true;
						continue;
					}
				}
			} else {
				if (ch==pvt->_delimiter) {
					pvt->_state=FIELD_END;
					field(current.getString(),quoted);
					current.clear();
					ignore=false;
					pvt->_state=FIELD_START;
					continue;
				} else if (ch=='\r' || ch=='\n') {
					pvt->_state=FIELD_END;
					field(current.getString(),quoted);
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
