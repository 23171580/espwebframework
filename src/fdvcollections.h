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

#ifndef _FDVCOLLECTIONS_H_
#define _FDVCOLLECTIONS_H_

#include "fdv.h"

namespace fdv {

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// VectorBase
// A non-template vector
// Max item size: 65535 bytes
// Max items: 65535

class VectorBase {
public:
  VectorBase(uint16_t itemSize);
  VectorBase(VectorBase const & c);
  ~VectorBase();
  void add(void const * item);
  void insert(uint32_t position, void const * item);
  void remove(uint32_t position);
  int32_t indexof(void const * item) const;
  void clear();
  uint32_t size() const { return m_itemsCount; }
  void setSize(uint32_t value);
  void setData(void * data, uint16_t itemsCount);
  void * getItem(uint32_t position);
  void const * getItem(uint32_t position) const;
  void operator=(VectorBase const & c);
  void * begin() { return m_data; }
  void const * begin() const { return m_data; }
  bool empty() const { return m_itemsCount == 0; }

private:
  void allocate(uint32_t itemsCount);

private:
  uint16_t m_itemSize;
  uint16_t m_itemsCount;
  uint16_t m_itemsAllocated;
  uint8_t * m_data;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Generic vector based on VectorBase

template <typename T>
class Vector {
public:
  Vector() : m_data(sizeof(T)) {}
  void add(T const & value) { m_data.add(&value); }
  void insert(uint32_t position, T const & value) { m_data.insert(position, &value); }
  void remove(uint32_t position) { m_data.remove(position); }
  int32_t indexof(T const & value) const { return m_data.indexof(&value); }
  void clear() { m_data.clear(); }
  uint32_t size() const { return m_data.size(); }
  void setSize(uint32_t value) { m_data.setSize(value); }
  void setData(T * data, uint32_t items) { m_data.setData(data, items); }
  T & operator[](uint32_t position) { return *static_cast<T *>(m_data.getItem(position)); }
  T const & operator[](uint32_t position) const { return *static_cast<T const *>(m_data.getItem(position)); }
  T & last() { return operator[](size() - 1); }
  T * begin() { return static_cast<T *>(m_data.begin()); }
  T const * begin() const { return static_cast<T const *>(m_data.begin()); }
  T * end() { return begin() + size(); }
  T const * end() const { return begin() + size(); }
  bool empty() const { m_data.empty(); }
  int32_t compare(Vector<T> const &rhs) const {
    T const * s1 = begin();
    T const * s2 = rhs.begin();
    while (s1 < end() && s2 < rhs.end()) {
      if (*s1 != *s2)
        return *s1 - *s2;
      ++s1, ++s2;
    }
    return size() - rhs.size();
  }

private:
  VectorBase m_data;
};

template <typename T>
inline bool operator==(Vector<T> const & lhs, Vector<T> const & rhs) {
  if (lhs.size() != rhs.size())
    return false;
  T const * v1 = lhs.begin();
  T const * v2 = rhs.begin();
  while (v1 < lhs.end() && v2 < rhs.end()) {
    if (*v1 != *v2)
      return false;
    ++v1, ++v2;
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Generic stack based on Vector

template <typename T>
class Stack {
public:
  void push(T const &value) { m_data.add(value); }

  T pop() {
    uint32_t lastIndex = m_data.size() - 1;
    T ret = m_data[lastIndex];
    m_data.remove(lastIndex);
    return ret;
  }

  T &operator[](uint32_t position) { return m_data[position]; }

  uint32_t size() { return m_data.size(); }

  void clear() { m_data.clear(); }

private:
  Vector<T> m_data;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Generic list

template <typename T>
class List {
public:
  List() : m_first(NULL) {}

  ~List() { clear(); }

  void clear() {
    Item *next;
    for (Item *cur = m_first; cur; cur = next) {
      next = cur->next;
      reinterpret_cast<T *>(cur->value)->~T(); // destruct value
      delete cur;
    }
    m_first = NULL;
  }

  T *add() {
    Item *item = new Item;
    item->next = NULL;
    *getPtrOfLastNext() = item;
    return new (&item->value[0]) T; // placement new into value[] array
  }

private:
  struct Item {
    Item *next;
    uint8_t value[sizeof(T)];
  };

public:
  struct Iterator {
    Iterator(Item *current) : m_current(current) {}
    Iterator(Iterator const &c) { m_current = c.m_current; }
    Iterator() : m_current(NULL) {}
    T &operator*() { return *reinterpret_cast<T *>(m_current->value); }
    Iterator operator++(int) {
      Iterator c = *this;
      next();
      return c;
    }
    Iterator &operator++() {
      next();
      return *this;
    }
    void next() { m_current = m_current->next; }
    bool operator!=(Iterator const &rhs) { return m_current != rhs.m_current; }
    bool operator==(Iterator const &rhs) { return m_current == rhs.m_current; }

  private:
    Item *m_current;
  };

  Iterator getIterator() { return Iterator(m_first); }

private:
  Item **getPtrOfLastNext() {
    Item *cur = m_first;
    while (cur && cur->next)
      cur = cur->next;
    return cur ? &cur->next : &m_first;
  }

  Item *m_first;
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// CharChunk

struct CharChunkBase {
  CharChunkBase *next;
  char *data;
  uint8_t type; // refers to ::TYPE value in inherited classes

  CharChunkBase(CharChunkBase *next_, char *data_, uint8_t type_) : next(next_), data(data_), type(type_) {}

  uint32_t getItems();
  void setItems(uint32_t value);
  uint32_t getCapacity();
} __attribute__((packed));

template <uint8_t TYPE_V, typename SIZE_T> struct CharChunkReference : public CharChunkBase {
  static uint8_t const TYPE = TYPE_V;

  SIZE_T items;

  CharChunkReference(char *data_, SIZE_T items_) : CharChunkBase(NULL, data_, TYPE), items(items_) {}
} __attribute__((packed));

typedef CharChunkReference<0, uint8_t> CharChunkReference8;
typedef CharChunkReference<1, uint16_t> CharChunkReference16;
typedef CharChunkReference<2, uint32_t> CharChunkReference32;

template <uint8_t TYPE_V, typename SIZE_T> struct CharChunkOwn : public CharChunkReference<TYPE_V, SIZE_T> {
  CharChunkOwn(char *data_, SIZE_T items_) : CharChunkReference<TYPE_V, SIZE_T>(data_, items_) {}

  ~CharChunkOwn() { delete[] CharChunkBase::data; }
} __attribute__((packed));

typedef CharChunkOwn<3, uint8_t> CharChunkOwn8;
typedef CharChunkOwn<4, uint16_t> CharChunkOwn16;
typedef CharChunkOwn<5, uint32_t> CharChunkOwn32;

template <uint8_t TYPE_V, typename SIZE_T> struct CharChunkAllocated : public CharChunkOwn<TYPE_V, SIZE_T> {
  SIZE_T capacity;

  CharChunkAllocated(SIZE_T capacity_) : CharChunkOwn<TYPE_V, SIZE_T>(new char[capacity_], 0), capacity(capacity_) {}
} __attribute__((packed));

typedef CharChunkAllocated<6, uint8_t> CharChunkAllocated8;
typedef CharChunkAllocated<7, uint8_t> CharChunkAllocated16;
typedef CharChunkAllocated<8, uint8_t> CharChunkAllocated32;

struct CharChunkLink : public CharChunkBase {
  static uint8_t const TYPE = 9;

  uint32_t items; // count of all items in the linked list
  CharChunkBase *link;

  CharChunkLink(CharChunkBase *firstChunk, uint32_t items_)
      : CharChunkBase(NULL, NULL, TYPE), items(items_), link(firstChunk) {}
} __attribute__((packed));

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// CharChunksIterator

struct CharChunksIterator {
  CharChunksIterator(CharChunkBase *chunk = NULL);
  CharChunksIterator(CharChunksIterator const &c);
  CharChunksIterator &operator=(CharChunksIterator const &c);
  char &operator*();
  CharChunksIterator operator++(int);
  CharChunksIterator &operator++();
  CharChunksIterator &operator+=(int32_t rhs);
  CharChunksIterator operator+(int32_t rhs);
  int32_t operator-(CharChunksIterator rhs);
  bool operator==(CharChunksIterator const &rhs);
  bool operator!=(CharChunksIterator const &rhs);
  uint32_t getPosition();
  bool isLast();
  bool isValid();
  CharChunkBase *moveToNextChunk();
  CharChunkBase *getCurrentChunk();

private:
  void next();
  void checkLinkedChunks();

private:
  CharChunkBase *m_chunk;
  uint32_t m_pos;    // position inside this chunk
  uint32_t m_absPos; // absolute position (starting from beginning of LinkedCharChunks)
  Stack<CharChunkBase *> m_linkedNext;
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// LinkedCharChunks
//
// Warn: copy constructor copies only pointers. Should not used with heap or stack buffers.

struct LinkedCharChunks {

  LinkedCharChunks() : m_chunks(NULL), m_current(NULL) {}

  // copy constructor
  // Only data pointers are copied and they will be not freed
  LinkedCharChunks(LinkedCharChunks &c) : m_chunks(NULL), m_current(NULL) { *this = c; }

  ~LinkedCharChunks() { clear(); }

  void clear();
  CharChunkBase *addChunk(CharChunkBase *chunk);
  CharChunkBase *addChunk(uint32_t capacity);
  CharChunkBase *addChunk(char *data, uint32_t items, bool freeOnDestroy);
  CharChunkBase *addChunk(char const *data, uint32_t items, bool freeOnDestroy);
  void addChunk(char const *str, bool freeOnDestroy = false);
  void addChunks(LinkedCharChunks *src);
  void append(char value, uint32_t newChunkSize = 1);
  CharChunkBase *getFirstChunk();
  CharChunksIterator getIterator();
  uint32_t getItemsCount() const;
  // void dump();
  // void dumpChunks();
  void operator=(LinkedCharChunks &c);

private:
  CharChunkBase *m_chunks;
  CharChunkBase *m_current;
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// CharChunkFactory

struct CharChunkFactory {
  static CharChunkBase *createCharChunkReference(char *data, uint32_t items);
  static CharChunkBase *createCharChunkOwn(char *data, uint32_t items);
  static CharChunkBase *createCharChunkAllocated(uint32_t capacity);
  static CharChunkBase *createCharChunkLink(LinkedCharChunks *link);

  // this to avoid virtual destructors usage
  static void deleteCharChunk(CharChunkBase *chunk);
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// IterDict
// A dictionary where key and value are iterators

template <typename KeyIterator, typename ValueIterator>
class IterDict {
public:
  struct Item {
    Item *next;
    KeyIterator key;
    KeyIterator keyEnd;
    ValueIterator value;
    ValueIterator valueEnd;
    APtr<char> valueStr; // dynamically allocated zero terminated value string (created by operator[])

    Item(KeyIterator key_, KeyIterator keyEnd_, ValueIterator value_, ValueIterator valueEnd_)
        : next(NULL), key(key_), keyEnd(keyEnd_), value(value_), valueEnd(valueEnd_) {}
    Item() : next(NULL), key(KeyIterator()), keyEnd(KeyIterator()), value(ValueIterator()), valueEnd(ValueIterator()) {}
    bool TMTD_FLASHMEM operator==(Item const &rhs) {
      return next == rhs.next && key == rhs.key && keyEnd == rhs.keyEnd && value == rhs.value &&
             valueEnd == rhs.valueEnd;
    }
    bool TMTD_FLASHMEM operator!=(Item const &rhs) { return !(*this == rhs); }
  };

  IterDict() : m_items(NULL), m_current(NULL), m_itemsCount(0), m_urlDecode(false) {}

  ~IterDict() { clear(); }

  void TMTD_FLASHMEM clear() {
    Item *item = m_items;
    while (item) {
      Item *next = item->next;
      delete item;
      item = next;
    }
    m_items = m_current = NULL;
    m_itemsCount = 0;
  }

  void TMTD_FLASHMEM add(KeyIterator key, KeyIterator keyEnd, ValueIterator value, ValueIterator valueEnd) {
    if (m_items) {
      m_current = m_current->next = new Item(key, keyEnd, value, valueEnd);
      ++m_itemsCount;
    } else {
      m_current = m_items = new Item(key, keyEnd, value, valueEnd);
      ++m_itemsCount;
    }
  }

  // key and value must terminate with a Zero
  void TMTD_FLASHMEM add(KeyIterator key, ValueIterator value) {
    add(key, key + t_strlen(key), value, value + t_strlen(value));
  }

  // automatically embeds key and value into CharIterator, so key and value can stay in RAM or Flash
  // key and value must terminate with a Zero
  // Applies only when KeyIterator == ValueIterator == CharIterator
  void TMTD_FLASHMEM add(char const *key, char const *value) {
    add(CharIterator(key), CharIterator(value)); 
  }

  uint32_t TMTD_FLASHMEM getItemsCount() { return m_itemsCount; }

  // warn: this doesn't check "index" range!
  Item *TMTD_FLASHMEM getItem(uint32_t index) {
    Item *item = m_items;
    for (; index > 0; --index)
      item = item->next;
    return item;
  }

  // key stay in RAM or Flash
  Item *TMTD_FLASHMEM getItem(char const *key, char const *keyEnd) {
    Item *item = m_items;
    while (item) {
      if (t_compare(item->key, item->keyEnd, CharIterator(key), CharIterator(keyEnd)))
        return item; // found
      item = item->next;
    }
    return NULL; // not found
  }

  // warn: this doesn't check "index" range!
  Item *TMTD_FLASHMEM operator[](uint32_t index) { return getItem(index); }

  // key can stay in RAM or Flash and must terminate with zero
  // creates a RAM stored temporary (with the same lifetime of IterDict class) zero terminated string with the value
  // content
  // if m_urlDecode is true then the temporary in RAM string is url decoded
  char const *TMTD_FLASHMEM operator[](char const *key) {
    Item *item = getItem(key, key + f_strlen(key));
    if (item) {
      if (item->valueStr.get() == NULL) {
        item->valueStr.reset(t_strdup(item->value, item->valueEnd));
        if (m_urlDecode)
          inplaceURLDecode(item->valueStr.get());
      }
      return item->valueStr.get();
    }
    return NULL;
  }

  void TMTD_FLASHMEM setUrlDecode(bool value) { m_urlDecode = value; }

  // debug
  /*
      void TMTD_FLASHMEM dump()
      {
              for (uint32_t i = 0; i != m_itemsCount; ++i)
              {
                      Item* item = getItem(i);
                      for (KeyIterator k = item->key; k != item->keyEnd; ++k)
                              debug(*k);
                      debug(" = ");
                      for (KeyIterator v = item->value; v!= item->valueEnd; ++v)
                              debug(*v);
                      debug("\r\n");
              }
      }
  //*/

private:
  Item *m_items;
  Item *m_current;
  uint32_t m_itemsCount;
  bool m_urlDecode;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// ObjectDict

template <typename T>
struct ObjectDict {
  struct Item {
    Item *next;
    char const *key;
    char const *keyEnd;
    T value;

    Item(char const *key_, char const *keyEnd_, T const &value_)
        : next(NULL), key(key_), keyEnd(keyEnd_), value(value_) {}
    Item(char const *key_, char const *keyEnd_) : next(NULL), key(key_), keyEnd(keyEnd_) {}
    Item() : next(NULL), key(NULL), keyEnd(NULL) {}
    bool MTD_FLASHMEM operator==(Item const &rhs) {
      return next == rhs.next && key == rhs.key && keyEnd == rhs.keyEnd && value == rhs.value;
    }
    bool MTD_FLASHMEM operator!=(Item const &rhs) { return !(*this == rhs); }
  };

  ObjectDict() : m_items(NULL), m_current(NULL), m_itemsCount(0) {}

  ~ObjectDict() { clear(); }

  void TMTD_FLASHMEM clear() {
    Item *item = m_items;
    while (item) {
      Item *next = item->next;
      delete item;
      item = next;
    }
    m_items = m_current = NULL;
    m_itemsCount = 0;
  }

  void TMTD_FLASHMEM add(char const *key, char const *keyEnd, T const &value) {
    if (m_items) {
      m_current = m_current->next = new Item(key, keyEnd, value);
      ++m_itemsCount;
    } else {
      m_current = m_items = new Item(key, keyEnd, value);
      ++m_itemsCount;
    }
  }

  // same of before, but using default constructed value
  void TMTD_FLASHMEM add(char const *key, char const *keyEnd) {
    if (m_items) {
      m_current = m_current->next = new Item(key, keyEnd);
      ++m_itemsCount;
    } else {
      m_current = m_items = new Item(key, keyEnd);
      ++m_itemsCount;
    }
  }

  // add zero terminated string
  void TMTD_FLASHMEM add(char const *key, T value) { add(key, key + f_strlen(key), value); }

  // add an empty item, returning pointer to the created object value
  T *TMTD_FLASHMEM add(char const *key) {
    add(key, key + f_strlen(key));
    return &(getItem(key)->value);
  }

  uint32_t TMTD_FLASHMEM getItemsCount() { return m_itemsCount; }

  // key stay in RAM or Flash
  Item *TMTD_FLASHMEM getItem(char const *key, char const *keyEnd) {
    // search into local dictionary
    Item *item = m_items;
    while (item) {
      if (t_compare(CharIterator(item->key), CharIterator(item->keyEnd), CharIterator(key), CharIterator(keyEnd)))
        return item; // found
      item = item->next;
    }
    // not found
    return NULL;
  }

  // key stay in RAM or Flash
  Item *TMTD_FLASHMEM getItem(char const *key) { return getItem(key, key + f_strlen(key)); }

  // warn: this doesn't check "index" range!
  Item *TMTD_FLASHMEM operator[](uint32_t index) { return getItem(index); }

  // key can stay in RAM or Flash and must terminate with zero
  Item *TMTD_FLASHMEM operator[](char const *key) { return getItem(key); }

  /*
      void TMTD_FLASHMEM dump()
      {
              Item* item = m_items;
              while (item)
              {
                      debugstrn(item->key, item->keyEnd - item->key);
                      debug(FSTR(" = "));
                      item->value.dump();
                      debug(FSTR("\r\n"));
                      item = item->next;
              }
      }
  */

private:
  Item *m_items;
  Item *m_current;
  uint32_t m_itemsCount;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// FlashDictionary
// Maximum one page (4096 bytes) of storage
// Maximum key length = 254 bytes
// Maximum value length = 65536 bytes (actually less than 4096!)
// All methods which accepts key and/or value allows Flash or RAM storage
//
// Examples:
//   FlashDictionary::setString("name", "Fabrizio");
//   debug("name = %s\r\n", FlashDictionary::getString("name", ""));
//
//   FlashDictionary::setInt("code", 1234);
//   debug("code = %d\r\n", FlashDictionary::getInt("name", 0));
//
// No need to call eraseContent the first time because it is automatically called if MAGIC is not found
// Call eraseContent only if you want to erase old dictionary

struct FlashDictionary {
  static uint32_t const MAGIC = 0x46445631;

  // clear the entire available space and write MAGIC at the beginning of the dictionary
  // This is required only if you want to remove previous content
  // erase all values to 0xFF
  static void eraseContent();

  // requires 4K free heap to execute!
  static void setValue(char const *key, void const *value, uint32_t valueLength);

  // return NULL if key point to a free space (aka key doesn't exist)
  // return pointer may be Unaligned pointer to Flash
  static uint8_t const *getValue(char const *key, uint32_t *valueLength = NULL);

  static void setString(char const *key, char const *value);

  // always returns a Flash stored string (if not found return what contained in defaultValue)
  static char const *getString(char const *key, char const *defaultValue);

  static void setInt(char const *key, int32_t value);

  static int32_t getInt(char const *key, int32_t defaultValue);

  static void setBool(char const *key, bool value);

  static bool getBool(char const *key, bool defaultValue);

  // return starting of specified key or starting of next free position
  // what is stored:
  //   byte: key length (or 0xFF for end of keys, hence starting of free space). Doesn't include ending zero
  //   ....: key string
  //   0x00: key ending zero
  //   word: value length (little endian)
  //   ....: value data
  // automatically erase content if not already initialized
  static void const *findKey(char const *key);

  static bool isContentValid();

  static uint32_t getUsedSpace();
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// FlashFileSystem
// It is just a files extractor from the flash.
// You can write files into the flash using "binarydir.py" (to prepare) and "esptool.py" (to flash) tools.
// For example, having some files in "www" subdirectory you can do:
//   python binarydir.py www webcontent.bin 57344
//   python ../esptool.py --port COM7 write_flash 0x6C000 webcontent.bin
// Then you can use FlashFileSystem static methods to get actual files content
//
// At the top of files flash memory there is following magick:
//   uint32_t: MAGIC = 0x93841A03
// Each file has following structure:
//   file entries:
//     uint8_t:  flags
//         bit 0: 1 = end of files
//  If file exists (bit0 = 0):
//     uint8_t:  filename length including terminating zero
//     uint8_t:  mime type length including terminating zero
//     uint32_t: file content length
//     x-bytes:  filename data + zero
//     x-bytes:  mime type data + zero
//     x-bytes:  raw file data
// All values are little-endian

class FlashFile;

class FlashFileSystem {
  friend class FlashFile;

public:
  struct Item {
    char const *thispos;
    char const *nextpos;
    char const *filename;
    char const *mimetype;
    uint32_t datalength;
    void const *data;

    Item() : nextpos(NULL) {}
    void reset() { nextpos = NULL; }
  };

  static bool getNext(Item *item);
  static bool find(char const *filename, Item *item);
  static bool remove(char const *filename);
  static uint32_t getFreeSpace();

  static uint32_t getTotalSpace() { return getBeginOfSDKSettings() - FLASHFILESYSTEM_PTR; }

protected:
  static char const *getFreePos();

private:
  static uint32_t const MAGIC = 0x93841A03;

  static char const *getBase();
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// FlashFile
//
// Example:
//
// FlashFile file;
// file.create(FSTR("test.txt"), STR_TEXTPLAIN);
// file.write(FSTR("This is a test file\r\n"));
// file.write(FSTR("Second line\r\n"));
// file.write(FSTR("Ending line\r\n"));
// file.close();    // not really necessary if FlashFile destructor has been called
// Now you can open http://192.168.4.1/test.txt

class FlashFile {
public:
  FlashFile();
  ~FlashFile();
  void create(char const *filename, char const *mimetype);
  bool write(void const *data, uint32_t size);
  bool write(char const *string);
  void close();

private:
  char const *m_startPosition;
  uint32_t m_headerLength;
  FlashWriter m_writer;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// StringList
// A list of zero terminated strings
// If storage is Heap then the string is allocated into the heap

class StringList {
public:
  enum Storage { Reference, Heap };

  StringList();
  ~StringList();

  void add(char const *string, Storage storage);
  void insert(uint32_t position, char const *string, Storage storage);
  void remove(uint32_t position);
  int32_t indexof(const char *string);
  void clear();
  uint32_t size();
  char const *getItem(uint32_t position);

private:
  void freeItem(uint32_t position);

  struct Item {
    Storage storage;
    char const *ptr;

    Item(Storage storage_, char const *ptr_) : storage(storage_), ptr(ptr_) {}
  };

  Vector<Item> m_items;
};
}

#endif