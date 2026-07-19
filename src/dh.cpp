// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/dh.h>
#include <rudiments/bytestring.h>
#include <rudiments/error.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/dh.h>
	#include <openssl/err.h>
#endif

class dhprivate {
	friend class dh;
	private:
		dherror_t	_err;
		bytebuffer	_dhp;
		bytebuffer	_dhg;
		byte_t		*_privkey;
		size_t		_privkeysize;
		byte_t		*_pubkey;
		size_t		_pubkeysize;
		bytebuffer	_peerpubkey;
		byte_t		*_sharedsecret;
		size_t		_sharedsecretsize;
		#if defined(RUDIMENTS_HAS_SSL)
		DH		*_dh;
		#endif
};

dh::dh() {
	pvt=new dhprivate;
	pvt->_err=DH_ERROR_NULL;
	pvt->_privkey=NULL;
	pvt->_privkeysize=0;
	pvt->_pubkey=NULL;
	pvt->_pubkeysize=0;
	pvt->_sharedsecret=NULL;
	pvt->_sharedsecretsize=0;
	#if defined(RUDIMENTS_HAS_SSL)
		pvt->_dh=DH_new();
	#endif
}

dh::~dh() {
	#if defined(RUDIMENTS_HAS_SSL)
		DH_free(pvt->_dh);
	#endif
	delete pvt;
}

void dh::setPrimeModulus(const byte_t *dhp, size_t dhpsize) {
	pvt->_dhp.clear();
	pvt->_dhp.append(dhp,dhpsize);
}

const byte_t *dh::getPrimeModulus() {
	return pvt->_dhp.getBuffer();
}

size_t dh::getPrimeModulusSize() {
	return pvt->_dhp.getSize();
}

void dh::setGenerator(const byte_t *dhg, size_t dhgsize) {
	pvt->_dhg.clear();
	pvt->_dhg.append(dhg,dhgsize);
}

const byte_t *dh::getGenerator() {
	return pvt->_dhg.getBuffer();
}

size_t dh::getGeneratorSize() {
	return pvt->_dhg.getSize();
}

bool dh::generateKeys() {

	// reset the error
	setError(DH_ERROR_SUCCESS);
	error::clearError();

	// clear our keys
	delete[] pvt->_privkey;
	pvt->_privkey=NULL;
	pvt->_privkeysize=0;
	delete[] pvt->_pubkey;
	pvt->_pubkey=NULL;
	pvt->_pubkeysize=0;

	#if defined(RUDIMENTS_HAS_SSL)

		// set the prime modulus and generator/base
		#if defined(RUDIMENTS_HAS_DH_SET0_PQG)
			// DH_set0_pqg() takes ownership of the BIGNUMs on
			// success but not on failure, so free them ourselves
			// if it fails
			BIGNUM	*p=BN_bin2bn(pvt->_dhp.getBuffer(),
						pvt->_dhp.getSize(),NULL);
			BIGNUM	*g=BN_bin2bn(pvt->_dhg.getBuffer(),
						pvt->_dhg.getSize(),NULL);
			if (!DH_set0_pqg(pvt->_dh,p,NULL,g)) {
				BN_free(p);
				BN_free(g);
				setError(DH_ERROR_INVALID_PARAMETERS);
				return false;
			}
		#else
			// assigning to ->p and ->g transfers ownership to the
			// DH; the old values (NULL on the first call) are freed
			// here before being replaced
			BN_free(pvt->_dh->p);
			pvt->_dh->p=BN_bin2bn(pvt->_dhp.getBuffer(),
						pvt->_dhp.getSize(),NULL);
			BN_free(pvt->_dh->g);
			pvt->_dh->g=BN_bin2bn(pvt->_dhg.getBuffer(),
						pvt->_dhg.getSize(),NULL);
		#endif

		// check for invalid parameters
		int	codes=0;
		if (!DH_check(pvt->_dh,&codes)) {
			setError(DH_ERROR_INVALID_PARAMETERS);
			return false;
		}

		// generate the keys
		if (!DH_generate_key(pvt->_dh)) {
			setError(DH_ERROR_GENERATE_KEYS_FAILED);
			return false;
		}

		// get the keys
		const BIGNUM	*privkey=NULL;
		const BIGNUM	*pubkey=NULL;
		#if defined(RUDIMENTS_HAS_DH_GET0_KEY)
			DH_get0_key(pvt->_dh,&pubkey,&privkey);
		#else
			privkey=pvt->_dh->priv_key;
			pubkey=pvt->_dh->pub_key;
		#endif

		// copy out the keys
		pvt->_privkeysize=BN_num_bytes(privkey);
		pvt->_privkey=new byte_t[pvt->_privkeysize];
		BN_bn2bin(privkey,pvt->_privkey);
		pvt->_pubkeysize=BN_num_bytes(pubkey);
		pvt->_pubkey=new byte_t[pvt->_pubkeysize];
		BN_bn2bin(pubkey,pvt->_pubkey);

		return true;
	#else
		setError(DH_ERROR_UNSUPPORTED);
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

const byte_t *dh::getPrivateKey() {
	return pvt->_privkey;
}

size_t dh::getPrivateKeySize() {
	return pvt->_privkeysize;
}

const byte_t *dh::getPublicKey() {
	return pvt->_pubkey;
}

size_t dh::getPublicKeySize() {
	return pvt->_pubkeysize;
}

void dh::setPeerPublicKey(const byte_t *pk, size_t pksize) {
	pvt->_peerpubkey.clear();
	pvt->_peerpubkey.append(pk,pksize);
}

const byte_t *dh::getPeerPublicKey() {
	return pvt->_peerpubkey.getBuffer();
}

size_t dh::getPeerPublicKeySize() {
	return pvt->_peerpubkey.getSize();
}

bool dh::generateSharedSecret() {

	// reset the error
	setError(DH_ERROR_SUCCESS);
	error::clearError();

	// clear the shared secret
	delete[] pvt->_sharedsecret;

	#if defined(RUDIMENTS_HAS_SSL)

		// allocate space for the shared secret
		pvt->_sharedsecretsize=DH_size(pvt->_dh);
		pvt->_sharedsecret=new byte_t[pvt->_sharedsecretsize];

		// get the peer public key as a BIGNUM
		BIGNUM	*ppk=BN_bin2bn(pvt->_peerpubkey.getBuffer(),
					pvt->_peerpubkey.getSize(),NULL);

		// compute the shared secret
		// (NOTE: the size of the actual secret might be less than
		// the size returned by DH_size(dh))
		int	result=DH_compute_key(pvt->_sharedsecret,ppk,pvt->_dh);

		// clean up
		BN_free(ppk);

		// handle error
		if (result==-1) {
			delete[] pvt->_sharedsecret;
			pvt->_sharedsecret=NULL;
			pvt->_sharedsecretsize=0;
			setError(DH_ERROR_GENERATE_SECRET_FAILED);
			return false;
		}

		// set the actual size of the shared secret
		pvt->_sharedsecretsize=result;

		return true;

	#else

		pvt->_sharedsecret=NULL;
		pvt->_sharedsecretsize=0;

		setError(DH_ERROR_UNSUPPORTED);
		RUDIMENTS_SET_ENOSYS;
		return false;
	#endif
}

const byte_t *dh::getSharedSecret() {
	return pvt->_sharedsecret;
}

uint64_t dh::getSharedSecretSize() {
	return pvt->_sharedsecretsize;
}

dherror_t dh::getError() {
	return pvt->_err;
}

bool dh::clear() {
	pvt->_err=DH_ERROR_NULL;
	delete[] pvt->_privkey;
	pvt->_privkey=NULL;
	pvt->_privkeysize=0;
	delete[] pvt->_pubkey;
	pvt->_pubkey=NULL;
	pvt->_pubkeysize=0;
	pvt->_peerpubkey.clear();
	delete[] pvt->_sharedsecret;
	pvt->_sharedsecret=NULL;
	pvt->_sharedsecretsize=0;
	return true;
}

bool dh::reset() {
	clear();
	pvt->_dhp.clear();
	pvt->_dhg.clear();
	return true;
}

void dh::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL)
		// FIXME: implement this
		setError(DH_ERROR_NULL);
		// clear the queue
		while(ERR_get_error()) {}
	#else
		setError(DH_ERROR_NULL);
	#endif
}

void dh::setError(dherror_t err) {
	pvt->_err=err;
}

bool dh::isSupported() {
	#if defined(RUDIMENTS_HAS_SSL)
		return true;
	#else
		return false;
	#endif
}
