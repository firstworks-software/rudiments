// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

inline
collection::collection() : object() {
	managevalues=false;
	managearrayvalues=false;
	managekeys=false;
	managearraykeys=false;
}

inline
collection::collection(const collection &c) : object(c) {
	managevalues=false;
	managearrayvalues=false;
	managekeys=false;
	managearraykeys=false;
}

inline
collection::~collection() {
}

inline
void collection::setManageValues(bool manage) {
	managevalues=manage;
	managearrayvalues=false;
}

inline
bool collection::getIsReadOnly() {
	return false;
}

inline
bool collection::getIsBlockBased() {
	return false;
}

inline
uint64_t collection::getBlockSize() {
	return 0;
}

inline
bool collection::getIsSequentialAccess() {
	return false;
}

inline
bool collection::getManageValues() {
	return managevalues;
}

inline
void collection::setManageArrayValues(bool manage) {
	managearrayvalues=manage;
	managevalues=false;
}

inline
bool collection::getManageArrayValues() {
	return managearrayvalues;
}

inline
void collection::setManageKeys(bool manage) {
	managekeys=manage;
	managearraykeys=false;
}

inline
bool collection::getManageKeys() {
	return managekeys;
}

inline
void collection::setManageArrayKeys(bool manage) {
	managearraykeys=manage;
	managekeys=false;
}

inline
bool collection::getManageArrayKeys() {
	return managearraykeys;
}
