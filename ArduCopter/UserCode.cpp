#include "Copter.h"
#include "DRA_macros.h"
#include "DRA_send_status.h"
#include "DRA_uart_receiver.h"

//-------------------------------------------------
//------------------- constants -------------------
//-------------------------------------------------
const uint32_t dva_val_timeout_us = 1000000;


//-------------------------------------------------
//------------------- macros ----------------------
//-------------------------------------------------

//-------------------------------------------------
//---------------- prototypes ---------------------
//-------------------------------------------------


//-------------------------------------------------
//--------------- methods -------------------------
//-------------------------------------------------
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
    hal.uartE->begin(115200);

    hal.uartE->printf("------------------------------------------------------------\n");
    hal.uartE->printf("---------------- SERGE EST FORT!! --------------------------\n");
    hal.uartE->printf("------------------------------------------------------------\n");
    hal.uartE->flush();

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
    // 10 Hz loop
    static uint32_t lastValidDVAValue = AP_HAL::micros();

    //----------------------------
    // decode frame from K64
    //----------------------------
    static int32_t dva_val=-1;
    
    // read dva val from serial... must check returned value!
    int8_t retVal = recv_uart_dra_message(&dva_val);
    if (retVal !=0){ // unable to read => keep the old one
        // dva_val = -1;
        dbg2("unable to read a message: error: %d\n",retVal);
    }
    else{ // value read successfully, keep track of measurement time
        dbg2("dva value received: %d\n", dva_val);
        lastValidDVAValue = AP_HAL::micros();
    }

    //check validity of measure against time. If measure if too old, we clear it.
    if ((AP_HAL::micros() - lastValidDVAValue) > dva_val_timeout_us){
        dva_val = -1;
        dbg("data not valid anymore\n");
    }
    
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{
    // 3.3 Hz loop
    // hal.uartE->printf("[VPY] 3.3Hz task heartbeat\n");
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
            mavlink_msg_vpy_test_1_send(chan, vpy_test_var_static);
            // mavlink_msg_power_status_send(chan, myVar1, myVar2, myVar3);

            // hal.uartE->printf("[VPY] Mavlink message sent: 'VPY_TEST: {vpy_test=%d}'\n", vpy_test_var_static);
        }
    }
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{
    //1Hz loop
    //hal.uartE->printf("[VPY] 1Hz task heartbeat\n");
    send_dra_status();
}
#endif


