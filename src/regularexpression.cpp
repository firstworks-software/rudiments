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
	#include "regex.cpp"
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
		size_t		_length;

		// True between a successful match and the failed one that ends
		// the walk. Can't be inferred from _matchcount, because
		// match(NULL) returns early without touching it.
		bool		_walking;

		#if !defined(RUDIMENTS_HAS_PCRE2) && !defined(RUDIMENTS_HAS_PCRE)
		// null-terminated buffer regexec() is pointed at - match(str)
		// uses the caller's string, match(str,length) uses _strcopy;
		// remembering which avoids re-duplicating it on each match
		const char	*_subject;
		#endif

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
		// ovector lives inside match data, never reallocated,
		// so this pointer is good for the object's lifetime
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
		pvt->_subject=NULL;
	#endif
	pvt->_null=false;
	pvt->_pattern=NULL;
	pvt->_substringcount=0;
	pvt->_matchcount=0;
	pvt->_str=NULL;
	pvt->_length=0;
	pvt->_walking=false;
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
		// regfree() takes the regex_t by address, so there's no
		// pointer to test like the PCRE arms - _compiled stands in
		if (pvt->_compiled) {
			regfree(&pvt->_expr);
		}
		delete[] pvt->_strcopy;
	#endif
	delete pvt;
}

bool regularexpression::setPattern(const char *pattern) {
	return setPattern(pattern,0);
}

bool regularexpression::setPattern(const char *pattern, uint32_t options) {
	pvt->_null=false;
	pvt->_substringcount=0;
	pvt->_matchcount=0;
	pvt->_walking=false;
	if (!pattern) {
		pvt->_null=true;
		return true;
	}
	pvt->_pattern=pattern;

	// map the portable options onto the ones this engine spells them with
	#if defined(RUDIMENTS_HAS_PCRE2) || defined(RUDIMENTS_HAS_PCRE)
		uint32_t	engineoptions=0;
		if (options&REGULAR_EXPRESSION_CASE_INSENSITIVE) {
			#if defined(RUDIMENTS_HAS_PCRE2)
				engineoptions|=PCRE2_CASELESS;
			#else
				engineoptions|=PCRE_CASELESS;
			#endif
		}
		if (options&REGULAR_EXPRESSION_MULTILINE) {
			#if defined(RUDIMENTS_HAS_PCRE2)
				engineoptions|=PCRE2_MULTILINE;
			#else
				engineoptions|=PCRE_MULTILINE;
			#endif
		}
		if (options&REGULAR_EXPRESSION_DOT_ALL) {
			#if defined(RUDIMENTS_HAS_PCRE2)
				engineoptions|=PCRE2_DOTALL;
			#else
				engineoptions|=PCRE_DOTALL;
			#endif
		}
	#else
		int	engineoptions=REG_EXTENDED;
		if (options&REGULAR_EXPRESSION_CASE_INSENSITIVE) {
			engineoptions|=REG_ICASE;
		}
		// REG_NEWLINE is the closest POSIX has to multiline, but it
		// also blocks . from matching newlines (the opposite of
		// dot-all) - dot-all loses when both options are requested.
		if (options&REGULAR_EXPRESSION_MULTILINE) {
			engineoptions|=REG_NEWLINE;
		}
	#endif

	// compile, and get the count of capture groups from the pattern
	int32_t	capturecount=0;
	#if defined(RUDIMENTS_HAS_PCRE2)
		if (pvt->_expr) {
			pcre2_code_free(pvt->_expr);
		}
		int32_t		error;
		PCRE2_SIZE	erroroffset;
		if (!(pvt->_expr=pcre2_compile((PCRE2_SPTR)pattern,
						PCRE2_ZERO_TERMINATED,
						engineoptions,
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
		if (!(pvt->_expr=pcre_compile(pattern,engineoptions,&error,
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
		if (regcomp(&pvt->_expr,pattern,engineoptions)) {
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
		// a library without jit support returns
		// PCRE2_ERROR_JIT_BADOPTION, same as pcre1's pcre_study()
		// returning NULL with no error - nothing to do either way
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
	pvt->_str=str;
	pvt->_length=length;
	#if !defined(RUDIMENTS_HAS_PCRE2) && !defined(RUDIMENTS_HAS_PCRE)
		// regexec() needs a null-terminated subject; only copy
		// when runMatch() will accept the offset, since it rejects
		// an out-of-range one without running the engine - keep
		// this test in step with the one there
		if (offset>=0 && (size_t)offset<=length) {
			delete[] pvt->_strcopy;
			pvt->_strcopy=charstring::duplicate(str,length);
			pvt->_subject=pvt->_strcopy;
		}
	#endif
	return runMatch(offset);
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
		pvt->_length=charstring::getLength(str);
		pvt->_subject=str;
		return runMatch(0);
	#endif
}

bool regularexpression::matchNext() {

	// nothing to continue - the same state a failed match() leaves,
	// so the substring methods already report nothing
	if (!pvt->_walking || pvt->_null) {
		return false;
	}

	// Resume at the match's end, or one character past its start if
	// empty (so it can't repeat).  pcre1's \K in a lookahead can end
	// before it starts, which would spin backward forever - use
	// whichever point is further ahead.
	int32_t	fromstart=getSubstringStartOffset(0);
	int32_t	fromend=getSubstringEndOffset(0);
	return runMatch((fromend>fromstart)?fromend:fromstart+1);
}

bool regularexpression::runMatch(int32_t offset) {

	pvt->_matchcount=0;
	pvt->_walking=false;

	// An offset equal to the length is still matchable - that's where an
	// empty match at the very end of the subject is found.
	if (offset<0 || (size_t)offset>pvt->_length) {
		return false;
	}

	#if defined(RUDIMENTS_HAS_PCRE2)
		bool	success=(pvt->_expr &&
				pcre2_match(pvt->_expr,
					(PCRE2_SPTR)pvt->_str,pvt->_length,
					offset,0,pvt->_matchdata,NULL)>-1);
	#elif defined(RUDIMENTS_HAS_PCRE)
		bool	success=(pvt->_expr &&
				pcre_exec(pvt->_expr,pvt->_extra,
					pvt->_str,pvt->_length,
					offset,0,pvt->_matches,
					RUDIMENTS_REGEX_MATCHES*3)>-1);
	#else
		bool	success=runRegexec(pvt->_subject,offset);
	#endif

	if (!success) {
		return false;
	}

	pvt->_matchcount=pvt->_substringcount;
	pvt->_walking=true;
	return true;
}

#if !defined(RUDIMENTS_HAS_PCRE2) && !defined(RUDIMENTS_HAS_PCRE)
bool regularexpression::runRegexec(const char *subject, int32_t offset) {

	// regexec() has no start offset, so point it at the resume point
	// instead; REG_NOTBOL keeps ^ from matching there - the reason for
	// taking an offset instead of a pointer into the string
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
