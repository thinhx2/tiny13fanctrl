/* This file was automatically generated by µCurve (ucurve).
 * DO NOT EDIT MANUALLY, WILL BE OVERWRITTEN.
 * Creation: 2018-01-11 20:18:39
 */

#include <stdint.h>
#include <avr/pgmspace.h>
#include "ntc_adu2degc.h"

struct lookup_entry_t {
	uint8_t x;
	int16_t y;
};

/* Maximum error: (y_true - y_interpolated) = -1.00 °C */
static const struct lookup_entry_t PROGMEM lookup_table[] = {
	{ .x = 8, .y = 148 },
	{ .x = 13, .y = 129 },		/* +1.0 °C */
	{ .x = 20, .y = 112 },		/* +1.0 °C */
	{ .x = 31, .y = 97 },		/* +1.0 °C */
	{ .x = 47, .y = 82 },		/* +1.0 °C */
	{ .x = 64, .y = 71 },		/* +1.0 °C */
	{ .x = 79, .y = 63 },		/* +0.9 °C */
	{ .x = 99, .y = 54 },		/* +0.9 °C */
	{ .x = 127, .y = 43 },		/* +1.0 °C */
	{ .x = 194, .y = 16 },		/* -1.0 °C */
	{ .x = 219, .y = 3 },		/* -1.0 °C */
	{ .x = 234, .y = -8 },		/* -1.0 °C */
	{ .x = 243, .y = -19 },		/* -0.9 °C */
	{ .x = 247, .y = -26 },		/* -0.5 °C */
	{ .x = 251, .y = -37 },		/* -0.7 °C */
	{ .x = 252, .y = -40 },		/* +0.0 °C */
};

static struct lookup_entry_t get_lookup_entry(uint8_t index) {
	struct lookup_entry_t entry;
	memcpy_P(&entry, lookup_table + index, sizeof(struct lookup_entry_t));
	return entry;
}

static int16_t interpolate(uint8_t xvalue, const struct lookup_entry_t *low, const struct lookup_entry_t *high) {
	uint8_t xspan = high->x - low->x;
	int8_t yspan = high->y - low->y;
	uint8_t xwindow = xvalue - low->x;

	/* slope = yspan / xspan = (high->y - low->y) / (high->x - low->x)
	   return low->y + (xwindow / xspan) * yspan
	   i.e., return low->y + xwindow / xspan * yspan
	*/
	return low->y + (int16_t)xwindow * yspan / (int16_t)xspan;
}

int16_t ntc_adu2degc(uint8_t xvalue) {
	if (xvalue < 8) {
		return 148;
	} else if (xvalue >= 252) {
		return -40;
	} else {
		struct lookup_entry_t low = get_lookup_entry(0);
		for (uint8_t i = 1; i < 16; i++) {
			struct lookup_entry_t high = get_lookup_entry(i);
			if ((xvalue >= low.x) && (xvalue < high.x)) {
				return interpolate(xvalue, &low, &high);
			}
			low = high;
		}
	}
	return 0;
}

#ifdef __TEST_LOOKUP_CODE__
/* gcc -std=c11 -Wall -O3 -D__TEST_LOOKUP_CODE__ -o ntc_adu2degc ntc_adu2degc.c && ./ntc_adu2degc */

#include <stdio.h>

int main() {
	for (int16_t x = 8; x < 252; x++) {
		int16_t y = ntc_adu2degc(x);
		printf("%5d -> %d\n", x, y);
	}
	return 0;
}
#endif
