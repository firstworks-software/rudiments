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
	if (!pattern) {
		pvt->_null=true;
		return true;
	}
	pvt->_pattern=pattern;
	#if defined(RUDIMENTS_HAS_PCRE2)
		if (pvt->_expr) {
			pcre2_code_free(pvt->_expr);
		}
		int32_t		error;
		PCRE2_SIZE	erroroffset;
		return (pvt->_expr=pcre2_compile((PCRE2_SPTR)pattern,
						PCRE2_ZERO_TERMINATED,0,
						&error,&erroroffset,
						NULL))!=NULL;
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
		return (pvt->_expr=pcre_compile(pattern,0,&error,
						&erroroffset,NULL))!=NULL;
	#else
		if (pvt->_compiled) {
			regfree(&pvt->_expr);
			pvt->_compiled=false;
		}
		return (pvt->_compiled=
			!regcomp(&pvt->_expr,pattern,REG_EXTENDED));
	#endif
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
	if (!str) {
		return pvt->_null;
	}
	#if defined(RUDIMENTS_HAS_PCRE2)
		pvt->_str=str;
		pvt->_matchcount=-1;
		return (pvt->_expr &&
			(pvt->_matchcount=pcre2_match(pvt->_expr,
						(PCRE2_SPTR)pvt->_str,length,
						0,0,pvt->_matchdata,
						NULL))>-1);
	#elif defined(RUDIMENTS_HAS_PCRE)
		pvt->_str=str;
		pvt->_matchcount=-1;
		return (pvt->_expr &&
			(pvt->_matchcount=pcre_exec(pvt->_expr,pvt->_extra,
						pvt->_str,length,
						0,0,pvt->_matches,
						RUDIMENTS_REGEX_MATCHES*3))>-1);
	#else
		delete[] pvt->_strcopy;
		pvt->_strcopy=charstring::duplicate(str,length);
		return match(pvt->_strcopy);
	#endif
}

bool regularexpression::match(const char *str) {
	if (!str) {
		return pvt->_null;
	}
	#if defined(RUDIMENTS_HAS_PCRE2) || defined(RUDIMENTS_HAS_PCRE)
		return match(str,charstring::getLength(str));
	#else
		pvt->_str=str;
		for (int32_t i=0; i<pvt->_matchcount; i++) {
			pvt->_matches[i].rm_so=-1;
		}
		pvt->_matchcount=-1;
		bool	retval=(pvt->_compiled &&
				!regexec(&pvt->_expr,pvt->_str,
					RUDIMENTS_REGEX_MATCHES,
					pvt->_matches,0));
		getSubstringCount();
		return retval;
	#endif
}

int32_t regularexpression::getSubstringCount() {
	if (pvt->_null) {
		return 0;
	}
	#if !defined(RUDIMENTS_HAS_PCRE2) && !defined(RUDIMENTS_HAS_PCRE)
		if (pvt->_matchcount==-1) {
			for (int32_t i=0; i<RUDIMENTS_REGEX_MATCHES; i++) {
				if (pvt->_matches[i].rm_so==-1) {
					pvt->_matchcount=i;
					break;
				}
			}
		}
	#endif
	if (pvt->_matchcount==-1) {
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

bool regularexpression::match(const char *str, size_t length,
						const char *pattern) {
	regularexpression	re;
	return (re.setPattern(pattern) && re.match(str,length));
}

bool regularexpression::match(const char *str, const char *pattern) {
	regularexpression	re;
	return (re.setPattern(pattern) && re.match(str));
}
