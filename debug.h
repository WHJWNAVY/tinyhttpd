#ifndef __HTTPD_DEBUG__
#define __HTTPD_DEBUG__

#include <string.h>
#include <errno.h>

#define __DEBUG__ 1

#define HTTPD_LOG_FILE NULL //"httpd_debug.log"
#define HTTPD_LOG_LEVEL_DEBUG "DEBUG"
#define HTTPD_LOG_LEVEL_WARNING "WARNING"
#define HTTPD_LOG_LEVEL_ERROR "ERROR"

#define HTTPD_DEBUG_TO_FILE(file, level, ...)                           \
    do                                                                  \
    {                                                                   \
        FILE *__HTTPD_DBG_FD__ = fopen(file, "a+");                     \
        if (__HTTPD_DBG_FD__ == NULL)                                   \
        {                                                               \
            __HTTPD_DBG_FD__ = stderr;                                  \
        }                                                               \
        fprintf(__HTTPD_DBG_FD__, "===[%ld][%s]=== %s(%s:%d) ",         \
                time(NULL), level, __func__, __FILE__, __LINE__);       \
        fprintf(__HTTPD_DBG_FD__, __VA_ARGS__);                         \
        fprintf(__HTTPD_DBG_FD__, "\n");                                \
        if ((__HTTPD_DBG_FD__ != NULL) && (__HTTPD_DBG_FD__ != stderr)) \
        {                                                               \
            fclose(__HTTPD_DBG_FD__);                                   \
        }                                                               \
    } while (0)

#ifdef __DEBUG__

#define HTTPD_ERROR(...)                                                         \
    do                                                                           \
    {                                                                            \
        HTTPD_DEBUG_TO_FILE(HTTPD_LOG_FILE, HTTPD_LOG_LEVEL_ERROR, __VA_ARGS__); \
    } while (0)

#define HTTPD_WARNING(...)                                                         \
    do                                                                             \
    {                                                                              \
        HTTPD_DEBUG_TO_FILE(HTTPD_LOG_FILE, HTTPD_LOG_LEVEL_WARNING, __VA_ARGS__); \
    } while (0)

#define HTTPD_DEBUG(...)                                                         \
    do                                                                           \
    {                                                                            \
        HTTPD_DEBUG_TO_FILE(HTTPD_LOG_FILE, HTTPD_LOG_LEVEL_DEBUG, __VA_ARGS__); \
    } while (0)

#define DEBUG_PERROR(es)                                                          \
    do                                                                            \
    {                                                                             \
        HTTPD_ERROR("[%s], errno[%d], strerror[%s]", es, errno, strerror(errno)); \
    } while (0)

#else
#define HTTPD_ERROR(...)

#define HTTPD_WARNING(...)

#define HTTPD_DEBUG(...)

#define DEBUG_PERROR(es)

#endif

#endif