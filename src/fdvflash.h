/*
# Created by Fabrizio Di Vittorio (fdivitto2013@gmail.com)
# Copyright (c) 2015/2016 Fabrizio Di Vittorio.
# All rights reserved.

# GNU GPL LICENSE
#
# This module is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; latest version thereof,
# available at: <http://www.gnu.org/licenses/gpl.txt>.
#
# This module is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this module; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
*/

#ifndef _FDVFLASH_H_
#define _FDVFLASH_H_

#include "fdv.h"

// Flash from 0x0 to 0x8000      mapped at 0x40100000 (RAM), len = 0x8000 (32KBytes) -> ".text"
// Flash from 0x11000 to 0x6C000 mapped at 0x40211000, len = 0x5B000 (364KBytes)     -> ".irom.text", ".irom0.text",
// ".irom1.text", ".irom2.text", ".irom3.text", ".irom4.text"
// Flash from 0x6C000 to 0x6D000 mapped at 0x4026C000, len = 0x1000  (4KBytes)       -> FlashDictionary content
// Flash from 0x6D000 to 0x7B000 mapped at 0x4026D000, len = 0xE000  (56KBytes)      -> FlashFileSystem content

static uint32_t const FLASH_MAP_START = 0x40200000; // based on the CPU address space
static uint8_t const *FLASH_MAP_START_PTR = (uint8_t const *)FLASH_MAP_START;

// info from look at "eagle.app.v6.ld". Used in getBeginOfSDKSettings()
static uint32_t const SDKFLASHSETTINGSZE = 0x5000; 

// Flash address space
static uint32_t const FLASHFILESYSTEM_POS = 0x6D000;
static uint8_t const *FLASHFILESYSTEM_PTR = FLASH_MAP_START_PTR + FLASHFILESYSTEM_POS;
static uint32_t const FLASH_DICTIONARY_POS = 0x6C000;

extern "C" {
void Cache_Read_Enable(uint32 odd_even, uint32 mb_count, uint32 unk); 
}

namespace fdv {

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// selectFlashBank
// selectFlashBankSafe

// Bank 0..3 (each bank maps 1MB bank to FLASH_MAP_START
void selectFlashBank(uint32_t bank);

// automatically surrounded by enterCritical/exitCritical
void selectFlashBankSafe(uint32_t bank);

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// SafeBankSelector
// Automatically call selectFlashBank (if necessary)
// Also call enterCritical in the constructor and exitCritical in the destructor (if necessary)

class SafeBankSelector {
public:
  SafeBankSelector(void const *address);
  ~SafeBankSelector();

private:
  uint32_t m_bank;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// getFlashSize
// getFlashSpeed
// getActualFlashSize
uint32_t getFlashSize();
uint32_t getFlashSpeed();
uint32_t getActualFlashSize();

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// fixActualFlashSize

bool fixActualFlashSize();

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// getBeginOfSDKSettings

inline uint8_t const *getBeginOfSDKSettings() {
  return FLASH_MAP_START_PTR + (getFlashSize() - SDKFLASHSETTINGSZE); 
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// isStoredInFlash

inline bool isStoredInFlash(void const *ptr) {
  return (uint32_t)ptr >= FLASH_MAP_START; 
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// getChar
// flash mapped "text" is readable as 4 byte aligned blocks
// works with both RAM and Flash stored data
// str must be 32 bit aligned
char getChar(char const *str, uint32_t index);

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// getChar
// flash mapped "text" is readable as 4 byte aligned blocks
// works with both RAM and Flash stored data
// Returns first char of str
// str may Not be 32 bit aligned
char getChar(char const *str);

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// getByte
// like getChar but for uint8_t
// buffer can be unaligned
uint8_t getByte(void const *address);

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// getWord
// like getByte but for uint16_t
// buffer can be unaligned
// read as little-endian
uint16_t getWord(void const *buffer);

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// getDWord
// like getWord but for uint32_t
// buffer can be unaligned
// read as little-endian
uint32_t getDWord(void const *buffer);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// FlashWriter
//
// Copy memory between two areas of flash memory or RAM
// Destination must be flash memory
// Take care of flash pages
// Buffers can overlaps
// Buffers can be unaligned
// Requires 4096 bytes of free heap
// Multiple calls to "write" may not re-read/re-write flash

class FlashWriter {
public:
  FlashWriter(void const *dest = NULL);
  ~FlashWriter();

  bool write(void const *source, uint32_t size);

  void seek(void const *newDestination) { m_dest = (uint8_t *)newDestination; }

  void const *getCurrentPos() { return m_dest; }

  void flush();

private:
  void loadPage();

  uint8_t const *m_dest;
  uint8_t *m_pageBuffer;
  uint8_t const *m_currentPage;
  uint8_t *m_writePtr;
  uint8_t const *m_maxPosition;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// CharIterator
// Can iterate both RAM and Flash strings

struct CharIterator {
  CharIterator(char const *str = NULL) : m_str(str) {}

  char const *get();
  char operator*();
  CharIterator operator++(int);
  CharIterator operator++();
  CharIterator operator+(int32_t rhs);
  int32_t operator-(CharIterator const &rhs);
  bool operator==(char const *rhs);
  bool operator==(CharIterator const &rhs);
  bool operator!=(char const *rhs);
  bool operator!=(CharIterator const &rhs);

private:
  char const *m_str;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// ByteIterator
// Can iterate both RAM and Flash buffers

struct ByteIterator {
  ByteIterator(uint8_t const *buf = NULL) : m_buf(buf) {}

  uint8_t const *MTD_FLASHMEM get() { return m_buf; }

  uint8_t MTD_FLASHMEM operator*() { return getByte(m_buf); }

  ByteIterator MTD_FLASHMEM operator++(int) {
    ByteIterator p = *this;
    ++m_buf;
    return p;
  }

  ByteIterator MTD_FLASHMEM operator++() {
    ++m_buf;
    return *this;
  }

  bool MTD_FLASHMEM operator==(uint8_t const *rhs) { return getByte(m_buf) == *rhs; }

  bool MTD_FLASHMEM operator!=(uint8_t const *rhs) { return getByte(m_buf) != *rhs; }

private:
  uint8_t const *m_buf;
};


}

#endif