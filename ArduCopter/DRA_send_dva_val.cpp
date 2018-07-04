#include "DRA_send_dva_val.h"
#include "Copter.h"
#include "DRA_macros.h"

int8_t send_dva_val(int32_t dva_val, int8_t marked){
    // Send dva value
    dbg("trying to send rt infos\n");

    uint8_t chan_mask = GCS_MAVLINK::active_channel_mask();
    for (uint8_t i=0; i<MAVLINK_COMM_NUM_BUFFERS; i++) {
        dbg2("#1: %d\n", i);
        if ((chan_mask & (1U<<i)) == 0) {
            // not active
            continue;
        }
        dbg2("#2: %d\n", i);
        mavlink_channel_t chan = (mavlink_channel_t)i;
        if (HAVE_PAYLOAD_SPACE(chan, DRA_RT_INFOS)) {
            mavlink_msg_dra_rt_infos_send(chan, dva_val, marked);
            dbg("[VPY] Mavlink message sent: DRA_RT_INFOS\n");
        }
        else{
            dbg("[VPY] [-] Mavlink message not sent: DRA_RT_INFOS; #1\n");
            return -1;
        }
    } 
    return 0;
}