/*
 * W959D8_Driver.h
 *
 *  Created on: 2026/09/27
 *      Author: Hiroyuki Yamada (Y-Logic)
 */

#ifndef INC_W959D8_DRIVER_H_
#define INC_W959D8_DRIVER_H_


/* HyperRAM レジスタ読み出し (16bit) */
HAL_StatusTypeDef HyperRAM_ReadRegister(uint32_t reg_addr, uint16_t *pData);

/* HyperRAM レジスタ書き込み (16bit) */
HAL_StatusTypeDef HyperRAM_WriteRegister(uint32_t reg_addr, uint16_t data);

/* メモリマップトモードにスイッチ */
HAL_StatusTypeDef HyperRAM_SwitchToMemoryMappedMode();

/* 間接モードにスイッチ */
HAL_StatusTypeDef HyperRAM_SwitchToIndirectMode();

#endif /* INC_W959D8_DRIVER_H_ */
