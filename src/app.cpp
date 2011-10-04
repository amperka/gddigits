
#include <WProgram.h>
#include <Ethernet.h>
#include "config.h"

byte serverIP[] = SERVER_IP;
byte deviceMac[] = DEVICE_MAC;
byte deviceIP[] = DEVICE_IP;

Client client(serverIP, SERVER_PORT);
int currentNumber = 0;


/*
 * Performs proper number of clicks to set current
 * number displayed to a `target` value
 */
void clickToward(int target)
{
    int n = target - currentNumber;
    while (n < 0) {
        // we have to click through an overflow to
        // go backward
        n += MAX_DISPLAY_NUMBER;
    }

    while (n--) {
        digitalWrite(CLICK_PIN, HIGH);
        delayMicroseconds(CLICK_PULSE_DURATION);
        digitalWrite(CLICK_PIN, LOW);
    }

    currentNumber = target;
}


/*
 * Does single HTTP request, parses response and
 * click digits toward received value
 */
void update()
{
    int number = 0;
    bool headerRead = false;
    byte lastByte = 0;
    byte x = 0;

    if (!client.connect()) {
        // server is unavailable, do nothing
        return;
    }

    client.println("GET " SERVER_PATH " HTTP/1.0");
    client.println();

    // skip response header, i.e. read until
    // two new lines in a row
    while (!headerRead && client.connected()) {
        while (client.available()) {
            x = client.read();
            if (x == '\r')
                continue;

            if (lastByte == '\n' && x == '\n') {
                headerRead = true;
                break;
            }

            lastByte = x;
        }
    }

    // now read the body
    while (client.available()) {
        x = client.read();
        if (x == '\r' || x == '\n') {
            // we've read first line completly
            // that's enough
            break;
        }

        x -= '0';
        if (x < 0 || x > 9) {
            // unexpected response, cancel update
            client.stop();
            return;
        }

        number = number * 10 + x;
    }
    
    client.stop();
    clickToward(number);
}


void setup() 
{
    Ethernet.begin(deviceMac, deviceIP);
    Serial.begin(9600);
    pinMode(CLICK_PIN, OUTPUT);

    delay(5000);
}


void loop()
{
    update();
    delay(POLL_INTERVAL);
}
