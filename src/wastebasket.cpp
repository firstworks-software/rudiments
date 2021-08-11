// Copyright (c) 1999-2018 David Muse
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

void wastebasket::attach(char *value) {
	ATTACHARRAY(char *,"char *",value);
}

void wastebasket::attach(unsigned char *value) {
	ATTACHARRAY(unsigned char *,"unsigned char *",value);
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

void wastebasket::empty() {
	pvt->_dict.clear();
}
