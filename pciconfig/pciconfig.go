package main

// #include <sys/io.h>
import "C"

import (
	"flag"
	"fmt"
	"log"
	"os"
	"strconv"
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

func pciBuildAddress(bus, dev, function, offset uint32) (address uint32) {
	address = 0x80000000 |
		bus&PciBusMask<<16 |
		dev&PciDevMask<<11 |
		function&PciFuncMask<<8 |
		offset&PciRegMask

	return address
}

func pciReadConfReg(bus, dev, function, offset uint32) (value uint32) {
	address := pciBuildAddress(bus, dev, function, offset)

	C.outl(C.uint(address), PciAddrPort)

	value = uint32(C.inl(PciDataPort))

	return value
}

func pciWriteConfReg(bus, dev, function, offset, value uint32) {
	address := pciBuildAddress(bus, dev, function, offset)

	C.outl(C.uint(address), PciAddrPort)
	C.outl(C.uint(value), PciDataPort)
}

func usage() {
	fmt.Fprintf(os.Stderr, "usage: pciconfig options [value]\n")

	flag.PrintDefaults()

	os.Exit(1)
}

func main() {
	var isWrite bool

	bus := flag.Uint("bus", 0, "bus number, [0 : 255]")
	dev := flag.Uint("dev", 0, "device number, [0 : 63]")
	function := flag.Uint("function", 0, "function number, [0 : 7]")
	offset := flag.Uint("offset", 0, "offset, [0 : 255] and 4 byte aligned")

	flag.Usage = usage
	flag.Parse()

	// Check parameters
	if *bus >= PciBusLimit || *dev >= PciDevLimit ||
		*function >= PciFuncLimit ||
		*offset >= PciRegLimit || *offset&0x03 != 0 {
		usage()
	}

	switch flag.NArg() {
	case 0:
		isWrite = false
	case 1:
		isWrite = true
	default:
		usage()
	}

	busVal := uint32(*bus)
	devVal := uint32(*dev)
	funcVal := uint32(*function)
	offsetVal := uint32(*offset)

	ioLevel(3)
	defer ioLevel(0)

	if isWrite {
		value, err := strconv.ParseUint(flag.Arg(0), 0, 32)
		if err != nil {
			log.Fatal(err)
		}

		pciWriteConfReg(busVal, devVal, funcVal, offsetVal,
			uint32(value))
	}

	data := pciReadConfReg(busVal, devVal, funcVal, offsetVal)

	fmt.Printf("[%02X:%02X.%X-%02X] = %08x\n",
		busVal, devVal, funcVal, offsetVal, data)
}
