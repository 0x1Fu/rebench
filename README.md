# rebench

Supported Benchmarks,

**Antutu 7.1.6**

CPU COMMON
> * 603 MAP
> * 609 PNG
> * 614 PHYSICS

CPU MATH
> * 600 FFT
> * 601 GEMM

UX SEC
> * 607 HASH

**Command Line**

```bash
qemu-system-aarch64 -machine virt -cpu cortex-a53 -nographic -monitor null \
  -serial null -semihosting -semihosting-config ram_start=0x40000000 -m 2048M \
  -kernel ututna.elf -append "0 1"
```