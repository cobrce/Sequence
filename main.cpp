#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "sequence.cpp"

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

int main()
{
    DDRB = 1 << PB1;
    Sequence sequence;

    while (true)
    {
        sequence.If(condition).Do(action).Wait(1000).Break(breakCondition).Reset(resetCondition, resetAction);

        // // same as above but with lambda functions
        // sequence.
        // If([]() -> bool { return PINB & 1; }).
        // Do([]() { PORTB |= 1 << PB1; }).
        // Wait(1000).
        // Break([]() -> bool { return PINB & (1 << PB2);}).
        // Reset([]() -> bool {return !(PINB & 1); }, []() { PORTB &= ~(1 << PB1); });
    }
}