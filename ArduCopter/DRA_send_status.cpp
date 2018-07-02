#include "Copter.h"
#include "DRA_macros.h"


void send_dra_status(void) {
    // TODO VPY: use real values
    uint32_t serial_number = 3141592;
    uint32_t immatriculation = 3354;

    // Send drone status
    uint8_t chan_mask = GCS_MAVLINK::active_channel_mask();
    for (uint8_t i=0; i<MAVLINK_COMM_NUM_BUFFERS; i++) {
        if ((chan_mask & (1U<<i)) == 0) {
            // not active
            continue;
        }
        mavlink_channel_t chan = (mavlink_channel_t)i;
        if (HAVE_PAYLOAD_SPACE(chan, DRA_STATUS)) {
  
            mavlink_msg_dra_status_send(chan, serial_number, immatriculation);
            dbg("[VPY] Mavlink message sent: DRA_STATUS");
        }
        else{
            dbg("[VPY] [-] Mavlink message not sent: DRA_STATUS; #1");
        }
    } 
}