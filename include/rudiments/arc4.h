// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ARC4_H
#define RUDIMENTS_ARC4_H

#include <rudiments/private/arc4includes.h>

/** The arc4 class provides a simple implementation of the RC4 stream
 *  cipher.
 *
 *  Unlike the classes derived from the encryption class, arc4 is not a
 *  block cipher.  It has no key size restrictions, no initialization
 *  vector, no padding, and no block cipher mode.  Instead, each instance
 *  generates a running keystream from its key, and crypt() XORs that
 *  keystream against a buffer, advancing the keystream as it goes.
 *
 *  Encryption and decryption are the same operation, so there is only one
 *  method, crypt(), which is used for both.
 *
 *  Each instance keeps its own independent, running keystream.  This makes
 *  it well suited to duplex streams, such as sockets, where one instance
 *  can be constructed to encrypt or decrypt outgoing data and another,
 *  constructed from the same key, can independently encrypt or decrypt
 *  incoming data, without either instance's keystream affecting the
 *  other's.  crypt() may be called repeatedly, buffer by buffer, across
 *  the life of the stream, and the keystream will simply continue from
 *  where the previous call left off.
 *
 *  When available, this class uses OpenSSL's RC4 implementation
 *  internally.  Otherwise it falls back to a built-in implementation. */
class RUDIMENTS_DLLSPEC arc4 {
	public:

		/** Creates an instance of the arc4 class with no key set.
		 *  setKey() must be called before crypt() may be used. */
		arc4();

		/** Creates an instance of the arc4 class and initializes it
		 *  with the given key.  "key" may be between 1 and 256 bytes
		 *  long, as specified by "keysize". */
		arc4(const byte_t *key, size_t keysize);

		/** Deletes this instance of the arc4 class.
		 *
		 *  The key and any keystream state maintained internally by
		 *  this instance are cleared before the instance is
		 *  deleted. */
		~arc4();

		/** Initializes this instance's keystream using "key", which
		 *  may be between 1 and 256 bytes long, as specified by
		 *  "keysize".
		 *
		 *  Calling this method resets this instance's keystream to
		 *  the beginning, discarding any keystream state from a
		 *  previous call to setKey() or crypt().
		 *
		 *  Returns true on success or false if an error occurred. */
		bool	setKey(const byte_t *key, size_t keysize);

		/** XOR's "size" bytes of "buffer", in place, against the
		 *  next "size" bytes of this instance's running keystream.
		 *
		 *  Since RC4 is a symmetric stream cipher, this same method
		 *  is used for both encryption and decryption.
		 *
		 *  This method may be called repeatedly - for example once
		 *  per read or write on a socket - without resetting
		 *  anything in between.  Each call picks up the keystream
		 *  where the previous call left off, so the sequence of
		 *  calls, taken together, must be applied in the same order
		 *  by both sides of the exchange.
		 *
		 *  Returns true on success or false if an error occurred,
		 *  such as if no key has been set yet. */
		bool	crypt(byte_t *buffer, size_t size);

		/** Returns true if this platform supports arc4 and false
		 *  otherwise.  Since a fallback implementation is built in,
		 *  this method always returns true. */
		bool	isSupported();

	#include <rudiments/private/arc4.h>
};

#endif
