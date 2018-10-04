import re
import sys

file = open(sys.argv[1], 'r')

lineno = 0

symbols = []
local_symbols = []
unsafe_symbols = [
    'malloc', 'free',
    '_ZdaPv', '_ZdlPv', '_Znam', '_Znwm',
    'fopen', 'fread', 'fwrite', 'fclose',
    'exit', 'abort',
    #'gettimeofday',
    'setjmp', 'longjmp',
    '__stack_chk_fail',
    '__sfp_handle_exceptions',
    '__cxa_begin_catch', '__cxa_end_catch', '__cxa_rethrow', '_Unwind_Resume'
]
static_symbols = [
    # 'memset', 'memcpy', 'sin', 'cos', 'sqrt', 'pow'
]

ADDR = "(?P<addr>[0-9A-Fa-f]+)"
SYM = "<(?P<sym>\w+)>"
SYM_OFFSET = "<(?P<sym>\w+)\+0x(?P<offset>[0-9A-Fa-f]+)>"
SYM_PLT = "<(?P<sym>\w+)@plt>"
SYM_BASE = "<(?P<sym>\w+)@@Base>"
SYM_BASE_OFFSET = "<(?P<sym>\w+)@@Base\+0x(?P<offset>[0-9A-Fa-f]+)>"
BIN = "(?P<bin>[0-9A-Fa-f]{8})"
INST = "(?P<inst>.+)"
OPERANDS = "(?P<operands>.+)"
REG = "(?P<reg>[x|d][0-9]+)"
SPACE = "\s+"
TAB = "\t"

def import_sym(sym):
    if sym not in symbols:
        symbols.append(sym)
    if sym in unsafe_symbols:
        return "o_" + sym
    if sym in static_symbols:
        return "s_" + sym
    return sym

def process_insts(line, m):
    inst = m.group('inst')
    addr = int(m.group('addr'), 16)
    operands = m.group('operands')

    if inst in ['b', 'bl'] or inst.startswith('b.'):
        # 446228 <__kernel_sin>
        mo = re.match(ADDR + SPACE + SYM, operands)
        if mo:
            print("L{:x}:\t{} {}".format(addr, inst, import_sym(mo.group('sym'))))
            return True
        # 38a90 <kiss_fft_alloc@plt>
        mo = re.match(ADDR + SPACE + SYM_PLT, operands)
        if mo:
            print("L{:x}:\t{} {}".format(addr, inst, import_sym(mo.group('sym'))))
            return True
        # 4455a4 <sin+0x58>
        mo = re.match(ADDR + SPACE + SYM_OFFSET, operands)
        if mo:
            print("L{:x}:\t{} L{} // <{}+0x{}>".format(addr, inst,
                mo.group('addr'), mo.group('sym'), mo.group('offset')))
            return True
        # 577ac <kiss_fftr_alloc@@Base+0x184>
        mo = re.match(ADDR + SPACE + SYM_BASE_OFFSET, operands)
        if mo:
            print("L{:x}:\t{} L{} // <{}@@Base+0x{}>".format(addr, inst,
                mo.group('addr'), mo.group('sym'), mo.group('offset')))
            return True
        return False

    # tbnz    w23, #0, 576c4 <kiss_fftr_alloc@@Base+0x9c>
    # tbz     w0, #0, 581cc <test_sgemm@@Base+0x230>
    # tbz     w2, #3, 4068f0 <memset+0x30>
    if inst in ['tbz', 'tbnz']:
        mo = re.match("(?P<op1>.+), (?P<op2>.+), " + ADDR + SPACE + SYM_BASE_OFFSET, operands)
        if not mo:
             mo = re.match("(?P<op1>.+), (?P<op2>.+), " + ADDR + SPACE + SYM_OFFSET, operands)
        if mo:
            print("L{:x}:\t{} {}, {}, L{} // <{}@@Base+0x{}>".format(addr, inst, mo.group('op1'), mo.group('op2'),
                mo.group('addr'), mo.group('sym'), mo.group('offset')))
            return True
    # cbz    x22, 576ec <kiss_fftr_alloc@@Base+0xc4>
    if inst in ['cbz', 'cbnz']:
        mo = re.match("(?P<op1>.+), " + ADDR + SPACE + SYM_BASE_OFFSET, operands)
        if not mo:
             mo = re.match("(?P<op1>.+), " + ADDR + SPACE + SYM_OFFSET, operands)
        if mo:
            print("L{:x}:\t{} {}, L{} // <{}@@Base+0x{}>".format(addr, inst, mo.group('op1'),
                mo.group('addr'), mo.group('sym'), mo.group('offset')))
            return True

    # adrp    x9, 14a000 <av_sha_size@@Base+0x830>
    # adrp    x1, e1000 <_ZNSt13bad_exceptionD1Ev@@Base>
    # adrp    x8, 44d000 <_ZL16kLongLegacyError+0x1a9>
    if m.group('inst') == 'adrp':
        mo = re.match(REG + ", " + ADDR + SPACE + SYM_BASE_OFFSET, operands)
        if not mo:
            mo = re.match(REG + ", " + ADDR + SPACE + SYM_BASE, operands)
        if not mo:
            mo = re.match(REG + ", " + ADDR + SPACE + SYM_OFFSET, operands)
        if mo:
            print("L{:x}:\t{} {}, ?{}".format(addr, inst, mo.group('reg'),
                mo.group('addr')))
            return True
        return False

    # adr    x1, e127c <__cxa_get_globals@@Base+0x108>
    if m.group('inst') == 'adr':
        mo = re.match(REG + ", " + ADDR + SPACE + SYM_BASE_OFFSET, operands)
        if mo:
            print("L{:x}:\t{} {}, L{}".format(addr, inst, mo.group('reg'),
                                              mo.group('addr')))
            return True

    if '@' in m.group('operands'):
        return False
    if '+0x' in m.group('operands'):
        return False

    print("L{:x}:\t{} {}".format(addr, inst, operands))
    return True

while True:
    lineno = lineno + 1
    line = file.readline()
    if not line:
        break

    line = line.strip()
    if len(line) == 0:
        continue
    # #          kiss_fftr_alloc 0000000000057628 00000000000577e4 00000000000001bc .text
    # # 56d54:
    if line.startswith("#"):
        continue
    # .so:     file format elf64-littleaarch64
    if "file format" in line:
        continue
    # Disassembly of section .text:
    if line.startswith("Disassembly"):
        continue
    # 000000000044554c <sin>:
    # 0000000000000008 <syms@@Base>:
    m = re.match(ADDR + SPACE + SYM + ":", line)
    if not m:
        m = re.match(ADDR + SPACE + SYM_BASE + ":", line)
    if m:
        sym = m.group('sym')
        if sym not in local_symbols:
            local_symbols.append(sym)
        print("{0}:".format(sym))
        continue

    # 0000000000056d54 <kiss_fft_stride@@Base+0xb0>:
    m = re.match(ADDR + SPACE + SYM_BASE_OFFSET + ":", line)
    if m:
        continue

    #57628:    6db92beb     stp    d11, d10, [sp,#-112]!
    m = re.match(ADDR + ":" + SPACE + BIN + SPACE + INST + TAB + OPERANDS, line)

    if m:
        if process_insts(line, m):
            continue
    else:
    #577dc:    d65f03c0     ret
        m = re.match(ADDR + ":" + SPACE + BIN + SPACE + INST, line)
        if m and m.group('inst') in ['ret', 'nop']:
            print("L{:x}:\t{}".format(int(m.group('addr'), 16), m.group('inst')))
            continue

    sys.stdout.flush()
    sys.stderr.write("error at line #{0}\n".format(lineno))
    sys.stderr.write(line + "\n")
    sys.exit(-1)
    break

sys.stdout.flush()
sys.stderr.write("symbols:\n")

_symbols = []
for sym in symbols:
    if sym in unsafe_symbols:
        _symbols.append("(unsafe) " + sym)
    elif sym in local_symbols:
        _symbols.append("(local) " + sym)
    elif sym in static_symbols:
        _symbols.append("(static) " + sym)
    else:
        _symbols.append("" + sym)
for sym in sorted(_symbols):
    sys.stderr.write("  " + sym + "\n")
