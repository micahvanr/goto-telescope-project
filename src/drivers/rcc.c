#include "rcc.h"

/***************************************************************************
Function: rcc_get_pll_freq
Overview: Gets the PLL clock frequency from the PLL register in RCC
Parameters:
    None
Return: 
    PLL clock frequency
Note: None
***************************************************************************/
uint32_t rcc_get_pll_freq(void)
{
    uint32_t rcc_pllcfgr_reg;

    uint8_t pllm;
    uint8_t plln;

    uint8_t pllp_reg;
    uint8_t const pllp_values[] = {2, 4, 6, 8};
    uint8_t pllp;

    clock_sources_e pll_src;

    uint32_t vco_clk_freq;
    uint32_t pll_input_clk_freq;
    uint32_t pll_output_clk_freq;

    // Get register values
    rcc_pllcfgr_reg = RCC->PLLCFGR;
    pllm            = ((rcc_pllcfgr_reg >> RCC_PLL_CFGR_PLLM_POS) & RCC_PLL_CFGR_PLLM_MASK);
    plln            = ((rcc_pllcfgr_reg >> RCC_PLL_CFGR_PLLN_POS) & RCC_PLL_CFGR_PLLN_MASK);
    pllp_reg        = ((rcc_pllcfgr_reg >> RCC_PLL_CFGR_PLLP_POS) & RCC_PLL_CFGR_PLLP_MASK);
    pllp            = pllp_values[pllp_reg];

    pll_src = ((rcc_pllcfgr_reg >> RCC_PLL_CFGR_PLLSRC_POS) & RCC_PLL_CFGR_PLLSRC_MASK);

    // Select PLL clock source frequency
    switch (pll_src) {
    case CLOCK_SRC_HSI: pll_input_clk_freq = HSI_CLOCK_FREQ; break;
    case CLOCK_SRC_HSE: pll_input_clk_freq = HSE_CLOCK_FREQ; break;
    default:            ASSERT(FALSE); break;
    }

    // Calculations of PLL output frequency
    vco_clk_freq        = (pll_input_clk_freq) * (plln / pllm);
    pll_output_clk_freq = vco_clk_freq / pllp;

    return pll_output_clk_freq;
}

/***************************************************************************
Function: rcc_get_sys_clock_freq
Overview: Gets the system clock frequency depending on what the current clock source is
Parameters:
    None
Return: 
    System clock frequency
Note: None
***************************************************************************/
uint32_t rcc_get_sys_clock_freq(void)
{
    // Get active clock source (HSI/HSE/PLL)
    clock_sources_e clock_source = (clock_sources_e)((RCC->CFGR >> RCC_CFGR_SWS_POS) & RCC_CFGR_SWS_MASK);
    ASSERT((clock_source == CLOCK_SRC_HSI) || (clock_source == CLOCK_SRC_HSE) || (clock_source == CLOCK_SRC_PLL));

    // Return clock depending on what the source is
    switch (clock_source) {
    case CLOCK_SRC_HSI: return HSI_CLOCK_FREQ;

    case CLOCK_SRC_HSE: return HSE_CLOCK_FREQ;

    case CLOCK_SRC_PLL: return rcc_get_pll_freq();

    default:            ASSERT(FALSE); return 0;
    }
}

/***************************************************************************
Function: rcc_get_bus_clock_freq
Overview: Gets the given bus clock frequency depending on what the system clock is and what the prescaler values are set to
Parameters:
    bus: Different bus options to be returned
        AHB1_BUS
        AHB2_BUS
        APB1_BUS
        APB2_BUS
Return: 
    Clock frequency for the given bus
Note: None
***************************************************************************/
uint32_t rcc_get_bus_clock_freq(bus_types bus)
{
    uint16_t const ahb_clock_prescaler[] = {2, 4, 8, 16, 64, 128, 256, 512};
    uint16_t const apb_clock_prescaler[] = {2, 4, 8, 16};

    uint32_t sysclock_freq;

    // Highspeed = AHB
    // Lowspeed = APB
    uint8_t ahb_prescaler;
    uint32_t ahb_clock;

    uint8_t apb1_ppre1_reg;
    uint8_t apb1_prescaler;
    uint32_t apb1_clock;

    uint8_t apb2_ppre2_reg;
    uint8_t apb2_prescaler;
    uint32_t apb2_clock;

    // Calculate different clocks (AHB1/AHB2/APB1/APB2) from prescalers

    // Get active system clock source (HSI/HSE/PLL) - Use function to get the clock for that source
    sysclock_freq = rcc_get_sys_clock_freq();

    // Calculate AHB clock
    ahb_prescaler = ((RCC->CFGR >> RCC_CFGR_HPRE_POS) & RCC_CFGR_HPRE_MASK);
    // If 4th bit is 1, select prescaler, otherwise the prescaler is 1
    ahb_prescaler = ((ahb_prescaler & 0b1000) != 0) ? ahb_clock_prescaler[ahb_prescaler & ~(0b1000)] : 1;
    ahb_clock     = sysclock_freq / ahb_prescaler;

    switch (bus) {
    case AHB1_BUS:
    case AHB2_BUS: return ahb_clock;

    case APB1_BUS:
        apb1_ppre1_reg = ((RCC->CFGR >> RCC_CFGR_PPRE1_POS) & RCC_CFGR_PPRE1_MASK);
        apb1_prescaler = ((apb1_ppre1_reg & 0b100) != 0) ? apb_clock_prescaler[apb1_ppre1_reg & ~(0b100)] : 1;
        apb1_clock     = ahb_clock / apb1_prescaler;
        return apb1_clock;

    case APB2_BUS:
        apb2_ppre2_reg = ((RCC->CFGR >> RCC_CFGR_PPRE2_POS) & RCC_CFGR_PPRE2_MASK);
        apb2_prescaler = ((apb2_ppre2_reg & 0b100) != 0) ? apb_clock_prescaler[apb2_ppre2_reg & ~(0b100)] : 1;
        apb2_clock     = ahb_clock / apb2_prescaler;
        return apb2_clock;

    default: ASSERT(FALSE); return 0;
    }
}