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
bool tablecollection<valuetype>::write(output *out) const {
	for (uint64_t i=0; i<getRowCount(); i++) {
		out->printf("row %lld:\n",i);
		for (uint64_t j=0; j<getColumnCount(); j++) {
			out->printf("  col %lld: ",j);
			this->getWriter()->write(getValue(i,j));
			out->write('\n');
		}
	}
	return true;
}
