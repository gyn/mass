#include "cvmx.h"
#include "cvmx-core.h"
#include "cvmx-l2c.h"


#define CONFIG_TAD_NR	4


/*
 *
 *
 * IPC
 *       CVMX_CORE_PERF_RET / CVMX_CORE_PERF_CLK
 *       cvmx_core_start_stat(3, 1)
 *
 * L1I miss cycle rate
 *       CVMX_CORE_PERF_CIMISS / CVMX_CORE_PERF_CLK
 *       cvmx_core_start_stat(29, 1)
 *
 * L1D miss rate
 *       CVMX_CORE_PERF_DMLDS / CVMX_CORE_PERF_LMLDS
 *       cvmx_core_start_stat(46, 44)
 *
 * UULOAD/UUSTORE count
 *       CVMX_CORE_PERF_UULOAD | CVMX_CORE_PERF_UUSTORE
 *       cvmx_core_start_stat(16, 17)
 *
 */
static inline void cvmx_core_stat_start(int event0, int event1)
{
	cvmx_core_perf_control_t control;

	control.u32 = 0;
	control.s.u = 1;
	control.s.s = 1;
	control.s.k = 1;
	control.s.ex = 1;
	control.s.w = 1;
	control.s.m = 1;	/* counter 0 */
	control.s.event = event0;

	CVMX_MT_COP0(control, COP0_PERFCONTROL0);

	control.s.m = 0;	/* counter 1 */
	control.s.event = event1;
	CVMX_MT_COP0(control, COP0_PERFCONTROL1);

	/* clear all counters */
	CVMX_MT_COP0(0ULL, COP0_PERFVALUE0);
	CVMX_MT_COP0(0ULL, COP0_PERFVALUE1);
}

static inline void cvmx_core_stat_show(void)
{
	uint64_t v0;
	uint64_t v1;

	CVMX_MF_COP0(v0, COP0_PERFVALUE0);
	CVMX_MF_COP0(v1, COP0_PERFVALUE1);

	printf("0 : %16lx 1: %16lx\n", v0, v1);
}

/*
 *
 *
 * L2 miss rate
 *
 */
static inline void cvmx_l2_stat_start(void)
{
	int i;
	int max_nr;

	cvmx_l2c_tadx_prf_t l2c_tadx_prf;

	max_nr = CONFIG_TAD_NR;

	for (i = 0; i < max_nr; ++i) {
		l2c_tadx_prf.u64 = cvmx_read_csr(CVMX_L2C_TADX_PRF(i));

		l2c_tadx_prf.s.cnt0sel = CVMX_L2C_TAD_EVENT_TAG_HIT;
		l2c_tadx_prf.s.cnt1sel = CVMX_L2C_TAD_EVENT_TAG_MISS;
		l2c_tadx_prf.s.cnt2sel = CVMX_L2C_TAD_EVENT_TAG_NOALLOC;
		l2c_tadx_prf.s.cnt3sel = CVMX_L2C_TAD_EVENT_TAG_VICTIM;

		cvmx_write_csr(CVMX_L2C_TADX_PRF(i), l2c_tadx_prf.u64);

		/* clear all counters */
		cvmx_write_csr(CVMX_L2C_TADX_PFC0(i), 0ULL);
		cvmx_write_csr(CVMX_L2C_TADX_PFC1(i), 0ULL);
		cvmx_write_csr(CVMX_L2C_TADX_PFC2(i), 0ULL);
		cvmx_write_csr(CVMX_L2C_TADX_PFC3(i), 0ULL);
	}
}

static inline void cvmx_l2_stat_show(void)
{
	int i;
	int max_nr;

	uint64_t v0;
	uint64_t v1;
	uint64_t v2;
	uint64_t v3;

	max_nr = CONFIG_TAD_NR;

	for (i = 0; i < max_nr; ++i) {
		v0 = cvmx_read_csr(CVMX_L2C_TADX_PFC0(i));
		v1 = cvmx_read_csr(CVMX_L2C_TADX_PFC1(i));
		v2 = cvmx_read_csr(CVMX_L2C_TADX_PFC2(i));
		v3 = cvmx_read_csr(CVMX_L2C_TADX_PFC3(i));

		printf("0 : %16lx 1: %16lx 2 : %16lx 3: %16lx\n",
			v0, v1, v2, v3);
	}
}

static inline void cvmx_ddr_stat_start(void)
{
	int i;
	int max_nr;

	cvmx_lmcx_config_t lmcx_config;

	max_nr = CONFIG_TAD_NR;

	for (i = 0; i < max_nr; ++i) {
		lmcx_config.u64 = cvmx_read_csr(CVMX_LMCX_CONFIG(i));

		lmcx_config.s.reset = 1;
		cvmx_write_csr(CVMX_LMCX_CONFIG(i), lmcx_config.u64);

		cvmx_wait(10);

		lmcx_config.s.reset = 0;
		cvmx_write_csr(CVMX_LMCX_CONFIG(i), lmcx_config.u64);
	}
}

static inline void cvmx_ddr_stat_show(void)
{
	int i;
	int max_nr;

	uint64_t v0;
	uint64_t v1;

	max_nr = CONFIG_TAD_NR;

	for (i = 0; i < max_nr; ++i) {
		v0 = cvmx_read_csr(CVMX_LMCX_OPS_CNT(i));
		v1 = cvmx_read_csr(CVMX_LMCX_DCLK_CNT(i));

		printf("0 : %16lx 1: %16lx\n", v0, v1);
	}
}
