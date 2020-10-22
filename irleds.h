#ifndef IRLEDS_H
#define IRLEDS_H

#include <QObject>
#include <QObject>

class IRleds : QObject
{
    Q_OBJECT
    int leds_pin;
    bool state;

public:
    explicit IRleds();
    void setPin(int pin);
    bool toggle();
};

#endif // IRLEDS_H
