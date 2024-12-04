/*
 * Copyright (c) 2024 Zissis Pap <zissis.papadopoulos@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of itscontributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file max7219.c
 *
 * ESP-IDF driver for MAX7219/MAX7221
 * Serially Interfaced, 8-Digit LED Display Drivers
 * Serially Interfaced, 8x8-Dot LED Matrix Drivers
 *
 * Forked form Ruslan V. Uss <unclerus@gmail.com> esp-idf-lib max7219 driver 
 * 
 * Copyright (c) 2024 Zissis Pap <zissis.papadopoulos@gmail.com>
 *
 * BSD Licensed as described in the file LICENSE
 */
#include "max7219.h"
#include <string.h>
#include <esp_log.h>

#include "max7219_priv.h"
#include "5x7_fonts.h"

static const char *TAG = "max7219";

#define ALL_CHIPS 0xff
#define ALL_DIGITS 8

#define REG_DIGIT_0      (1 << 8)
#define REG_DECODE_MODE  (9 << 8)
#define REG_INTENSITY    (10 << 8)
#define REG_SCAN_LIMIT   (11 << 8)
#define REG_SHUTDOWN     (12 << 8)
#define REG_DISPLAY_TEST (15 << 8)

#define VAL_CLEAR_BCD    0x0f
#define VAL_CLEAR_NORMAL 0x00

#define CHECK(x) do { esp_err_t __; if ((__ = x) != ESP_OK) return __; } while (0)
#define CHECK_ARG(VAL) do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)

static inline uint16_t shuffle(uint16_t val)
{
    return (val >> 8) | (val << 8);
}

esp_err_t send(max7219_t *dev, uint8_t chip, uint16_t value)
{
    uint16_t buf[MAX7219_MAX_CASCADE_SIZE] = { 0 };
    if (chip == ALL_CHIPS)
    {
        for (uint8_t i = 0; i < dev->cascade_size; i++)
            buf[i] = shuffle(value);
    }
    else buf[chip] = shuffle(value);

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = dev->cascade_size * 16;
    t.tx_buffer = buf;
    return spi_device_transmit(dev->spi_dev, &t);
}

inline static uint8_t get_char(max7219_t *dev, char c)
{
    if (dev->bcd)
    {
        if (c >= '0' && c <= '9')
            return c - '0';
        switch (c)
        {
            case '-':
                return 0x0a;
            case 'E':
            case 'e':
                return 0x0b;
            case 'H':
            case 'h':
                return 0x0c;
            case 'L':
            case 'l':
                return 0x0d;
            case 'P':
            case 'p':
                return 0x0e;
        }
        return VAL_CLEAR_BCD;
    }

    return font_7seg[(c - 0x20) & 0x7f];
}

///////////////////////////////////////////////////////////////////////////////

esp_err_t max7219_init_desc(max7219_t *dev, spi_host_device_t host, uint32_t clock_speed_hz, gpio_num_t cs_pin)
{
    CHECK_ARG(dev);

    memset(&dev->spi_cfg, 0, sizeof(dev->spi_cfg));
    dev->spi_cfg.spics_io_num = cs_pin;
    dev->spi_cfg.clock_speed_hz = clock_speed_hz;
    dev->spi_cfg.mode = 0;
    dev->spi_cfg.queue_size = 1;
    dev->spi_cfg.flags = SPI_DEVICE_NO_DUMMY;

    return spi_bus_add_device(host, &dev->spi_cfg, &dev->spi_dev);
}

esp_err_t max7219_free_desc(max7219_t *dev)
{
    CHECK_ARG(dev);

    return spi_bus_remove_device(dev->spi_dev);
}

esp_err_t max7219_init(max7219_t *dev)
{
    CHECK_ARG(dev);
    if (!dev->cascade_size || dev->cascade_size > MAX7219_MAX_CASCADE_SIZE)
    {
        ESP_LOGE(TAG, "Invalid cascade size %d", dev->cascade_size);
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t max_digits = dev->cascade_size * ALL_DIGITS;
    if (dev->digits > max_digits)
    {
        ESP_LOGE(TAG, "Invalid digits count %d, max %d", dev->digits, max_digits);
        return ESP_ERR_INVALID_ARG;
    }
    if (!dev->digits)
        dev->digits = max_digits;

    // Shutdown all chips
    CHECK(max7219_set_shutdown_mode(dev, true));
    // Disable test
    CHECK(send(dev, ALL_CHIPS, REG_DISPLAY_TEST));
    // Set max scan limit
    CHECK(send(dev, ALL_CHIPS, REG_SCAN_LIMIT | (ALL_DIGITS - 1)));
    // Set normal decode mode & clear display
    CHECK(max7219_set_decode_mode(dev, false));
    // Set minimal brightness
    CHECK(max7219_set_brightness(dev, 0));
    // Wake up
    CHECK(max7219_set_shutdown_mode(dev, false));

    return ESP_OK;
}

esp_err_t max7219_set_decode_mode(max7219_t *dev, bool bcd)
{
    CHECK_ARG(dev);

    dev->bcd = bcd;
    CHECK(send(dev, ALL_CHIPS, REG_DECODE_MODE | (bcd ? 0xff : 0)));
    CHECK(max7219_clear(dev));

    return ESP_OK;
}

esp_err_t max7219_set_brightness(max7219_t *dev, uint8_t value)
{
    CHECK_ARG(dev);
    CHECK_ARG(value <= MAX7219_MAX_BRIGHTNESS);

    CHECK(send(dev, ALL_CHIPS, REG_INTENSITY | value));

    return ESP_OK;
}

esp_err_t max7219_set_shutdown_mode(max7219_t *dev, bool shutdown)
{
    CHECK_ARG(dev);

    CHECK(send(dev, ALL_CHIPS, REG_SHUTDOWN | !shutdown));

    return ESP_OK;
}

esp_err_t max7219_set_digit(max7219_t *dev, uint8_t digit, uint8_t val)
{
    CHECK_ARG(dev);
    if (digit >= dev->digits)
    {
        ESP_LOGE(TAG, "Invalid digit: %d", digit);
        return ESP_ERR_INVALID_ARG;
    }

    if (dev->mirrored)
        digit = dev->digits - digit - 1;

    uint8_t c = digit / ALL_DIGITS;
    uint8_t d = digit % ALL_DIGITS;

    ESP_LOGV(TAG, "Chip %d, digit %d val 0x%02x", c, d, val);

    CHECK(send(dev, c, (REG_DIGIT_0 + ((uint16_t)d << 8)) | val));

    return ESP_OK;
}

esp_err_t max7219_clear(max7219_t *dev)
{
    CHECK_ARG(dev);

    uint8_t val = dev->bcd ? VAL_CLEAR_BCD : VAL_CLEAR_NORMAL;
    for (uint8_t i = 0; i < ALL_DIGITS; i++)
        CHECK(send(dev, ALL_CHIPS, (REG_DIGIT_0 + ((uint16_t)i << 8)) | val));

    return ESP_OK;
}

esp_err_t max7219_draw_text_7seg(max7219_t *dev, uint8_t pos, const char *s)
{
    CHECK_ARG(dev && s);

    while (*s && pos < dev->digits)
    {
        uint8_t c = get_char(dev, *s);
        if (*(s + 1) == '.')
        {
            c |= 0x80;
            s++;
        }
        CHECK(max7219_set_digit(dev, pos, c));
        pos++;
        s++;
    }

    return ESP_OK;
}

esp_err_t max7219_draw_image_8x8(max7219_t *dev, uint8_t pos, const void *image)
{
    CHECK_ARG(dev && image);

    for (uint8_t i = pos, offs = 0; i < dev->digits && offs < 8; i++, offs++)
        max7219_set_digit(dev, i, *((uint8_t *)image + offs));

    return ESP_OK;
}

esp_err_t max7219_process_text(const char * text, uint8_t *byte_array)
{
    uint8_t text_len = strlen(text);

    for (int bit = 0; bit < MAX7219_MATRIX_WIDTH; bit++) // Iterate through each bit-plane
    {
        int matrix = 0;
        int col_index = 0;

        for (size_t i = 0; i < text_len; i++) // Process each character in the input text
        {
            char ch = text[i];
            if (ch < 32 || ch > 127) continue; // Skip invalid characters

            const uint8_t *bitmap = font_5x7[ch - 32]; // Get the bitmap for the character

            for (int font_col = 0; font_col < bitmap[0]; font_col++) // Process each column in the character's bitmap
            {
                if (bitmap[font_col+1] & (1 << bit)) 
                {
                    byte_array[8*matrix + bit] |= (1 << col_index);
                }
                col_index++; // Advance column index and handle overflow to next matrix
                if (col_index >= MAX7219_MATRIX_WIDTH) 
                {
                    col_index = 0;
                    matrix++;
                }
            }

            col_index++; // Add spacing between characters (to be removed)
            if (col_index >= MAX7219_MATRIX_WIDTH) 
            {
                col_index = 0;
                matrix++;
            }
        }
    }
    return ESP_OK;
}

esp_err_t max7219_print_static_text(max7219_t *dev, const char *text)
{
    CHECK_ARG(dev && text); // Ensure device and text pointers are valid.
    max7219_clear(dev);
    uint16_t text_len = strlen(text);
    if (text_len > MAX7219_CASCADE_SIZE) 
        text_len = MAX7219_CASCADE_SIZE; // Limit text length to the cascade size.

    uint8_t *processed_text = (uint8_t *)calloc(8 * text_len, sizeof(uint8_t)); // Allocate memory for processed text
    if (!processed_text)
        return ESP_ERR_NO_MEM; // Memory allocation failed.

    max7219_process_text(text, processed_text);       // Convert text into a format suitable for MAX7219

    for (int matrix = 0; matrix < text_len; matrix++) // Send processed data to the MAX7219 device
    {
        for (uint8_t row = 0; row < 8; row++) // MAX7219 has 8 rows
        {
            uint8_t data = processed_text[8 * matrix + row];
            send(dev, MAX7219_CASCADE_SIZE - matrix - 1, ((8 - row) << 8) | data);
        }
    }

    free(processed_text); // Free allocated memory
    return ESP_OK;
}


esp_err_t max7219_scroll_text(max7219_t *dev, const char *text, uint16_t delay_ms)
{
    CHECK_ARG(dev && text); // Validate input arguments.

    uint16_t text_len = strlen(text);
    if (text_len == 0) return ESP_OK; // No text to scroll.

    uint8_t *processed_text = (uint8_t *)calloc(8 * text_len, sizeof(uint8_t)); // Allocate memory for processed text representation.
    if (!processed_text) return ESP_ERR_NO_MEM; // Memory allocation failed.

    max7219_clear(dev); // Clear all matrices before scrolling.
    max7219_process_text(text, processed_text); // Convert the text into a format compatible with the MAX7219.

    // Calculate the total scrolling width based on character widths and spacing.
    uint16_t total_scroll_width = 0;
    for (uint16_t i = 0; i < text_len; i++) 
    {
        char ch = text[i];
        if (ch >= 32 && ch <= 127) // Ensure character is within valid range.
        {
            total_scroll_width += font_5x7[ch - 32][0] + 1; // Character width + spacing.
        }
    }

    uint16_t display_matrices = (text_len > MAX7219_CASCADE_SIZE) ? MAX7219_CASCADE_SIZE : text_len;
    for (uint16_t bit = 0; bit < total_scroll_width; bit++) // Scroll text bit-by-bit across the display.
    {
        for (int matrix = 0; matrix < display_matrices; matrix++) // Send the processed data to the MAX7219 matrices.
        {
            for (uint8_t row = 0; row < MAX7219_MATRIX_WIDTH; row++) 
            {
                uint16_t index = row + (matrix * MAX7219_MATRIX_WIDTH);
                uint8_t data = (index < 8 * text_len) ? processed_text[index] : 0; // Bounds-safe access.
                send(dev, MAX7219_CASCADE_SIZE - matrix - 1, ((MAX7219_MATRIX_WIDTH - row) << 8) | data);
            }
        }

        for (uint8_t row = 0; row < MAX7219_MATRIX_WIDTH; row++)  // Shift bits in `processed_text` for scrolling effect.
        {
            uint8_t carry = 0;
            for (int col = text_len - 1; col >= 0; col--) 
            {
                int index = row + (col * MAX7219_MATRIX_WIDTH);
                uint8_t next_carry = (processed_text[index] & 1); // Extract least significant bit.
                processed_text[index] = (processed_text[index] >> 1) | (carry << 7); // Shift and add carry.
                carry = next_carry;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms)); // Apply scrolling delay.
    }

    free(processed_text); // Free allocated memory.
    return ESP_OK;
}

