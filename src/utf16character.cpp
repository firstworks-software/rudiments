// Copyright (c) David Muse
// See the COPYING file for more information.

#include <rudiments/utf16character.h>

uint16_t utf16character::getNullSize() {
	return sizeof(utf16_t);
}

uint16_t utf16character::getBomSize() {
	return sizeof(utf16_t);
}
