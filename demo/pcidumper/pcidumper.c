#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>


#define PCI_CONFIG_ADDRESS_PORT	0x0CF8
#define PCI_CONFIG_DATA_PORT	0x0CFC

#define PCI_BUS_MASK		0xFFU
#define PCI_DEV_MASK		0x1FU
#define PCI_FUNC_MASK		0x07U
#define PCI_REG_MASK		0xFCU

#define PCI_BUS_LIMIT		(PCI_BUS_MASK + 1)
#define PCI_DEV_LIMIT		(PCI_DEV_MASK + 1)
#define PCI_FUNC_LIMIT		(PCI_FUNC_MASK + 1)
#define PCI_REG_LIMIT		(PCI_REG_MASK + 4)


static inline void privilege_level(int level)
{
	int rc;

	rc = iopl(level);
	if (rc == -1) {
		perror("iopl");

		exit(EXIT_FAILURE);
	}
}

static inline u_int32_t
x86_io_address(u_int32_t bus, u_int32_t dev, u_int32_t func,
	       u_int32_t reg)
{
	u_int32_t address;

	address = (0x80000000U) |
		  ((bus & PCI_BUS_MASK) << 16) |
		  ((dev & PCI_DEV_MASK) << 11) |
		  ((func & PCI_FUNC_MASK) << 8) |
		  ((reg & PCI_REG_MASK));

	return address;
}

static u_int32_t
pci_read_conf_reg(u_int32_t bus, u_int32_t dev, u_int32_t func,
		  u_int32_t reg)
{
	u_int32_t value;
	u_int32_t address;

	address = x86_io_address(bus, dev, func, reg);

	outl(address, PCI_CONFIG_ADDRESS_PORT);

	value = inl(PCI_CONFIG_DATA_PORT);

	return value;
}

static inline void
pci_dump_header(u_int32_t bus, u_int32_t dev, u_int32_t func)
{
	u_int32_t value;
	u_int32_t offset;

	value = pci_read_conf_reg(bus, dev, func, 0);
	/* check PCI Vendor ID */
	if ((value & 0xFFFFU) == 0xFFFFU) {
		return;
	}

	printf("%d:%d.%d\n00 : 0x%08x ", bus, dev, func, value);

	for (offset = 4; offset < PCI_REG_LIMIT; offset += 4) {
		value = pci_read_conf_reg(bus, dev, func, offset);
		if (((offset & 0x0CU) == 0x0CU)) {
			if (offset != (PCI_BUS_LIMIT - 4)) {
				printf("0x%08x\n%02x : ", value, (offset + 4));
			} else {
				printf("0x%08x\n", value);
			}
		} else {
			printf("0x%08x ", value);
		}
	}
}

int main(int argc, char **argv)
{
	int bus;
	int dev;
	int func;

	/* go to privilege level 3 */
	privilege_level(3);

	/* scan pci buses */
	for (bus = 0; bus < PCI_BUS_LIMIT; bus++) {
		for (dev = 0; dev < PCI_DEV_LIMIT; dev++ ) {
			for (func = 0; func < PCI_FUNC_LIMIT; func++) {
				pci_dump_header(bus, dev, func);
			}
		}
	}

	/* back to privilege level for a normal process */
	privilege_level(0);

	return 0;
}

