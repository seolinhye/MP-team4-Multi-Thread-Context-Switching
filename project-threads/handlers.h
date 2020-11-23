// ======================================================================

#ifndef _HANDLERS_H_
#define _HANDLERS_H_

// ======================================================================

extern unsigned int tick;

// =====================================================================
// Cortex-M4 SysTick register address

#define SYST_CSR        0xe000e010
#define SYST_RVR        0xe000e014
#define SYST_CVR        0xe000e018
#define SYST_CALIB      0xe000e01c

// ======================================================================

#define SCB_ICSR        0xe000ed04
#define SCB_CCR         0xe000ed14

#endif

// ======================================================================