# Rudiments

Rudiments is a C++ foundation class library. It wraps the operating system
facilities, data structures, and parsers that most C++ programs end up
rewriting, and it does it portably, so the same code builds on systems that are
decades apart.

There is roughly one class per feature, and the classes stay close to the
underlying system call rather than hiding it behind an abstraction.

- Project site: http://rudiments.sourceforge.net
- Class reference: http://rudiments.sourceforge.net/classes
- Main consumer: [SQL Relay](https://github.com/firstworks-software/sqlrelay)

## What is in it

- **Operating system facilities.** Files, directories, processes, threads,
  signals, sockets, shared memory, and semaphores.
- **Data structures.** dynamicarray, linkedlist, avltree, dictionary, and
  friends.
- **Crypto and hashing.** md5, sha1, sha256, aes128, des, pbkdf2, and dh.
- **Parsers.** xml, json, csv, ini, and prop, each with matching DOM and SAX
  interfaces.
- **Networking.** An http client and an http server API.
- **Strings and encodings.** String helpers and character set conversion.

## Portability

Rudiments runs on Linux, Solaris, FreeBSD, NetBSD, OpenBSD, Minix, SCO
OpenServer, SCO UnixWare, GNU Hurd, Haiku, Syllable, and modern Windows. Other
Unix-like systems often work too.

It builds for x86_64 and x86, and for non-x86 architectures including ARM,
Sparc, Mips, and PPC. uClibc based systems are supported.

Where a platform offers a better function, Rudiments uses it and falls back
elsewhere. For example, it prefers kqueue, epoll, port_create, and /dev/poll to
select, and prefers reentrant functions to mutex-protected thread-unsafe ones.

## Building

```
./configure
make
sudo make install
```

The default install prefix is /usr/local/firstworks. Run the following for the
full list of options.

```
./configure --help
```

Rudiments also ships the autoconf macros in autoconf/ that dependent firstworks
projects pull in at their own autogen.sh time.

## License

The library itself, under src/ and include/, is covered by the GNU Library
General Public License version 2, so it is free to use in free software and in
commercial systems alike. The tests, documentation, and example code have their
own terms. See [COPYING](COPYING) for all of them.

## Contributing

Issues, discussions, and pull requests are welcome here on GitHub.

One thing to know before you push: this repository is a mirror. The canonical
repository is the firstworks git server, and both this one and the SourceForge
one are refreshed from it every hour. A pull request is applied on the
canonical repository and then shows up here on the next refresh, so it lands as
an ordinary commit rather than a merge commit.

Please do not push directly to this repository. A direct push makes the mirror
diverge from the canonical repository, and the next refresh fails.
