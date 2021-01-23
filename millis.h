#ifndef MILLIS_H_
#define MILLIS_H_

#include <avr/io.h>
#include <avr/interrupt.h> 


typedef unsigned long millis_t;
static volatile millis_t milliseconds;

#if F_CPU > 16320000 // 16.32MHz - 65.28MHz
#define CLOCKSEL (_BV(CS20))
#define PRESCALER 256
#elif F_CPU > 2040000 // 2.04MHz - 16.32MHz
#define CLOCKSEL (_BV(CS01)|_BV(CS00))
#define PRESCALER 64
#elif F_CPU > 255 // 256Hz - 2.04MHz
#define CLOCKSEL (_BV(CS01))
#define PRESCALER 8
#endif

#define REG_TCCRA		TCCR0A
#define REG_TCCRB		TCCR0B
#define REG_TIMSK		TIMSK0
#define REG_OCR			OCR0A
#define BIT_WGM			WGM01
#define BIT_OCIE		OCIE0A
#ifdef TIMER0_COMPA_vect
#define ISR_VECT		TIMER0_COMPA_vect
#else
#define ISR_VECT		TIM0_COMPA_vect
#endif
#define pwr_enable()	power_timer0_enable()
#define pwr_disable()	power_timer0_disable()

#define SET_TCCRA()	(REG_TCCRA = _BV(BIT_WGM))
#define SET_TCCRB()	(REG_TCCRB = CLOCKSEL)


millis_t millis()
{
    millis_t time;
    uint8_t oldSreg = SREG;
    cli();
    time = milliseconds;
    SREG = oldSreg;
    return time;

}

void millis_init()
{
	// Timer settings
	SET_TCCRA();
	SET_TCCRB();
	REG_TIMSK = _BV(BIT_OCIE);
	REG_OCR = ((F_CPU / PRESCALER) / 1000);
}

ISR(ISR_VECT)
{
	++milliseconds;
}

#endif