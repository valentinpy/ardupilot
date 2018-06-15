#include "Copter.h"

#ifdef USERHOOK_INIT

static void setup_uart(AP_HAL::UARTDriver *uart, const char *name)
{
    if (uart == nullptr) {
        // that UART doesn't exist on this platform
        return;
    }
    uart->begin(57600);
}

static void test_uart(AP_HAL::UARTDriver *uart, const char *name, int counter)
{
    if (uart == nullptr) {
        // that UART doesn't exist on this platform
        return;
    }
    uart->printf("[VPY] Hello on UART %s at %.3f seconds, counter: %d\n",
                 name, (double)(AP_HAL::millis() * 0.001f), counter);
}


void Copter::userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
    hal.scheduler->delay(1000); //Ensure that the uartA can be initialized
    setup_uart(hal.uartD, "uartD");  // telemetry 2
    setup_uart(hal.uartE, "uartE"); // 2nd GPS
}
#endif

#ifdef USERHOOK_FASTLOOP
void Copter::userhook_FastLoop()
{
    // put your 100Hz code here
}
#endif

#ifdef USERHOOK_50HZLOOP
void Copter::userhook_50Hz()
{
    // put your 50Hz code here
}
#endif

#ifdef USERHOOK_MEDIUMLOOP
void Copter::userhook_MediumLoop()
{
    // put your 10Hz code here
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{
    // put your 3.3Hz code here
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{
    // put your 1Hz code here
    // static int vpy_test_var_static = 0;
    // vpy_test_var_static ++;

    vpy_test_var ++;

    // test_uart(hal.uartD, "uartD", vpy_test_var);
    test_uart(hal.uartE, "uartE", vpy_test_var);
}
#endif
