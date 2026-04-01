#include "quantum.h"
#include "matrix.h"

// COL2ROW: drive each col LOW, read rows.
// Verified working via discovery firmware.
static const pin_t col_pins[] = {
    GP0,  GP1,  GP2,  GP3,  GP4,  GP5,  GP6,  GP7,
    GP8,  GP9,  GP10, GP11, GP12, GP13, GP14, GP15
};
static const pin_t row_pins[] = {
    GP16, GP17, GP18, GP19, GP20, GP21
};

#define COL_COUNT (sizeof(col_pins) / sizeof(col_pins[0]))
#define ROW_COUNT (sizeof(row_pins) / sizeof(row_pins[0]))

void matrix_init_custom(void) {
    for (uint8_t i = 0; i < ROW_COUNT; i++) {
        gpio_set_pin_input_high(row_pins[i]);
    }
    for (uint8_t i = 0; i < COL_COUNT; i++) {
        gpio_set_pin_input_high(col_pins[i]);
    }
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;

    for (uint8_t col = 0; col < COL_COUNT; col++) {
        gpio_set_pin_output(col_pins[col]);
        gpio_write_pin_low(col_pins[col]);
        wait_us(10);

        for (uint8_t row = 0; row < ROW_COUNT; row++) {
            bool pressed    = !gpio_read_pin(row_pins[row]);
            bool was_pressed = (current_matrix[row] >> col) & 1;

            if (pressed != was_pressed) {
                if (pressed) {
                    current_matrix[row] |= (matrix_row_t)1 << col;
                } else {
                    current_matrix[row] &= ~((matrix_row_t)1 << col);
                }
                changed = true;
            }
        }

        gpio_set_pin_input_high(col_pins[col]);
    }

    return changed;
}
