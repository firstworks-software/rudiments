// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/utf8character.h>

uint8_t utf8character::getNullSize() {
	return sizeof(utf8_t);
}

uint8_t utf8character::getBomSize() {
	return 3*sizeof(byte_t);
}
