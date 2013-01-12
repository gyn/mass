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
 * File version info: $Id: hello.c 70030 2012-02-16 04:23:43Z cchavva $
 */
 
#include "cvmx.h"


uint64_t bitswap64(uint64_t value)
{
	uint64_t result;

	result = ((value & 0x5555555555555555) << 1) |
		((value & 0xAAAAAAAAAAAAAAAA) >> 1);
	result = ((result & 0x3333333333333333) << 2) |
		((result & 0xCCCCCCCCCCCCCCCC) >> 2);
	result = ((result & 0x0F0F0F0F0F0F0F0F) << 4) |
		((result & 0xF0F0F0F0F0F0F0F0) >> 4);
	result = ((result & 0x00FF00FF00FF00FF) << 8) |
		((result & 0xFF00FF00FF00FF00) >> 8);
	result = ((result & 0x0000FFFF0000FFFF) << 16) |
		((result & 0xFFFF0000FFFF0000) >> 16);
	result = ((result & 0x00000000FFFFFFFF) << 32) |
		((result & 0xFFFFFFFF00000000) >> 32);

	return result;
}

uint32_t bitswap32(uint32_t value)
{
	uint32_t temp;
	uint32_t result;

	temp = value;

	temp = ((temp & 0x55555555) << 1) | ((temp & 0xAAAAAAAA) >> 1);
	temp = ((temp & 0x33333333) << 2) | ((temp & 0xCCCCCCCC) >> 2);
	temp = ((temp & 0x0F0F0F0F) << 4) | ((temp & 0xF0F0F0F0) >> 4);
	temp = ((temp & 0x00FF00FF) << 8) | ((temp & 0xFF00FF00) >> 8);
	temp = ((temp & 0x0000FFFF) << 16) | ((temp & 0xFFFF0000) >> 16);

	result = temp;

	return result;
}

int main(void)
{
	uint32_t v32;
	uint64_t v64;

	uint64_t cycle_point;
	uint64_t cycle_delta;

	cycle_point = cvmx_get_cycle();
	v32 = bitswap32(1);
	cycle_delta = cvmx_get_cycle() - cycle_point;

	printf("%s %d : %x %lx\n", __FUNCTION__, __LINE__, v32, cycle_delta);

	cycle_point = cvmx_get_cycle();
	v64 = bitswap64(1);
	cycle_delta = cvmx_get_cycle() - cycle_point;

	printf("%s %d : %lx %lx\n", __FUNCTION__, __LINE__, v64, cycle_delta);

	return 0;
}
