#include <QDebug>

#include <pigpio/pigpio.h>
#include "irleds.h"

IRleds::IRleds()
{

}

void IRleds::setPin(int pin)
{
    int ret;
    leds_pin = pin;

    if (gpioSetMode(leds_pin, PI_OUTPUT) != 0) {
        qInfo("in BIRleds::setPin. Error in gpioSetMode pin %d", leds_pin);
    }

    // initial state LEDs off
    state = 0;
    if ((ret = gpioWrite(leds_pin, state)) != 0) {
        qInfo("in IRleds::IRleds. Error writing pin %d. ret = %d", leds_pin, ret);
    }
}

bool IRleds::toggle() {
    int ret = 0;

    //qInfo("in IRleds::toggle");
    state = !state;
    if ((ret = gpioWrite(leds_pin, state)) != 0) {
        qInfo("in IRleds::toggle. Error writing pin %d", leds_pin);
    }
    return state;
}
