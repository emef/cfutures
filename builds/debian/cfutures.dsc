Format:         1.0
Source:         cfutures
Version:        0.0.0-1
Binary:         libcfutures0, cfutures-dev
Architecture:   any all
Maintainer:     John Doe <John.Doe@example.com>
Standards-Version: 3.9.5
Build-Depends: bison, debhelper (>= 8),
    pkg-config,
    automake,
    autoconf,
    libtool,
    dh-autoreconf

Package-List:
 libcfutures0 deb net optional arch=any
 cfutures-dev deb libdevel optional arch=any

