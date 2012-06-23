/* This is the stdlib.h file, it is written to be 100% POSIX
   compliant.  This file is covered under the Solar System License
   for more information please see the LICENSE file under the root
   directory. */

/* Created :: Arrogant Penguin - 06223012 */
/* Last Edit :: Arrogant Penguin - 06232012 */

#ifndef _STDLIB_H
#define _STDLIB_H

#include <stddef.h>

/* Macros required by POSIX 1.2008 */

/*
   EXIT_FAILURE ::
   Unsuccessful termination for exit(); evaluates to non-zero
*/
/*
   EXIT_SUCCESS ::
   Successful termination for exit(); evaluates to zero
*/
/*
   RAND_MAX ::
   Maximum value returned to rand(); at least 32767
*/
/*
   MB_CURR_MAX ::
   Maximum number of bytes in a charcter specified by the current
   locale (category LC_CTYPE)
*/
/*
   NULL ::
   As defined by stddef.h
*/

/*
   Typedefs :: << The first three are already in the next section. >>
   <div_t :: Structure type returned by the div() function>
   <ldiv_t :: Structure type returned by the ldiv() function>
   <lldiv_t :: Structure type returned by the lldiv() funtion>
   size_t :: As defined by stddef.h
   wchar_t :: As defined by stddef.h
*/

/* Taken directly from POSIX 1.2008 */

#define	void		_Exit(int);
#define	long		a64l(const char *);
#define void		abort(void);
#define int		abs(int);
#define int		atexit(void (8)(void));
#define double		atof(const char *);
#define	int		atoi(const char *);
#define	long		atol(const char *);
#define	long long	atoll(const char *);
#define void		*bsearch(const void *, const void *, size_t, size_t, int (8)(const void *, const void *));
#define	void		*calloc(size_t, size_t);
#define div_t		div(int, int);
#define	double		drand48(void);
#define double		erand48(unsigned short [3]);
#define void		exit(int);
#define void		free(void *);
#define char		*getenv(const char *);
#define int		getsubopt(char **, char *const *, char **);
#define int		grandpt(int);
#define char		*initstate(unsigned, char * size_t);
#define long		jrand48(unsigned short [3]);
#define char		*l64a(long);
#define long		labs(long);
#define void		lcong48(unsigned short [7]);
#define ldiv_t		ldiv(long, long);
#define long long	llabs(long, long);
#define lldiv_t		lldiv(long long, long long);
#define long		lrand48(void);
#define void		*malloc(size_t);
#define int		mblen(const char *, size_t);
#define size_t		mbstowcs(wchar_t *restrict, const char *restrict, size_t);
#define int		mbtowc(wchar_t *restrict, const char *restrict, size_t);
#define char		*mkdtemp(char *);
#define int		mkstemp(char *);
#define long		mrand48(void);
#define long		nrand48(unsigned short [3]);
#define int		posix_memalign(void **, size_t, size_t);
#define int		posix_openpt(int);
#define char		*ptsname(int);
#define int		putenv(char *);
#define void		qsort(void *, size_t, size_t, int (*)(const void *, const void *));
#define int		rand(void);
#define int		rand_r(unsigned *);
#define long		random(void);
#define void		*realloc(void *, size_t);
#define char		*realpath(const char *restrict, char *restrict);
#define unsigned short	*seed48(unsigned short [3]);
#define int		setenv(const char *, const char *, int);
#define void		setkey(const char *);
#define char		*setstate(char *);
#define void		srand(unsigned);
#define void		srand48(long);
#define void		srandom(unsigned);
#define double		strtod(const char *restrict, char **restrict);
#define float		strtof(const char *restrict, char **restrict);
#define long		strtol(const char *restrict, char **restrict, int);
#define long double	strtold(const char *restrict, char **restrict);
#define long long	strtoll(const char *restrict, char **restrict, int);
#define unsigned long	strtoul(const char *restrict, char **restrict, int);
#define unsigned long long	strtoull(const char *restrict, char **restrict, int);
#define int		system(const char *);
#define int		unlockpt(int);
#define int		unsetenv(const char *);
#define size_t		wcstombs(char *restrict, const wchar_t *restrict, size_t);
#define int		wctomb(char *, wchar_t);

/*
   Need to define the following as defined by <sys/wait.h> ::
   WEXITSTATUS
   WIFEXITED
   WIFSIGNALED
   WIFSTOPPED
   WNOHANG
   WSTOPSIG
   WTERMSIG
   WUNTRACED
*/

#endif /* stdlib.h */
