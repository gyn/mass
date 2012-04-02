#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <unistd.h>


#define PCI_CONFIG_ADDRESS_PORT	0x0CF8
#define PCI_CONFIG_DATA_PORT	0x0CFC

#define PCI_BUS_MASK		0xFFU
#define PCI_DEV_MASK		0x1FU
#define PCI_FUNC_MASK		0x07U
#define PCI_REG_MASK		0xFCU

#define PCI_BUS_LIMIT		(PCI_BUS_MASK + 1)
#define PCI_DEV_LIMIT		(PCI_DEV_MASK + 1)
#define PCI_FUNC_LIMIT		(PCI_FUNC_MASK + 1)

#define PCI_CMD_FORMAT		"b:d:f:v"

#define PCI_OP_READ		0
#define PCI_OP_WRITE		1


struct pci_cmd_param {
	u_int32_t bus;
	u_int32_t dev;
	u_int32_t func;

	int access;

	u_int32_t offset;
	u_int32_t value;
};


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

static inline u_int32_t
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
pci_write_conf_reg(u_int32_t bus, u_int32_t dev, u_int32_t func,
		   u_int32_t reg, u_int32_t value)
{
	u_int32_t address;

	address = x86_io_address(bus, dev, func, reg);

	outl(value, PCI_CONFIG_DATA_PORT);
	outl(address, PCI_CONFIG_ADDRESS_PORT);
}

static inline void
print_help(char *name)
{
	fprintf(stderr, "\nUsage:\t%s [ -v ] -b bus -d dev -f func offset [ data ]\n"
		"\tbus    : PCI bus number\n"
		"\tdev    : PCI device number\n"
		"\tfunc   : PCI function number\n"
		"\toffset : PCI register offset\n"
		"\tdata   : data to be written\n\n",
		name);
}

static inline void
parse_cmdline(int argc, char **argv, struct pci_cmd_param *param)
{
	int c;
	int bus_flag;
	int dev_flag;
	int func_flag;

	bus_flag = 0;
	dev_flag = 0;
	func_flag = 0;

	while ((c = getopt (argc, argv, PCI_CMD_FORMAT)) != -1) {
		switch (c) {
		case 'b' :
			param->bus = strtoul(optarg, 0, 0);
			bus_flag = 1;
			break;

		case 'd' :
			param->dev = strtoul(optarg, 0, 0);
			dev_flag = 1;
			break;

		case 'f' :
			param->func = strtoul(optarg, 0, 0);
			func_flag = 1;
			break;

		default:
			print_help(argv[0]);
			
			exit(EXIT_FAILURE);
			break;
		}
	}

	/* we need the bus/dev/func parameters */
	if (!(bus_flag && dev_flag && func_flag)) {
		print_help(argv[0]);

		exit(EXIT_FAILURE);
	}


	/* check the offset and data parameters */
	switch ((argc - optind)) {
	case 1:
		param->access = PCI_OP_READ;
		param->offset = atoi(argv[optind]);
		break;

	case 2:
		param->access = PCI_OP_WRITE;
		param->offset = strtoul(argv[optind], 0, 0);
		param->value  = strtoul(argv[optind + 1], 0, 0);
		break;

	default:
		print_help(argv[0]);

		exit(EXIT_FAILURE);
		break;
	}
}

int main(int argc, char **argv)
{
	u_int32_t value;

	struct pci_cmd_param param = { 0 };

	/* get options */
	parse_cmdline(argc, argv, &param);

	/* go to privilege level 3 */
	privilege_level(3);

	if (param.access) {
		pci_write_conf_reg(param.bus, param.dev,
				   param.func, param.offset, param.value);
	}

	value = pci_read_conf_reg(param.bus, param.dev,
				  param.func, param.offset);

	printf("%04d:%02d.%d 0x%08x => 0x%08x\n",
	       param.bus, param.dev, param.func, param.offset, value);

	/* back to privilege level for a normal process */
	privilege_level(0);

	return 0;
}

