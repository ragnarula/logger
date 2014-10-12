#ifndef LOG_HPP
#define LOG_HPP

#define LOG_INIT logging::logger log_inst;
#define LOG_INST log_inst

#include "log_impl.hpp"

extern LOG_INIT


#ifdef LOGGING_LEVEL_1

#define LOGGING_LEVEL_WARNING

#endif

#ifdef LOGGING_LEVEL_2

#define LOGGING_LEVEL_WARNING
#define LOGGING_LEVEL_ERROR

#endif

#ifdef LOGGING_LEVEL_3

#define LOGGING_LEVEL_WARNING
#define LOGGING_LEVEL_ERROR
#define LOGGING_LEVEL_DEBUG

#endif

#ifdef LOGGING_LEVEL_DEBUG

#define LOG log_inst.print< logging::severity_type::debug >

#else

#define LOG(...)

#endif

#ifdef LOGGING_LEVEL_ERROR

#define LOG_ERR log_inst.print< logging::severity_type::error >

#else

#define LOG_ERR(...)

#endif

#ifdef LOGGING_LEVEL_WARNING

#define LOG_WARN log_inst.print< logging::severity_type::warning >

#else

#define LOG_WARN(...)

#endif

#endif // LOG_HPP
