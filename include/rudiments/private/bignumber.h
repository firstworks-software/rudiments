// Copyright (c) David Muse
// See the COPYING file for more information.

	private:
		void	setError(bignumbererror_t err);

		// helpers used by the bitwise methods...
		//
		// getTwosComplementSize() returns the number of bytes
		// needed to represent this instance, and "value", as
		// two's complement values of the same width.
		//
		// toTwosComplement() writes this instance to "buffer" as
		// a big-endian, sign-extended, two's complement value of
		// exactly "size" bytes.
		//
		// fromTwosComplement() sets this instance from the
		// big-endian, two's complement value in the first "size"
		// bytes of "buffer".
		size_t	getTwosComplementSize(const bignumber &value);
		bool	toTwosComplement(byte_t *buffer, size_t size);
		bool	fromTwosComplement(const byte_t *buffer, size_t size);

		bignumberprivate	*pvt;
