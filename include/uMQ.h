#include <nRF24L01.h>
#include <RF24.h>

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
      radio.setPALevel(RF24_PA_LOW);
    }

    bool send(String data, String address, uint32_t timeout = 1000)
    {
      radio.closeReadingPipe(0);
      radio.openWritingPipe((uint8_t*)address.c_str());
      radio.stopListening();
      delay(50);
      return radio.write(data.c_str(), data.length());
    }

    String recv(String address, uint32_t timeout = -1)
    {
      radio.closeReadingPipe(0);
      radio.openReadingPipe(0, (uint8_t*)address.c_str());
      radio.startListening();

      uint32_t start = millis();
      while (!radio.available())
      {
        if (millis() - start > timeout)
        {
          return "";
        }
      }

      char buf[32] = {0};

      radio.read(&buf, sizeof(buf));

      return String(buf);
    }
};