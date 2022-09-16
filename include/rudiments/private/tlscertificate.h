// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		void	initCertificate();
		void	freeCertificate();
		void	setCertificate(void *cert);

	private:
		tlscertificate(tlscertificate &t) {};
		tlscertificate	&operator=(tlscertificate &t) { return *this; };

		tlscertificateprivate	*pvt;
