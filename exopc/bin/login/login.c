/*	$OpenBSD: login.c,v 1.26 1998/07/13 02:11:36 millert Exp $	*/
/*	$NetBSD: login.c,v 1.13 1996/05/15 23:50:16 jtc Exp $	*/

/*-
 * Copyright (c) 1980, 1987, 1988, 1991, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
static char copyright[] =
"@(#) Copyright (c) 1980, 1987, 1988, 1991, 1993, 1994\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)login.c	8.4 (Berkeley) 4/2/94";
#endif
static char rcsid[] = "$OpenBSD: login.c,v 1.26 1998/07/13 02:11:36 millert Exp $";
#endif /* not lint */

/*
 * login [ name ]
 * login -h hostname	(for telnetd, etc.)
 * login -f name	(for pre-authenticated login: datakit, xterm, etc.)
 */

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/file.h>
#include <sys/wait.h>

#include <err.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <ttyent.h>
#include <tzfile.h>
#include <unistd.h>
#include <utmp.h>
#include <util.h>
#include <skey.h>

#include "pathnames.h"

void	 badlogin __P((char *));
void	 checknologin __P((void));
void	 dolastlog __P((int));
void	 getloginname __P((void));
void	 motd __P((void));
int	 rootterm __P((char *));
void	 sigint __P((int));
void	 sighup __P((int));
void	 sleepexit __P((int));
char	*stypeof __P((char *));
void	 timedout __P((int));
int	 pwcheck __P((char *, char *, char *, char *));
#if defined(KERBEROS) || defined(KERBEROS5)
int	 klogin __P((struct passwd *, char *, char *, char *));
void	 kdestroy __P((void));
void	 dofork __P((void));
void	 kgettokens __P((char *));
#endif

extern void login __P((struct utmp *));
extern int check_failedlogin __P((uid_t));
extern void log_failedlogin __P((uid_t, char *, char *, char *));

#define	TTYGRPNAME	"tty"		/* name of group to own ttys */

/*
 * This bounds the time given to login.  Not a define so it can
 * be patched on machines where it's too small.
 */
u_int	timeout = 300;

#if defined(KERBEROS) || defined(KERBEROS5)
int	notickets = 1;
char	*instance;
char	*krbtkfile_env;
int	authok;
#endif

struct	passwd *pwd;
int	failures;
char	term[64], *hostname, *tty;
char	*username = NULL, *rusername = NULL;

int
main(argc, argv)
	int argc;
	char *argv[];
{
	extern char **environ;
	struct group *gr;
	struct stat st;
	struct timeval tp;
	struct utmp utmp;
	int ask, ch, cnt, fflag, hflag, pflag, uflag, quietlog, rootlogin, rval;
	uid_t uid;
	char *domain, *p, *salt, *ttyn;
	char tbuf[MAXPATHLEN + 2], tname[sizeof(_PATH_TTY) + 10];
	char localhost[MAXHOSTNAMELEN];

	(void)signal(SIGALRM, timedout);
	(void)alarm(timeout);
	(void)signal(SIGQUIT, SIG_IGN);
	(void)signal(SIGINT, SIG_IGN);
	(void)signal(SIGHUP, sighup);
	(void)setpriority(PRIO_PROCESS, 0, 0);

	openlog("login", LOG_ODELAY, LOG_AUTH);

	/*
	 * -p is used by getty to tell login not to destroy the environment
	 * -f is used to skip a second login authentication
	 * -h is used by other servers to pass the name of the remote
	 *    host to login so that it may be placed in utmp and wtmp
	 */
	domain = NULL;
	if (gethostname(localhost, sizeof(localhost)) < 0)
		syslog(LOG_ERR, "couldn't get local hostname: %m");
	else
		domain = strchr(localhost, '.');
	if (domain) {
		domain++;
		if (*domain && strchr(domain, '.') == NULL)
			domain = localhost;
	}

	fflag = hflag = pflag = 0;
	uid = getuid();
	while ((ch = getopt(argc, argv, "fh:u:p")) != -1)
		switch (ch) {
		case 'f':
			fflag = 1;
			break;
		case 'h':
			if (uid)
				errx(1, "-h option: %s", strerror(EPERM));
			hflag = 1;
			if (domain && (p = strchr(optarg, '.')) &&
			    strcasecmp(p+1, domain) == 0)
				*p = 0;
			hostname = optarg;
			break;
		case 'p':
			pflag = 1;
			break;
		case 'u':
			if (uid)
				errx(1, "-u option: %s", strerror(EPERM));
			uflag = 1;
			rusername = optarg;
			break;
		case '?':
		default:
			if (!uid)
				syslog(LOG_ERR, "invalid flag %c", ch);
			(void)fprintf(stderr,
			    "usage: login [-fp] [-h hostname] [username]\n");
			exit(1);
		}
	argc -= optind;
	argv += optind;

	if (*argv) {
		username = *argv;
		ask = 0;
	} else
		ask = 1;

	for (cnt = getdtablesize(); cnt > 2; cnt--)
		(void)close(cnt);

	ttyn = ttyname(STDIN_FILENO);
	if (ttyn == NULL || *ttyn == '\0') {
		(void)snprintf(tname, sizeof(tname), "%s??", _PATH_TTY);
		ttyn = tname;
	}
	if ((tty = strrchr(ttyn, '/')))
		++tty;
	else
		tty = ttyn;

	for (cnt = 0;; ask = 1) {
#if defined(KERBEROS) || defined(KERBEROS5)
	        kdestroy();
#endif
		if (ask) {
			fflag = 0;
			getloginname();
		}
		rootlogin = 0;

#if defined(KERBEROS) || defined(KERBEROS5)
		/*
		 * Why should anyone with a root instance be able
		 * to be root here?
		 */
		instance = "";
#endif
#ifdef	KERBEROS
		if ((instance = strchr(username, '.')) != NULL) {
			if (strncmp(instance, ".root", 5) == 0)
				rootlogin = 1;
			*instance++ = '\0';
		} else
			instance = "";
#endif
#ifdef KERBEROS5
		if ((instance = strchr(username, '/')) != NULL) {
			if (strncmp(instance, "/root", 5) == 0)
				rootlogin = 1;
			*instance++ = '\0';
		} else
			instance = "";
#endif
		if (strlen(username) > UT_NAMESIZE)
			username[UT_NAMESIZE] = '\0';

		/*
		 * Note if trying multiple user names; log failures for
		 * previous user name, but don't bother logging one failure
		 * for nonexistent name (mistyped username).
		 */
		if (failures && strcmp(tbuf, username)) {
			if (failures > (pwd ? 0 : 1))
				badlogin(tbuf);
			failures = 0;
		}
		(void)strncpy(tbuf, username, sizeof tbuf-1);
		tbuf[sizeof tbuf-1] = '\0';

		if ((pwd = getpwnam(username)))
			salt = pwd->pw_passwd;
		else
			salt = "xx";

		/*
		 * if we have a valid account name, and it doesn't have a
		 * password, or the -f option was specified and the caller
		 * is root or the caller isn't changing their uid, don't
		 * authenticate.
		 */
		if (pwd) {
			if (pwd->pw_uid == 0)
				rootlogin = 1;

			if (fflag && (uid == 0 || uid == pwd->pw_uid)) {
				/* already authenticated */
				break;
			} else if (pwd->pw_passwd[0] == '\0') {
				/* pretend password okay */
				rval = 0;
				goto ttycheck;
			}
		}

		fflag = 0;

		(void)setpriority(PRIO_PROCESS, 0, -4);

		p = getpass("Password:");

		if (pwd) {
#if defined(KERBEROS) || defined(KERBEROS5)
			rval = klogin(pwd, instance, localhost, p);
			if (rval != 0 && rootlogin && pwd->pw_uid != 0)
				rootlogin = 0;
			if (rval == 0)
				authok = 1;
			else if (rval == 1) {
				if (pwd->pw_uid != 0)
					rootlogin = 0;
				rval = pwcheck(username, p, salt, pwd->pw_passwd);
			}
#else
			rval = pwcheck(username, p, salt, pwd->pw_passwd);
#endif
		} else {
#ifdef SKEY
			if (strcasecmp(p, "s/key") == 0)
				(void)skey_authenticate(username);
			else
#endif
			{
				useconds_t us;

				/*
				 * Sleep between 1 and 3 seconds
				 * to emulate a crypt.
				 */
				us = arc4random() % 3000000;
				usleep(us);
			}
			rval = 1;
		}
		memset(p, 0, strlen(p));

		(void)setpriority(PRIO_PROCESS, 0, 0);

	ttycheck:
		/*
		 * If trying to log in as root without Kerberos,
		 * but with insecure terminal, refuse the login attempt.
		 */
#if defined(KERBEROS) || defined(KERBEROS5)
		if (authok == 0)
#endif
		if (pwd && !rval && rootlogin && !rootterm(tty)) {
			(void)fprintf(stderr,
			    "%s login refused on this terminal.\n",
			    pwd->pw_name);
			if (hostname)
				syslog(LOG_NOTICE,
				    "LOGIN %s REFUSED FROM %s%s%s ON TTY %s",
				    pwd->pw_name, rusername ? rusername : "",
				    rusername ? "@" : "", hostname, tty);
			else
				syslog(LOG_NOTICE,
				    "LOGIN %s REFUSED ON TTY %s",
				     pwd->pw_name, tty);
			continue;
		}

		if (pwd && !rval)
			break;

		(void)printf("Login incorrect\n");
		failures++;
		if (pwd)
			log_failedlogin(pwd->pw_uid, hostname, rusername, tty);
		/* we allow 10 tries, but after 3 we start backing off */
		if (++cnt > 3) {
			if (cnt >= 10) {
				badlogin(username);
				sleepexit(1);
			}
			sleep((u_int)((cnt - 3) * 5));
		}
	}

	/* committed to login -- turn off timeout */
	(void)alarm((u_int)0);

	endpwent();

	/* if user not super-user, check for disabled logins */
	if (!rootlogin)
		checknologin();

	setegid(pwd->pw_gid);
	seteuid(pwd->pw_uid);

	if (chdir(pwd->pw_dir) < 0) {
		(void)printf("No home directory %s!\n", pwd->pw_dir);
		if (chdir("/"))
			exit(0);
		pwd->pw_dir = "/";
		(void)printf("Logging in with home = \"/\".\n");
	}

	quietlog = ((strcmp(pwd->pw_shell, "/sbin/nologin") == 0) ||
			(access(_PATH_HUSHLOGIN, F_OK) == 0));

	seteuid(0);
	setegid(0);	/* XXX use a saved gid instead? */

	if (pwd->pw_change || pwd->pw_expire)
		(void)gettimeofday(&tp, (struct timezone *)NULL);
	if (pwd->pw_change) {
		if (tp.tv_sec >= pwd->pw_change) {
			(void)printf("Sorry -- your password has expired.\n");
			sleepexit(1);
		} else if (pwd->pw_change - tp.tv_sec <
		    2 * DAYSPERWEEK * SECSPERDAY && !quietlog)
			(void)printf("Warning: your password expires on %s",
			    ctime(&pwd->pw_change));
	}
	if (pwd->pw_expire) {
		if (tp.tv_sec >= pwd->pw_expire) {
			(void)printf("Sorry -- your account has expired.\n");
			sleepexit(1);
		} else if (pwd->pw_expire - tp.tv_sec <
		    2 * DAYSPERWEEK * SECSPERDAY && !quietlog)
			(void)printf("Warning: your account expires on %s",
			    ctime(&pwd->pw_expire));
	}

	/* Nothing else left to fail -- really log in. */
	(void)signal(SIGHUP, SIG_DFL);
	memset((void *)&utmp, 0, sizeof(utmp));
	(void)time(&utmp.ut_time);
	(void)strncpy(utmp.ut_name, username, sizeof(utmp.ut_name));
	if (hostname)
		(void)strncpy(utmp.ut_host, hostname, sizeof(utmp.ut_host));
	(void)strncpy(utmp.ut_line, tty, sizeof(utmp.ut_line));
	login(&utmp);

	if (!quietlog)
		(void)check_failedlogin(pwd->pw_uid);
	dolastlog(quietlog);

	login_fbtab(tty, pwd->pw_uid, pwd->pw_gid);

	(void)chown(ttyn, pwd->pw_uid,
	    (gr = getgrnam(TTYGRPNAME)) ? gr->gr_gid : pwd->pw_gid);
#if defined(KERBEROS) || defined(KERBEROS5)
	/* Fork so that we can call kdestroy */
	if (krbtkfile_env)
	    dofork();
#endif
	(void)setegid(pwd->pw_gid);
	(void)setgid(pwd->pw_gid);

	initgroups(username, pwd->pw_gid);

	if (*pwd->pw_shell == '\0')
		pwd->pw_shell = _PATH_BSHELL;

	/* Destroy environment unless user has requested its preservation. */
	if (!pflag) {
		if ((environ = calloc(1, sizeof (char *))) == NULL)
			err(1, "calloc");
	} else {
		char **cpp, **cpp2;

		for (cpp2 = cpp = environ; *cpp; cpp++) {
			if (strncmp(*cpp, "LD_", 3) &&
			    strncmp(*cpp, "IFS=", 4))
				*cpp2++ = *cpp;
		}
		*cpp2 = 0;
	}
	(void)setenv("HOME", pwd->pw_dir, 1);
	(void)setenv("SHELL", pwd->pw_shell, 1);
	if (term[0] == '\0')
		(void)strncpy(term, stypeof(tty), sizeof(term));
	(void)setenv("TERM", term, 0);
	(void)setenv("LOGNAME", pwd->pw_name, 1);
	(void)setenv("USER", pwd->pw_name, 1);
	(void)setenv("PATH", _PATH_DEFPATH, 0);
	if (hostname)
		(void)setenv("REMOTEHOST", hostname, 1);
	if (rusername)
		(void)setenv("REMOTEUSER", rusername, 1);
#ifdef KERBEROS
	if (krbtkfile_env)
		(void)setenv("KRBTKFILE", krbtkfile_env, 1);
#endif
#ifdef KERBEROS5
	if (krbtkfile_env)
		(void)setenv("KRB5CCNAME", krbtkfile_env, 1);
#endif

	/* If fflag is on, assume caller/authenticator has logged root login. */
	if (rootlogin && fflag == 0) {
		if (hostname)
			syslog(LOG_NOTICE, "ROOT LOGIN (%s) ON %s FROM %s%s%s",
			    username, tty, rusername ? rusername : "",
			    rusername ? "@" : "", hostname);
		else
			syslog(LOG_NOTICE, "ROOT LOGIN (%s) ON %s", username, tty);
	}

#if defined(KERBEROS) || defined(KERBEROS5)
	if (!quietlog && notickets == 1)
		(void)printf("Warning: no Kerberos tickets issued.\n");
#endif

	if (!quietlog) {
#if 0
		(void)printf("%s\n\t%s  %s\n\n",
	    "Copyright (c) 1980, 1983, 1986, 1988, 1990, 1991, 1993, 1994",
		    "The Regents of the University of California. ",
		    "All rights reserved.");
#endif
		motd();
		(void)snprintf(tbuf,
		    sizeof(tbuf), "%s/%s", _PATH_MAILDIR, pwd->pw_name);
		if (stat(tbuf, &st) == 0 && st.st_size != 0)
			(void)printf("You have %smail.\n",
			    (st.st_mtime > st.st_atime) ? "new " : "");
	}

	(void)signal(SIGALRM, SIG_DFL);
	(void)signal(SIGQUIT, SIG_DFL);
	(void)signal(SIGINT, SIG_DFL);
	(void)signal(SIGTSTP, SIG_IGN);

	tbuf[0] = '-';
	(void)strncpy(tbuf + 1, (p = strrchr(pwd->pw_shell, '/')) ?
	    p + 1 : pwd->pw_shell, sizeof tbuf - 1 - 1);
	tbuf[sizeof tbuf - 1] = '\0';

	if (setlogin(pwd->pw_name) < 0)
		syslog(LOG_ERR, "setlogin() failure: %m");

	/* Discard permissions last so can't get killed and drop core. */
	if (rootlogin)
		(void) setuid(0);
	else {
		(void) seteuid(pwd->pw_uid);
		(void) setuid(pwd->pw_uid);
	}
#ifdef KERBEROS
	kgettokens(pwd->pw_dir);
#endif

	execlp(pwd->pw_shell, tbuf, 0);
	err(1, "%s", pwd->pw_shell);
}

int
pwcheck(user, p, salt, passwd)
	char *user, *p, *salt, *passwd;
{
#ifdef SKEY
	if (strcasecmp(p, "s/key") == 0)
		return skey_authenticate(user);
#endif
	return strcmp(crypt(p, salt), passwd);
}

#if defined(KERBEROS) || defined(KERBEROS5)
#define	NBUFSIZ		(UT_NAMESIZE + 1 + 5)	/* .root suffix */
#else
#define	NBUFSIZ		(UT_NAMESIZE + 1)
#endif

#if defined(KERBEROS) || defined(KERBEROS5)
/*
 * This routine handles cleanup stuff, and the like.
 * It exists only in the child process.
 */
#include <sys/wait.h>
void
dofork()
{
    int child;

    if (!(child = fork()))
	    return; /* Child process */

    /* Setup stuff?  This would be things we could do in parallel with login */
    (void) chdir("/");	/* Let's not keep the fs busy... */
    
    /* If we're the parent, watch the child until it dies */
    while (wait(0) != child)
	    ;

    /* Cleanup stuff */
    /* Run kdestroy to destroy tickets */
    kdestroy();

    /* Leave */
    exit(0);
}
#endif

void
getloginname()
{
	int ch;
	char *p;
	static char nbuf[NBUFSIZ];

	for (;;) {
		(void)printf("login: ");
		for (p = nbuf; (ch = getchar()) != '\n'; ) {
			if (ch == EOF) {
				badlogin(username);
				exit(0);
			}
			if (p < nbuf + (NBUFSIZ - 1))
				*p++ = ch;
		}
		if (p > nbuf) {
			if (nbuf[0] == '-')
				(void)fprintf(stderr,
				    "login names may not start with '-'.\n");
			else {
				*p = '\0';
				username = nbuf;
				break;
			}
		}
	}
}

int
rootterm(ttyn)
	char *ttyn;
{
	struct ttyent *t;

	return ((t = getttynam(ttyn)) && t->ty_status & TTY_SECURE);
}

jmp_buf motdinterrupt;

void
motd()
{
	int fd, nchars;
	sig_t oldint;
	char tbuf[8192];

	if ((fd = open(_PATH_MOTDFILE, O_RDONLY, 0)) < 0)
		return;
	oldint = signal(SIGINT, sigint);
	if (setjmp(motdinterrupt) == 0)
		while ((nchars = read(fd, tbuf, sizeof(tbuf))) > 0)
			(void)write(fileno(stdout), tbuf, nchars);
	(void)signal(SIGINT, oldint);
	(void)close(fd);
}

/* ARGSUSED */
void
sigint(signo)
	int signo;
{
	longjmp(motdinterrupt, 1);
}

/* ARGSUSED */
void
timedout(signo)
	int signo;
{
	(void)fprintf(stderr, "Login timed out after %d seconds\n", timeout);
	exit(0);
}

void
checknologin()
{
	int fd, nchars;
	char tbuf[8192];

	if ((fd = open(_PATH_NOLOGIN, O_RDONLY, 0)) >= 0) {
		while ((nchars = read(fd, tbuf, sizeof(tbuf))) > 0)
			(void)write(fileno(stdout), tbuf, nchars);
		sleepexit(0);
	}
}

void
dolastlog(quiet)
	int quiet;
{
	struct lastlog ll;
	int fd;

	if ((fd = open(_PATH_LASTLOG, O_RDWR, 0)) >= 0) {
		(void)lseek(fd, (off_t)pwd->pw_uid * sizeof(ll), SEEK_SET);
		if (!quiet) {
			if (read(fd, (char *)&ll, sizeof(ll)) == sizeof(ll) &&
			    ll.ll_time != 0) {
				(void)printf("Last login: %.*s ",
				    24-5, (char *)ctime(&ll.ll_time));
				(void)printf("on %.*s",
				    (int)sizeof(ll.ll_line),
				    ll.ll_line);
				if (*ll.ll_host != '\0')
					(void)printf(" from %.*s",
					    (int)sizeof(ll.ll_host),
					    ll.ll_host);
				(void)putchar('\n');
			}
			(void)lseek(fd, (off_t)pwd->pw_uid * sizeof(ll),
			    SEEK_SET);
		}
		memset((void *)&ll, 0, sizeof(ll));
		(void)time(&ll.ll_time);
		(void)strncpy(ll.ll_line, tty, sizeof(ll.ll_line));
		if (hostname)
			(void)strncpy(ll.ll_host, hostname, sizeof(ll.ll_host));
		(void)write(fd, (char *)&ll, sizeof(ll));
		(void)close(fd);
	}
}

void
badlogin(name)
	char *name;
{
	if (failures == 0)
		return;
	if (hostname) {
		syslog(LOG_NOTICE, "%d LOGIN FAILURE%s FROM %s%s%s",
		    failures, failures > 1 ? "S" : "",
		    rusername ? rusername : "", rusername ? "@" : "", hostname);
		syslog(LOG_AUTHPRIV|LOG_NOTICE,
		    "%d LOGIN FAILURE%s FROM %s%s%s, %s",
		    failures, failures > 1 ? "S" : "",
		    rusername ? rusername : "", rusername ? "@" : "",
		    hostname, name);
	} else {
		syslog(LOG_NOTICE, "%d LOGIN FAILURE%s ON %s",
		    failures, failures > 1 ? "S" : "", tty);
		syslog(LOG_AUTHPRIV|LOG_NOTICE,
		    "%d LOGIN FAILURE%s ON %s, %s",
		    failures, failures > 1 ? "S" : "", tty, name);
	}
}

#undef	UNKNOWN
#define	UNKNOWN	"su"

char *
stypeof(ttyid)
	char *ttyid;
{
	struct ttyent *t;

	return (ttyid && (t = getttynam(ttyid)) ? t->ty_type : UNKNOWN);
}

void
sleepexit(eval)
	int eval;
{
	(void)sleep(5);
	exit(eval);
}

void
sighup(signum)
	int signum;
{
	if (username)
		badlogin(username);

	exit(W_EXITCODE(0, signum));
}
