#include <stdio.h>
#include <stdint.h>


#if defined(CONFIG_CPU_CYCLE)
#define CYCLE_DECL							\
	uint64_t cycle_point;					\
	uint32_t cycle_delta;

#define CYCLE_START							\
	cycle_point = cvmx_get_cycle();

#define CYCLE_END							\
	cycle_delta = cvmx_get_cycle() - cycle_point;
#else
#define CYCLE_DECL
#define CYCLE_START
#define CYCLE_END
#endif

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

	CYCLE_DECL

	CYCLE_START
	v32 = bitswap32(1);
	CYCLE_END

	printf("%s %d : %x\n", __FUNCTION__, __LINE__, v32);

	CYCLE_START
	v64 = bitswap64(1);
	CYCLE_END

	printf("%s %d : %lx\n", __FUNCTION__, __LINE__, v64);

	return 0;
}
