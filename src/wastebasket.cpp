// Copyright (c) David Muse
// See the COPYING file for more information.

#include <rudiments/wastebasket.h>
#include <rudiments/singlylinkedlist.h>
#include <rudiments/dictionary.h>

class wastebasketprivate {
	friend class wastebasket;
	private:
		dictionary<const char *,collection *>	_dict;
};

wastebasket::wastebasket() : object() {
	pvt=new wastebasketprivate;
	pvt->_dict.setManageValues(true);
}

wastebasket::~wastebasket() {
	delete pvt;
}

#define ATTACH(type,typestring,value) \
	collection	*c=pvt->_dict.getValue(typestring); \
	if (!c) { \
		singlylinkedlist<type>	*l=new singlylinkedlist<type>(); \
		l->setManageValues(true); \
		l->append(value); \
		pvt->_dict.setValue(typestring,l); \
	} else { \
		((singlylinkedlist<type> *)c)->append(value); \
	}

#define ATTACHARRAY(type,typestring,value) \
	collection	*c=pvt->_dict.getValue(typestring); \
	if (!c) { \
		singlylinkedlist<type>	*l=new singlylinkedlist<type>(); \
		l->setManageArrayValues(true); \
		l->append(value); \
		pvt->_dict.setValue(typestring,l); \
	} else { \
		((singlylinkedlist<type> *)c)->append(value); \
	}

#define DETACH(type,typestring,value) \
	singlylinkedlist<type>	*l=(singlylinkedlist<type> *) \
					pvt->_dict.getValue(typestring); \
	if (!l) { \
		return; \
	} \
	for (listnode<type> *node=l->getFirst(); \
					node; node=l->getNext(node)) { \
		if (node->getValue()==value) { \
			l->detach(node); \
			delete node; \
			return; \
		} \
	}

void wastebasket::attach(char *value) {
	ATTACHARRAY(char *,"char *",value);
}

void wastebasket::attach(wchar_t *value) {
	ATTACHARRAY(wchar_t *,"wchar_t *",value);
}

void wastebasket::attach(byte_t *value) {
	ATTACHARRAY(byte_t *,"byte_t *",value);
}

void wastebasket::attach(int16_t *value) {
	ATTACHARRAY(int16_t *,"int16_t *",value);
}

void wastebasket::attach(int32_t *value) {
	ATTACHARRAY(int32_t *,"int32_t *",value);
}

void wastebasket::attach(int64_t *value) {
	ATTACHARRAY(int64_t *,"int64_t *",value);
}

void wastebasket::attach(uint16_t *value) {
	ATTACHARRAY(uint16_t *,"uint16_t *",value);
}

void wastebasket::attach(uint32_t *value) {
	ATTACHARRAY(uint32_t *,"uint32_t *",value);
}

void wastebasket::attach(uint64_t *value) {
	ATTACHARRAY(uint64_t *,"uint64_t *",value);
}

void wastebasket::attach(float *value) {
	ATTACHARRAY(float *,"float *",value);
}

void wastebasket::attach(double *value) {
	ATTACHARRAY(double *,"double *",value);
}

void wastebasket::attach(long double *value) {
	ATTACHARRAY(long double *,"long double *",value);
}

void wastebasket::attach(object *o) {
	ATTACH(object *,"object *",o);
}

void wastebasket::attach(object **o) {
	ATTACHARRAY(object **,"object **",o);
}

void wastebasket::detach(char *value) {
	DETACH(char *,"char *",value);
}

void wastebasket::detach(wchar_t *value) {
	DETACH(wchar_t *,"wchar_t *",value);
}

void wastebasket::detach(byte_t *value) {
	DETACH(byte_t *,"byte_t *",value);
}

void wastebasket::detach(int16_t *value) {
	DETACH(int16_t *,"int16_t *",value);
}

void wastebasket::detach(int32_t *value) {
	DETACH(int32_t *,"int32_t *",value);
}

void wastebasket::detach(int64_t *value) {
	DETACH(int64_t *,"int64_t *",value);
}

void wastebasket::detach(uint16_t *value) {
	DETACH(uint16_t *,"uint16_t *",value);
}

void wastebasket::detach(uint32_t *value) {
	DETACH(uint32_t *,"uint32_t *",value);
}

void wastebasket::detach(uint64_t *value) {
	DETACH(uint64_t *,"uint64_t *",value);
}

void wastebasket::detach(float *value) {
	DETACH(float *,"float *",value);
}

void wastebasket::detach(double *value) {
	DETACH(double *,"double *",value);
}

void wastebasket::detach(long double *value) {
	DETACH(long double *,"long double *",value);
}

void wastebasket::detach(object *o) {
	DETACH(object *,"object *",o);
}

void wastebasket::detach(object **o) {
	DETACH(object **,"object **",o);
}

void wastebasket::empty() {
	pvt->_dict.clear();
}

ssize_t wastebasket::write() {
	return write(&stdoutput);
}

ssize_t wastebasket::write(output *out) {
	ssize_t	retval=0;
	for (listnode<const char *> *node=pvt->_dict.getKeys()->getFirst();
						node; node=node->getNext()) {
		if (!(incOrErr(&retval,out->write(node->getValue())) &&
			incOrErr(&retval,out->write(":\n",2),2) &&
			incOrErr(&retval,pvt->_dict.getValue(
						node->getValue())->
						write(out)))) {
			break;
		}
	}
	return retval;
}
