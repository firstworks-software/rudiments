// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

inline
comparator::comparator() {
	multiplier=1;
	natural=false;
	delimiters=".";
}

inline
comparator::~comparator() {
}

inline
void comparator::setReverse(bool reverse) {
	multiplier=(reverse)?-1:1;
}

inline
bool comparator::getReverse() {
	return multiplier==-1;
}

inline
void comparator::setNatural(bool natural) {
	this->natural=natural;
}

inline
bool comparator::getNatural() {
	return natural;
}

inline
void comparator::setNumberDelimiters(const char *delimiters) {
	this->delimiters=delimiters;
}

inline
const char *comparator::getNumberDelimiters() {
	return delimiters;
}

inline
int32_t comparator::compare(char *value1, char *value2) {
	return ((natural)?
		charstring::compareNatural(value1,value2,delimiters):
		charstring::compare(value1,value2))*
		multiplier;
}

inline
int32_t comparator::compare(const char *value1, const char *value2) {
	return ((natural)?
		charstring::compareNatural(value1,value2,delimiters):
		charstring::compare(value1,value2))*
		multiplier;
}

inline
int32_t comparator::compare(unsigned char *value1, unsigned char *value2) {
	return ((natural)?
		charstring::compareNatural((const char *)value1,
						(const char *)value2,
						delimiters):
		charstring::compare((const char *)value1,
						(const char *)value2))*
		multiplier;
}

inline
int32_t comparator::compare(const unsigned char *value1,
				const unsigned char *value2) {
	return ((natural)?
		charstring::compareNatural((const char *)value1,
						(const char *)value2,
						delimiters):
		charstring::compare((const char *)value1,
						(const char *)value2))*
		multiplier;
}

inline
int32_t comparator::compare(char value1, char value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}

inline
int32_t comparator::compare(int16_t value1, int16_t value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}

inline
int32_t comparator::compare(int32_t value1, int32_t value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}

inline
int32_t comparator::compare(int64_t value1, int64_t value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}

inline
int32_t comparator::compare(unsigned char value1, unsigned char value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}

inline
int32_t comparator::compare(uint16_t value1, uint16_t value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}

inline
int32_t comparator::compare(uint32_t value1, uint32_t value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}

inline
int32_t comparator::compare(uint64_t value1, uint64_t value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}

inline
int32_t comparator::compare(float value1, float value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}

inline
int32_t comparator::compare(double value1, double value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}

inline
int32_t comparator::compare(long double value1, long double value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}

inline
int32_t comparator::compare(object *value1, object *value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}

inline
int32_t comparator::compare(void *value1, void *value2) {
	if (value1<value2) {
		return -1*multiplier;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1*multiplier;
	}
}
