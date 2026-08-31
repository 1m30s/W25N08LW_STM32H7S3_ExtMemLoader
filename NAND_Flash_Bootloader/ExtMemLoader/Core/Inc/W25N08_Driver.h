/*
 * W25N08_Driver.h
 *
 *  Created on: 2026/08/29
 *      Author: Hiroyuki Yamada (Y-Logic)
 */

#ifndef INC_W25N08_DRIVER_H_
#define INC_W25N08_DRIVER_H_


// Status Register 読み出し
HAL_StatusTypeDef NAND_ReadRegister(uint8_t reg_addr, uint8_t* reg_val);
// Status Register 書き込み
HAL_StatusTypeDef NAND_WriteRegister(uint8_t reg_addr, uint8_t reg_val);

HAL_StatusTypeDef NAND_WriteEnable(void);
// BUSY 解除待ち
HAL_StatusTypeDef NAND_WaitBusy(void);
// ページリード（Page Address からデータバッファを経由して RAM へ転送）
HAL_StatusTypeDef NAND_ReadPage(uint32_t page_addr, uint16_t column_addr, uint8_t *pData, uint32_t size);
// Continuous Read モードの有効化 (BUF=0)
HAL_StatusTypeDef NAND_EnableContinuousRead(void);

// Returns 1 if block is bad
int NAND_IsBlockBad(uint16_t block_addr);

void NAND_ScanBadBlocks(void);

// BBM LUT に Bad Block 置換ペアを登録
HAL_StatusTypeDef NAND_WriteBBM_LUT(uint16_t bad_lba, uint16_t good_pba);

// 内蔵 LUT テーブル（全エントリ）の取得
HAL_StatusTypeDef NAND_ReadBBM_LUT(uint16_t *pLbaBuffer, uint16_t *pPbaBuffer, uint8_t max_entries);

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
HAL_StatusTypeDef NAND_EraseBlock(uint32_t page_addr);

// ページライト
HAL_StatusTypeDef NAND_WritePage(uint32_t page_addr, uint16_t column_addr, const uint8_t *pData, uint16_t size);

#endif /* INC_W25N08_DRIVER_H_ */
