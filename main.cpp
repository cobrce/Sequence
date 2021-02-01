#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "sequence.cpp"

#define USE_LAMBDA

#ifndef USE_LAMBDA
bool condition()
{
    return PINB & (1 << PB0); // is button pressed?
}

void action()
{
    PORTB |= 1 << PB1; // turn led on
}

void resetAction()
{
    PORTB &= ~(1 << PB1); // turn led off
}

bool resetCondition()
{
    return !condition(); // is the button not pressed?
}

bool breakCondition()
{
    return PINB & (1 << PB2); // is the second button pressed?
}
#endif

int main()
{
    DDRB = 1 << PB1;
    DDRD = 1 << PD1;
    Sequence sequence,sequence2;

    while (true)
    {
        #ifndef USE_LAMBDA
        sequence.If(condition).Do(action).Wait(1000).Break(breakCondition).Reset(resetCondition, resetAction);
        #else

        sequence.
        If([]() -> bool { return PINB & 1; }).
        Do([]() { PORTB |= 1 << PB1; }).
        Wait(1000).
        Break([]() -> bool { return PINB & (1 << PB2);}).
        Reset([]() -> bool {return !(PINB & 1); }, []() { PORTB &= ~(1 << PB1); });
        
        #endif

        sequence2.
        If([]() -> bool { return PIND & 1; }).
        Do([]() { PORTD |= 1 << PD1; }).
        Wait(3000).
        Break([]() -> bool { return PIND & (1 << PD2);}).
        Reset([]() -> bool {return !(PIND & 1); }, []() { PORTD &= ~(1 << PD1); });
    }
}