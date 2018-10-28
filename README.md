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

Android

```bash
# usage
./ututna [id] [loops] [-h <hw event#>] [-c <hw cache event#>] [-r <raw event#>] [-i <predefined#>]
-i 0: retired, cycles, ipc
-i 1: l1d refill, l1d, l1d refill ratio
-i 2: l2d refill, l2d, l2d refill ratio
```

```bash
# sample output
./ututna -i 0 -i 2 -r 1d
NAME              SCORE    LOOPS  DURATION(s)      RETIRED       CYCLES   IPC   L2D_REFILL          L2D     %    RAW(0x1D)
600_FFT           58742   200000  3.404696     14854000022   7656080140  1.94           96       806912  0.01   2018154985
[SGEMM]           99285     1000  3.379598      9607694023   7782375119  1.23       161767   1235243803  0.01   2594125519
[DGEMM]           33136      500  5.063063     25505654523  11657832564  2.19      1004577   1126414443  0.09   3885944374
601_GEMM         132422     1000  8.442661   
[MAP.ORDERED]         -      500  3.304518      2312394758   7577396730  0.31        81197    334849532  0.02   2525793734
[MAP.UNORDERED]       -     5000  3.393717      6015728104   7611439569  0.79       347739   1165577244  0.03   2537333941
603_MAP           78695      500  6.698235   
609_PNG          221030     1000  4.524271      9865680080   9144157330  1.08      3620152    228759565  1.58   3111158851
614_PHYSICS      203770      600  2.944500      1974499991   6104204800  0.32     22712386    234817603  9.67   2034734984
607_HASH          10152     5000  2.955023      9497655023   6804741344  1.40          328      8068051  0.00   2292686977
```

Semihosting
```bash
qemu-system-aarch64 -machine virt -cpu cortex-a53 -nographic -monitor null \
  -serial null -semihosting -semihosting-config ram_start=0x40000000 -m 2048M \
  -kernel ututna.elf -append "0 1"
```
