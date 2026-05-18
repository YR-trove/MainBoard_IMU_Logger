/*
 * Memory_operations.h
 *
 *  Created on: Mar 27, 2024
 *      Author: alice
 */

#ifndef INC_MEMORY_OPERATIONS_H_
#define INC_MEMORY_OPERATIONS_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "main.h"
#include "SPI.h"
#include "SPI_NAND.h"

/*
 * Packet layout per sample (37 bytes):
 *   [0]      hh  (timestamp hours)
 *   [1]      mm  (timestamp minutes)
 *   [2]      ss  (timestamp seconds)
 *   [3..4]   sss (milliseconds, little-endian uint16)
 *   [5..10]  accelerometer XYZ (6 bytes raw, LSB first per axis)
 *   [11..16] gyroscope     XYZ (6 bytes raw, LSB first per axis)
 *   [17..24] light spectral 4x4 filters (F1..F8: 8 bytes, 4 selected filters
 *             from low SMUX group and 4 from high SMUX group, uint8 LSB/MSB
 *             pairs, see host-side parser for channel mapping)
 *   [25..26] light Clear channel (2 bytes, LSB first uint16)
 *   [27..28] light NIR   channel (2 bytes, LSB first uint16)
 *   [29..30] flicker frequency estimate in Hz (uint16, 0, 1, 100 or 120)
 *   [31..36] reserved for future use / alignment (currently zeroed)
 */
#define BYTES_PER_SAMPLE 37
#define SAMPLES_PER_PAGE (4096 / BYTES_PER_SAMPLE)

typedef struct bookmark
{
  uint16_t blocco_scritto;
  uint8_t pagina_scritta;
  int b;

}NAND_info;

typedef struct Time
{
  uint8_t hh;
  uint8_t mm;
  uint8_t ss;
  uint16_t sss;
} Time_Struct;

void find_bad_blocks(uint16_t *bad_blocks);
void erase_good_blocks(uint8_t *bad_blocks);
NAND_info read_memory(int b, NAND_info indice, uint16_t *blocco_letto, uint8_t *pagina_letta, uint16_t bad_blocks[2048], uint8_t *data_letto);
void write_info(NAND_info segnalibro, uint16_t bad_blocks[2048]);
NAND_info read_info(uint16_t bad_blocks[2048]);

void write_packet(uint16_t sample, Time_Struct timestamp,
                  uint8_t *accelerometer, uint8_t *gyroscope,
                  uint8_t *light_raw,
                  uint8_t *NAND_packet);

void erase_memory(void);
void write_memory(void);
void read_memory_and_transmit(void);

#endif /* INC_MEMORY_OPERATIONS_H_ */
