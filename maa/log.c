/* log.c -- Logging routines, for a single, program-wide logging facility
 * Created: Mon Mar 10 09:37:21 1997 by faith@dict.org
 * Copyright 1997-1999, 2001-2002 Rickard E. Faith (faith@dict.org)
 * Copyright 2002-2008 Aleksey Cheusov (vle@gmx.net)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "maaP.h"
#ifdef HAVE_SYSLOG_NAMES
#define SYSLOG_NAMES
#endif
#include <syslog.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/param.h>

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

static int        logFd = -1;
static FILE       *logUserStream;
static int        logSyslog;
static int        inhibitFull = 0;

static int        logOpen;
static int        logFacility = LOG_USER;

static const char *logIdent;
static const char *logFilenameOrig;
static char       *logFilename;
static char       *logFilenameTmp;
static int        logFilenameLen;
static char       logHostname[MAXHOSTNAMELEN];

#ifndef HAVE_SYSLOG_NAMES
typedef struct _code {
	const char *c_name;
	int        c_val;
} CODE;
CODE facilitynames[] = {
#if LOG_AUTH
    { "auth",     LOG_AUTH },
#endif
#if LOG_AUTHPRIV
    { "authpriv", LOG_AUTHPRIV },
#endif
#if LOG_CRON
    { "cron",     LOG_CRON },
#endif
#if LOG_DAEMON
    { "daemon",   LOG_DAEMON },
#endif
#if LOG_FTP
    { "ftp",      LOG_FTP },
#endif
#if LOG_KERN
    { "kern",     LOG_KERN },
#endif
#if LOG_LPR
    { "lpr",      LOG_LPR },
#endif
#if LOG_MAIL
    { "mail",     LOG_MAIL },
#endif
#if LOG_NEWS
    { "news",     LOG_NEWS },
#endif
#if LOG_SYSLOG
    { "syslog",   LOG_SYSLOG },
#endif
#if LOG_USER
    { "user",     LOG_USER },
#endif
#if LOG_UUCP
    { "uucp",     LOG_UUCP },
#endif
#if LOG_LOCAL0
    { "local0",   LOG_LOCAL0 },
#endif
#if LOG_LOCAL1
    { "local1",   LOG_LOCAL1 },
#endif
#if LOG_LOCAL2
    { "local2",   LOG_LOCAL2 },
#endif
#if LOG_LOCAL3
    { "local3",   LOG_LOCAL3 },
#endif
#if LOG_LOCAL4
    { "local4",   LOG_LOCAL4 },
#endif
#if LOG_LOCAL5
    { "local5",   LOG_LOCAL5 },
#endif
#if LOG_LOCAL6
    { "local6",   LOG_LOCAL6 },
#endif
#if LOG_LOCAL7
    { "local7",   LOG_LOCAL7 },
#endif
    { NULL,       -1 },
};
#endif

static void _log_set_hostname(void)
{
	static int hostnameSet = 0;
	char       *pt;

	if (!hostnameSet) {
		memset(logHostname, 0, sizeof(logHostname));
		gethostname(logHostname, sizeof(logHostname)-1);
		if ((pt = strchr(logHostname, '.'))) *pt = '\0';
		++hostnameSet;
	}
}

void log_set_facility(const char *facility)
{
	CODE *pt;

	for (pt = facilitynames; pt->c_name; pt++) {
	    if (!strcmp(pt->c_name, facility)) {
	        logFacility = pt->c_val;
	        return;
	    }
	}
	err_fatal(__func__, "%s is not a valid facility name", facility);
}

const char *log_get_facility(void)
{
	CODE *pt;

	for (pt = facilitynames; pt->c_name; pt++)
	    if (pt->c_val == logFacility) return pt->c_name;
	return NULL;
}

void log_option(int option)
{
	if (option == LOG_OPTION_NO_FULL) inhibitFull = 1;
	else                              inhibitFull = 0;
}

void log_syslog(const char *ident)
{
	if (ident){
		if (logSyslog)
			err_internal(__func__, "Syslog facility already open");

		openlog(ident, LOG_PID|LOG_NOWAIT, logFacility);
		++logOpen;
		++logSyslog;
	}else{
		if (!logSyslog)
			return;

		closelog ();
		--logOpen;
		--logSyslog;
	}
}

static void log_mkpath(const char *filename)
{
	char *tmp = xstrdup(filename);
	char *pt;

	for (pt = tmp; *pt; pt++) {
	    if (*pt == '/' && pt != tmp) {
	        *pt = '\0';
	        mkdir(tmp, 0755);
	        *pt = '/';
	    }
	}

	xfree(tmp);
}

static void _log_check_filename(void)
{
	time_t    t;
	struct tm *tm;
   
	if (!logFilename || !logFilenameTmp || !logFilenameLen) return;

	time(&t);
	tm = localtime(&t);
   
	strftime(logFilenameTmp, logFilenameLen, logFilenameOrig, tm);
	if (strcmp(logFilenameTmp, logFilename)) {
		strcpy(logFilename, logFilenameTmp);
		if (logFd >= 0) close(logFd);
		log_mkpath(logFilename);
		if ((logFd = open(logFilename, O_WRONLY|O_CREAT|O_APPEND, 0644)) < 0)
			err_fatal_errno(__func__,
							 "Cannot open \"%s\" for append", logFilename);
	}
}

void log_file(const char *ident, const char *filename)
{
	if (ident && filename){
		if (logFd >= 0)
			err_internal(__func__,
						  "Log file \"%s\" open when trying to open \"%s\"",
						  logFilename, filename);

		logIdent        = str_find(ident);
		logFilenameOrig = str_find(filename);
		logFilenameLen  = strlen(filename)*3+1024;
		logFilename     = xmalloc(logFilenameLen + 1);
		logFilenameTmp  = xmalloc(logFilenameLen + 1);
		logFilename[0]  = '\0';

		_log_check_filename();
		_log_set_hostname();

		++logOpen;
	}else{
		if (logFd < 0)
			return;

		close (logFd);
		logFd          = -1;

		if (logFilename) xfree (logFilename);
		logFilename    = NULL;

		if (logFilenameTmp) xfree (logFilenameTmp);
		logFilenameTmp = NULL;

		logFilenameLen = 0;

		--logOpen;
	}
}

void log_stream(const char *ident, FILE *stream)
{
	if (ident && stream){
		if (logUserStream)
			err_internal(__func__, "User stream already open");

		logUserStream = stream;
		logIdent      = str_find(ident);

		_log_set_hostname();
		++logOpen;
	}else{
		if (!logUserStream)
			return;

		if (logUserStream != stdout &&
			logUserStream != stderr)
		{
			fclose (logUserStream);
		}

		logUserStream = NULL;

		--logOpen;
	}
}

void log_close(void)
{
	log_file (NULL, NULL);
	log_stream (NULL, NULL);
	log_syslog (NULL);
}

static void _log_base_va(
	const char *routine,
	int log_facility,
	const char *format, va_list ap)
{
	va_list ap_copy;
	time_t t;
	static char   buf [8192] = "";
	static char   buf_main [4096] = "";
	static char   buf_preamble [256] = "";

	va_copy (ap_copy, ap);

	if (!logOpen) return;

	time(&t);

	if (logFd >= 0 || logUserStream) {
		/* preamble */
		if (inhibitFull) {
			buf_preamble [0] = 0;
		} else {
			snprintf (buf_preamble, sizeof (buf_preamble),
					  "%24.24s %s %s[%ld]: ",
					  ctime(&t),
					  logHostname,
					  logIdent,
					  (long int) getpid());
		}

		/* main part of log message */
		vsnprintf (buf_main, sizeof (buf_main), format, ap);

		/* full log message */
		if (routine){
			snprintf (buf, sizeof (buf), "%s(%s) %s\n",
					  buf_preamble, routine, buf_main);
		}else{
			snprintf (buf, sizeof (buf), "%s%s\n",
					  buf_preamble, buf_main);
		}

		/* writing */
		if (logFd >= 0) {
			_log_check_filename();
			while (-1 == write (logFd, buf, strlen (buf)) && errno == EINTR);
		}
		if (logUserStream) {
			fseek(logUserStream, 0L, SEEK_END); /* might help if luser didn't
													 open stream with "a" */
			if (logUserStream == stdout || logUserStream == stderr)
				fprintf(logUserStream, "%s", buf_main);
			else
				fprintf(logUserStream, "%s", buf);

			fflush(logUserStream);
		}
	}

	if (logSyslog) {
		vsyslog(log_facility, format, ap_copy);
	}
}

void log_error_va(const char *routine, const char *format, va_list ap)
{
   _log_base_va (routine, LOG_ERR, format, ap);
}

void log_error(const char *routine, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	log_error_va(routine, format, ap);
	va_end(ap);
}

void log_info_va(const char *format, va_list ap)
{
	_log_base_va (NULL, LOG_INFO, format, ap);
}

void log_info(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	log_info_va(format, ap);
	va_end(ap);
}
