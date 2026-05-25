#pragma once

#include "hardware/regs/xip.h"
#include "hardware/structs/xip.h"
#include "pico/stdlib.h"
#include <cstdio>
#include <cstdlib>

namespace psram {

const static size_t BASE_ADDRESS = 0x11'000'000;
const static size_t BASE_ADDRESS_UNCACHED = 0x15'000'000;
const static size_t SIZE = (8 * 1024 * 1024);

void init(uint32_t pin) {
  gpio_set_function(pin, GPIO_FUNC_XIP_CS1);
  xip_ctrl_hw->ctrl |= XIP_CTRL_WRITABLE_M1_BITS;
}

struct Block {
  size_t space;
  Block *next;
  bool free;
};
static Block *entry = (Block *)BASE_ADDRESS;
void init_heap() {
  entry->space = SIZE - sizeof(Block);
  entry->next = nullptr;
  entry->free = true;
}

void *ps_malloc(size_t size) {
  Block *block = entry;
  while (block != nullptr) {
    if (!block->free || block->space < size) {
      block = block->next;
      continue;
    }

    // pointer where memory region begins
    auto *ptr = (uint8_t *)block + sizeof(Block);

    // space can hold size and another block with at least 1 byte
    // block can be inserted
    if (block->space >= (size + sizeof(Block) + 1)) {
      auto *next_block = (Block *)(ptr + size);

      next_block->next = block->next;
      block->next = next_block;

      next_block->space = block->space - (size + sizeof(Block));
      block->space = size;

      next_block->free = true;
    }
    block->free = false;

    return ptr;
  }
  // no block found
  return nullptr;
}

void ps_free(void *ptr) {
  Block *prev = nullptr;
  Block *block = entry;
  while (block != nullptr) {

    auto *data = (void *)(block + 1);
    if (data != ptr) {
      prev = block;
      block = block->next;
      continue;
    }

    block->free = true;
    if (block->next != nullptr && block->next->free) {
      // join with next block
      block->space += block->next->space + sizeof(Block);
      block->next = block->next->next;
    }
    if (prev != nullptr && prev->free) {
      // join with previous
      prev->space += block->space + sizeof(Block);
      prev->next = block->next;
    }
    break;
  }
}

bool is_psram(void *ptr) {
  auto addr = (uintptr_t)ptr;
  return addr >= BASE_ADDRESS && addr < BASE_ADDRESS + SIZE;
}

void probe(bool show_data) {
  Block *block = entry;
  while (block != nullptr) {
    printf("Block %p: data: %p, size: %d, free: %u, next: %p\n", block,
           block + 1, block->space, block->free, block->next);

    if (show_data && block->next != nullptr) {
      auto *data = (uint8_t *)block + sizeof(Block);

      printf("bytes: ");
      for (size_t i = 0; i < block->space; i++) {
        printf("%u", data[i]);
        if (i != block->space - 1)
          printf(",");
      }
      printf("\n");
    }

    block = block->next;
  }
}

}; // namespace psram
