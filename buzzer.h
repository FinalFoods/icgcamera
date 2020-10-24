#ifndef BUZZER_H
#define BUZZER_H

#include <QObject>
#include <QDebug>

class Buzzer : QObject
{
    Q_OBJECT
    int buzzer_pin;
    static bool busy;

private:
    static void *_play(void *arg);

public:
    explicit Buzzer();
    ~Buzzer();

    void setPin(int pin);
    void play(void);

    bool isPlaying(void);
};

#endif // BUZZER_H
