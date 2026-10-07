# Document id

## Title

Somewhat urgent Gled developments

## Author

Matevz Tadel

## Date

**20.07.2008** -- Initial version

**14.12.2009** -- Small updates: OSX build, DevIL.

# External software, build and portability

## Build on FreeBSD, MacOSX, Windows

I succeeded building on OSX 10.5 with several packages installed via fink in
May 2009. At the moment (14.12.2009) this build is broken again (fails in
OpenAL). Somebody who knows more about OSX should decide whether to use fink
or MacPorts and check build options for Gled and external software.

### Major issues

1. Gled uses posix-threads. Make sure windows version is ok.
2. Build relies heavily on GNUmake and perl - use cygwin to build.
3. Makefiles are written for gcc - generalize that.
4. need portable hash\_map and hash\_set, both with multi variants.
5. Gled opens another X display and lets root run its own
one. Each run in separate thread and things mostly work. One needs to
lock root display when opening root windows from fltk.

    On mac root uses X11 and fltk uses quartz. As already said, this worked OK at
    some point. Fltk had to be put into the main thread.

6. On windows root uses gdk. I have no clue what this means for parallel
running of fltk.

## Reconsider usage of DevIL as image library

DevIL is not thread-safe but there were rumors on the list that somebody is
beginning to work on that (Dayton, from the top of my head). Someone should
investigate this. For now all DevIL access is protected with a global mutex.

### Tasks

1. Make DevIL thread-safe by using TLD. Either use posix-threads or
implement separate solution for windows.
2. Evaluate ROOT's after-step image library. My major complain
against it was that it stores image internally as a set of float
arrays and copies them to byte-formats on demand. Check if this is
still so and if there is a reasonable way to bypass this. Is it
thread-safe?
3. Evaluate other options (also g3d). Consider portability and
thread-safety.
4. Move image library into GledCore. Use it for screen dumping.
5. Use GL functions for image rescaling. While at it, optionally
generate mipmaps in ZImage\_GL\_Rnr.

# Central Gled

## Do not stream rays.

Store in ray:

a) Raw array of Eye\*s to which it was delivered. Each Eye sets its
value to zero.

b) Pointer to next/prev ray (global).

c) Add a mutex for access to _custom buffer_.

Thus we can cleanup the mess if some Eye goes banana.

Eyes should connect to each queen individually. Then, queens can send
rays only to eyes that observe them.

## class/libset ids.

better management of those

what about method ids?

## lens ids

    make each king (queen?) an independent object space.

## should we use namespaces for libsets?

class-naming consistency. now we have a mess (well, I have a mess).

## Object instantiation.

Especially for glasses and stones.

Check what root does lately with operator new for TObject.

Check Hoard.

# Minor annoyances
