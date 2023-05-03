// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_URL_H
#define RUDIMENTS_URL_H

#include <rudiments/private/urlincludes.h>

/** The url class provides read access to urls.
 *
 *  Its parent class: file provides provides open(), read(), and close()
 *  methods.
 *
 *  http urls, with Basic Authentication, are supported on all platforms.
 *
 *  urls for other protocols, including ftp, https, scp, sftp, and others, are
 *  supported if rudiments was compiled with support for libcurl.
 *
 *  Examples of supported urls include:
 *
 *    * http://somedomain.com/path/to/file.html
 *    * http://user:password@somedomain.com/path/to/file.html
 *    * http://[/path/to/userpwdfile]@somedomain.com/path/to/file.html
 *
 *  The url class supports storing the user:password portion of a url in a
 *  user-password file.  These files may be referenced in the url by enclosing
 *  the full pathname of the file in square brackets.  The file should consist
 *  of a single line containing the user and password, separated by a colon.
 *
 *  For example:
 *  
 *    myusername:mypassword
 *
 *  */
class RUDIMENTS_DLLSPEC url : public file {
	public:

		/** Creates an instance of the url class. */
		url();

		/** Deletes this instance of the url class. */
		virtual	~url();

		/** Sets whether to use HTTP GET or POST when making an
		 *  HTTP/HTTPS request and post data has been set using
		 *  setHttpPostData().
		 *
		 *  If "usehttppost" is set to true then POST is used. If
		 *  "usehttppost" is set to false then GET is used.
		 *
		 *  Defaults to false. */
		void	setUseHttpPost(bool usehttppost);

		/** Returns true if HTTP POST is used when making an
		 *  HTTP/HTTPS request and false if HTTP GET is used. */
		bool	getUseHttpPost();

		/** Sets the content type to send when doing an HTTP POST.
		 *
		 *  Defaults to application/x-www-form-urlencoded, which is
		 *  correct for sending key/value pairs.
		 *
		 *  May be set though, for example, to application/json when
		 *  sending JSON. */
		void	setHttpPostContentType(const char *contenttype);

		/** Returuns the content type to send when doing an
		 *  HTTP POST. */
		const char	*getHttpPostContentType();

		/** Posts "size" bytes of "data" during an HTTP POST.  Data in
		 *  any format that agrees with the http post content type may
		 *  be sent. */
		void	setHttpPostData(const char *data, uint64_t size);

		/** Returns the "data" that was set during a previous call to
		 *  setHttpPostData(). */
		const char	*getHttpPostData();

		/** Returns the "size" that was set duringby a previous call to
		 *  setHttpPostData(). */
		uint64_t	getHttpPostDataSize();

		/** Sets the value sent in the User-Agent header that is sent
		 *  during an HTTP/HTTPS request to "useragent".
		 *
		 *  Defaults to rudiments/RUDIMENTS_VERSION where
		 *  RUDIMENTS_VERSION is replaced with the current Rudiments
		 *  version string. */
		void	setHttpUserAgent(const char *useragent);

		/** Returns the value that will be sent in the User-Agent
		 *  header that is sent during an HTTP/HTTPS request. */
		const char	*getHttpUserAgent();

		/** Sets raw HTTP headers to send when making HTTP/HTTPS
		 *  requests.
		 *
		 *  Note that if these raw HTTP headers include a User-Agent
		 *  header, then it will override any value set by
		 *  setHttpUserAgent().
		 *
		 *  Similarly, if these raw HTTP headers include a Host, or
		 *  Authorization: Basic header, then it will override what
		 *  would have been sent by parsing the url. */
		void	setHttpHeaders(const char *headers);

		/** Returns raw HTTP headers set by a previous call to
		 *  setHttpHeaders(). */
		const char	*getHttpHeaders();

		/** Sets whether or not to perform SSL validation of the peer
		 *  when the url is any url that uses SSL. */
		void	setValidatePeer(bool validatepeer);

		/** Returns true if SSL validation of the peer will be done or
		 *  false otherwise. */
		bool	getValidatePeer();

		/** Sets the certificate authority to use when SSL validation
		 *  of the peer is being done. */
		void	setCertificateAuthority(const char *ca);

		/** Returns the certificate authority that will be used to
		 *  perform SSL validation of the peer. */
		const char	*getCertificateAuthority();

		/** If an error occurs, returns a higher level, possibly more
		 *  descriptive or useful error than might be returned by the
		 *  system using error::getErrorMessage(). */
		const char	*getError();

		/** Returns true if "protocol" is supported and false if it
		 *  is not supported. */
		static bool	supportsProtocol(const char *protocol);

	#include <rudiments/private/url.h>
};

#endif
