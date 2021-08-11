// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SCALAR_H
#define RUDIMENTS_SCALAR_H

#include <rudiments/private/scalarincludes.h>

template <class valuetype>
class RUDIMENTS_DLLSPEC scalar : public scalarcollection<valuetype> {
	public:
		void		setValue(valuetype v);
		valuetype	getValue() const;
		void		clear();

	#include <rudiments/private/scalar.h>
};

#include <rudiments/private/scalarinlines.h>

#endif
