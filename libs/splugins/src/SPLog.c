

#include "SPLog.h"
#include <stdarg.h>


void spSetLogFunction(void (*logFunction_)(const char *format, va_list args))
{
	logFunction = logFunction_;
}

void spLog(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	logFunction(format, args);
	va_end(args);
}
