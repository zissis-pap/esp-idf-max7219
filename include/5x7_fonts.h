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

#include <stdio.h>
const uint8_t font_5x7[96][6] = 
{
    // Space ' '
    {0x01, 0x00},
    // '!'
    {0x01, 0x5F},
    // '"'
    {0x03, 0x07, 0x00, 0x07},
    // '#'
    {0x05, 0x14, 0x7F, 0x14, 0x7F, 0x14},
    // '$'
    {0x05, 0x24, 0x2A, 0x7F, 0x2A, 0x12},
    // '%'
    {0x05, 0x23, 0x13, 0x08, 0x64, 0x62},
    // '&'
    {0x05, 0x36, 0x49, 0x55, 0x22, 0x50},
    // '''
    {0x02, 0x05, 0x03},
    // '('
    {0x03, 0x1C, 0x22, 0x41},
    // ')'
    {0x03, 0x41, 0x22, 0x1C},
    // '*'
    {0x05, 0x14, 0x08, 0x3E, 0x08, 0x14},
    // '+'
    {0x05, 0x08, 0x08, 0x3E, 0x08, 0x08},
    // ','
    {0x02, 0x50, 0x30},
    // '-'
    {0x05, 0x08, 0x08, 0x08, 0x08, 0x08},
    // '.'
    {0x02, 0x60, 0x60},
    // '/'
    {0x05, 0x20, 0x10, 0x08, 0x04, 0x02},
    // '0'
    {0x05, 0x3E, 0x51, 0x49, 0x45, 0x3E},
    // '1'
    {0x03, 0x42, 0x7F, 0x40},
    // '2'
    {0x05, 0x42, 0x61, 0x51, 0x49, 0x46},
    // '3'
    {0x05, 0x21, 0x41, 0x45, 0x4B, 0x31},
    // '4'
    {0x05, 0x18, 0x14, 0x12, 0x7F, 0x10},
    // '5'
    {0x05, 0x27, 0x45, 0x45, 0x45, 0x39},
    // '6'
    {0x05, 0x3C, 0x4A, 0x49, 0x49, 0x30},
    // '7'
    {0x05, 0x01, 0x71, 0x09, 0x05, 0x03},
    // '8'
    {0x05, 0x36, 0x49, 0x49, 0x49, 0x36},
    // '9'
    {0x05, 0x06, 0x49, 0x49, 0x29, 0x1E},
    // ':'
    {0x02, 0x36, 0x36},
    // ';'
    {0x02, 0x56, 0x36},
    // '<'
    {0x04, 0x08, 0x14, 0x22, 0x41},
    // '='
    {0x05, 0x14, 0x14, 0x14, 0x14, 0x14},
    // '>'
    {0x04, 0x41, 0x22, 0x14, 0x08},
    // '?'
    {0x05, 0x02, 0x01, 0x51, 0x09, 0x06},
    // '@'
    {0x05, 0x3E, 0x41, 0x5D, 0x59, 0x4E},
    // 'A'
    {0x05, 0x7E, 0x11, 0x11, 0x11, 0x7E},
    // 'B'
    {0x05, 0x7F, 0x49, 0x49, 0x49, 0x36},
    // 'C'
    {0x05, 0x3E, 0x41, 0x41, 0x41, 0x22},
    // 'D'
    {0x05, 0x7F, 0x41, 0x41, 0x22, 0x1C},
    // 'E'
    {0x05, 0x7F, 0x49, 0x49, 0x49, 0x41},
    // 'F'
    {0x05, 0x7F, 0x09, 0x09, 0x09, 0x01},
	// 'G'
    {0x05, 0x3E, 0x41, 0x49, 0x49, 0x7A},
    // 'H'
    {0x05, 0x7F, 0x08, 0x08, 0x08, 0x7F},
    // 'I'
    {0x03, 0x41, 0x7F, 0x41},
    // 'J'
    {0x05, 0x20, 0x40, 0x41, 0x3F, 0x01},
    // 'K'
    {0x05, 0x7F, 0x08, 0x14, 0x22, 0x41},
    // 'L'
    {0x05, 0x7F, 0x40, 0x40, 0x40, 0x40},
    // 'M'
    {0x05, 0x7F, 0x02, 0x04, 0x02, 0x7F},
    // 'N'
    {0x05, 0x7F, 0x04, 0x08, 0x10, 0x7F},
    // 'O'
    {0x05, 0x3E, 0x41, 0x41, 0x41, 0x3E},
    // 'P'
    {0x05, 0x7F, 0x09, 0x09, 0x09, 0x06},
    // 'Q'
    {0x05, 0x3E, 0x41, 0x51, 0x21, 0x5E},
    // 'R'
    {0x05, 0x7F, 0x09, 0x19, 0x29, 0x46},
    // 'S'
    {0x05, 0x46, 0x49, 0x49, 0x49, 0x31},
    // 'T'
    {0x05, 0x01, 0x01, 0x7F, 0x01, 0x01},
    // 'U'
    {0x05, 0x3F, 0x40, 0x40, 0x40, 0x3F},
    // 'V'
    {0x05, 0x1F, 0x20, 0x40, 0x20, 0x1F},
    // 'W'
    {0x05, 0x7F, 0x20, 0x10, 0x20, 0x7F},
    // 'X'
    {0x05, 0x63, 0x14, 0x08, 0x14, 0x63},
    // 'Y'
    {0x05, 0x07, 0x08, 0x70, 0x08, 0x07},
    // 'Z'
    {0x05, 0x61, 0x51, 0x49, 0x45, 0x43},
    // '['
    {0x03, 0x7F, 0x41, 0x41},
    // '\'
    {0x05, 0x02, 0x04, 0x08, 0x10, 0x20},
    // ']'
    {0x03, 0x41, 0x41, 0x7F},
    // '^'
    {0x05, 0x04, 0x02, 0x01, 0x02, 0x04},
    // '_'
    {0x05, 0x40, 0x40, 0x40, 0x40, 0x40},
    // '`'
    {0x03, 0x01, 0x02, 0x04, 0x03},
    // 'a'
    {0x05, 0x20, 0x54, 0x54, 0x54, 0x78},
    // 'b'
    {0x05, 0x7F, 0x48, 0x44, 0x44, 0x38},
    // 'c'
    {0x05, 0x38, 0x44, 0x44, 0x44, 0x20},
    // 'd'
    {0x05, 0x38, 0x44, 0x44, 0x48, 0x7F},
    // 'e'
    {0x05, 0x38, 0x54, 0x54, 0x54, 0x18},
    // 'f'
    {0x05, 0x08, 0x7E, 0x09, 0x01, 0x02},
    // 'g'
    {0x05, 0x0C, 0x52, 0x52, 0x52, 0x3E},
    // 'h'
    {0x05, 0x7F, 0x08, 0x04, 0x04, 0x78},
    // 'i'
    {0x03, 0x44, 0x7D, 0x40},
    // 'j'
    {0x04, 0x20, 0x40, 0x44, 0x3D},
    // 'k'
    {0x04, 0x7F, 0x10, 0x28, 0x44},
    // 'l'
    {0x03, 0x41, 0x7F, 0x40},
    // 'm'
    {0x05, 0x7C, 0x04, 0x18, 0x04, 0x78},
    // 'n'
    {0x05, 0x7C, 0x08, 0x04, 0x04, 0x78},
    // 'o'
    {0x05, 0x38, 0x44, 0x44, 0x44, 0x38},
    // 'p'
    {0x05, 0x7C, 0x14, 0x14, 0x14, 0x08},
    // 'q'
    {0x05, 0x08, 0x14, 0x14, 0x18, 0x7C},
    // 'r'
    {0x05, 0x7C, 0x08, 0x04, 0x04, 0x08},
    // 's'
    {0x05, 0x48, 0x54, 0x54, 0x54, 0x20},
    // 't'
    {0x05, 0x04, 0x3F, 0x44, 0x40, 0x20},
    // 'u'
    {0x05, 0x3C, 0x40, 0x40, 0x20, 0x7C},
    // 'v'
    {0x05, 0x1C, 0x20, 0x40, 0x20, 0x1C},
    // 'w'
    {0x05, 0x3C, 0x40, 0x30, 0x40, 0x3C},
    // 'x'
    {0x05, 0x44, 0x28, 0x10, 0x28, 0x44},
    // 'y'
    {0x05, 0x0C, 0x50, 0x50, 0x50, 0x3C},
    // 'z'
    {0x05, 0x44, 0x64, 0x54, 0x4C, 0x44},
    // '{'
    {0x03, 0x08, 0x36, 0x41},
    // '|'
    {0x03, 0x00, 0x7F, 0x00},
    // '}'
    {0x03, 0x41, 0x36, 0x08},
    // '->'
    {0x05, 0x08, 0x08, 0x2A, 0x1C, 0x08}
};
