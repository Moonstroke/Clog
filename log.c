#include "log.h"

#include <ctype.h> /* for isspace */
#include <stdbool.h>



static FILE *logfile = NULL; /* cannot initialize to stderr :'( error is
                                "initializer element is not constant" */
static LogLevel logfilter = LOGF_ALL;
static const char* headers[] = {
		[LOG_DEBUG] = "DEBUG",
		[LOG_VERBOSE] = "VERBOSE",
		[LOG_INFO] = "INFO",
		[LOG_NOTICE] = "NOTICE",
		[LOG_WARNING] = "WARNING",
		[LOG_ERROR] = "ERROR",
		[LOG_FATAL] = "FATAL"
};


static inline bool msgblank(const char *const msg) {
	return *msg == '\0' || (isspace(*msg) && msgblank(msg + 1));
}


void log_setlogfile(FILE *const f) {
	logfile = f;
}

FILE *log_getlogfile(void) {
	return logfile;
}

const char *log_getheader(void) {
	return header;
}

void log_setfilter(const LogLevel lvl) {
	logfilter = lvl;
}

LogLevel log_getfilter(void) {
	return logfilter;
}

const char *log_getfiltername(void) {
	return headers[logfilter];
}



void logmsg(const LogLevel level, const char *const fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vlogmsg(level, fmt, args);
	va_end(args);
}

void vlogmsg(const LogLevel lvl, const char *const fmt, va_list a) {
	/* Initialize logfile if it has not been already */
	if(logfile == NULL)
		logfile = stderr;
	if(msgblank(fmt))
		fputs(fmt, logfile);
	else if(logfilter <= lvl) {
		va_list args;
		if(*fmt == '\n')
			fputs("\n", logfile);
		fprintf(logfile, "%s -- ", headers[lvl]);
		va_copy(args, a);
		vfprintf(logfile, fmt + (*fmt == '\n'), args);
		va_end(args);
		fprintf(logfile, "\n");
	}
}
