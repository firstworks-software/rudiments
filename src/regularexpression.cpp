// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/regularexpression.h>
#include <rudiments/bytestring.h>

#if defined(RUDIMENTS_HAS_PCRE2)
	#define PCRE2_CODE_UNIT_WIDTH 8
	#include <pcre2.h>
#elif defined(RUDIMENTS_HAS_PCRE)
	#include <pcre.h>
#elif defined(RUDIMENTS_HAVE_REGCOMP)
	#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
		#include <sys/types.h>
	#endif
	#include <rudiments/charstring.h>
	extern "C" {
		#include <regex.h>
	}
#else
	#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
		#include <sys/types.h>
	#endif
	#include <rudiments/charstring.h>
	#include "regex.h"
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <stdio.h>
#if defined(RUDIMENTS_HAS_PCRE2) || defined(RUDIMENTS_HAS_PCRE)
	#include <rudiments/charstring.h>
#endif

class regularexpressionprivate {
	friend class regularexpression;
	private:
		bool		_null;
		const char	*_pattern;

		#if defined(RUDIMENTS_HAS_PCRE2)
			pcre2_code		*_expr;
			pcre2_match_data	*_matchdata;
		#elif defined(RUDIMENTS_HAS_PCRE)
			pcre		*_expr;
			pcre_extra	*_extra;
		#else
			regex_t	_expr;
			bool	_compiled;
			char	*_strcopy;
		#endif
	
		#define RUDIMENTS_REGEX_MATCHES 128
		int32_t		_substringcount;
		int32_t		_matchcount;
		const char	*_str;

		#if defined(RUDIMENTS_HAS_PCRE2)
		PCRE2_SIZE	*_matches;
		#elif defined(RUDIMENTS_HAS_PCRE)
		int32_t		_matches[RUDIMENTS_REGEX_MATCHES*3];
		#else
		regmatch_t	_matches[RUDIMENTS_REGEX_MATCHES];
		#endif
};

regularexpression::regularexpression() : object() {
	construct();
}

regularexpression::regularexpression(const char *pattern) : object() {
	construct();
	setPattern(pattern);
}

void regularexpression::construct() {
	pvt=new regularexpressionprivate;
	#if defined(RUDIMENTS_HAS_PCRE2)
		pvt->_expr=NULL;
		// The ovector lives inside the match data and is never
		// reallocated, so this pointer is good for the life of
		// the object.
		pvt->_matchdata=pcre2_match_data_create(
					RUDIMENTS_REGEX_MATCHES,NULL);
		pvt->_matches=pcre2_get_ovector_pointer(pvt->_matchdata);
	#elif defined(RUDIMENTS_HAS_PCRE)
		pvt->_expr=NULL;
		pvt->_extra=NULL;
	#else
		bytestring::zero(&pvt->_expr,sizeof(pvt->_expr));
		pvt->_compiled=false;
		pvt->_strcopy=NULL;
	#endif
	pvt->_null=false;
	pvt->_pattern=NULL;
	pvt->_substringcount=0;
	pvt->_matchcount=0;
	pvt->_str=NULL;
	#if defined(RUDIMENTS_HAS_PCRE2)
		bytestring::zero(pvt->_matches,
				sizeof(PCRE2_SIZE)*RUDIMENTS_REGEX_MATCHES*2);
	#else
		bytestring::zero(pvt->_matches,sizeof(pvt->_matches));
	#endif
}

regularexpression::~regularexpression() {
	#if defined(RUDIMENTS_HAS_PCRE2)
		if (pvt->_expr) {
			pcre2_code_free(pvt->_expr);
		}
		pcre2_match_data_free(pvt->_matchdata);
	#elif defined(RUDIMENTS_HAS_PCRE)
		if (pvt->_expr) {
			pcre_free(pvt->_expr);
		}
		if (pvt->_extra) {
			pcre_free(pvt->_extra);
		}
	#else
		// regfree() takes the regex_t by address, so there is no
		// pointer to test like the PCRE arms have.  _compiled stands
		// in for it.
		if (pvt->_compiled) {
			regfree(&pvt->_expr);
		}
		delete[] pvt->_strcopy;
	#endif
	delete pvt;
}

bool regularexpression::setPattern(const char *pattern) {
	pvt->_null=false;
	pvt->_substringcount=0;
	pvt->_matchcount=0;
	if (!pattern) {
		pvt->_null=true;
		return true;
	}
	pvt->_pattern=pattern;

	// compile, and get the count of capture groups from the pattern
	int32_t	capturecount=0;
	#if defined(RUDIMENTS_HAS_PCRE2)
		if (pvt->_expr) {
			pcre2_code_free(pvt->_expr);
		}
		int32_t		error;
		PCRE2_SIZE	erroroffset;
		if (!(pvt->_expr=pcre2_compile((PCRE2_SPTR)pattern,
						PCRE2_ZERO_TERMINATED,0,
						&error,&erroroffset,
						NULL))) {
			return false;
		}
		uint32_t	cc=0;
		pcre2_pattern_info(pvt->_expr,PCRE2_INFO_CAPTURECOUNT,&cc);
		capturecount=(int32_t)cc;
	#elif defined(RUDIMENTS_HAS_PCRE)
		if (pvt->_expr) {
			pcre_free(pvt->_expr);
		}
		if (pvt->_extra) {
			pcre_free(pvt->_extra);
			pvt->_extra=NULL;
		}
		const char	*error;
		int32_t		erroroffset;
		if (!(pvt->_expr=pcre_compile(pattern,0,&error,
						&erroroffset,NULL))) {
			return false;
		}
		int	cc=0;
		pcre_fullinfo(pvt->_expr,NULL,PCRE_INFO_CAPTURECOUNT,&cc);
		capturecount=(int32_t)cc;
	#else
		if (pvt->_compiled) {
			regfree(&pvt->_expr);
			pvt->_compiled=false;
		}
		if (regcomp(&pvt->_expr,pattern,REG_EXTENDED)) {
			return false;
		}
		pvt->_compiled=true;
		capturecount=(int32_t)pvt->_expr.re_nsub;
	#endif

	// the whole match, plus a substring per capture group, but no more
	// than the match array can hold
	pvt->_substringcount=(capturecount+1>RUDIMENTS_REGEX_MATCHES)?
				RUDIMENTS_REGEX_MATCHES:capturecount+1;
	return true;
}

const char *regularexpression::getPattern() {
	return pvt->_pattern;
}

bool regularexpression::study() {
	if (pvt->_null) {
		return true;
	}
	#if defined(RUDIMENTS_HAS_PCRE2)
		// A library built without jit support returns
		// PCRE2_ERROR_JIT_BADOPTION.  That just means there was
		// nothing to do, like pcre1's pcre_study() returning NULL
		// with no error.
		int32_t	result=pcre2_jit_compile(pvt->_expr,
						PCRE2_JIT_COMPLETE);
		return (!result || result==PCRE2_ERROR_JIT_BADOPTION);
	#elif defined(RUDIMENTS_HAS_PCRE)
		const char	*error;
		if (pvt->_extra) {
			pcre_free(pvt->_extra);
			pvt->_extra=NULL;
		}
		pvt->_extra=pcre_study(pvt->_expr,0,&error);
		return (!pvt->_extra && error)?false:true;
	#else
		return true;
	#endif
}

bool regularexpression::match(const char *str, size_t length) {
	return match(str,length,0);
}

bool regularexpression::match(const char *str, size_t length,
						int32_t offset) {
	if (!str) {
		return pvt->_null;
	}
	pvt->_matchcount=0;
	if (offset<0 || (size_t)offset>length) {
		return false;
	}
	#if !defined(RUDIMENTS_HAS_PCRE2) && !defined(RUDIMENTS_HAS_PCRE)
		// regexec() needs a null-terminated subject, but the
		// substrings still have to be reported relative to str
		delete[] pvt->_strcopy;
		pvt->_strcopy=charstring::duplicate(str,length);
		pvt->_str=str;
		return runRegexec(pvt->_strcopy,offset);
	#else
		pvt->_str=str;
		#if defined(RUDIMENTS_HAS_PCRE2)
			bool	retval=(pvt->_expr &&
					pcre2_match(pvt->_expr,
						(PCRE2_SPTR)pvt->_str,length,
						offset,0,pvt->_matchdata,
						NULL)>-1);
		#else
			bool	retval=(pvt->_expr &&
					pcre_exec(pvt->_expr,pvt->_extra,
						pvt->_str,length,
						offset,0,pvt->_matches,
						RUDIMENTS_REGEX_MATCHES*3)>-1);
		#endif
		if (retval) {
			pvt->_matchcount=pvt->_substringcount;
		}
		return retval;
	#endif
}

bool regularexpression::match(const char *str) {
	if (!str) {
		return pvt->_null;
	}
	#if defined(RUDIMENTS_HAS_PCRE2) || defined(RUDIMENTS_HAS_PCRE)
		return match(str,charstring::getLength(str),0);
	#else
		// no copy is needed, str is already null-terminated
		pvt->_str=str;
		return runRegexec(str,0);
	#endif
}

#if !defined(RUDIMENTS_HAS_PCRE2) && !defined(RUDIMENTS_HAS_PCRE)
bool regularexpression::runRegexec(const char *subject, int32_t offset) {

	pvt->_matchcount=0;

	// regexec() has no start offset, so it has to be pointed at the
	// resume point instead.  REG_NOTBOL keeps ^ from matching there,
	// which is the whole reason for taking an offset rather than
	// letting the caller pass a pointer into the middle of the string.
	if (!pvt->_compiled ||
		regexec(&pvt->_expr,subject+offset,
				RUDIMENTS_REGEX_MATCHES,
				pvt->_matches,(offset)?REG_NOTBOL:0)) {
		return false;
	}

	// regexec() reports positions relative to where it was pointed, but
	// they have to come back relative to the start of the subject
	if (offset) {
		for (int32_t i=0; i<pvt->_substringcount; i++) {
			if (pvt->_matches[i].rm_so>-1) {
				pvt->_matches[i].rm_so+=offset;
				pvt->_matches[i].rm_eo+=offset;
			}
		}
	}

	pvt->_matchcount=pvt->_substringcount;
	return true;
}
#endif

int32_t regularexpression::getSubstringCount() {
	if (pvt->_null) {
		return 0;
	}
	return pvt->_matchcount;
}

int32_t regularexpression::getSubstringStartOffset(int32_t index) {
	if (pvt->_null || index<0 || index>=pvt->_matchcount) {
		return -1;
	}
	#if defined(RUDIMENTS_HAS_PCRE2) || defined(RUDIMENTS_HAS_PCRE)
		// An unset offset is PCRE2_UNSET under pcre2, which casts
		// to the same -1 that pcre1 stores directly.
		return (int32_t)pvt->_matches[index*2];
	#else
		return pvt->_matches[index].rm_so;
	#endif
}

int32_t regularexpression::getSubstringEndOffset(int32_t index) {
	if (pvt->_null || index<0 || index>=pvt->_matchcount) {
		return -1;
	}
	#if defined(RUDIMENTS_HAS_PCRE2) || defined(RUDIMENTS_HAS_PCRE)
		return (int32_t)pvt->_matches[index*2+1];
	#else
		return pvt->_matches[index].rm_eo;
	#endif
}

const char *regularexpression::getSubstringStart(int32_t index) {
	int32_t	offset=getSubstringStartOffset(index);
	return (offset>-1)?(pvt->_str+offset):NULL;
}

const char *regularexpression::getSubstringEnd(int32_t index) {
	int32_t	offset=getSubstringEndOffset(index);
	return (offset>-1)?(pvt->_str+offset):NULL;
}

int32_t regularexpression::getSubstringIndex(const char *name) {
	if (pvt->_null || !name) {
		return -1;
	}
	#if defined(RUDIMENTS_HAS_PCRE2)
		if (!pvt->_expr) {
			return -1;
		}
		int32_t	index=pcre2_substring_number_from_name(
						pvt->_expr,(PCRE2_SPTR)name);
	#elif defined(RUDIMENTS_HAS_PCRE)
		if (!pvt->_expr) {
			return -1;
		}
		// Unlike pcre2, pcre1 returns one of the numbers when more
		// than one group has the name, and doesn't define which.
		// The name table has to be consulted to rule that out.
		char	*first;
		char	*last;
		if (pcre_get_stringtable_entries(pvt->_expr,name,
							&first,&last)<0 ||
			first!=last) {
			return -1;
		}
		int32_t	index=pcre_get_stringnumber(pvt->_expr,name);
	#else
		// the POSIX regular expression engines have no named
		// capture groups
		int32_t	index=-1;
	#endif
	// an unknown name, or one used by more than one group, comes back
	// as a negative error code
	return (index<0)?-1:index;
}

int32_t regularexpression::getSubstringStartOffset(const char *name) {
	return getSubstringStartOffset(getSubstringIndex(name));
}

int32_t regularexpression::getSubstringEndOffset(const char *name) {
	return getSubstringEndOffset(getSubstringIndex(name));
}

const char *regularexpression::getSubstringStart(const char *name) {
	return getSubstringStart(getSubstringIndex(name));
}

const char *regularexpression::getSubstringEnd(const char *name) {
	return getSubstringEnd(getSubstringIndex(name));
}

bool regularexpression::match(const char *str, size_t length,
						const char *pattern) {
	regularexpression	re;
	return (re.setPattern(pattern) && re.match(str,length));
}

bool regularexpression::match(const char *str, const char *pattern) {
	regularexpression	re;
	return (re.setPattern(pattern) && re.match(str));
}
