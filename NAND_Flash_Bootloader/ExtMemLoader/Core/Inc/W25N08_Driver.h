/*
 * W25N08_Driver.h
 *
 *  Created on: 2026/08/29
 *      Author: Hiroyuki Yamada (Y-Logic)
 */

#ifndef INC_W25N08_DRIVER_H_
#define INC_W25N08_DRIVER_H_

#include <stdint.h>

// Insert write enable before write and erase operation
#ifndef NAND_INSERT_WRITE_ENABLE
#define NAND_INSERT_WRITE_ENABLE 1
#endif

// Call diagnosis log
#define NAND_CALL_DIAGNOSIS_LOG_ENABLE 0

// DMA, Polling or Interrupt
#ifndef NAND_ACCESS_MODE_DMA
#define NAND_ACCESS_MODE_DMA 0
#endif
#ifndef NAND_ACCESS_MODE_INTERRUPT
#define NAND_ACCESS_MODE_INTERRUPT 0
#endif
// when both 0, polling mode used

// Return value (returns HAL_OK if okay)
#define NAND_RESULT_ECC_ERROR				0x82
#define NAND_RESULT_ECC_DETECTED			0x81
#define NAND_RESULT_ECC_EXCEED_THRESHOLD	0x83

// NAND Flash Parameter for W25N08LW

#define NAND_PAGE_SIZE		4096 // bytes
#define NAND_SPARE_SIZE		(128) // without parity
#define NAND_PAGE_SIZE_ALL	(4096+NAND_SPARE_SIZE)
#define NAND_BLOCK_SIZE		64 // pages
#define NAND_BLOCK_COUNT	2048 // blocks
#define NAND_ROM_SIZE		(NAND_BLOCK_COUNT * NAND_BLOCK_SIZE * NAND_PAGE_SIZE)

#define NAND_MFR_ID			0xEF
#define NAND_DEVICE_ID		0xB324

// For write protect
#define NAND_WRITE_PROTECT_ALL_LOCKED 	0x7C // TB=1, BP=0xF
#define NAND_WRITE_PROTECT_NOT_LOCKED	0x04 // TB=1, BP=0x0
#define NAND_WRITE_PROTECT_LOWER_512KB	((1<<3) | 0x04) // TB=1, BP=0x0
#define NAND_WRITE_PROTECT_LOWER_1MB	((2<<3) | 0x04)  // TB=1, BP=0x1
#define NAND_WRITE_PROTECT_LOWER_2MB	((3<<3) | 0x04)  // TB=1, BP=0x2
#define NAND_WRITE_PROTECT_LOWER_4MB	((4<<3) | 0x04)  // TB=1, BP=0x3
#define NAND_WRITE_PROTECT_LOWER_8MB	((5<<3) | 0x04)  // TB=1, BP=0x4
#define NAND_WRITE_PROTECT_LOWER_16MB	((6<<3) | 0x04)  // TB=1, BP=0x5
#define NAND_WRITE_PROTECT_LOWER_32MB	((7<<3) | 0x04)  // TB=1, BP=0x6
#define NAND_WRITE_PROTECT_LOWER_64MB	((8<<3) | 0x04)  // TB=1, BP=0x7
#define NAND_WRITE_PROTECT_LOWER_128MB	((9<<3) | 0x04)  // TB=1, BP=0x8
#define NAND_WRITE_PROTECT_LOWER_256MB	((10<<3) | 0x04)  // TB=1, BP=0x9
#define NAND_WRITE_PROTECT_LOWER_512MB	((11<<3) | 0x04)  // TB=1, BP=0xA
#define NAND_WRITE_PROTECT_HIGHER_512KB	((1<<3) | 0x0) // TB=0, BP=0x0
#define NAND_WRITE_PROTECT_HIGHER_1MB	((2<<3) | 0x0)  // TB=0, BP=0x1
#define NAND_WRITE_PROTECT_HIGHER_2MB	((3<<3) | 0x0)  // TB=0, BP=0x2
#define NAND_WRITE_PROTECT_HIGHER_4MB	((4<<3) | 0x0)  // TB=0, BP=0x3
#define NAND_WRITE_PROTECT_HIGHER_8MB	((5<<3) | 0x0)  // TB=0, BP=0x4
#define NAND_WRITE_PROTECT_HIGHER_16MB	((6<<3) | 0x0)  // TB=0, BP=0x5
#define NAND_WRITE_PROTECT_HIGHER_32MB	((7<<3) | 0x0)  // TB=0, BP=0x6
#define NAND_WRITE_PROTECT_HIGHER_64MB	((8<<3) | 0x0)  // TB=0, BP=0x7
#define NAND_WRITE_PROTECT_HIGHER_128MB	((9<<3) | 0x0)  // TB=0, BP=0x8
#define NAND_WRITE_PROTECT_HIGHER_256MB	((10<<3) | 0x0)  // TB=0, BP=0x9
#define NAND_WRITE_PROTECT_HIGHER_512MB	((11<<3) | 0x0)  // TB=0, BP=0xA


// Status Register 読み出し
HAL_StatusTypeDef NAND_ReadRegister(uint8_t reg_addr, uint8_t* reg_val);
// Status Register 書き込み
HAL_StatusTypeDef NAND_WriteRegister(uint8_t reg_addr, uint8_t reg_val);

// Set new value to write protect
#define NAND_SetWriteProtect(val) NAND_WriteRegister (0xA0, val); // Disable Write Protect

// Set or reset flag of write enable
// Note: Write enable turns to disable state every time when page write or erase operated.
HAL_StatusTypeDef NAND_WriteEnable();
HAL_StatusTypeDef NAND_WriteDisable();
// BUSY 解除待ち
HAL_StatusTypeDef NAND_WaitBusy();

// ページリード（Page Address からデータバッファを経由して RAM へ転送）
// Fast Read Quad I/O (0xEB) を使用
HAL_StatusTypeDef NAND_ReadPage(uint32_t page_addr, uint16_t column_addr, uint8_t *pData, uint32_t size);

// Fast Read (0x0B)
HAL_StatusTypeDef NAND_ReadPage_SingleIO(uint32_t page_addr, uint16_t column_addr, uint8_t *pData, uint32_t size);
// Fast Read Quad
HAL_StatusTypeDef NAND_ReadPage_QuadOutput(uint32_t page_addr, uint16_t column_addr, uint8_t *pData, uint32_t size);

// Continuous Read モードの有効化 (enable=1 -> BUF=0, enable=0 -> BUF=1)
HAL_StatusTypeDef NAND_EnableContinuousRead(uint8_t enable);

// 1: continuous mode, 0: buffer mode, -1: error
int NAND_IsContinuousReadMode();

// Returns 1 if block is bad
int NAND_IsBlockBad(uint16_t block_addr);


// BBM LUT に Bad Block 置換ペアを登録
HAL_StatusTypeDef NAND_WriteBBM_LUT(uint16_t bad_lba, uint16_t good_pba);

// 内蔵 LUT テーブル（全エントリ）の取得
HAL_StatusTypeDef NAND_ReadBBM_LUT(uint16_t *pLbaBuffer, uint16_t *pPbaBuffer, uint8_t max_entries, uint8_t bankGroup);

typedef enum {
    NAND_ECC_OK                  = 0,
    NAND_ECC_CORRECTED           = 1,
    NAND_ECC_UNCORRECTABLE       = 2,
    NAND_ECC_CORRECTED_EXCEED    = 3,
	ECC_RETURN_ERROR
} NAND_ECC_StatusTypeDef;

// Check ECC Status
NAND_ECC_StatusTypeDef NAND_CheckECC_Status(void);


// ブロック消去
// - block_addr: block address by 4k block
HAL_StatusTypeDef NAND_EraseBlock(uint32_t block_addr);

// ページライト
HAL_StatusTypeDef NAND_WritePage(uint32_t page_addr, uint16_t column_addr, const uint8_t *pData, uint16_t size);

// データ読み込みなしでページコピー
HAL_StatusTypeDef NAND_CopyPage(uint32_t page_addr_src, uint32_t page_addr_dst);


#if NAND_CALL_DIAGNOSIS_LOG_ENABLE
void NAND_ShowCallDiagnosis();
void NAND_ResetCallDiagnosis();
#endif

#endif /* INC_W25N08_DRIVER_H_ */
