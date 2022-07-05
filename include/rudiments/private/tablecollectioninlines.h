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
	ssize_t	retval=0;
	for (uint64_t i=0; i<getRowCount(); i++) {
		retval+=out->printf("row %lld:\n",i);
		for (uint64_t j=0; j<getColumnCount(); j++) {
			retval+=out->printf("  col %lld: ",j);
			retval+=this->writeDelegate(out,getValue(i,j));
			retval+=out->write('\n');
		}
	}
	return retval;
}
