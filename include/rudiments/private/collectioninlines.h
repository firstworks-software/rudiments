// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/character.h>

#include <float.h>

inline
collection::collection() :
	object(),
	managevalues(false),
	managearrayvalues(false),
	managekeys(false),
	managearraykeys(false),
	compptr(&comp) {
}

inline
collection::collection(collection &c) :
	object(c),
	managevalues(c.managevalues),
	managearrayvalues(c.managearrayvalues),
	managekeys(c.managekeys),
	managearraykeys(c.managearraykeys),
	comp(c.comp),
	compptr(&comp) {
}

inline
collection &collection::operator=(collection &c) {
	if (this!=&c) {
		object::operator=(c);
		managevalues=c.managevalues;
		managearrayvalues=c.managearrayvalues;
		managekeys=c.managekeys;
		managearraykeys=c.managearraykeys;
		comp=c.comp;
		compptr=&comp;
	}
	return *this;
}

inline
collection::~collection() {
}

inline
comparator *collection::getComparator() {
	return compptr;
}

inline
void collection::setComparator(comparator *newcomp) {
	if (newcomp) {
		compptr=newcomp;
	} else {
		compptr=&comp;
	}
}

inline
void collection::setManageValues(bool manage) {
	managevalues=manage;
	managearrayvalues=false;
}

inline
bool collection::isReadOnly() {
	return false;
}

inline
bool collection::isBlockBased() {
	return false;
}

inline
uint64_t collection::getBlockSize() {
	return 0;
}

inline
bool collection::isSequentialAccess() {
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

inline
ssize_t collection::writeValue(output *out, const char *value) {
	return out->printf("%s",value);
}

inline
ssize_t collection::writeValue(output *out, char *value) {
	return out->printf("%s",value);
}

inline
ssize_t collection::writeValue(output *out, const wchar_t *value) {
	return writeWideText(out,value);
}

inline
ssize_t collection::writeValue(output *out, wchar_t *value) {
	return writeWideText(out,value);
}

inline
ssize_t collection::writeValue(output *out, char value) {
	return out->printf("%c",value);
}

inline
ssize_t collection::writeValue(output *out, wchar_t value) {

	// The converted text of a null character is empty, so write the
	// one null byte that the char overload writes.
	if (value==L'\0') {
		return out->write('\0');
	}

	// convert and write the character
	wchar_t	str[2];
	str[0]=value;
	str[1]=L'\0';
	return writeWideText(out,str);
}

inline
ssize_t collection::writeValue(output *out, bool value) {
	return out->printf("%s",(value)?"true":"false");
}

inline
ssize_t collection::writeValue(output *out, signed char value) {
	// Why signed char and not int8_t?  Where int8_t is char, such as
	// solaris with _CHAR_IS_SIGNED, an int8_t overload would duplicate
	// the char overload.  signed char is always a distinct type.
	// Where int8_t is char, the char overload writes it as a character.
	return out->printf("%d",(int32_t)value);
}

inline
ssize_t collection::writeValue(output *out, int16_t value) {
	return out->printf("%hd",value);
}

inline
ssize_t collection::writeValue(output *out, int32_t value) {
	return out->printf("%d",value);
}

inline
ssize_t collection::writeValue(output *out, int64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%lld",(long long)value);
	#else
		return out->printf("%ld",(long)value);
	#endif
}

inline
ssize_t collection::writeValue(output *out, const byte_t *value) {
	return out->printf("%s",value);
}

inline
ssize_t collection::writeValue(output *out, byte_t *value) {
	return out->printf("%s",value);
}

inline
ssize_t collection::writeValue(output *out, byte_t value) {
	return out->printf("%c",value);
}

inline
ssize_t collection::writeValue(output *out, uint16_t value) {
	return out->printf("%hu",value);
}

inline
ssize_t collection::writeValue(output *out, uint32_t value) {
	return out->printf("%u",value);
}

inline
ssize_t collection::writeValue(output *out, uint64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%llu",(unsigned long long)value);
	#else
		return out->printf("%lu",(unsigned long)value);
	#endif
}

inline
ssize_t collection::writeValue(output *out, float value) {
	// write a finite value with enough digits to read back as is
	if (value-value==0) {
		return out->printf("%.*g",2+FLT_MANT_DIG*30103/100000,value);
	}
	return out->printf("%f",value);
}

inline
ssize_t collection::writeValue(output *out, double value) {
	// write a finite value with enough digits to read back as is
	if (value-value==0) {
		return out->printf("%.*g",2+DBL_MANT_DIG*30103/100000,value);
	}
	return out->printf("%f",value);
}

inline
ssize_t collection::writeValue(output *out, long double value) {
	// write a finite value with enough digits to read back as is
	if (value-value==0) {
		return out->printf("%.*Lg",2+LDBL_MANT_DIG*30103/100000,value);
	}
	return out->printf("%Lf",value);
}

inline
ssize_t collection::writeValue(output *out, const void *value) {
	return out->printf("%p",value);
}

inline
ssize_t collection::writeValue(output *out, void *value) {
	return out->printf("%p",value);
}

inline
ssize_t collection::writeValue(output *out, const object *value) {
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("%p",(const void *)value);
}

inline
ssize_t collection::writeValue(output *out, object *value) {
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("%p",(const void *)value);
}

template <class valuetype>
inline
ssize_t collection::writeValue(output *out, const valuetype &value) {
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("%p",(const void *)&value);
}

inline
bool collection::isJsonNumber(const char *value) {

	// sign
	if (*value=='-') {
		value++;
	}

	// integer part, with no leading zero
	if (*value=='0') {
		value++;
	} else if (*value>='1' && *value<='9') {
		while (*value>='0' && *value<='9') {
			value++;
		}
	} else {
		return false;
	}

	// fraction, with at least one digit
	if (*value=='.') {
		value++;
		if (!(*value>='0' && *value<='9')) {
			return false;
		}
		while (*value>='0' && *value<='9') {
			value++;
		}
	}

	return !*value;
}

inline
bool collection::isJsonNumber(const wchar_t *value) {

	// sign
	if (*value==L'-') {
		value++;
	}

	// integer part, with no leading zero
	if (*value==L'0') {
		value++;
	} else if (*value>=L'1' && *value<=L'9') {
		while (*value>=L'0' && *value<=L'9') {
			value++;
		}
	} else {
		return false;
	}

	// fraction, with at least one digit
	if (*value==L'.') {
		value++;
		if (!(*value>=L'0' && *value<=L'9')) {
			return false;
		}
		while (*value>=L'0' && *value<=L'9') {
			value++;
		}
	}

	return !*value;
}

inline
ssize_t collection::writeJsonValue(output *out, const char *value) {
	if (!value) {
		return out->write("null");
	} else if (isJsonNumber(value)) {
		return out->printf("%s",value);
	}
	return writeJsonString(out,value,charstring::getLength(value));
}

inline
ssize_t collection::writeJsonValue(output *out, char *value) {
	if (!value) {
		return out->write("null");
	} else if (isJsonNumber(value)) {
		return out->printf("%s",value);
	}
	return writeJsonString(out,value,charstring::getLength(value));
}

inline
ssize_t collection::writeJsonValue(output *out, const wchar_t *value) {
	if (!value) {
		return out->write("null");
	} else if (isJsonNumber(value)) {
		return writeWideText(out,value);
	}
	return writeJsonWideString(out,value,wcharstring::getLength(value));
}

inline
ssize_t collection::writeJsonValue(output *out, wchar_t *value) {
	if (!value) {
		return out->write("null");
	} else if (isJsonNumber(value)) {
		return writeWideText(out,value);
	}
	return writeJsonWideString(out,value,wcharstring::getLength(value));
}

inline
ssize_t collection::writeJsonValue(output *out, char value) {
	if (character::isDigit(value)) {
		return out->printf("%c",value);
	}
	return writeJsonString(out,&value,1);
}

inline
ssize_t collection::writeJsonValue(output *out, wchar_t value) {
	if (character::isDigit(value)) {
		wchar_t	digit[2];
		digit[0]=value;
		digit[1]=L'\0';
		return writeWideText(out,digit);
	}
	return writeJsonWideString(out,&value,1);
}

inline
ssize_t collection::writeJsonValue(output *out, bool value) {
	return out->printf("%s",(value)?"true":"false");
}

inline
ssize_t collection::writeJsonValue(output *out, signed char value) {
	// Why signed char and not int8_t?  Where int8_t is char, such as
	// solaris with _CHAR_IS_SIGNED, an int8_t overload would duplicate
	// the char overload.  signed char is always a distinct type.
	// Where int8_t is char, the char overload writes it as a character.
	return out->printf("%d",(int32_t)value);
}

inline
ssize_t collection::writeJsonValue(output *out, int16_t value) {
	return out->printf("%hd",value);
}

inline
ssize_t collection::writeJsonValue(output *out, int32_t value) {
	return out->printf("%d",value);
}

inline
ssize_t collection::writeJsonValue(output *out, int64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%lld",(long long)value);
	#else
		return out->printf("%ld",(long)value);
	#endif
}

inline
ssize_t collection::writeJsonValue(output *out, const byte_t *value) {
	if (!value) {
		return out->write("null");
	} else if (isJsonNumber((const char *)value)) {
		return out->printf("%s",value);
	}
	return writeJsonString(out,(const char *)value,
				charstring::getLength((const char *)value));
}

inline
ssize_t collection::writeJsonValue(output *out, byte_t *value) {
	if (!value) {
		return out->write("null");
	} else if (isJsonNumber((const char *)value)) {
		return out->printf("%s",value);
	}
	return writeJsonString(out,(const char *)value,
				charstring::getLength((const char *)value));
}

inline
ssize_t collection::writeJsonValue(output *out, byte_t value) {
	if (character::isDigit(value)) {
		return out->printf("%c",value);
	}
	return writeJsonString(out,(const char *)&value,1);
}

inline
ssize_t collection::writeJsonValue(output *out, uint16_t value) {
	return out->printf("%hu",value);
}

inline
ssize_t collection::writeJsonValue(output *out, uint32_t value) {
	return out->printf("%u",value);
}

inline
ssize_t collection::writeJsonValue(output *out, uint64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%llu",(unsigned long long)value);
	#else
		return out->printf("%lu",(unsigned long)value);
	#endif
}

inline
ssize_t collection::writeJsonValue(output *out, float value) {
	// json has no nan or inf
	if (value!=value || value-value!=value-value) {
		return out->write("null");
	}
	return writeValue(out,value);
}

inline
ssize_t collection::writeJsonValue(output *out, double value) {
	// json has no nan or inf
	if (value!=value || value-value!=value-value) {
		return out->write("null");
	}
	return writeValue(out,value);
}

inline
ssize_t collection::writeJsonValue(output *out, long double value) {
	// json has no nan or inf
	if (value!=value || value-value!=value-value) {
		return out->write("null");
	}
	return writeValue(out,value);
}

inline
ssize_t collection::writeJsonValue(output *out, const void *value) {
	if (!value) {
		return out->write("null");
	}
	return out->printf("\"%p\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, void *value) {
	if (!value) {
		return out->write("null");
	}
	return out->printf("\"%p\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, const object *value) {
	if (!value) {
		return out->write("null");
	}
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("\"%p\"",(const void *)value);
}

inline
ssize_t collection::writeJsonValue(output *out, object *value) {
	if (!value) {
		return out->write("null");
	}
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("\"%p\"",(const void *)value);
}

template <class valuetype>
inline
ssize_t collection::writeJsonValue(output *out, const valuetype &value) {
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("\"%p\"",(const void *)&value);
}

inline
ssize_t collection::writeJsonString(output *out,
					const char *value,
					uint64_t length) {

	// encode the value
	char		*encoded;
	uint64_t	encodedlen;
	charstring::jsonEncode(value,length,&encoded,&encodedlen);
	if (!encoded) {
		return RESULT_ERROR;
	}

	// write the encoded value
	ssize_t	result=out->write(encoded,(size_t)encodedlen);
	delete[] encoded;
	return result;
}

inline
ssize_t collection::writeJsonWideString(output *out,
					const wchar_t *value,
					size_t length) {

	// check the size, including the quotes and the terminator
	if (length>(~((size_t)0)/sizeof(wchar_t)-3)/6) {
		return RESULT_ERROR;
	}

	// size the encoded value
	size_t	size=3;
	for (size_t i=0; i<length; i++) {
		if (value[i]==L'"' || value[i]==L'\\') {
			size+=2;
		} else if ((uint32_t)value[i]<0x20) {
			size+=6;
		} else {
			size++;
		}
	}

	// encode the value
	const wchar_t	*hex=L"0123456789abcdef";
	wchar_t		*encoded=new wchar_t[size];
	wchar_t		*e=encoded;
	*e++=L'"';
	for (size_t i=0; i<length; i++) {
		wchar_t	c=value[i];
		if (c==L'"' || c==L'\\') {
			*e++=L'\\';
			*e++=c;
		} else if ((uint32_t)c<0x20) {
			*e++=L'\\';
			*e++=L'u';
			*e++=L'0';
			*e++=L'0';
			*e++=hex[(c>>4)&0x0F];
			*e++=hex[c&0x0F];
		} else {
			*e++=c;
		}
	}
	*e++=L'"';
	*e=L'\0';

	// write the encoded value
	ssize_t	result=writeWideText(out,encoded);
	delete[] encoded;
	return result;
}

inline
ssize_t collection::writeWideText(output *out, const wchar_t *value) {

	// Where wide printf is unsupported, the other wide writers fail, so
	// fail here too, rather than rely on %ls in the narrow printf.
	if (!wcharstring::supportsPrintf()) {
		return RESULT_ERROR;
	}

	// convert the value
	char	*converted=NULL;
	ssize_t	length=charstring::printf(&converted,"%ls",value);
	if (length<0) {
		delete[] converted;
		return RESULT_ERROR;
	}

	// write the converted value
	ssize_t	result=out->write(converted,(size_t)length);
	delete[] converted;
	return result;
}

inline
bool collection::writeJsonKey(ssize_t *retval, output *out,
						stringbuffer *buf,
						float key) {

	// format the key into the buffer
	buf->clear();
	ssize_t	result=writeValue(buf,key);
	if (result<0) {
		return incOrErr(retval,RESULT_ERROR);
	}
	return writeJsonKeyBuffer(retval,out,buf);
}

inline
bool collection::writeJsonKey(ssize_t *retval, output *out,
						stringbuffer *buf,
						double key) {

	// format the key into the buffer
	buf->clear();
	ssize_t	result=writeValue(buf,key);
	if (result<0) {
		return incOrErr(retval,RESULT_ERROR);
	}
	return writeJsonKeyBuffer(retval,out,buf);
}

inline
bool collection::writeJsonKey(ssize_t *retval, output *out,
						stringbuffer *buf,
						long double key) {

	// format the key into the buffer
	buf->clear();
	ssize_t	result=writeValue(buf,key);
	if (result<0) {
		return incOrErr(retval,RESULT_ERROR);
	}
	return writeJsonKeyBuffer(retval,out,buf);
}

template <class keytype>
inline
bool collection::writeJsonKey(ssize_t *retval, output *out,
						stringbuffer *buf,
						const keytype &key) {

	// format the key into the buffer
	buf->clear();
	if (writeJsonValue(buf,key)<0) {
		return incOrErr(retval,RESULT_ERROR);
	}
	return writeJsonKeyBuffer(retval,out,buf);
}

inline
bool collection::writeJsonKeyBuffer(ssize_t *retval, output *out,
						stringbuffer *buf) {

	// a json string starts with a quote, so write the buffer as is
	const char	*value=(const char *)buf->getBuffer();
	uint64_t	length=buf->getSize();
	if (length && value[0]=='"') {
		return incOrErr(retval,out->write(value,(size_t)length),
							(ssize_t)length);
	}

	// quote anything else
	char		*encoded;
	uint64_t	encodedlen;
	charstring::jsonEncode(value,length,&encoded,&encodedlen);
	if (!encoded) {
		return incOrErr(retval,RESULT_ERROR);
	}
	ssize_t	result=out->write(encoded,(size_t)encodedlen);
	delete[] encoded;
	return incOrErr(retval,result,(ssize_t)encodedlen);
}

template <class valuetype>
inline
bool collection::writeJsonValue(ssize_t *retval, output *out,
						stringbuffer *buf,
						const valuetype &value) {

	// format the value into the buffer and write the buffer
	buf->clear();
	if (writeJsonValue(buf,value)<0) {
		return incOrErr(retval,RESULT_ERROR);
	}
	return incOrErr(retval,out->write(buf->getBuffer(),buf->getSize()),
							buf->getSize());
}

template <class valuetype>
inline
bool collection::writeValue(ssize_t *retval, output *out,
						stringbuffer *buf,
						const valuetype &value) {

	// format the value into the buffer and write the buffer
	buf->clear();
	if (writeValue(buf,value)<0) {
		return incOrErr(retval,RESULT_ERROR);
	}
	return incOrErr(retval,out->write(buf->getBuffer(),buf->getSize()),
							buf->getSize());
}
