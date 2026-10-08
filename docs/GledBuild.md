# STATUS

From the gled.org wiki (Foswiki), converted to markdown in 2026. Last edited in May 2013. Build notes for gled-builder on the distributions of that time.

# Gled build

## Build notes

This is system specific information, supplementing stuff available in
[gledimp](https://github.com/gled-project/gled/blob/main/docs/gledimp.md)
document.

### Debian / Ubuntu

`gled-builder` works OK, you need to pre-install some packages.

      apt-get install dpkg-dev autoconf libtool subversion cmake g++ gfortran \
        libglu1-mesa-dev libssl-dev libxft-dev libxpm-dev libfreetype6-dev libxi-dev \
        libxmu-dev libjpeg-dev libtiff-dev libpng-dev libmng-dev \
        libnetpbm10-dev libvorbis-dev libglib2.0-dev dialog \
        libgsl0ldbl libgsl0-dev \
        libpcre3-dev
      

### Fedora

Works fine from Fedora 14 to 18. The same list as for RHEL-6 below is OK.

### RHEL-6, CentOS-6, SLC-6

      yum install gcc gcc-c++ gcc-gfortran subversion cmake autoconf automake libtool \
        libXpm-devel libXft-devel glew glew-devel gsl gsl-devel pcre-devel openssl-devel \
        libpng-devel libtiff-devel libjpeg-devel libmng-devel netpbm-devel glib2-devel

### Mac OSX 10.5 with Fink

`gled-builder` works OK, you will need to get quite some stuff via fink:

      apt-get install subversion cmake pkgconfig dialog \
        netpbm10 netpbb10-shlibs glib2 glib2-dev glib2-shlibs xdg-base

Configure `gled-builder` with: `./configure CC=gcc ...`, `cc` is not
recognized properly by the compiler-version.pl.

There are some peculiarities, handled properly by the logick in package perl
scripts:

- `root` Configure for 32-bit build, fink stuff is 32-bit.
- `DevIL` Build without ILUT - GL is taken from X11 and some symbols are
  re-exported.

Issues with `gled` build:

- `.dylib` libs are created and `.so` sym-linked to them. This is not needed
  any more on 10.5, `.so` libraries can be built directly. This will
  simplify a couple of things - but then again - this issue is even worse on
  windows so some logick handling this issue could be kept.
- `MACOSX_DEPLOYMENT_TARGET=10.5` is set in environment scripts, script
  generation code and also in `greed` module scripts. 10.6 is coming soon,
  so this will have to be done better.

### Mac OSX 10.6, 10.7 and 10.8 with MacPorts

Get the MacPorts, configure them for universal binaries (by putting
`+unversal` into variants.conf) or for 64-bit builds (by setting arch to
`x86_64` in macports.conf). I think the defaults are OK, so you shouldn't
have to do anything.

Install the following ports, this will pull in all necessary stuff:

      sudo port install netpbm glib2 gsl libvorbis cmake pcre

`gled-builder` should work out of the box, the following `configure` options
should be used:

      ./configure EXTRA_PATHS=/opt/local MAKE_J_OPT=-j4 --build=x86_64-apple-darwin11.0.0   # --prefix=/opt/gled

## General issues with external packages

### DevIL

1.7.4 is used (slightly modified, global variable `color_type` had to be
declared as static in `il_png.c` and `il_icon.c` to avoid symbol collision
on mac). 1.7.8 is out, but had some problems building it. Somebody should
investigate.

-- Main.MatevzTadel - 03 Apr 2009
