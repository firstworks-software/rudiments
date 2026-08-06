# Rudiments - agent notes

## What this is

Rudiments: a C++ foundation class library from firstworks. It provides portable
wrappers over operating-system facilities (files, directories, processes,
threads, sockets, shared memory, semaphores), common data structures
(dynamicarray, linkedlist, avltree, dictionary), crypto and hash routines (md5,
sha1, sha256, aes128, des, pbkdf2, dh), parsers with matching DOM and SAX
interfaces (xml, json, csv, ini, prop), an http client and server API, string
and character-encoding helpers, and more. There is roughly one class per
feature.

SQL Relay and several sibling firstworks repos link against this library, so
API and behavior changes here ripple out to them. Rudiments also ships the
autoconf macros (`autoconf/acfirstworks.m4`, `autoconf/acrudiments.m4`) that
those dependent projects pull in at their own `autogen.sh` time.

## Build

Autoconf plus libtool build. The install prefix defaults to
`/usr/local/firstworks` (`AC_PREFIX_DEFAULT` in `configure.in`).

```
./autogen.sh            # regenerate configure from configure.in (rarely needed)
./configure [opts]      # produces config.mk, config.h, libtool, etc.
make                    # top-level; descends into src/
make install            # installs to $prefix
make clean
make distclean          # also removes configure-generated files
make cppcheck           # run cppcheck across src/
```

`autogen.sh` runs `libtoolize`, assembles `aclocal.m4` from the system libtool
macros plus `autoconf/acfirstworks.m4` and `autoconf/acrudiments.m4`, then runs
`autoconf` and `autoheader`.

`configure` generates `config.mk` (from `config.mk.in`),
`include/rudiments/private/config.h` (from `config.h.in` via autoheader),
`libtool`, `rudiments.pc`, `bin/rudiments-config`, `src/Makefile`, and
`include/Makefile`. The top-level `Makefile` descends into `src/` and links the
libtool library `librudiments.la`; it also builds `librudiments-apache.la` when
the Apache http server API module is enabled.

Common `./configure` toggles (see `configure.in`): `--enable-debug`,
`--enable-fortify`, `--enable-profiling`, `--enable-small-code`, and
`--disable-{ssl,gss,pcre,libcurl,libedit,libiconv,wall,werror}`. Each optional
dependency also takes `--with-<dep>-includes` and `--with-<dep>-libs`; otherwise
the dependency is auto-probed.

## Code layout

- `src/` - flat, one `.cpp` per class, no subdirectories. All compiled into
  `librudiments`.
- `include/rudiments/` - the public headers, one per class, carrying the
  doxygen doc comments.
- `include/rudiments/private/` - the private implementation details each public
  header pulls in: a `<name>includes.h` (system headers), `*inlines.h` bodies,
  node and helper classes, and the generated `config.h`.
- `test/` - one standalone test program per class.
- `examples/` - small standalone example programs (also built by make).
- `bench/` - micro-benchmarks, often comparing a rudiments class against its STL
  counterpart.
- `behaviors/` - tiny probes of platform behavior.
- `doc/` - the project website and reference docs (see below).
- `autoconf/` - the `.m4` macros installed for dependent projects to consume.
- `bin/` - `rudiments-config`, a pkg-config-style helper, generated from
  `rudiments-config.in`.
- `msvc/`, `config_windows.*`, `configure.vbs` - the Windows build.

## Tests

`make tests` builds everything under `test/` and runs `test/testall.sh`, which
runs each per-class program in turn and writes `test/testresults.log`. The exit
code is non-zero if any test fails, except `handoff`, which is reported but not
counted as a failure. Tests are standalone programs linked against the freshly
built library - there are no external services to stand up (a few use the TLS
certs generated under `test/certificates/`). To run just one:
`cd test && make charstring && ./charstring`.

Run a single test through its libtool wrapper, not the binary under test/.libs.
The wrapper prepends src/.libs, so the test links the library you just built.
The binary under test/.libs resolves librudiments from the install prefix
instead, so it silently tests the installed library - it will happily pass
while your change is not in it. testall.sh runs the wrappers, so make tests is
unaffected.

```
cd test
make charstring
./charstring          # the wrapper - links the build tree
./.libs/charstring    # links the INSTALLED library, not what you want
```

## Conventions worth knowing

- **C++ standard is C++98.** Configure compiles with `-std=gnu++98` when the
  compiler supports it (`FW_CHECK_STDGNU98`). Don't use C++11+ features: no
  `auto` deduction, no lambdas, no range-based `for`, no `nullptr` (use `NULL`),
  no `std::move`, no `=delete`/`=default`, no strongly-typed enums.
- **Copyright header**: every `.cpp` and `.h` starts with
  `// Copyright (c) David Muse` / `// See the COPYING file for more information`.
  Preserve it on edits; add it to new files.
- **The public headers carry doxygen comments** and are the source for the API
  reference (see `doc/classes`). Keep the doc comment in sync when you change a
  method signature or its behavior.
- **Generated files - never hand-edit.** Edit the `.in` source and rerun
  configure instead: `config.mk` (from `config.mk.in`),
  `include/rudiments/private/config.h` (from `config.h.in`; add new `#undef`
  entries there for autoheader), `rudiments.pc` (from `rudiments.pc.in`),
  `bin/rudiments-config` (from `.in`), and `src/Makefile` / `include/Makefile`
  (from their `.in` files). `configure`, `aclocal.m4`, and `acsite.m4` are
  produced by `autogen.sh`. The `doc/*.html` pages are generated from `.wt`.
- **Adding a new class means editing `configure.in`.** The `SRCS`, `LOBJS`,
  `HEADERS*`, and `PRIVATEHEADERS*` file lists are built up in `configure.in`
  (each class appended, most of them gated behind a feature probe) and
  substituted into `src/Makefile` and `include/Makefile`. Add your
  `src/<name>.cpp`, `include/rudiments/<name>.h`, and any private headers, then
  add them to those lists in `configure.in` and rerun configure. Editing the
  `Makefile.in` files alone will not pick a new file up.
- **Docs.** `doc/` is the website: `.wt` files are compiled to `.html` by the
  `wt` tool (run `make` in `doc/`). `doc/classes/` holds the doxygen API
  reference - `doc/classes/makedoc` runs `doxygen doxygen.config` against the
  public headers. `doc/programming/` has per-class tutorial pages. None of this
  is built by the top-level `make`; it is only handled on `make install`.
- **ChangeLog.** Append entries for user-facing changes under the current
  version section at the top of the file (the newest section is first). Don't
  log test-only changes.
- **Windows build is separate**: `config_windows.mk`, `config_windows.h`,
  `configure.vbs`, and the `msvc/` solutions. Changes that touch build
  variables usually need to be mirrored there.
- **License**: the library (`src/` and `include/`) is LGPL 2; see `COPYING`,
  which sets distinct terms for the library, tests, docs, and examples. Check it
  before copying code across those boundaries.
