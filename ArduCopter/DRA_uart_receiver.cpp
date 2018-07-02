#include "Copter.h"
#include "DRA_uart_receiver.h"
#include "DRA_macros.h"



int8_t recv_uart_dra_message(int32_t * dva_val_ptr){

    // frame specifications, must comply with K64 messages!
    const int8_t begin_delimiter[7]="magic!";
    const uint8_t begin_delimiter_size = 6;
    const int8_t end_delimiter[7]="end";
    const uint8_t end_delimiter_size = 3;
    const uint8_t message_size = 10;
    
    // flags and buffers
    uint8_t synchronized = 0;
    uint8_t i = 0;
    static uint8_t last_framecounter = 0;
    
    // Ensure we have received enough data to process if we are synchronized, else we do nothing yet (return -1)
    uint16_t available_bytes = hal.uartE->available();
    if(available_bytes < 2*(begin_delimiter_size + message_size + end_delimiter_size)){
        // dbg("NOT enough data ... waiting\n\n");
        return -1;
    }
    //dbg("enough data... processing");

    //Ensure we are synchronized (the received messages start with the correct begin_delimiter)
    uint8_t j = 0;
    for (i=0; i<2*(begin_delimiter_size + message_size + end_delimiter_size); i++){ // loop until we find the delimiter
        int8_t readData = hal.uartE->read();

        if (readData == begin_delimiter[j]){ //correct character detected
            j++;
        }
        else{ //bad character, restart!
            j=0;
        }

        //once ok for the whole delimiter, we are synchronized and can continue
        if (j>=begin_delimiter_size){
            synchronized = 1;
            break;
        }
    }

    // ensure we are synchronized!
    if(synchronized==0){
        return -2;
    }
    dbg("synchronized!");

    // read size
    int8_t size = hal.uartE->read();
    dbg2("size of frame: %d\n", size);

    //read dva_val
    uint8_t dva_val_8[4];
    for(i=0; i<4; i++){
        dva_val_8[i] = hal.uartE->read();
    }
    uint32_t dva_valu32 = (dva_val_8[0]<<24) + (dva_val_8[1]<<16) + (dva_val_8[2]<<8) + (dva_val_8[3]<<0);
    dbg2("dva val: %d\n", dva_valu32);

    //read timestamp
    uint8_t timestamp[4];
    for(i=0; i<4; i++){
        timestamp[i] = hal.uartE->read();
    }
    uint32_t timestamp32 = (timestamp[0]<<24) + (timestamp[1]<<16) + (timestamp[2]<<8) + (timestamp[3]<<0);
   dbg2("timestamp: %d\n", timestamp32);

    //read framecounter
    uint8_t framecounter = hal.uartE->read();
    dbg2("framecounter: %d\n", framecounter);

    if ((last_framecounter+1)%256 != framecounter){
        dbg3("frame counter not monotonic. might have lost some frames: old: %d, new: %d\n", last_framecounter, framecounter);
    }
    last_framecounter = framecounter;


    //read end
    for(i=0; i<3; i++){
        int8_t readData = hal.uartE->read();
        dbg2("%c", readData);
        if (readData != end_delimiter[i]){
            dbg("bad end of frame!");
            return -3;
        }
    }
    dbg("\n\n");

    // change value only if message read successfully!
    *dva_val_ptr = (int32_t)dva_valu32;
    return 0;

}