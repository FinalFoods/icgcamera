#ifndef SHUTTER_H
#define SHUTTER_H

class Shutter
{
    int buzzer_pin;
    static bool busy;

private:
    static void *_buzzerPlay(void *arg);

public:
    Shutter();

    void setBuzzerPin(int pin);
    void buzzerPlay();
    bool isBuzzerPlaying(void);

};

#endif // SHUTTER_H
