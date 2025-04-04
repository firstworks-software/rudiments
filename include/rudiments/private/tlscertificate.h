// Copyright (c) David Muse
// See the COPYING file for more information.

		tlscertificate(tlscertificate &t) {};
		tlscertificate	&operator=(tlscertificate &t) { return *this; };

		void	initCertificate();
		void	freeCertificate();
		void	setCertificate(void *cert);

	private:
		tlscertificateprivate	*pvt;
