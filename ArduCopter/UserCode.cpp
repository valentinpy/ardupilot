#include "Copter.h"

#ifdef USERHOOK_INIT

void Copter::userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
    hal.scheduler->delay(1000); //Ensure that the uartA can be initialized

    if (hal.uartE == nullptr) {
        // that UART doesn't exist on this platform
        return;
    }
    hal.uartE->begin(57600);

    hal.uartE->printf("\n\n[Serial VPY] Setup finished!\n\n");
    hal.uartE->printf("------------------------------------------------------------\n");
    hal.uartE->printf("---------------- SERGE EST FORT!! --------------------------\n");
    hal.uartE->printf("------------------------------------------------------------\n");

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
    // vpy_test_var ++;

    uint32_t data_length = hal.uartE->available();

    static int i = 0;
    static char buffer[100]="\0";

    // read char by char
    while(hal.uartE->available() > 0){
        buffer[i] = hal.uartE->read();
        i++;
    }

    // if last char was and end of line, print
    if (buffer[i-1]== '\n' || buffer[i-1]== '\r'){
        i = 0;
        hal.uartE->printf("[VPY] at %.3f seconds, data_length: %d, data: %s\n", (double)(AP_HAL::millis() * 0.001f), data_length, buffer);
    }
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{
    // put your 3.3Hz code here
    static int16_t vpy_test_var_static = 0;
    vpy_test_var_static ++;
  
    uint8_t chan_mask = GCS_MAVLINK::active_channel_mask();
    for (uint8_t i=0; i<MAVLINK_COMM_NUM_BUFFERS; i++) {
        if ((chan_mask & (1U<<i)) == 0) {
            // not active
            continue;
        }
        mavlink_channel_t chan = (mavlink_channel_t)i;
        if (HAVE_PAYLOAD_SPACE(chan, BUTTON_CHANGE)) {
            mavlink_msg_vpy_test_send(chan, vpy_test_var_static);
            // mavlink_msg_power_status_send(chan, myVar1, myVar2, myVar3);

            hal.uartE->printf("[VPY] Mavlink message sent: 'VPY_TEST: {vpy_test=%d}'\n", vpy_test_var_static);
        }
    }
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{
    // put your 1Hz code here   
    
}
#endif
