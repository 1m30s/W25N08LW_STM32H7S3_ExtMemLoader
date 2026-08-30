/*
 * W25N08_Driver.c
 *
 *  Created on: 2026/08/29
 *      Author: Hiroyuki Yamada (Y-Logic)
 */




#include "stm32h7rsxx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "extmem_manager.h"
#include "W25N08_Driver.h"


extern XSPI_HandleTypeDef hxspi2;

#define NAND_CMD_PAGE_READ          0x13
#define NAND_CMD_READ_BUFFER_QUAD   0xEB
#define NAND_CMD_READ_STATUS_REG    0x0F

// Winbond W25N08LW コマンド定義
#define NAND_CMD_READ_STATUS_REG    0x0F  // Read Status / Configuration Register
#define NAND_CMD_WRITE_STATUS_REG   0x1F  // Write Status / Configuration Register (0x01も可)
#define NAND_CMD_WRITE_ENABLE       0x06  // Write Enable (WEL=1)

// レジスタアドレス定義
#define NAND_REG_PROTECTION         0xA0  // Status Register-1 (Protection)
#define NAND_REG_CONFIG             0xB0  // Status Register-2 (Configuration)
#define NAND_REG_STATUS_3           0xC0  // Status Register-3 (Status)


// Status Register 読み出し
HAL_StatusTypeDef NAND_ReadStatus(uint8_t reg_addr, uint8_t* reg_val)
{
	XSPI_RegularCmdTypeDef sCommand =
	{ 0 };
	sCommand.Instruction = NAND_CMD_READ_STATUS_REG;
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.Address = reg_addr;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_8_BITS;
	sCommand.DataMode = HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength = 1;

	if (HAL_XSPI_Command(&hxspi2, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) == HAL_OK)
	{
		return HAL_XSPI_Receive(&hxspi2, reg_val, HAL_XSPI_TIMEOUT_DEFAULT_VALUE);
	}
	else return HAL_ERROR;
}
// Status Register 書き込み
HAL_StatusTypeDef NAND_WriteStatus(uint8_t reg_addr, uint8_t reg_val)
{
    XSPI_RegularCmdTypeDef sCommand = {0};

    sCommand.OperationType   = HAL_XSPI_OPTYPE_COMMON_CFG;
    sCommand.Instruction     = NAND_CMD_WRITE_STATUS_REG;  // 0x1F
    sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
    sCommand.Address         = reg_addr;                   // レジスタアドレス
    sCommand.AddressMode     = HAL_XSPI_ADDRESS_1_LINE;
    sCommand.AddressWidth    = HAL_XSPI_ADDRESS_8_BITS;
    sCommand.DataMode        = HAL_XSPI_DATA_1_LINE;
    sCommand.DataLength      = 1;                          // 1 バイト書き込み

    if (HAL_XSPI_Command(&hxspi2, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_XSPI_Transmit(&hxspi2, &reg_val, HAL_XSPI_TIMEOUT_DEFAULT_VALUE);
}

HAL_StatusTypeDef NAND_WriteEnable(void)
{
    XSPI_RegularCmdTypeDef sCommand = {0};

    sCommand.OperationType   = HAL_XSPI_OPTYPE_COMMON_CFG;
    sCommand.Instruction     = NAND_CMD_WRITE_ENABLE;      // 0x06
    sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
    sCommand.AddressMode     = HAL_XSPI_ADDRESS_NONE;      // アドレスなし
    sCommand.DataMode        = HAL_XSPI_DATA_NONE;         // データ転送なし

    return HAL_XSPI_Command(&hxspi2, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE);
}

// BUSY 解除待ち
HAL_StatusTypeDef NAND_WaitBusy(void)
{
	uint32_t timeout = HAL_GetTick() + 100;
	while (HAL_GetTick() < timeout)
	{
		uint8_t sr3 = 0;
		if(NAND_ReadStatus(NAND_REG_STATUS_3, &sr3) != HAL_OK) return HAL_ERROR;

		if ((sr3 & 0x01) == 0) return HAL_OK; // Bit 0: BUSY = 0
	}
	return HAL_TIMEOUT;
}

// ページリード（Page Address からデータバッファを経由して RAM へ転送）
HAL_StatusTypeDef NAND_ReadPage(uint32_t page_addr, uint16_t column_addr, uint8_t *pData, uint16_t size)
{
	XSPI_RegularCmdTypeDef sCommand =
	{ 0 };

	// Step 1: Page Data Read (0x13) - セルからバッファへ転送
	sCommand.Instruction = NAND_CMD_PAGE_READ;
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.Address = page_addr;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;

	if (HAL_XSPI_Command(&hxspi2, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return HAL_ERROR;

	// Step 2: BUSY 完了待ち
	if (NAND_WaitBusy() != HAL_OK) return HAL_ERROR;

	// Step 3: Fast Read Quad I/O (0xEB) - バッファから MCU へデータ読み出し
	memset(&sCommand, 0, sizeof(sCommand));
	sCommand.Instruction = NAND_CMD_READ_BUFFER_QUAD;
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.Address = column_addr;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_4_LINES;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_16_BITS;
	sCommand.DummyCycles = 4; // 0xEB: 4 dummy cycles
	sCommand.DataMode = HAL_XSPI_DATA_4_LINES;
	sCommand.DataLength = size;

	if (HAL_XSPI_Command(&hxspi2, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return HAL_ERROR;
	return HAL_XSPI_Receive(&hxspi2, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE);
}

#define NAND_REG_CONFIG       0xB0  // Status Register 2 (Configuration)
#define NAND_CFG_BUF_MASK     (1 << 3) // Bit 3: BUF (1: Buffer Mode, 0: Continuous Read)

// Continuous Read モードの有効化 (BUF=0)
HAL_StatusTypeDef NAND_EnableContinuousRead(void)
{
	uint8_t status_reg2 = 0;
	if(NAND_ReadStatus(NAND_REG_CONFIG, &status_reg2) != HAL_OK) return HAL_ERROR; //

	// BUF ビットを 0 にクリア
	status_reg2 &= ~NAND_CFG_BUF_MASK;

	// Status Register-2 に書き戻し (0x1F コマンドを使用)
	return NAND_WriteStatus(NAND_REG_CONFIG, status_reg2); //
}

// Continuous Read による複数ページ（シームレス）読み出し
HAL_StatusTypeDef NAND_ReadContinuous(uint32_t start_page_addr, uint8_t *pData, uint32_t total_size)
{
	// 1. 事前に Page Data Read (0x13) で開始ページをセルからバッファへロード
	XSPI_RegularCmdTypeDef sCommand =
	{ 0 };
	sCommand.Instruction = NAND_CMD_PAGE_READ;
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.Address = start_page_addr;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;

	if (HAL_XSPI_Command(&hxspi2, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return HAL_ERROR;
	if (NAND_WaitBusy() != HAL_OK) return HAL_ERROR;

	// 2. Continuous Read コマンド発行 (0x6B: Fast Read Quad Output などを推奨)
	memset(&sCommand, 0, sizeof(sCommand));
	sCommand.Instruction = 0x6B; // Fast Read Quad Output
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.Address = 0x0000; // Column Address = 0 から開始
	sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_16_BITS;
	sCommand.DummyCycles = 8; // 0x6B: 8 dummy cycles (BUF=0 時)
	sCommand.DataMode = HAL_XSPI_DATA_4_LINES;
	sCommand.DataLength = total_size; // 全データサイズ分を一括指定

	if (HAL_XSPI_Command(&hxspi2, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return HAL_ERROR;
	return HAL_XSPI_Receive(&hxspi2, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE);
}


NAND_ECC_StatusTypeDef NAND_CheckECC_Status(void)
{
    uint8_t sr1 = 0;
    if( NAND_ReadStatus(0xC0, &sr1) != HAL_OK) return ECC_RETURN_ERROR; // Status Register 1 取得

    uint8_t ecc_bits = (sr1 >> 4) & 0x03; // Bit 5:4 (ECC1, ECC0)
    return (NAND_ECC_StatusTypeDef) ecc_bits;
}
#define NAND_TOTAL_BLOCKS   4096 // 8Gb = 4096 ブロック (2 dies x 2048)
#define NAND_PAGES_PER_BLOCK 64  // 1ブロック = 64ページ

static uint8_t g_BadBlockTable[NAND_TOTAL_BLOCKS / 8]; // RAM上にビットマップ形式で保持

int NAND_IsBlockBad(uint16_t block_addr)
{
	uint8_t spare_buf[4];
	uint32_t page_addr = (uint32_t) block_addr * NAND_PAGES_PER_BLOCK;

	// Page 0 の Main 先頭(Column 0) または Spare 先頭(Column 4096) を読み出す
	NAND_ReadPage(page_addr, 4096, spare_buf, 2); // Spare 先頭チェック

	if (spare_buf[0] != 0xFF)
	{
		return 1; // Bad Block
	}
	return 0; // Good Block
}

void NAND_ScanBadBlocks(void)
{
	memset(g_BadBlockTable, 0, sizeof(g_BadBlockTable));

	for (uint16_t blk = 0; blk < NAND_TOTAL_BLOCKS; blk++)
	{
		if (NAND_IsBlockBad(blk))
		{
			g_BadBlockTable[blk / 8] |= (1 << (blk % 8)); // Bad Block ビットを立てる
		}
	}
}
#define NAND_CMD_WRITE_LUT  0xA1 // Swap Blocks
#define NAND_CMD_READ_LUT   0xA5 // Read BBM LUT

// BBM LUT に Bad Block 置換ペアを登録
HAL_StatusTypeDef NAND_WriteBBM_LUT(uint16_t bad_lba, uint16_t good_pba)
{
	if (bad_lba == good_pba)
	{
		return HAL_ERROR; // 同一LBAは指定不可
	}

	// 2-die 構成の Block Group 同一判定チェック
	uint8_t lba_group = (bad_lba >= 2048) ? 1 : 0;
	uint8_t pba_group = (good_pba >= 2048) ? 1 : 0;
	if (lba_group != pba_group)
	{
		return HAL_ERROR; // 異なるダイ間での置換は仕様違反
	}

	// Write Enable (0x06) 発行
	NAND_WriteEnable();

	XSPI_RegularCmdTypeDef sCommand =
	{ 0 };
	uint8_t lut_data[4];

	// LBA と PBA を 16-bit ビッグエンディアンで設定
	lut_data[0] = (bad_lba >> 8) & 0xFF;  // LBA [15:8]
	lut_data[1] = bad_lba & 0xFF;         // LBA [7:0]
	lut_data[2] = (good_pba >> 8) & 0xFF; // PBA [15:8]
	lut_data[3] = good_pba & 0xFF;        // PBA [7:0]

	sCommand.Instruction = NAND_CMD_WRITE_LUT;
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.DataMode = HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength = 4;

	if (HAL_XSPI_Command(&hxspi2, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return HAL_ERROR;
	if (HAL_XSPI_Transmit(&hxspi2, lut_data, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return HAL_ERROR;

	return NAND_WaitBusy();
}

// 内蔵 LUT テーブル（全エントリ）の取得
HAL_StatusTypeDef NAND_ReadBBM_LUT(uint16_t *pLbaBuffer, uint16_t *pPbaBuffer, uint8_t max_entries)
{
	XSPI_RegularCmdTypeDef sCommand =
	{ 0 };
	uint8_t raw_lut[160]; // 最大 40 entries x 4 bytes = 160 bytes

	sCommand.Instruction = NAND_CMD_READ_LUT;
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.DummyCycles = 8; // 8 dummy clocks
	sCommand.DataMode = HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength = max_entries * 4;

	if (HAL_XSPI_Command(&hxspi2, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return HAL_ERROR;
	if (HAL_XSPI_Receive(&hxspi2, raw_lut, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return HAL_ERROR;

	for (uint8_t i = 0; i < max_entries; i++)
	{
		pLbaBuffer[i] = (raw_lut[i * 4] << 8) | raw_lut[i * 4 + 1];
		pPbaBuffer[i] = (raw_lut[i * 4 + 2] << 8) | raw_lut[i * 4 + 3];
	}
	return HAL_OK;
}
