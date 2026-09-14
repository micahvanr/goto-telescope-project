#ifndef PWR_H
#define PWR_H

#include "stm32f4xx.h"

//======================================================================================//}
//                  Address Definitions
//======================================================================================//{

typedef enum {
    PWR_BASE_ADDR = ((APB1_BASE_ADDR) + (0x7000U))
} pwr_base_addr_e;

//======================================================================================//}
//                  Peripheral Constants
//======================================================================================//{

//======================================================================================//}
//                  Register Constants
//======================================================================================//{

// clang-format off
typedef enum : uint32_t {
    PWR_CR_FPDS_POS = 9U,   // Flash power down in Stop mode
    PWR_CR_DBP_POS  = 8U,   // Disable backup domain write protection
    PWR_CR_PLS_POS  = 5U,   // PVD level selection
    PWR_CR_PVDE_POS = 4U,   // Power voltage detector enable
    PWR_CR_CSBF_POS = 3U,   // Clear standby flag
    PWR_CR_CWUF_POS = 2U,   // Clear wakeup flag
    PWR_CR_PDDS_POS = 1U,   // Power down deepsleep
    PWR_CR_LPDS_POS = 0U,   // Low-power deep sleep
} pwr_cr_pos_e;

typedef enum : uint32_t {
    PWR_CR_FPDS = (1U << PWR_CR_FPDS_POS),   // Flash power down in Stop mode
    PWR_CR_DBP  = (1U << PWR_CR_DBP_POS),    // Disable backup domain write protection
    PWR_CR_PLS  = (1U << PWR_CR_PLS_POS),    // PVD level selection
    PWR_CR_PVDE = (1U << PWR_CR_PVDE_POS),   // Power voltage detector enable
    PWR_CR_CSBF = (1U << PWR_CR_CSBF_POS),   // Clear standby flag
    PWR_CR_CWUF = (1U << PWR_CR_CWUF_POS),   // Clear wakeup flag
    PWR_CR_PDDS = (1U << PWR_CR_PDDS_POS),   // Power down deepsleep
    PWR_CR_LPDS = (1U << PWR_CR_LPDS_POS),   // Low-power deep sleep
} pwr_cr_e;

typedef enum : uint32_t {
    PWR_CR_FPDS_MASK = 0b1U,     // 1 bit(s)
    PWR_CR_DBP_MASK  = 0b1U,     // 1 bit(s)
    PWR_CR_PLS_MASK  = 0b111U,   // 3 bit(s)
    PWR_CR_PVDE_MASK = 0b1U,     // 1 bit(s)
    PWR_CR_CSBF_MASK = 0b1U,     // 1 bit(s)
    PWR_CR_CWUF_MASK = 0b1U,     // 1 bit(s)
    PWR_CR_PDDS_MASK = 0b1U,     // 1 bit(s)
    PWR_CR_LPDS_MASK = 0b1U,     // 1 bit(s)
} pwr_cr_mask_e;

typedef enum : uint32_t {
    PWR_CSR_WUF_POS    = 0U,    // Wakeup flag
    PWR_CSR_SBF_POS    = 1U,    // Standby flag
    PWR_CSR_PVDO_POS   = 2U,    // PVD output
    PWR_CSR_BRR_POS    = 3U,    // Backup regulator ready
    PWR_CSR_EWUP_POS   = 8U,    // Enable WKUP pin
    PWR_CSR_BRE_POS    = 9U,    // Backup regulator enable
    PWR_CSR_VOSRDY_POS = 14U,   // Regulator voltage scaling output selection ready bit
} pwr_csr_pos_e;

typedef enum : uint32_t {
    PWR_CSR_WUF    = (1U << PWR_CSR_WUF_POS),      // Wakeup flag
    PWR_CSR_SBF    = (1U << PWR_CSR_SBF_POS),      // Standby flag
    PWR_CSR_PVDO   = (1U << PWR_CSR_PVDO_POS),     // PVD output
    PWR_CSR_BRR    = (1U << PWR_CSR_BRR_POS),      // Backup regulator ready
    PWR_CSR_EWUP   = (1U << PWR_CSR_EWUP_POS),     // Enable WKUP pin
    PWR_CSR_BRE    = (1U << PWR_CSR_BRE_POS),      // Backup regulator enable
    PWR_CSR_VOSRDY = (1U << PWR_CSR_VOSRDY_POS),   // Regulator voltage scaling output selection ready bit
} pwr_csr_e;

typedef enum : uint32_t {
    PWR_CSR_WUF_MASK    = 0b1U,   // 1 bit(s)
    PWR_CSR_SBF_MASK    = 0b1U,   // 1 bit(s)
    PWR_CSR_PVDO_MASK   = 0b1U,   // 1 bit(s)
    PWR_CSR_BRR_MASK    = 0b1U,   // 1 bit(s)
    PWR_CSR_EWUP_MASK   = 0b1U,   // 1 bit(s)
    PWR_CSR_BRE_MASK    = 0b1U,   // 1 bit(s)
    PWR_CSR_VOSRDY_MASK = 0b1U,   // 1 bit(s)
} pwr_csr_mask_e;

// clang-format on

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

typedef __vo struct {
    uint32_t CR;  // power control register            Offset: 0x0
    uint32_t CSR; // power control/status register     Offset: 0x4
} pwr_reg_def;

//======================================================================================//}
//                  Peripheral Structure Macros
//======================================================================================//{

#define PWR ((pwr_reg_def *)PWR_BASE_ADDR)

//======================================================================================//}
//                  Function API Prototypes
//======================================================================================//{

void pwr_clock_enable(void);
void pwr_backup_write_protection_dis(void);

#endif
