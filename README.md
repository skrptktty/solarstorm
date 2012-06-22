== README ==
== Arrogant Penguin - 2012 ==

Welcome to Project Solar Storm, this is a rethinking of the operating
system, and the ecosystem surrounding it.  I borrow heavily from the
XOmB kernel in the concepts surrounding the first iteration of the
build.  However, because I am using a differant programming language,
it will be a completely new system.

== Notes ==

As I continue to work on this there are somethings that are
noteworthy ::

1: I have decided that nothing that has been written previously is
within the scope of this project and as such a complete rewrite is
necessary.

2: Full POSIX 1.2008 compliance is necessary - and if a new release
of the POSIX compliance comes out before the release it shall be
converted to this new release.  The goal is to be fully POSIX
compliant. Link to POSIX 1.2008 ::
<http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html>

3: A new stdlib structure is needed to be POSIX compliant.  It should
be based on former stdlibs such as newlib however it should retire
deprecated syscalls and structures.

4: The graphical interface for the system - in particular the GUI -
is not defined with POSIX, however a compatible specification shall
be chosen to use for this.

5: The system shall strive to be easily ported to, for this reason I
have chosen to implement certain features that are optional within
the POSIX specification.
