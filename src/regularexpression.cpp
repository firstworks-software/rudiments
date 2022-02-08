// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/regularexpression.h>
#include <rudiments/bytestring.h>

#if defined(RUDIMENTS_HAS_PCRE)
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
#ifdef RUDIMENTS_HAS_PCRE
	#include <rudiments/charstring.h>
#endif

class regularexpressionprivate {
	friend class regularexpression;
	private:
		bool		_studied;
		bool		_null;
		const char	*_pattern;

		#ifdef RUDIMENTS_HAS_PCRE
			pcre		*_expr;
			pcre_extra	*_extra;
		#else
			regex_t	_expr;
			char	*_strcopy;
		#endif
	
		#define RUDIMENTS_REGEX_MATCHES 128
		int32_t		_matchcount;
		const char	*_str;

		#ifdef RUDIMENTS_HAS_PCRE
		int32_t		_matches[RUDIMENTS_REGEX_MATCHES*3];
		#else
		regmatch_t	_matches[RUDIMENTS_REGEX_MATCHES];
		#endif
};

regularexpression::regularexpression() : object() {
	init();
}

regularexpression::regularexpression(const char *pattern) : object() {
	init();
	setPattern(pattern);
}

regularexpression::regularexpression(const regularexpression &r) : object() {
	init();
	setPattern(r.getPattern());
	if (r.pvt->_studied) {
		study();
	}
}

regularexpression &regularexpression::operator=(const regularexpression &r) {
	if (this!=&r) {
		clear();
		init();
		setPattern(r.getPattern());
		if (r.pvt->_studied) {
			study();
		}
	}
	return *this;
}

void regularexpression::init() {
	pvt=new regularexpressionprivate;
	#ifdef RUDIMENTS_HAS_PCRE
		pvt->_expr=NULL;
		pvt->_extra=NULL;
	#else
		bytestring::zero(&pvt->_expr,sizeof(pvt->_expr));
		pvt->_strcopy=NULL;
	#endif
	pvt->_studied=false;
	pvt->_null=false;
	pvt->_pattern=NULL;
	pvt->_matchcount=0;
	pvt->_str=NULL;
	bytestring::zero(pvt->_matches,sizeof(pvt->_matches));
}

regularexpression::~regularexpression() {
	clear();
	delete pvt;
}

void regularexpression::clear() {
	#ifdef RUDIMENTS_HAS_PCRE
		if (pvt->_expr) {
			pcre_free(pvt->_expr);
		}
		if (pvt->_extra) {
			pcre_free(pvt->_extra);
		}
	#else
		regfree(&pvt->_expr);
		delete[] pvt->_strcopy;
	#endif
}

bool regularexpression::setPattern(const char *pattern) {
	pvt->_studied=false;
	pvt->_null=false;
	if (!pattern) {
		pvt->_null=true;
		return true;
	}
	pvt->_pattern=pattern;
	#ifdef RUDIMENTS_HAS_PCRE
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
		regfree(&pvt->_expr);
		return !regcomp(&pvt->_expr,pattern,REG_EXTENDED);
	#endif
}

const char *regularexpression::getPattern() const {
	return pvt->_pattern;
}

bool regularexpression::study() {
	if (pvt->_null) {
		return true;
	}
	pvt->_studied=true;
	#ifdef RUDIMENTS_HAS_PCRE
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
	#ifdef RUDIMENTS_HAS_PCRE
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
	#ifdef RUDIMENTS_HAS_PCRE
		return match(str,charstring::length(str));
	#else
		pvt->_str=str;
		for (int32_t i=0; i<pvt->_matchcount; i++) {
			pvt->_matches[i].rm_so=-1;
		}
		pvt->_matchcount=-1;
		bool	retval=!regexec(&pvt->_expr,pvt->_str,
				RUDIMENTS_REGEX_MATCHES,pvt->_matches,0);
		getSubstringCount();
		return retval;
	#endif
}

int32_t regularexpression::getSubstringCount() {
	if (pvt->_null) {
		return 0;
	}
	#ifndef RUDIMENTS_HAS_PCRE
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
	if (pvt->_null || index<0 || index>pvt->_matchcount) {
		return -1;
	}
	#ifdef RUDIMENTS_HAS_PCRE
		return pvt->_matches[index*2];
	#else
		return pvt->_matches[index].rm_so;
	#endif
}

int32_t regularexpression::getSubstringEndOffset(int32_t index) {
	if (pvt->_null || index<0 || index>pvt->_matchcount) {
		return -1;
	}
	#ifdef RUDIMENTS_HAS_PCRE
		return pvt->_matches[index*2+1];
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
