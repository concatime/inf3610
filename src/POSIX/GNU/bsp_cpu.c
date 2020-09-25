#include <cpu_core.h>

#if CPU_CFG_TS_32_EN == DEF_ENABLED
CPU_INT64U CPU_TS32_to_uSec(CPU_TS32 ts_cnts) {
	CPU_TS_TMR_FREQ freq;
	CPU_ERR cpu_err;

	freq = CPU_TS_TmrFreqGet(&cpu_err);
	if (cpu_err != CPU_ERR_NONE) {
		return 0U;
	}

	return ((CPU_INT64U)ts_cnts * 1000000UL) / freq;
}
#endif

#if CPU_CFG_TS_64_EN == DEF_ENABLED
CPU_INT64U CPU_TS64_to_uSec(CPU_TS64 ts_cnts) {
	CPU_TS_TMR_FREQ freq;
	CPU_ERR cpu_err;

	freq = CPU_TS_TmrFreqGet(&cpu_err);
	if (cpu_err != CPU_ERR_NONE) {
		return 0U;
	}

	return (ts_cnts * 1000000UL) / freq;
}
#endif
