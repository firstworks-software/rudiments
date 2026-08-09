// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETSOCKETCLIENT_H
#define RUDIMENTS_INETSOCKETCLIENT_H

#include <rudiments/private/inetsocketclientincludes.h>

/** The inetsocketclient class allows you to write programs that can talk to
 *  other programs across a network over TCP stream sockets.
 * 
 *  The inetsocketclient class provides methods for connecting to servers.
 *  Its ultimate parent class: filedescriptor provides methods for reading and 
 *  writing data and closing connections. */
class RUDIMENTS_DLLSPEC inetsocketclient :
			public socketclient, public inetsocketutil  {
	public:

		/** Creates an instance of the inetsocketclient class. */
		inetsocketclient();

		/** Deletes this instance of the inetsocketclient class. */
		virtual		~inetsocketclient();

		/** Queries "cd" for "host", "port", "timeoutsec",
		 *  "timeoutusec", "retrywait" and "tries" and configures this
		 *  instance to use the result when connect() is called. */
		void	setParameters(
				dictionary<const char *, const char *> *cd);

		/** "host" may be a list of comma-separated hosts/addresses.
		 *  This method instructs connect() whether or not to randomize
		 *  this list.
		 *
		 *  Randomizing the list can provide inexpensive load balancing.
		 *
		 *  Not randomizing the list can provide inexpensive failover.
		 *
		 *  Defaults to true. */
		void	randomizeHosts(bool randomize);

		/** Instructs connect() whether or not to randomize the list of
		 *  addresses that "host" resolves to.  Defaults to true.
 		 *
 		 *  See connect() for issues that this can cause on some
 		 *  platforms or environments. */
		void	randomizeAddresses(bool randomize);

		/** Sets thte seed used when randomizing addresses to "seed".
		 *  
		 *  By default the random number generator used internally is
		 *  seeded using prng::getSeed().  This may or may not
		 *  be suitable as it will fall back to seeding the generator
		 *  with the time of day on platforms that don't support any
		 *  other method.
		 *
		 *  This method allows you to provide a more suitable seed
		 *  if necessary. */
		void	setRandomSeed(uint32_t seed);

		/** Attempts to connect to the "host" and "port" set
		 *  earlier using one of the setParameters() methods.
		 *  If the connection fails, it will retry, for a total of
		 *  "tries" attempts, waiting "retrywait" seconds
		 *  between each attempt.
		 *
		 *  If "host" is a comma-separated list of hosts, then an
		 *  attempt will be made to connect to each until the attempt
		 *  succeeds, or there are no more hosts left to try.
		 *
		 *  If each host resolves to multiple addresses (ie. if
		 *  round-robin DNS or another DNS-based load-balancing
		 *  strategy is used), then an attempt will be made to connect
		 *  to each address until the attempt succeeds or there are no
		 *  more addresses left to try.  This process will be tried
		 *  "tries" times.
		 *
		 *  For example, if...
		 *    * "host" is: "serverset1,serverset2"
		 *    * serverset1 resolves to 10.0.0.1 and 10.0.0.2
		 *    * serverset2 resolves to 10.0.1.1 and 10.0.1.2
		 *    * "tries" is set to 2
		 *  then 2 attempts will be made to connect to each of
		 *  10.0.0.1, 10.0.0.2, 10.0.1.1, and 10.0.1.2, by default, in
		 *  random order.
		 * 
		 *  Setting "tries" to 0 will cause it to try to 
		 *  connect to the first host/address indefinitely
		 *  (not recommended with multiple hosts).  Setting "retrywait"
		 *  to 0 will cause it to try to connect over and over as fast
		 *  as possible (not recommended).
		 * 
		 *  Each attempt to connect will wait "timeoutsec"
		 *  seconds and "timeoutusec" microseconds for the
		 *  connect to succeed.  Specifying -1 for either
		 *  parameter will cause the attempt to wait until the
		 *  underlying protocol times out which may be up to 2
		 *  minutes.
		 * 
		 *  Returns RESULT_SUCCESS on success and RESULT_ERROR
		 *  on failure.
		 * 
		 *  Note that if your system does not support
		 *  getaddrinfo() then connect() uses the hostentry and
		 *  protocolentry classes.  If you are using this method
		 *  in a multithreaded application, you may need to
		 *  supply those classes with mutexes.  See hostentry.h
		 *  and protocolentry.h for more detail.
		 *
		 *
		 *  Notes on round-robin DNS and DNS-based load-balancing:
		 *
		 *  By default, connect() randomizes the list of addresses that
		 *  each host resolves to.  This is intended to work around two
		 *  issues.
		 *
		 *
		 *  Issue 1:
		 *  When round-robin DNS is used, if one host is down, then the
		 *  next host in the list receives all of the traffic that would
		 *  have gone to the downed host.  It is usually prefereable
		 *  for the traffic to be distributed among the remaining hosts
		 *  instead.  Randomization of the list of addresses
		 *  accomplishes this.
		 *
		 *
		 *  Issue 2:
		 *  Rudiments uses getaddrinfo() if it is available, but
		 *  implementations of getaddrinfo() vary widely.
		 *
		 *  Some implementations sort the list of addresses that each
		 *  host resolves to rather than returning them in the order
		 *  returned by the DNS server, defeating any attempt at
		 *  DNS-based load-balancing.
		 *
		 *  Some implementations support a sortv4=no directive in
		 *  /etc/gai.conf that instructs getaddrinfo() not to sort the
		 *  addresses but many do not.
		 *
		 *  Some implementations randomize the list.
		 *
		 *  Some implementations return the list in the order returned
		 *  by DNS.  
		 *
		 *  Rudiments doesn't have a good way of knowing how
		 *  getaddrinfo() is implemented or configured, so to err on
		 *  the side of caution, connect() randomizes the list of
		 *  addresses that each host resolves to.
		 *
		 *
		 *  Randomization can be disabled and configured using
		 *  randomizeHosts(false) and/or randomizeAddresses(false).
		 */
		int32_t	connect();

	#include <rudiments/private/inetsocketclient.h>
};

#endif
