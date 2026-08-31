/*
 * W959D8NF_Extmem_Driver.h
 *
 *  Created on: 2026/08/30
 *      Author: Hiroyuki Yamada (Y-Logic)
 *
 *  This file is modified from stm32_W959D8NF.h, which is derived from
 *  stm32-mw-extmem-mgr v1.6.0
 *  https://github.com/STMicroelectronics/stm32-mw-extmem-mgr
 *
 */

#ifndef INC_W959D8NF_EXTMEM_DRIVER_H_
#define INC_W959D8NF_EXTMEM_DRIVER_H_


#include "stm32_extmem.h"
#include "stm32_extmem_conf.h"
#if defined(EXTMEM_DRIVER_CUSTOM) && (EXTMEM_DRIVER_CUSTOM == 1)
#include "stm32_custom_driver_api.h"
#include "stm32_custom_driver_type.h"

// Configuration 0 - Die 0
#define EXTMEM_W959D8NF_CMD1_ADDR_CFGR           0x1000    /*!< Address of the register */
#define EXTMEM_W959D8NF_CMD1_VALUE_CFGR          0x7000    /*!< Value of the register [14-12] = 110b (19 ohms) */
#define EXTMEM_W959D8NF_CMD1_MASK_CFGR           0x7000    /*!< Mask for the register [14-12] */

// Configuration 0 - Die 1
#define EXTMEM_W959D8NF_CMD1B_ADDR_CFGR          0x02001000    /*!< Address of the register */

#define EXTMEM_W959D8NF_INIT { \
	   .MemType                                               = EXTMEM_CUSTOM_HYPERRAM,                /*!< Common field for memory type        */ \
	   .MemStandard                                           = EXTMEM_CUSTOM_STD_HYPERBUS,            /*!< Hyperbus */ \
	   .MemSize                                               = EXTMEM_CUSTOM_SIZE_512MB,              /*!< *Common field for memory Size        */ \
	   .MemChipSelectHighTimeCycle                            = 1,                                     /*!< Common field for memory Chip select high time */ \
	   .StartupConfig.Frequency                               = 50000000,                              /*!< *Operating frequency in Hz (50MHz)*/ \
	   .StartupConfig.AccessMode                              = EXTMEM_CUSTOM_8D_8D_8D,               /*!< *Access mode (Note: This PSRAM is 8bit IF) */ \
	   .StartupConfig.AddressSize                             = EXTMEM_CUSTOM_ADDRESS_32_BITS,         /*!< Size of the address */ \
	   .StartupConfig.DqsMode                                 = EXTMEM_CUSTOM_DQS_ENABLE,              /*!< Data strobe mode */ \
	   .NbRegisterConfig                                      = 3,                                     /*!< Common field for memory Chip select */ \
	   .RegisterConfig[0].ConfigStepType                      = EXTMEM_CUSTOM_CFGSTEP_HB_RWR_REG_ADDR, /*!< Config step to read/write/read a register */ \
	   .RegisterConfig[0].RWConfigStep.RegisterAddress        = EXTMEM_W959D8NF_CMD1_ADDR_CFGR,        /*!< Address of the register */ \
	   .RegisterConfig[0].RWConfigStep.RegisterValue          = EXTMEM_W959D8NF_CMD1_VALUE_CFGR,       /*!< Value of the register */ \
	   .RegisterConfig[0].RWConfigStep.RegisterMask           = EXTMEM_W959D8NF_CMD1_MASK_CFGR,        /*!< Mask for the register */ \
		.RegisterConfig[1].ConfigStepType                      = EXTMEM_CUSTOM_CFGSTEP_HB_RWR_REG_ADDR, /*!< * Config step to read/write/read a register */ \
		.RegisterConfig[1].RWConfigStep.RegisterAddress        = EXTMEM_W959D8NF_CMD1B_ADDR_CFGR,        /*!< Address of the register */ \
		.RegisterConfig[1].RWConfigStep.RegisterValue          = EXTMEM_W959D8NF_CMD1_VALUE_CFGR,       /*!< Value of the register */ \
		.RegisterConfig[1].RWConfigStep.RegisterMask           = EXTMEM_W959D8NF_CMD1_MASK_CFGR,        /*!< Mask for the register */ \
	   .RegisterConfig[2].ConfigStepType                      = EXTMEM_CUSTOM_CFGSTEP_EXEC_OPT_CFG,    /*!< Config step to use new Optional configuration */ \
	   .OptionalConfig.Frequency                              = 100000000,                             /*!< *Operating frequency in Hz (100MHz)*/ \
	   .HyperRamConfig.HyperbusConfig.RwRecoveryTimeCycle     = 7,                                     /*!< *Read/Write recovery time cycle */ \
	   .HyperRamConfig.HyperbusConfig.AccessTimeCycle         = 7,                                     /*!< *Access time cycle */ \
	   .HyperRamConfig.HyperbusConfig.WriteZeroLatency        = EXTMEM_CUSTOM_WR_LATENCY_ON,           /*!< Write zero latency */ \
	   .HyperRamConfig.HyperbusConfig.LatencyMode             = EXTMEM_CUSTOM_FIXED_LATENCY,           /*!< Latency mode */ \
	   .HyperRamConfig.RamConfig.WrapSize                     = EXTMEM_CUSTOM_WRAP_NOT_SUPPORTED,      /*!< Wrap mode support */ \
	   .HyperRamConfig.RamConfig.CsBoundarySize               = EXTMEM_CUSTOM_CS_BONDARYOF_NONE,       /*!< Chip Select boundary size */ \
	   .HyperRamConfig.RamConfig.Refresh                      = 0,                                     /*!< Refresh setting (units/controller-dependent) */ \
	 }

#if defined(EXTMEM_C)
EXTMEM_DRIVER_CUSTOM_ObjectTypeDef extmem_W959D8NF = EXTMEM_W959D8NF_INIT;
#else
extern EXTMEM_DRIVER_CUSTOM_ObjectTypeDef extmem_W959D8NF;
#endif /* EXTMEM_C */

#endif

#endif /* INC_W959D8NF_EXTMEM_DRIVER_H_ */
