#include "cvmx.h"

#define CONFIG_REG_RO	0
#define CONFIG_REG_RW	1

#define CONFIG_DEF_LVL	10


struct l2c_map_reg {
	int rd;
	int sel;

	int level;
	int writable;

	char *name;
} l2c_map_regs[] = {
	{4,  2, CONFIG_DEF_LVL, CONFIG_REG_RO, "UserLocal"},
	{7,  0, CONFIG_DEF_LVL, CONFIG_REG_RO, "HWREna"},
	{9,  0, CONFIG_DEF_LVL, CONFIG_REG_RO, "Count"},
	{9,  6, CONFIG_DEF_LVL, CONFIG_REG_RO, "CvmCount"},
	{9,  7, CONFIG_DEF_LVL, CONFIG_REG_RO, "CvmCtl"},
	{11, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "Compare"},
	{11, 6, CONFIG_DEF_LVL, CONFIG_REG_RW, "PowThrottle"},
	{12, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "Status"},
	{12, 1, CONFIG_DEF_LVL, CONFIG_REG_RO, "IntCtl"},
	{12, 2, CONFIG_DEF_LVL, CONFIG_REG_RO, "SRSCtl"},
	{13, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "Cause"},
	{14, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "EPC"},
	{15, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "PRId"},
	{15, 1, CONFIG_DEF_LVL, CONFIG_REG_RO, "Ebase"},
	{16, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "PCDebug"},
	{16, 1, CONFIG_DEF_LVL, CONFIG_REG_RO, "PCDebug1"},
	{16, 2, CONFIG_DEF_LVL, CONFIG_REG_RO, "PCDebug2"},
	{16, 3, CONFIG_DEF_LVL, CONFIG_REG_RO, "PCDebug3"},
	{18, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "WatchLo0"},
	{19, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "WatchHi0"},
	{22, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "MultiCore Debug"},
	{23, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "Debug"},
	{23, 6, CONFIG_DEF_LVL, CONFIG_REG_RO, "Debug2"},
	{23, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "DEPC"},
	{25, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "PerfCtl0"},
	{25, 1, CONFIG_DEF_LVL, CONFIG_REG_RO, "PerfCnt0"},
	{25, 2, CONFIG_DEF_LVL, CONFIG_REG_RO, "PerfCtl1"},
	{25, 3, CONFIG_DEF_LVL, CONFIG_REG_RO, "PerfCnt1"},
	{27, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "I-CacheErr"},
	{28, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "I-TagLo"},
	{28, 1, CONFIG_DEF_LVL, CONFIG_REG_RO, "I-DataLo"},
	{29, 1, CONFIG_DEF_LVL, CONFIG_REG_RO, "I-DataHi"},
	{30, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "ErrorEPC"},
	{31, 0, CONFIG_DEF_LVL, CONFIG_REG_RO, "DESAVE"},
	{31, 2, CONFIG_DEF_LVL, CONFIG_REG_RO, "KScratch1"},
	{31, 3, CONFIG_DEF_LVL, CONFIG_REG_RO, "KScratch2"},
	{31, 4, CONFIG_DEF_LVL, CONFIG_REG_RO, "KScratch3"},
};

static inline uint64_t read_reg(int id, int rd, int sel)
{
	int reg;

	uint64_t value;

	reg = (id << 8) | (rd << 3) | sel;

	value = cvmx_read_csr(CVMX_L2C_COP0_MAPX(reg));

	return value;
}

static inline void write_reg(int id, int rd, int sel, uint64_t value)
{
	int reg;

	reg = (id << 8) | (rd << 3) | sel;

	cvmx_write_csr(CVMX_L2C_COP0_MAPX(reg), value);
}

static inline void dump_regs(int id, int level)
{
	int i;
	int max_nr;

	uint64_t value;

	max_nr = sizeof(l2c_map_regs)/sizeof(struct l2c_map_reg);

	for (i = 0; i < max_nr; ++i) {
		value = read_reg(id, l2c_map_regs[i].rd, l2c_map_regs[i].sel);

		printf("core %d %16s : %16lx\n", id, l2c_map_regs[i].name, value);
	}
}