# Sequence
A library in C++ to create simple finite state machines following the logic of "if(condition) then (action), wait(time), reset(if condition, another action)". Initially programmed and tested for ATmega328, but can run with any other architecture as long as there is a function called "millis" that returns the number of milliseconds since the program started.

# Where should I use this library?
Finite state machines in general are a practical way to wait for events or creating delays without blocking the whole process, for example this is how async functions in C# are implemented. In the following examples we will see how to wait a button to be pressed without using a inifinte loop, then we wait 1 second without using _delay_ms.

# How to use
* In your program include the sequence.cpp
* Make sure that there is an implementation of the function "millis" and "millis_init"
* In your code create a "Sequence" object and call the functions *If*, *Do*, *Wait* (optional), *Break* (optional), *Reset* with their parameters, order doesn't matter and not necessarily called from the same place

## The functions
### The machine has 3 states : *init*, *verified* and *done*, these states are changed by and influence the execution of the following functions :

* **If** : accepts as parameter a function that returns a bool called *condition*, if the machine is in **init** state and *condition* returns true the machine moves to the next step : the state of **verified**
* **Do** : has one parameter, a function called *action*, if the machine is in the state of **verified**, *action* is executed then the machine moves to the state of **done**
* **Wait** (optional) : adds a delay between *Do* and *Reset*, preferably called before *Reset* or atleast before the reset condition, this function just modifies a variable, the actual waiting (sort of) is done in the function *Reset* (see below)
* **Break** (optional) : as a parameter it accepts a function *condition* to stop the timer. When the timer is reset, calling *Wait* won't restart it until the machine is reset.
* **Reset** : has two parameters, *codition* and *action*, if the machine is in **done** state and the timer (optional) is consumed and *condition* returns true, the function *action* is executed and the machine returns to **init** state.

The following example expects a button to be pressed in PB0, an LED in PB1 is turned on, if after 1 second the button is realease, the led is turned off (the function *Break* is not used)

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

In this example we will use another button too interrupt the timer and turn off the LED earlier, this is done with the function *Break*

``` c++
          ...
          Wait(1000).
          Break([]() -> bool { return PINB & (1 << PB2);}). // if the button (in PB2) is pressed the led is turned off
          ...
```

![](https://raw.githubusercontent.com/cobrce/Sequence/master/sim_break.gif)

In the following example we will use two machines to command 2 leds with different timers and different on/off buttons, the LED is in PD1, ON button is on PD0 and off button is on PD2 (the two sequences use lambda functions)

``` C++
DDRB = 1 << PB1;
DDRD = 1 << PD1;
Sequence sequence,sequence2;

while (true)
{
    sequence.
    If([]() -> bool { return PINB & 1; }).
    Do([]() { PORTB |= 1 << PB1; }).
    Wait(1000).
    Break([]() -> bool { return PINB & (1 << PB2);}).
    Reset([]() -> bool {return !(PINB & 1); }, []() { PORTB &= ~(1 << PB1); });

    sequence2.
    If([]() -> bool { return PIND & 1; }).
    Do([]() { PORTD |= 1 << PD1; }).
    Wait(3000).
    Break([]() -> bool { return PIND & (1 << PD2);}).
    Reset([]() -> bool {return !(PIND & 1); }, []() { PORTD &= ~(1 << PD1); });
}
```

![](https://raw.githubusercontent.com/cobrce/Sequence/master/sim_break_2leds.gif)