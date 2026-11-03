// Copyright (c) David Muse
// See the COPYING file for more information

template <class valuetype>
inline
tablecollection<valuetype>::tablecollection() :
	collection(),
	copycolumnnames(false) {
}

template <class valuetype>
inline
tablecollection<valuetype>::tablecollection(tablecollection &c) :
	collection(c),
	copycolumnnames(c.copycolumnnames) {
}

template <class valuetype>
inline
tablecollection<valuetype> &tablecollection<valuetype>::
					operator=(tablecollection &c) {
	if (this!=&c) {
		collection::operator=(c);
		copycolumnnames=c.copycolumnnames;
	}
	return *this;
}

template <class valuetype>
inline
tablecollection<valuetype>::~tablecollection() {
}

template <class valuetype>
inline
const char *tablecollection<valuetype>::getType() {
	return "table";
}

template <class valuetype>
inline
uint64_t tablecollection<valuetype>::getCount() {
	return getRowCount()*getColumnCount();
}

template <class valuetype>
inline
void tablecollection<valuetype>::setCopyColumnNames(bool copy) {
	copycolumnnames=copy;
}

template <class valuetype>
inline
bool tablecollection<valuetype>::getCopyColumnNames() {
	return copycolumnnames;
}

template <class valuetype>
inline
ssize_t tablecollection<valuetype>::write() {
	return write(&stdoutput);
}

template <class valuetype>
inline
ssize_t tablecollection<valuetype>::write(output *out) {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t tablecollection<valuetype>::writeJson() {
	return writeJson(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t tablecollection<valuetype>::writeJson(bool indent) {
	return writeJson(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t tablecollection<valuetype>::writeJson(output *out) {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t tablecollection<valuetype>::writeJson(output *out, bool indent) {

	ssize_t		retval=0;
	stringbuffer	buf;

	if (!(incOrErr(&retval,out->write('{'),1) &&
		((indent)?incOrErr(&retval,out->write("\n	",2),2):true) &&
		incOrErr(&retval,out->write("\"c\":",4),4) &&
		((indent)?incOrErr(&retval,out->write(' '),1):true) &&
		incOrErr(&retval,out->write('['),1)) && retval>-1) {
		retval=RESULT_ERROR;
	}

	for (uint64_t i=0; i<getColumnCount() && retval>-1; i++) {
		if (!(((i)?incOrErr(&retval,out->write(','),1):true) &&
			((indent)?incOrErr(&retval,
					out->write("\n		",3),3):true) &&
			this->writeJsonValue(&retval,out,&buf,
						getColumnName(i))) &&
			retval>-1) {
			retval=RESULT_ERROR;
		}
	}

	if (!(retval>-1 &&
		((indent)?incOrErr(&retval,out->write("\n	",2),2):true) &&
		incOrErr(&retval,out->write("],",2),2) &&
		((indent)?incOrErr(&retval,out->write("\n	",2),2):true) &&
		incOrErr(&retval,out->write("\"r\":",4),4) &&
		((indent)?incOrErr(&retval,out->write(' '),1):true) &&
		incOrErr(&retval,out->write('['),1)) && retval>-1) {
		retval=RESULT_ERROR;
	}

	for (uint64_t i=0; retval>-1; i++) {

		// past the end of the current block...
		if (i>=getRowCount()) {

			// bail if there are no more blocks
			if (getAllRowsAvailable() || !getColumnCount()) {
				break;
			}

			// Reading a row past the current block makes a
			// block-based source load the next block.  Bail if
			// the row still doesn't exist.
			getValue(i,(uint64_t)0);
			if (getRowCount()<=i) {
				break;
			}
		}

		if (!(((i)?incOrErr(&retval,out->write(','),1):true) &&
			((indent)?incOrErr(&retval,
					out->write("\n		",3),3):true) &&
			incOrErr(&retval,out->write('['),1)) && retval>-1) {
			retval=RESULT_ERROR;
		}

		for (uint64_t j=0; j<getColumnCount() && retval>-1; j++) {
			if (!(((j)?incOrErr(&retval,
					out->write(','),1):true) &&
				((indent)?incOrErr(&retval,
					out->write("\n			",4),4):true) &&
				this->writeJsonValue(&retval,out,&buf,
							getValue(i,j))) &&
				retval>-1) {
				retval=RESULT_ERROR;
			}
		}

		if (!(retval>-1 &&
			((indent)?incOrErr(&retval,
					out->write("\n		",3),3):true) &&
			incOrErr(&retval,out->write("]"),1)) && retval>-1) {
			retval=RESULT_ERROR;
		}
	}

	if (!(retval>-1 &&
		((indent)?incOrErr(&retval,out->write("\n	",2),2):true) &&
		incOrErr(&retval,out->write("]"),1) &&
		((indent)?incOrErr(&retval,out->write('\n'),1):true) &&
		incOrErr(&retval,out->write('}'),1) &&
		((indent)?incOrErr(&retval,out->write('\n'),1):true)) &&
		retval>-1) {
		retval=RESULT_ERROR;
	}

	return retval;
}
