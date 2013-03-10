/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Inc. (support@cavium.com). All rights 
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.  

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries. 

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/


/*
 * File version info: $Id: main.c 69988 2012-02-15 18:55:21Z cchavva $
 */
 
#include "cvmx.h"

#include "stat.h"


#define CONFIG_MAX_LOOP		5
#define CONFIG_POOL_SIZE	0x100000

static uint8_t pool[CONFIG_POOL_SIZE];
static uint8_t data[CONFIG_POOL_SIZE];

static void cvmx_core_stat_main(void)
{
	int i;

	cvmx_core_stat_start(CVMX_CORE_PERF_RET, CVMX_CORE_PERF_CLK);

	memset(pool, 0x55, CONFIG_POOL_SIZE);
	for (i = 0; i < CONFIG_POOL_SIZE; ++i) {
		pool[i] = data[i] + i;
	}

	cvmx_core_stat_show();
}

static void cvmx_l2_stat_main(void)
{
	int i;

	cvmx_l2_stat_start();

	memset(pool, 0x55, CONFIG_POOL_SIZE);
	for (i = 0; i < CONFIG_POOL_SIZE; ++i) {
		pool[i] = data[i] + i;
	}

	cvmx_l2_stat_show();
}

static void cvmx_ddr_stat_main(void)
{
	int i;

	cvmx_ddr_stat_start();

	memset(pool, 0x55, CONFIG_POOL_SIZE);
	for (i = 0; i < CONFIG_POOL_SIZE; ++i) {
		pool[i] = data[i] + i;
	}

	cvmx_ddr_stat_show();
}

int main(void)
{
	cvmx_core_stat_main();

	cvmx_l2_stat_main();

	cvmx_ddr_stat_main();

	return 0;
}
