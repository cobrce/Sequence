#include "millis.h"

class Sequence
{
private:
    enum
    {
        init,
        verified,
        done

    };
    unsigned long delay = 0;
    volatile unsigned long doneAt = 0;
    char state = init;

public:
    Sequence()
    {
        millis_init();
        sei();
    }
    Sequence &If(bool (*condition)())
    {
        if (state == init && condition())
            state = verified;
        return *this;
    }
    Sequence &Do(void (*action)())
    {
        if (state == verified)
        {
            state = done;
            doneAt = millis();
            action();
        }
        return *this;
    }

    Sequence &Wait(unsigned long milliseconds)
    {
        delay = milliseconds;
        return *this;
    }

    Sequence &Reset(bool (*condition)(), void (*action)())
    {
        if (state == done && (delay == 0 || ((millis() - doneAt) >= delay)) && condition())
        {
            state = init;
            delay = 0;
            action();
        }
        return *this;
    }
};
