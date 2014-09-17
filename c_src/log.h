#ifndef __LOG_H__
#define __LOG_H__


#define SGR(X)  "\033[" #X "m"

#define C_CODE_LOC    SGR(38;5;239)
#define C_CODE_LOC_P  SGR(38;5;236)
#define C_FUNC        SGR(38;5;44)
#define C_FUNC_P      SGR(38;5;24)
#define C_RESET       SGR()


#define LOG_FMT_PREFIX  C_CODE_LOC_P "<" C_CODE_LOC "%s" C_CODE_LOC_P ":" C_CODE_LOC "%d" C_CODE_LOC_P "> " \
	C_FUNC "%s" C_FUNC_P "():" C_RESET " "


#ifdef __GNUC__
#	define LOG(fmt, ...) \
		fprintf(stderr, LOG_FMT_PREFIX fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
		fflush(stderr)
#else //__GNUC__
#	define LOG(fmt, ...) \
		fprintf(stderr, LOG_FMT_PREFIX fmt "\n", __FILE__, __LINE__, __func__, __VA_ARGS__); \
		fflush(stderr)
#endif //__GNUC__


#endif //__LOG_H__
