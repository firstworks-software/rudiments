// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

template <class valuetype>
inline
tablecollection<valuetype>::tablecollection() :
	collection(),
	copycolumnnames(false) {
}

template <class valuetype>
inline
tablecollection<valuetype>::tablecollection(const tablecollection &c) :
	collection(c),
	copycolumnnames(c.copycolumnnames) {
}

template <class valuetype>
inline
tablecollection<valuetype> &tablecollection<valuetype>::
				operator=(const tablecollection &c) {
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
const char *tablecollection<valuetype>::getType() const {
	return "table";
}

template <class valuetype>
inline
void tablecollection<valuetype>::setCopyColumnNames(bool copy) {
	copycolumnnames=copy;
}

template <class valuetype>
inline
bool tablecollection<valuetype>::getCopyColumnNames() const {
	return copycolumnnames;
}

template <class valuetype>
inline
ssize_t tablecollection<valuetype>::write() const {
	return write(&stdoutput);
}

template <class valuetype>
inline
ssize_t tablecollection<valuetype>::write(output *out) const {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t tablecollection<valuetype>::writeJson() const {
	return writeJson(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t tablecollection<valuetype>::writeJson(bool indent) const {
	return writeJson(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t tablecollection<valuetype>::writeJson(output *out) const {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t tablecollection<valuetype>::writeJson(output *out, bool indent) const {

	ssize_t	retval=0;

	incOrErr(&retval,out->write('{'),1) &&
	((indent)?incOrErr(&retval,out->write("\n	",2),2):true) &&
	incOrErr(&retval,out->write("\"c\":",4),4) &&
	((indent)?incOrErr(&retval,out->write(' '),1):true) &&
	incOrErr(&retval,out->write('['),1);

	for (uint64_t i=0; i<getColumnCount() && retval>-1; i++) {
		((i)?incOrErr(&retval,out->write(','),1):true) &&
		((indent)?incOrErr(&retval,
				out->write("\n		",3),3):true) &&
		incOrErr(&retval,this->writeJsonValue(out,getColumnName(i)));
	}

	((indent)?incOrErr(&retval,out->write("\n	",2),2):true) &&
	incOrErr(&retval,out->write("],",2),2) &&
	((indent)?incOrErr(&retval,out->write("\n	",2),2):true) &&
	incOrErr(&retval,out->write("\"r\":",4),4) &&
	((indent)?incOrErr(&retval,out->write(' '),1):true) &&
	incOrErr(&retval,out->write('['),1);

	for (uint64_t i=0; i<getRowCount() && retval>-1; i++) {

		((i)?incOrErr(&retval,out->write(','),1):true) &&
		((indent)?incOrErr(&retval,
				out->write("\n		",3),3):true) &&
		incOrErr(&retval,out->write('['),1);

		for (uint64_t j=0; j<getColumnCount() && retval>-1; j++) {
			((j)?incOrErr(&retval,out->write(','),1):true) &&
			((indent)?incOrErr(&retval,
				out->write("\n			",4),4):true) &&
			incOrErr(&retval,
				this->writeJsonValue(out,getValue(i,j)));
		}

		((indent)?incOrErr(&retval,
				out->write("\n		",3),3):true) &&
		incOrErr(&retval,out->write("]"),1);
	}

	((indent)?incOrErr(&retval,out->write("\n	",2),2):true) &&
	incOrErr(&retval,out->write("]"),1) &&
	((indent)?incOrErr(&retval,out->write('\n'),1):true);
	incOrErr(&retval,out->write('}'),1) &&
	((indent)?incOrErr(&retval,out->write('\n'),1):true);

	return retval;
}

template< class valuetype >
inline
ssize_t tablecollection<valuetype>::writeXml() const {
	return writeXml(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t tablecollection<valuetype>::writeXml(bool indent) const {
	return writeXml(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t tablecollection<valuetype>::writeXml(output *out) const {
	return writeXml(out,true);
}

template< class valuetype >
inline
ssize_t tablecollection<valuetype>::writeXml(output *out, bool indent) const {
	// FIXME: implement this
	return RESULT_ERROR;
}
