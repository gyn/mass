package main

// #include <sys/io.h>
import "C"

import (
	"fmt"
	"log"
	"syscall"
)

const (
	PciAddrPort = 0x0cf8
	PciDataPort = 0x0cfc

	PciBusMask  = 0xff
	PciDevMask  = 0x1f
	PciFuncMask = 0x07
	PciRegMask  = 0xfc

	PciBusLimit  = PciBusMask + 1
	PciDevLimit  = PciDevMask + 1
	PciFuncLimit = PciFuncMask + 1
	PciRegLimit  = PciRegMask + 1
)

func ioLevel(level int) {
	if err := syscall.Iopl(level); err != nil {
		log.Fatal(err)
	}
}

func pciReadConfReg(bus, dev, function, offset uint32) (value uint32) {
	address := 0x80000000 |
		bus&PciBusMask<<16 |
		dev&PciDevMask<<11 |
		function&PciFuncMask<<8 |
		offset&PciRegMask

	C.outl(C.uint(address), PciAddrPort)

	value = uint32(C.inl(PciDataPort))

	return
}

func pciDumpHeader(bus, dev, function uint32) {
	var offset uint32

	/* check PCI Vendor ID */
	value := pciReadConfReg(bus, dev, function, 0)
	if value&0xffff == 0xffff {
		return
	}

	fmt.Printf("%d:%d.%d\n00 : 0x%08x ", bus, dev, function, value)

	for offset = 4; offset < PciRegLimit; offset += 4 {
		value = pciReadConfReg(bus, dev, function, offset)
		if offset&0x0c == 0x0c {
			if offset != PciRegLimit-1 {
				fmt.Printf("0x%08x\n%02x : ", value, offset+4)
			} else {
				fmt.Printf("0x%08x\n", value)
			}
		} else {
			fmt.Printf("0x%08x ", value)
		}
	}
}

func main() {
	var busId uint32
	var devId uint32
	var funcId uint32

	ioLevel(3)

	/* scan pci by bus, device and function number */
	for busId = 0; busId < PciBusLimit; busId++ {
		for devId = 0; devId < PciDevLimit; devId++ {
			for funcId = 0; funcId < PciFuncLimit; funcId++ {
				pciDumpHeader(busId, devId, funcId)
			}
		}
	}

	ioLevel(0)
}
