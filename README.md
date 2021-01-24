# Sequence
A library in C++ to create simple finite state machines following the logic of "if(condition) then (action), wait(time), reset(if condition, another action)". Initially programmed and tested for ATmega328, but can run with any other architecture as long as there is a function called "millis" that returns the number of milliseconds since the program started.

# How to use
* In your program include the sequence.cpp
* Make sure that there is an implementation of the function "millis" and "millis_init"
* In your code create a "Sequence" object and call the functions "If", "Do", "Wait" (optional), "Reset" with their parameters, order doesn't matter and not necessarily called from the same place

The following example expects a button to be pressed in PB0, an LED in PB1 is turned on, if after 1 second the button is released, the led is turned off

``` c++
#include "sequence.cpp"

int main()
{
    Sequence sequence;    
    DDRB = 1 << PB1;
    while (true)
    {
        sequence.
          If([]() -> bool { return PINB & 1; }).
          Do([]() { PORTB |= 1 << PB1; }).
          Wait(1000).
          Reset([]() -> bool {return !(PINB & 1); }, []() { PORTB &= ~(1 << PB1); });
    }

}
```
![](https://raw.githubusercontent.com/cobrce/Sequence/master/sim.gif)
