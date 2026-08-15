// Copyright (c) David Muse
// See the COPYING file for more information.

	private:
		void	setError(bignumbererror_t err);

		// helpers used by the bitwise methods, working in
		// big-endian two's complement
		size_t	getTwosComplementSize(const bignumber &value);
		// writes exactly "size" bytes, sign-extended
		bool	toTwosComplement(byte_t *buffer, size_t size);
		// reads the first "size" bytes
		bool	fromTwosComplement(const byte_t *buffer, size_t size);

		bignumberprivate	*pvt;
