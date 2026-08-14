// Copyright (c) David Muse
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
	RECORD_START,
	FIELD_START,
	FIELD,
	FIELD_END,
	RECORD_END,
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

bool csvsax::reset() {
	pvt->_state=HEADER_START;
	return sax::reset();
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

bool csvsax::column(const char *name, size_t namelength, bool quoted) {
	// by default, forward to the length-less version
	return column(name,quoted);
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

bool csvsax::recordStart() {
	// by default, just return success
	debugPrintf("    recordStart {\n");
	return true;
}

bool csvsax::field(const char *value, bool quoted) {
	// by default, just return success
	debugPrintf("        field: \"%s\" (%squoted)\n",
					value,(quoted)?"":"not ");
	return true;
}

bool csvsax::field(const char *value, size_t valuelength, bool quoted) {
	// by default, forward to the length-less version
	return field(value,quoted);
}

bool csvsax::recordEnd() {
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

	pvt->_state=HEADER_START;

	stringbuffer	current;
	bool		quoted=false;
	bool		inquotes=false;
	bool		ignore=false;
	bool		keepchar=false;
	char		ch='\0';
	bool		eof=false;

	for (;;) {

		// get a character
		if (!keepchar) {
			eof=!getCharacter(&ch);
		} else {
			keepchar=false;
		}

		// at the very beginning of the file, skip leading \n or \r
		if (pvt->_state==HEADER_START) {
			if (ch=='\n' || ch=='\r') {
				continue;
			} else if (eof) {
				// if the file was empty then create an empty
				// header and empty body
				if (!headerStart()) {
					return false;
				}
				pvt->_state=HEADER_END;
				if (!headerEnd()) {
					return false;
				}
				pvt->_state=BODY_START;
				if (!bodyStart()) {
					return false;
				}
				pvt->_state=BODY_END;
				return bodyEnd();
			}
		}

		// handle end of file/string
		if (eof) {
			break;
		}

		// handle various states
		if (pvt->_state==HEADER_START) {
			if (!headerStart()) {
				return false;
			}
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
					eof=!getCharacter(&ch);
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
					if (!column(current.getString(),
							current.getSize(),
							quoted)) {
						return false;
					}
					current.clear();
					ignore=false;
					pvt->_state=COLUMN_START;
					continue;
				} else if (ch=='\r' || ch=='\n') {
					pvt->_state=COLUMN_END;
					if (!column(current.getString(),
							current.getSize(),
							quoted)) {
						return false;
					}
					current.clear();
					ignore=false;
					pvt->_state=HEADER_END;
					if (!headerEnd()) {
						return false;
					}
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
			if (!bodyStart()) {
				return false;
			}
			pvt->_state=RECORD_START;
		}
		if (pvt->_state==RECORD_START) {
			if (ch=='\r' || ch=='\n') {
				continue;
			}
			if (!recordStart()) {
				return false;
			}
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
					eof=!getCharacter(&ch);
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
					if (!field(current.getString(),
							current.getSize(),
							quoted)) {
						return false;
					}
					current.clear();
					ignore=false;
					pvt->_state=FIELD_START;
					continue;
				} else if (ch=='\r' || ch=='\n') {
					pvt->_state=FIELD_END;
					if (!field(current.getString(),
							current.getSize(),
							quoted)) {
						return false;
					}
					current.clear();
					ignore=false;
					pvt->_state=RECORD_END;
					if (!recordEnd()) {
						return false;
					}
					pvt->_state=RECORD_START;
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
	return bodyEnd();
}
