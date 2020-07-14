// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		bytebuffer	*getIn();
		unsigned char	*getOut();
		uint32_t	*getOutLengthPointer();
		void		reallocateOut(uint32_t size);
		virtual void	setError(int32_t err);

	private:
		encryptionprivate	*pvt;
