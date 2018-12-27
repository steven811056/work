#pragma once

#define MDEBUG true

#define DEBUG_SERIAL Serial

#if MDEBUG
#define debug_print(a)						DEBUG_SERIAL.print(a);
#define debug_print_s()						DEBUG_SERIAL.print(";");
#define debug_print_sp()					DEBUG_SERIAL.print(" ");
#define debug_println()						DEBUG_SERIAL.println();

#define debug_print_r1(a)					debug_print(a)				debug_print_sp()
#define debug_print_r2(a, b)				debug_print_r1(a)			debug_print_r1(b)
#define debug_print_r3(a, b, c)				debug_print_r2(a, b)		debug_print_r1(c)
#define debug_print_r4(a, b, c, d)			debug_print_r3(a, b, c)		debug_print_r1(d)

#define debug_print_1(a)					debug_print_s()				debug_print_r1(a)
#define debug_print_2(a, b)					debug_print_s()				debug_print_r2(a, b)
#define debug_print_3(a, b, c)				debug_print_s()				debug_print_r3(a, b, c)	
#define debug_print_4(a, b, c, d)			debug_print_s()				debug_print_r4(a, b, c, d)

#define debug_println_1(a)					debug_print_1(a)			debug_println()
#define debug_println_2(a, b)				debug_print_2(a, b)			debug_println()
#define debug_println_3(a, b, c)			debug_print_3(a, b, c)		debug_println()
#define debug_println_4(a, b, c, d)			debug_print_4(a, b, c, d)	debug_println()

#else
inline size_t debug_print(...) {
	return 0;
}
inline size_t debug_print_1(...) {
	return 0;
}
inline size_t debug_print_2(...) {
	return 0;
}
inline size_t debug_print_3(...) {
	return 0;
}
inline size_t debug_print_4(...) {
	return 0;
}
inline size_t debug_println(...) {
	return 0;
}
inline size_t debug_println_1(...) {
	return 0;
}
inline size_t debug_println_2(...) {
	return 0;
}
inline size_t debug_println_3(...) {
	return 0;
}
inline size_t debug_println_4(...) {
	return 0;
}
#endif
