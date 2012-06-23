/* This is the limits.h file, it is written to be 100% POSIX
   compliant.  This file is covered under the Solar System License
   for more information please see the LICENSE file under the root
   directory. */

/* Arrogant Penguin - 06222012 */

#ifndef _LIMITS_H
#define _LIMITS_H

/* Currently set to minimum POSIX requirements */

/*
   Maximum number of I/O operations in a single list I/O call
*/
#define AIO_LISTIO_MAX _POSIX_AIO_LISTIO_MAX
/*
   Maximum number of outstanding asynchronous I/O operations
*/
#define AIO_MAX _POSIX_AIO_MAX
/*
   Maximum amount by which a process can decrease its asyncronous
   I/O priority level from its own scheduling priority
*/
#define AIO_PRIO_DELTA_MAX 0
/*
   Maximum length of argument to the exec functions including
   environment data
*/
#define ARG_MAX _POSIX_ARG_MAX
/*
   Maximum number of functions that may be registered with
   atexit()
*/
#define ATEXIT_MAX 32
/*
   Maximum number of simultaneous processes per real user ID
*/
#define CHILD_MAX _POSIX_CHILD_MAX
/*
   Maximum number of time expiration overruns
*/
#define DELAYTIMER_MAX _POSIX_DELAY_TIMER_MAX
/*
   Maximum length of a host name as returned from the
   gethostname() function
*/
#define HOST_NAME_MAX _POSIX_HOST_NAME_MAX
/*
   Maximum number of iovec structures that one process has
   available for use with readv() or writev()
*/
#define IOV_MAX _XOPEN_IOV_MAX
/*
   Maximum length of a login name
*/
#define LOGIN_NAME_MAX _POSIX_LOGIN_NAME_MAX
/*
   Maximum number of open message queue decriptors a process may
   hold
*/
#define MQ_OPEN_MAX _POSIX_MQ_OPEN_MAX
/*
   Maximum number of message priorities supported by the
   implementation
*/
#define MQ_PRIO_MAX _POSIX_MQ_PRIO_MAX
/*
   A value one greater than the maximum value that the system may
   assign to a newly-created file descriptor
*/
#define OPEN_MAX _POSIX_OPEN_MAX
/*
   Size in bytes of a page
*/
/* #define PAGESIZE 1 */ /* I am defaulting to the XSI specification */
#define PAGE_SIZE 1 /* XSI specification */
/*
   Maximum number of attempts made to destroy a thread's
   thread-specific data values on thread exit
*/
#define PTHREAD_DESTRUCTOR_ITERATIONS _POSIX_THREAD_DESTRUCTOR_ITERATIONS
/*
   Maximum number of data keys that can be created by a process
*/
#define PTHREAD_KEYS_MAX _POSIX_THREAD_KEYS_MAX
/*
   Minimum size in bytes of thread stack storage
*/
#define PTHREAD_STACK_MIN 0
/*
   Maximum number of threads that can be created per process
*/
#define PTHREAD_THREADS_MAX _POSIX_THREAD_THREADS_MAX
/*
   Maximum number of repeated occurrences of a BRE or ERE interval
   expression
*/
#define RE_DUP_MAX _POSIX2_RE_DUP_MAX
/*
   Maximum number of realtime signals reserved for application use in
   this implementation
*/
#define RTSIG_MAX _POSIX_RTSIG_MAX
/*
   Maximum number of semaphores that a process may have
*/
#define SEM_NSEMS_MAX _POSIX_SEMS_NSEMS_MAX
/*
   The maximum value a semaphore may have
*/
#define SEM_VALUE_MAX _POSIX_SEM_VALUE_MAX
/*
   Maximum number of queued signals that a process may send and have
   pending at the receiver(s) at any time
*/
#define SIGQUEUE_MAX _POSIX_SIGQUEUE_MAX
/*
   The maximum number of replenishment operations that may be
   simultaneously pending for a particular sporadic server scheduler
*/
#define SS_REPL_MAX _POSIX_SS_REPL_MAX /* Process Sporadic Server specific */
/*
   Maximum number of streams that one process can have open at one
   time.  If defined, it has the same value as {FOPEN_MAX}
*/
#define STREAM_MAX _POSIX_STREAM_MAX
/*
   Maximum number of symbolic links that can be reliably traversed in
   the resolution of a pathname in the absence of a loop
*/
#define SYMLOOP_MAX _POSIX_SYMLOOP_MAX
/*
   Maximum number of timers per process supported by the
   implementation
*/
#define TIMER_MAX _POSIX_TIMER_MAX
/*
   Maximum length of the trace event name (not including the
   terminating null)
*/
#define TRACE_EVENT_NAME_MAX _POSIX_TRACE_EVENT_NAME_MAX
/*
   Maximum length of the trace generation version string or of the
   trace stream name (not including the terminating null)
*/
#define TRACE_NAME_MAX _POSIX_TRACE_NAME_MAX
/*
   Maximum number of trace streams that may simultaneously exist in
   the system
*/
#define TRACE_SYS_MAX _POSIX_TRACE_SYS_MAX
/*
   Maximum number of user trace event type identifiers that may
   simultaneously exist in a traced process, including the predefined
   user trace event POSIX_TRACE_UNNAMED_USER_EVENT
*/
#define TRACE_USER_EVENT_MAX _POSIX_TRACE_USER_EVENT_MAX
/*
   Maximum length of terminal device name
*/
#define TTY_NAME_MAX _POSIX_TTY_NAME_MAX
/*
   Maximum number of bytes supported for the name of a timezone (not
   of the TZ variable)
*/
#define TZNAME_MAX _POSIX_TZNAME_MAX

/* The following is pathname variable specific */

/*
   Minimum number of bits needed to represent, as a signed integer
   value, the maximum size of a regular file allowed in the specified
   directory
*/
#define FILESIZEBITS 32
/*
   Maximum number of links to a single file
*/
#define LINK_MAX _POSIX_LINK_MAX
/*
   Maximum number of bytes in a terminal canonical input line
*/
#define MAX_CANON _POSIX_MAX_CANON
/*
   Minimum number of bytes for which space is available in a
   terminal input queue; therefore, the maximum number of bytes a
   conforming application may require to be typed as input before
   reading them
*/
#define MAX_INPUT _POSIX_MAX_INPUT
/*
   Maximum number of bytes in a filename (not including the
   terminating null)
*/
/* #define NAME_MAX _POSIX_NAME_MAX */ /* I am defaulting to the XSI specification */
#define NAME_MAX _XOPEN_NAME_MAX /* XSI Specification */
/*
   Maximum number of bytes the implementation will store as a
   pathname in a user-supplied buffer of unspecified size,
   including the terminating null character. Minimum number the
   implementation will accept as the maximum number of bytes in a
   pathname
*/
/* #define PATH_MAX _POSIX_PATH_MAX */ /* I am defaulting to the XSI specification */
#define PATH_MAX _XOPEN_PATH_MAX /* XSI Specification */
/*
   Maximum number of bytes that is guaranteed to be atomic when
   writing to a pipe
*/
#define PIPE_BUF _POSIX_PIPE_BUF
/*
   Minimum number of bytes of storage actually allocated for any
   portion of a file

   ~~ No minimum specified ~~
*/
#define POSIX_ALLOC_SIZE_MIN 1
/*
   Recommended increment for file transfer sizes between the
   {POSIX_REC_MIN_XFER_SIZE} and {POSIX_REC_MAX_XFER_SIZE} values

   ~~ No minimum specified ~~
*/
/* #define POSIX_REC_INCR_XFER_SIZE */
/*
   Maximum recommended file transfer size

   ~~ No minimum specified ~~
*/
/* #define POSIX_REC_MAX_XFER_SIZE */
/*
   Minimum recommended file transfer size

   ~~ No minimum specified ~~
*/
/* #define POSIX_REC_MIN_XFER_SIZE */
/*
   Recommended file transfer buffer alignment

   ~~ No minimum specified ~~
*/
/* #define POSIX_REC_XFER_ALLIGN */
/*
   Maximum number of bytes in a symbolic link
*/
#define SYMLINK_MAX _POSIX_SYMLINK_MAX

/* The following is runtime specific */

/*
   Maximum obase values allowed by the bc utility
*/
#define BC_BASE_MAX _POSIX2_BC_BASE_MAX
/*
   Maximum number of elements permitted in an array by the bc utility
*/
#define BC_DIM_MAX _POSIX2_BC_DIM_MAX
/*
   Maximum scale value allowed by the bc utility
*/
#define BC_SCALE_MAX _POSIX2_BC_SCALE_MAX
/*
   Maximum length of a string constant accepted by the bc utility
*/
#define BC_STRING_MAX _POSIX2_BC_STRING_MAX
/*
   Maximum number of bytes in a character class name
*/
#define CHARCLASS_NAME_MAX _POSIX2_CHARCLASS_NAME_MAX
/*
   Maximum number of weights that can be assigned to an entry of the
   LC_COLLATE order keyword in the locale definition file
*/
#define COLL_WEIGHTS_MAX POSIX2_COLL_WEIGHTS_MAX
/*
   Maximum number of expressions that can be nested within
   parentheses by the expr utility
*/
#define EXPR_NEST_MAX _POSIX2_EXPR_NEST_MAX
/*
   Unless otherwise noted, the maximum length, in bytes, of a
   utility's input line (either standard input or another file),
   when the utility is described as processing text files. The
   length includes room for the trailing <newline>
*/
#define LINE_MAX _POSIX2_LINE_MAX
/*
   Maximum number of simultaneous supplementary group IDs per process
*/
#define NGROUPS_MAX _POSIX_NGROUPS_MAX
/*
   Maximum number of repeated occurrences of a regular expression
   permitted when using the interval notation \{m,n\}
*/
#define RE_DUP_MAX _POSIX2_RE_DUP_MAX

/* The following is the maximum value restricted by POSIX */

/*
   The resolution of the CLOCK_REALTIME clock, in nanoseconds
*/
#define _POSIX_CLOCKRES_MIN 20000000

/* The following is the minimum values restricted by POSIX */

/*
   The number of I/O operations that can be specified in a list I/O
   call
*/
#define _POSIX_AIO_LISTIO_MAX 2
/*
   The number of outstanding asynchronous I/O operations
*/
#define _POSIX_AIO_MAX 1
/*
   Maximum length of argument to the exec functions including
   environment data
*/
#define _POSIX_ARG_MAX 4096
/*
   Maximum number of simultaneous processes per real user ID
*/
#define _POSIX_CHILD_MAX 25
/*
   The number of timer expiration overruns
*/
#define _POSIX_DELAYTIMER_MAX 32
/*
   Maximum length of a host name (not including the terminating null)
   as returned from the gethostname() function
*/
#define _POSIX_HOST_NAME_MAX 255
/*
   Maximum number of links to a single file
*/
#define _POSIX_LINK_MAX 8
/*
   The size of the storage required for a login name, in bytes
   (including the terminating null)
*/
#define _POSIX_LOGIN_NAME_MAX 9
/*
   Maximum number of bytes in a terminal canonical input queue
*/
#define _POSIX_MAX_CANON 255
/*
   Maximum number of bytes allowed in a terminal input queue
*/
#define _POSIX_MAX_INPUT 255
/*
   The number of message queues that can be open for a single
   process.
*/
#define _POSIX _POSIX_MQ_OPEN_MAX 8
/*
   The maximum number of message priorities supported by the
   implementation.
*/
#define _POSIX _POSIX_MQ_PRIO_MAX 32
/*
   Maximum number of bytes in a filename (not including the
   terminating null).
*/
#define _POSIX _POSIX_NAME_MAX 14
/*
   Maximum number of simultaneous supplementary group IDs per
   process.
*/
#define _POSIX _POSIX_NGROUPS_MAX 8
/*
   Maximum number of files that one process can have open at any one
   time.
*/
#define _POSIX _POSIX_OPEN_MAX 20
/*
   Minimum number the implementation will accept as the maximum
   number of bytes in a pathname.
*/
#define _POSIX _POSIX_PATH_MAX 256
/*
   Maximum number of bytes that is guaranteed to be atomic when
   writing to a pipe.
*/
#define _POSIX _POSIX_PIPE_BUF 512
/*
   The number of repeated occurrences of a BRE permitted by the
   regexec() and regcomp() functions when using the interval notation
   {\(m,n\}; see BREs Matching Multiple Characters .
*/
#define _POSIX _POSIX_RE_DUP_MAX 255
/*
   The number of realtime signal numbers reserved for application use.
*/
#define _POSIX _POSIX_RTSIG_MAX 8
/*
   The number of semaphores that a process may have.
*/
#define _POSIX _POSIX_SEM_NSEMS_MAX 256
/*
   The maximum value a semaphore may have.
*/
#define _POSIX _POSIX_SEM_VALUE_MAX 32767
/*
   The number of queued signals that a process may send and have
   pending at the receiver(s) at any time.
*/
#define _POSIX _POSIX_SIGQUEUE_MAX 32
/*
   The value that can be stored in an object of type ssize_t.
*/
#define _POSIX _POSIX_SSIZE_MAX 32767
/*
   The number of replenishment operations that may be
   simultaneously pending for a particular sporadic server scheduler.
*/
#define _POSIX _POSIX_SS_REPL_MAX 4
/*
   The number of streams that one process can have open at one time.
*/
#define _POSIX _POSIX_STREAM_MAX 8
/*
   The number of bytes in a symbolic link.
*/
#define _POSIX _POSIX_SYMLINK_MAX 255
/*
   The number of symbolic links that can be traversed in the
   resolution of a pathname in the absence of a loop.
*/
#define _POSIX _POSIX_SYMLOOP_MAX 8
/*
   The number of attempts made to destroy a thread's thread-specific
   data values on thread exit.
*/
#define _POSIX _POSIX_THREAD_DESTRUCTOR_ITERATIONS 4
/*
   The number of data keys per process.
*/
#define _POSIX _POSIX_THREAD_KEYS_MAX 128
/*
   The number of threads per process.
*/
   #define _POSIX _POSIX_THREAD_THREADS_MAX 64
/*
   The per-process number of timers.
*/
#define _POSIX _POSIX_TIMER_MAX 32
/*
   The length in bytes of a trace event name (not including the
   terminating null).
*/
#define _POSIX _POSIX_TRACE_EVENT_NAME_MAX 30
/*
   The length in bytes of a trace generation version string or a
   trace stream name (not including the terminating null).
*/
#define _POSIX _POSIX_TRACE_NAME_MAX 8
/*
   The number of trace streams that may simultaneously exist in the
   system.
*/
#define _POSIX _POSIX_TRACE_SYS_MAX 8
/*
   The number of user trace event type identifiers that may
   simultaneously exist in a traced process, including the predefined
   user trace event POSIX_TRACE_UNNAMED_USER_EVENT.
*/
#define _POSIX _POSIX_TRACE_USER_EVENT_MAX 32
/*
   The size of the storage required for a terminal device name, in
   bytes (including the terminating null).
*/
#define _POSIX _POSIX_TTY_NAME_MAX 9
/*
   Maximum number of bytes supported for the name of a timezone (not
   of the TZ variable).
*/
#define _POSIX _POSIX_TZNAME_MAX 6
/*
   Maximum obase values allowed by the bc utility.
*/
#define _POSIX2_BC_BASE_MAX 99
/*
   Maximum number of elements permitted in an array by the bc utility
*/
#define _POSIX2_BC_DIM_MAX 2048
/*
   Maximum scale value allowed by the bc utility.
*/
#define _POSIX2_BC_SCALE_MAX 99
/*
   Maximum length of a string constant accepted by the bc utility.
*/
#define _POSIX2_BC_STRING_MAX 1000
/*
   Maximum number of bytes in a character class name.
*/
#define _POSIX2_CHARCLASS_NAME_MAX 14
/*
   Maximum number of weights that can be assigned to an entry of the
   LC_COLLATE order keyword in the locale definition file; see Locale
*/
#define _POSIX2_COLL_WEIGHTS_MAX 2
/*
   Maximum number of expressions that can be nested within
   parentheses by the expr utility.
*/
#define _POSIX2_EXPR_NEST_MAX 32
/*
   Unless otherwise noted, the maximum length, in bytes, of a
   utility's input line (either standard input or another file), when
   the utility is described as processing text files. The length
   includes room for the trailing <newline>.
*/
#define _POSIX2_LINE_MAX 2048
/*
   Maximum number of repeated occurrences of a regular expression
   permitted when using the interval notation \{m,n\}; see Regular
   Expressions .
*/
#define _POSIX2_RE_DUP_MAX 255
/*
   Maximum number of iovec structures that one process has available
   for use with readv() or writev().
*/
#define _XOPEN_IOV_MAX 16
/*
   Maximum number of bytes in a filename (not including the
   terminating null).
*/
#define _XOPEN_NAME_MAX 255
/*
   Minimum number the implementation will accept as the maximum
   number of bytes in a pathname.
*/
#define _XOPEN_PATH_MAX 1024

/* Numerical Limits - as defined by POSIX /*

/*
   Number of bits in a type char.
*/
#define CHAR_BIT 8
/*
   Maximum value for an object of type char.
*/
#define CHAR_MAX SCHAR_MAX
/*
   Minimum value for an object of type char.
*/
#define CHAR_MIN SCHAR_MIN
/*
   Maximum value for an object of type int.
*/
#define INT_MAX 2147483647
/*
   Minimum value for an object of type int.
*/
#define INT_MIN -2147483647
/*
   Maximum value for an object of type long long.
*/
#define LLONG_MAX +9223372036854775807
/*
   Minimum value for an object of type long long.
*/
#define LLONG_MIN -9223372036854775807
/*
   Number of bits in an object of type long.
*/
#define LONG_BIT 32
/*
   Maximum value for an object of type long.
*/
#define LONG_MAX 2147483647
/*
   Minimum value for an object of type long.
*/
#define LONG_MIN -2147483647
/*
   Maximum number of bytes in a character, for any suppocrted locale.
*/
#define MB_LEN_MAX 1
/*
   Maximum value for an object of type signed char.
*/
#define SCHAR_MAX 127
/*
   Minimum value for an object of type signed char.
*/
#define SCHAR_MIN -128
/*
   Maximum value for an object of type short.
*/
#define SHRT_MAX 32767
/*
   Minimum value for an object of type short.
*/
#define SHRT_MIN -32767
/*
   Maximum value for an object of type ssize_t.
*/
#define SSIZE_MAX _POSIX_SSIZE_MAX
/*
   Maximum value for an object of type unsigned char.
*/
#define UCHAR_MAX 255
/*
   Maximum value for an object of type unsigned.
*/
#define UINT_MAX 4294967295
/*
   Maximum value for an object of type unsigned long long.
*/
#define ULLONG_MAX 18446744073709551615
/*
   Maximum value for an object of type unsigned long.
*/
#define ULONG_MAX 4294967295
/*
   Maximum value for an object of type unsigned short.
*/
#define USHRT_MAX 65535
/*
   Number of bits in an object of type int.
*/
#define WORD_BIT 32

/* Other Invariant Values /*

/*
   Maximum value of n in conversion specifications using the "%n$"
   sequence in calls to the printf() and scanf() families of
   functions.
*/
#define NL_ARGMAX 9
/*
   Maximum number of bytes in a LANG name.
*/
#define NL_LANGMAX 14 /*XSI Specification /*
/*
   Maximum message number.
*/
#define NL_MSGMAX 32767
/*
   Maximum set number.
*/
#define NL_SETMAX 255
/*
   Maximum number of bytes in a message string.
*/
#define NL_TEXTMAX _POSIX2_LINE_MAX
/*
   Default process priority.
*/
#define NZERO 20 /* XSI Specification */

#endif /* limits.h */
