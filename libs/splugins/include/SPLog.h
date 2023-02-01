
#ifndef SPLog_h
#define SPLog_h

#include <stdarg.h>

#ifndef MJ_EXPORT
#if defined(_MSC_VER)
#define MJ_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define MJ_EXPORT __attribute__((visibility("default")))
#else
#define MJ_EXPORT
#endif
#endif

static void (*logFunction)(const char *format, va_list args);

typedef void (* SPSetLogFunctionFunc) (void (*logFunction_)(const char *format, va_list args));

MJ_EXPORT void spSetLogFunction(void (*logFunction_)(const char *format, va_list args));

MJ_EXPORT void spLog(const char *format, ...);

#endif
