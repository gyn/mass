package main

import (
	"fmt"
	"os"
	"reflect"
	"strconv"
	"syscall"
	"unsafe"
)

const (
	size8bit = iota
	size16bit
	size32bit
	size64bit

	mapSize = 4096
	mapMask = mapSize - 1

	mapProt = syscall.PROT_READ | syscall.PROT_WRITE
	mapFlag = syscall.MAP_SHARED
)

func toSliceUint16(buffer []byte) []uint16 {
	ptr := make([]uint16, 0, 0)

	h := (*reflect.SliceHeader)(unsafe.Pointer(&ptr))

	h.Data = (*reflect.SliceHeader)(unsafe.Pointer(&buffer)).Data
	h.Len = len(buffer) >> 1
	h.Cap = cap(buffer) >> 1

	return ptr
}

func toSliceUint32(buffer []byte) []uint32 {
	ptr := make([]uint32, 0, 0)

	h := (*reflect.SliceHeader)(unsafe.Pointer(&ptr))

	h.Data = (*reflect.SliceHeader)(unsafe.Pointer(&buffer)).Data
	h.Len = len(buffer) >> 2
	h.Cap = cap(buffer) >> 2

	return ptr
}

func toSliceUint64(buffer []byte) []uint64 {
	ptr := make([]uint64, 0, 0)

	h := (*reflect.SliceHeader)(unsafe.Pointer(&ptr))

	h.Data = (*reflect.SliceHeader)(unsafe.Pointer(&buffer)).Data
	h.Len = len(buffer) >> 3
	h.Cap = cap(buffer) >> 3

	return ptr
}

func readMem(buffer []byte, width int, offset int64) (addr, data int64) {
	switch width {
	case size8bit:
		ptr := buffer
		addr = offset
		data = int64(ptr[offset])
	case size16bit:
		ptr := toSliceUint16(buffer)
		addr = offset &^ 0x01
		data = int64(ptr[offset>>1])
	case size32bit:
		ptr := toSliceUint32(buffer)
		addr = offset &^ 0x03
		data = int64(ptr[offset>>2])
	case size64bit:
		ptr := toSliceUint64(buffer)
		addr = offset &^ 0x07
		data = int64(ptr[offset>>3])
	default:
	}

	return
}

func writeMem(buffer []byte, width int, offset int64, value int64) {
	switch width {
	case size8bit:
		ptr := buffer
		ptr[offset] = uint8(value)
	case size16bit:
		ptr := toSliceUint16(buffer)
		ptr[offset>>1] = uint16(value)
	case size32bit:
		ptr := toSliceUint32(buffer)
		ptr[offset>>2] = uint32(value)
	case size64bit:
		ptr := toSliceUint64(buffer)
		ptr[offset>>3] = uint64(value)
	default:
	}
}

func usage() {
	fmt.Fprintf(os.Stderr, "\nUsage:\t%s address type [ data ]\n"+
		"\taddress : memory address\n"+
		"\ttype    : bit width, [b]yte, [h]alfword, [w]ord or [d]oubleword\n"+
		"\tdata    : data to be written\n\n",
		os.Args[0])
}

func main() {
	var width int

	//
	// check parameters
	//
	argc := len(os.Args)

	if argc < 3 || argc > 4 {
		usage()

		os.Exit(1)
	}

	address, err := strconv.ParseInt(os.Args[1], 0, 64)
	if err != nil {
		fmt.Println(err)

		os.Exit(1)
	}

	switch os.Args[2] {
	case "b":
		width = size8bit
	case "h":
		width = size16bit
	case "w":
		width = size32bit
	case "d":
		width = size64bit
	default:
		usage()

		os.Exit(1)
	}

	file, err := os.OpenFile("/dev/mem", os.O_RDWR|os.O_SYNC, 0644)
	defer file.Close()
	if err != nil {
		fmt.Println(err)

		os.Exit(1)
	}

	fd := int(file.Fd())
	base := address &^ mapMask
	offset := address & mapMask

	ptr, err := syscall.Mmap(fd, base, mapSize, mapProt, mapFlag)
	if err != nil {
		fmt.Println(err)

		os.Exit(1)
	}

	if argc == 4 {
		value, err := strconv.ParseInt(os.Args[3], 0, 64)
		if err != nil {
			fmt.Println(err)

			os.Exit(1)
		}

		writeMem(ptr, width, offset, value)
	}

	addr, data := readMem(ptr, width, offset)

	fmt.Printf("0x%x 0x%x\n", addr+base, data)
}
