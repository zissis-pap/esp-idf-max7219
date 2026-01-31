# esp-idf-max7219

[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/yourusername/esp-idf-max7219)
[![License](https://img.shields.io/badge/license-BSD--3--Clause-green.svg)](LICENSE)
[![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v4.0+-orange.svg)](https://github.com/espressif/esp-idf)
[![Platform](https://img.shields.io/badge/platform-ESP32-lightgrey.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Interface](https://img.shields.io/badge/interface-SPI-red.svg)](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface)

🔌 ESP-IDF driver for MAX7219/MAX7221 LED display drivers supporting both 7-segment displays and 8x8 LED dot matrix modules.

**Current Version: 1.0.0** 🚀

## ✨ Features

- Support for MAX7219/MAX7221 LED display drivers
- 7-segment display mode with BCD decoding
- 8x8 LED matrix mode with text rendering
- Static text printing on LED matrices using 5x7 fonts
- Smooth text scrolling with configurable speed
- Support for up to 8 cascaded MAX7219 modules (default: 4)
- Adjustable brightness control (16 levels)
- SPI communication interface
- Hardware mirroring support for different display orientations

## 🛠️ Hardware Requirements

- ESP32 or any ESP-IDF compatible microcontroller
- MAX7219 or MAX7221 LED display driver module
- 8x8 LED dot matrix or 7-segment display

## 📦 Installation

### As an ESP-IDF Component

1. Clone this repository into your project's `components` directory:
```bash
cd your_project/components
git clone https://github.com/yourusername/esp-idf-max7219.git
```

2. Include the header in your code:
```c
#include "max7219.h"
```

## 🔌 Wiring

Connect the MAX7219 module to your ESP32:

| MAX7219 Pin | ESP32 Pin | Description |
|-------------|-----------|-------------|
| VCC         | 5V/3.3V   | Power supply |
| GND         | GND       | Ground |
| DIN         | MOSI      | SPI Data In |
| CS          | Any GPIO  | Chip Select |
| CLK         | SCK       | SPI Clock |

## 💻 Usage

### Initialization

```c
#include "max7219.h"
#include <driver/spi_master.h>

// Initialize SPI bus
spi_bus_config_t bus_cfg = {
    .mosi_io_num = GPIO_NUM_23,      // MOSI pin
    .miso_io_num = -1,                // MISO not used
    .sclk_io_num = GPIO_NUM_18,      // Clock pin
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 0
};
ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_cfg, 1));

// Initialize MAX7219 device
max7219_t dev = {
    .cascade_size = 4,    // Number of cascaded modules
    .digits = 32,         // Total digits (4 modules * 8)
    .mirrored = false     // Set to true if display is mirrored
};

ESP_ERROR_CHECK(max7219_init_desc(&dev, SPI2_HOST, MAX7219_MAX_CLOCK_SPEED_HZ, GPIO_NUM_5));
ESP_ERROR_CHECK(max7219_init(&dev));
```

### 8x8 LED Matrix - Static Text

```c
// Display static text (up to 4 characters for 4 modules)
max7219_print_static_text(&dev, "HELLO");
```

### 8x8 LED Matrix - Scrolling Text

```c
// Scroll text with 100ms delay between frames
max7219_scroll_text(&dev, "Hello World!", 100);
```

### Brightness Control

```c
// Set brightness (0-15)
max7219_set_brightness(&dev, 8);
```

### 7-Segment Display

```c
// Enable BCD mode for 7-segment displays
max7219_set_decode_mode(&dev, true);

// Draw text on 7-segment display
max7219_draw_text_7seg(&dev, 0, "12.34");
```

### Custom 8x8 Image

```c
// Draw custom 64-bit image on matrix
uint64_t smiley = 0x3C4299A581A599423C;
max7219_draw_image_8x8(&dev, 0, &smiley);
```

### Shutdown/Wake

```c
// Put display to sleep (saves power)
max7219_set_shutdown_mode(&dev, true);

// Wake up display
max7219_set_shutdown_mode(&dev, false);
```

### Clear Display

```c
max7219_clear(&dev);
```

### Cleanup

```c
// Free resources when done
max7219_free_desc(&dev);
spi_bus_free(SPI2_HOST);
```

## 📚 API Reference

### Initialization Functions

- `esp_err_t max7219_init_desc(max7219_t *dev, spi_host_device_t host, uint32_t clock_speed_hz, gpio_num_t cs_pin)` - Initialize device descriptor
- `esp_err_t max7219_init(max7219_t *dev)` - Initialize and power on the display
- `esp_err_t max7219_free_desc(max7219_t *dev)` - Free device resources

### Display Control

- `esp_err_t max7219_set_brightness(max7219_t *dev, uint8_t value)` - Set brightness (0-15)
- `esp_err_t max7219_set_shutdown_mode(max7219_t *dev, bool shutdown)` - Power on/off
- `esp_err_t max7219_set_decode_mode(max7219_t *dev, bool bcd)` - Set BCD mode for 7-segment
- `esp_err_t max7219_clear(max7219_t *dev)` - Clear all displays

### Text and Graphics

- `esp_err_t max7219_print_static_text(max7219_t *dev, const char *text)` - Print static text on LED matrix
- `esp_err_t max7219_scroll_text(max7219_t *dev, const char *text, uint16_t delay_ms)` - Scroll text with delay
- `esp_err_t max7219_draw_text_7seg(max7219_t *dev, uint8_t pos, const char *s)` - Draw on 7-segment display
- `esp_err_t max7219_draw_image_8x8(max7219_t *dev, uint8_t pos, const void *image)` - Draw 64-bit image
- `esp_err_t max7219_set_digit(max7219_t *dev, uint8_t digit, uint8_t val)` - Set individual digit/column

## Configuration

The following constants can be modified in `max7219.h`:

```c
#define MAX7219_CASCADE_SIZE 4        // Number of cascaded modules (1-8)
#define MAX7219_MAX_CASCADE_SIZE 8    // Maximum supported modules
#define MAX7219_MAX_BRIGHTNESS 15     // Maximum brightness level
#define FONT_WIDTH 5                  // Character width in pixels
#define MAX7219_MATRIX_WIDTH 8        // Matrix width (fixed)
```

## Version Information

The library version can be accessed programmatically via macros defined in `max7219.h`:

```c
#include "max7219.h"

printf("MAX7219 Driver Version: %s\n", MAX7219_VERSION);
printf("Major: %d, Minor: %d, Patch: %d\n",
       MAX7219_VERSION_MAJOR,
       MAX7219_VERSION_MINOR,
       MAX7219_VERSION_PATCH);
```

## Changelog

### Version 1.0.0 (2024-12-16)

**Code Quality Improvements:**
- Added version macros (`MAX7219_VERSION`, `MAX7219_VERSION_MAJOR`, `MAX7219_VERSION_MINOR`, `MAX7219_VERSION_PATCH`)
- Fixed typos in copyright notices across all files
- Made internal `send()` function static for better encapsulation
- Added proper error checking for SPI transmission in text display functions
- Added input validation to `max7219_process_text()` function
- Improved code formatting and fixed inconsistent indentation
- Enhanced error handling with proper memory cleanup on failures

**Feature Improvements:**
- Improved 'g' character font rendering
- Fixed text scrolling to properly display first character with 1.5s delay
- Fixed static text printing for strings shorter than 4 characters
- Increased space width for better text readability

## 🙏 Credits

Forked from [Ruslan V. Uss](https://github.com/UncleRus)'s esp-idf-lib MAX7219 driver with enhancements for text rendering and scrolling.

## 📄 License

BSD 3-Clause License - see source files for full license text.

Copyright (c) 2024 Zissis Pap <zissis.papadopoulos@gmail.com>

## 👤 Author

Zissis Papadopoulos - zissis.papadopoulos@gmail.com
