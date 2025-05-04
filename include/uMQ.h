#include "nrf24L01.h"
#include "RF24.h"

class uMQ
{
    private:
        RF24 radio;
    
    public:
        uMQ() {}

        void init(int ce_pin, int csn_pin, int channel = 76)
        {
            if(!radio.begin(ce_pin, csn_pin)) {
                Serial.println("Radio init failed");
                while(1);
            }
            Serial.println("Radio init complete");
            radio.setChannel(channel);
            radio.setAutoAck(true);
            radio.setPALevel(RF24_PA_MIN);
        }

        bool send(void* data, uint8_t len, const char address[6], uint32_t timeout = 1000)
        {
            radio.stopListening();
            radio.openWritingPipe((uint8_t*) address);
            return radio.write(data, len);
        }

        bool recv(void* data, uint8_t len, const char address[6], uint32_t timeout = -1)
        {
            radio.openReadingPipe(0, (uint8_t*) address);
            radio.startListening();

            uint32_t start = millis();
            while(!radio.available())
            {
                if (millis() - start > timeout)
                {
                    return false;
                }
            }
        }

        uint8_t buf[32];

        radio.read(&buf, sizeof(buf));

        memcpy(data, buf, len);

        return true;
};