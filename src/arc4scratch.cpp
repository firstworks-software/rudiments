// Copyright (c) David Muse
// See the COPYING file for more information

// textbook RC4 key-scheduling algorithm.  The key is used raw, with no
// hashing and no "RC4-drop" discard of the first bytes of keystream.
static void arc4scratchksa(unsigned char *state,
				unsigned char *s1, unsigned char *s2,
				const byte_t *key, size_t keysize) {

	for (uint16_t n=0; n<256; n++) {
		state[n]=(unsigned char)n;
	}

	unsigned char	k2=0;
	for (uint16_t k1=0; k1<256; k1++) {
		k2=(unsigned char)(k2+key[k1%keysize]+state[k1]);
		unsigned char	tmp=state[k1];
		state[k1]=state[k2];
		state[k2]=tmp;
	}

	*s1=0;
	*s2=0;
}

// textbook RC4 pseudo-random generation algorithm, applied byte-by-byte as a
// keystream that's XORed against the buffer.  s1 and s2 are unsigned char so
// they wrap mod 256 on their own, and nothing here resets between calls, so a
// later call just continues the same keystream.
static void arc4scratchprga(unsigned char *state,
				unsigned char *s1, unsigned char *s2,
				byte_t *buffer, size_t size) {

	for (size_t i=0; i<size; i++) {
		(*s1)++;
		*s2=(unsigned char)(*s2+state[*s1]);
		unsigned char	tmp=state[*s1];
		state[*s1]=state[*s2];
		state[*s2]=tmp;
		unsigned char	k=state[(unsigned char)
					(state[*s1]+state[*s2])];
		buffer[i]=(byte_t)(buffer[i]^k);
	}
}
