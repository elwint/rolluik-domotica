#include "uart.c"

enum command_t {
	COMMAND_PING = 1,
	COMMAND_ECHO = 2,
	COMMAND_SENSOR = 3,
	COMMAND_STATUS = 4,
	COMMAND_SET_LIMITS = 5,
	COMMAND_FORCE = 6,
	COMMAND_AUTO = 7
};

enum status_t {
	STATUS_OK = 0,
	STATUS_UNKNOWN_COMMAND = 1,
	STATUS_MISSING_DATA = 2
};

void protocol_init() {
	uart_init();
}

void protocol_handler() {
	if (!uart_has_data()) {
		return;
	}

	enum command_t command = uart_get_uint8();
	uint16_t data = 0;

	switch (command) {
		case COMMAND_PING:
			uart_put_uint8(STATUS_OK);

			break;
		case COMMAND_ECHO:
			if (!uart_has_data()) {
				uart_put_uint8(STATUS_MISSING_DATA);
				break;
			}
			data = uart_get_uint16();
			uart_put_uint8(STATUS_OK);
			uart_put_uint16(data);

			break;
		case COMMAND_SENSOR:
			uart_put_uint8(STATUS_OK);

			uart_put_uint16(distance);
			uart_put_uint16(sensor_type);
			uart_put_uint16(sensor_data);

			break;
		case COMMAND_STATUS:
			uart_put_uint8(STATUS_OK);

			uart_put_uint16(forced);
			uart_put_uint16(state);
			uart_put_uint16(done);

			break;
		case COMMAND_SET_LIMITS:
			if (!uart_has_data()) {
				uart_put_uint8(STATUS_MISSING_DATA);
				break;
			}
			min_distance = uart_get_uint16();

			if (!uart_has_data()) {
				uart_put_uint8(STATUS_MISSING_DATA);
				break;
			}
			max_distance = uart_get_uint16();

			if (!uart_has_data()) {
				uart_put_uint8(STATUS_MISSING_DATA);
				break;
			}
			up_sensor    = uart_get_uint16();

			if (!uart_has_data()) {
				uart_put_uint8(STATUS_MISSING_DATA);
				break;
			}
			down_sensor  = uart_get_uint16();

			done = 0;

			uart_put_uint8(STATUS_OK);

			break;
		case COMMAND_FORCE:
			if (!uart_has_data()) {
				uart_put_uint8(STATUS_MISSING_DATA);
				break;
			}
			state = uart_get_uint16();
			forced = 1;

			uart_put_uint8(STATUS_OK);

			break;
		case COMMAND_AUTO:
			forced = 0;

			uart_put_uint8(STATUS_OK);

			break;
		default: // Unknown command
			uart_put_uint8(STATUS_UNKNOWN_COMMAND);
	}
}
