#include <avr/io.h>

#include <util/setbaud.h>

#define hi8(x)  ((x) >> 8)
#define lo8(x)  ((x) & 0xFF)

void uart_init(void) {
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

#if USE_2X
	UCSR0A |= _BV(U2X0);
#else
	UCSR0A &= ~(_BV(U2X0));
#endif

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

uint8_t uart_has_data() {
	for (uint8_t i = 0; i < 20; i++) {
		if (i != 0) {
			_delay_ms(0.5);
		}
		if (bit_is_set(UCSR0A, RXC0)) {
			return 1;
		}

	}
	return 0;
}

void uart_put_uint8(uint8_t i) {
	UDR0 = i;
	loop_until_bit_is_set(UCSR0A, TXC0); /* Wait until transmission ready. */
	_delay_ms(0.5);
}

void uart_put_uint16(uint16_t i) {
	uart_put_uint8(hi8(i));
	uart_put_uint8(lo8(i));
}

uint8_t uart_get_uint8() {
	loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
	return UDR0;
}

uint16_t uart_get_uint16() {
	return (uart_get_uint8() << 8) + uart_get_uint8();
}
