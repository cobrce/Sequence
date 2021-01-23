#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "sequence.cpp"

bool condition()
{
    return PINB & (1 << PB0);
}

void action()
{
    PORTB |= 1 << PB1;
}

void resetAction()
{
    PORTB &= ~(1 << PB1);
}

bool resetCondition()
{
    return !condition();
}

int main()
{
    DDRB = 1 << PB1;
    Sequence sequence;

    while (true)
    {
        sequence.If(condition).Do(action).Wait(1000).Reset(resetCondition, resetAction);

        //// same as above but with lambda functions
        // If([]() -> bool { return PINB & 1; }).
        // Do([]() { PORTB |= 1 << PB1; }).
        // Wait(1000).
        // Reset([]() -> bool {return !(PINB & 1); }, []() { PORTB &= ~(1 << PB1); });
    }
}