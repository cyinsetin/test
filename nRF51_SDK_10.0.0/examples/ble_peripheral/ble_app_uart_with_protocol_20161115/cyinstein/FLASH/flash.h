
#ifndef __FALSH_H
#define __FALSH_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


void flash_page_erase(uint32_t * page_address);
void flash_word_write(uint32_t * address, uint32_t value);

#endif