// prevent including the file twice
// identifiers starting with one underscore are reserved for libraries
// identifiers with two underscores are reserved for compiler vendors
#ifndef __dbg_h__
#define __dbg_h__

// includes for the functions that these macros need
#include <stdio.h>
#include <errno.h>
#include <string.h>

// lets you recompile the program so that all the debug log messages are removed
#ifdef NDEBUG
// if you compile with NDEBUG, then "no debug" messages will remain
// in this case the #define debug() is replaced with nothing (the right side is empty)
#define debug(M, ...)
#else
// the alternative #define debug() that translates any use of debug("format", arg1, arg2)
// into a fprintf call to stderr
// you can create macros that work like printf() and take variable arguments (using some compilers)
// ##__VA_ARGS__ says "put whatever extra arguments there are (...) here
// fprintf(FILE *stream, "formatted %d text", arg, arg, etc.)
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d:%s: " M "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#endif

// used in the other macros to get a safe, readable verson of errno; uses a "ternary operator"
// strerror() interprets the errno value into text
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

// log messages mean for the end user; work like debug() but can't be compiled out
// stderr is the default stream for error messages; it can be passed as an argument of type FILE*
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d:%s: errno: %s) " M "\n", __FILE__, __LINE__, __FUNCTION__, clean_errno(), ##__VA_ARGS__)
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d:%s: errno: %s) " M "\n", __FILE__, __LINE__, __FUNCTION__, clean_errno(), ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d:%s:) " M "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

// makes sure the condition "A" is true, and if not, it logs the error M (with variable
// arguments for log_err); then it jumps to the function's "error:" for cleanup
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// placed in any part of a function that shouldn't run, and if it does, prints
// an error message then jumps to "error:" label
// used in if- and switch-statements to catch conditions that shouldn't happen (like "default:")
#define sentinel(M, ...) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// makes sure a pointer is valid, and if it isn't, reports an "Out of memory" error
#define check_mem(A) check((A), "Out of memory.")

// alternate check_debug macro that still checks and handles an error, but if the error is
// common, you don't want to bother reporting it
// it uses "debug" instead of "log_err" to report the message, so when you define NDEBUG,
// the check still happens, the error jump goes off, but the message isn't printed
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif
