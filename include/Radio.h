#pragma once

#include "RF24.h"
#include "RF24_config.h"

#define RF_CSN          9
#define RF_CE           10

RF24 radio(RF_CSN, RF_CE);

const uint32_t pipe = 111156789;

int loc_ice[5][5];

enum {
    start_intelegence_service,
    mail_nrf1,
    go_home,
    mail_nrf2,
};

void radio_setup()
{
    radio.begin();
    delay(1000);
    radio.setDataRate(RF24_1MBPS);
    radio.setCRCLength(RF24_CRC_8);
    radio.setChannel(0x6f);
    radio.setAutoAck(false);
    radio.openWritingPipe(pipe);
    radio.startWrite(loc_ice, 100, false);
}

void radio_write(int message)
{
    radio.write(&message, sizeof(message));
}