#pragma once

#define DEBUG_PRINT 1

#if DEBUG_PRINT == 1
	#define dbg(msg) \
        do { if (DEBUG_PRINT) hal.uartE->printf("%s\n", msg); } while (0)
    #define dbg2(msg, var) \
        do { if (DEBUG_PRINT) hal.uartE->printf(msg, var); } while (0)
    #define dbg3(msg, var1, var2) \
        do { if (DEBUG_PRINT) hal.uartE->printf(msg, var1, var2); } while (0)
#else
	#define dbg(msg) (void) 0
    #define dbg2(msg, var1) (void) 0
    #define dbg3(msg, var1) (void) 0
#endif