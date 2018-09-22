import re
import sys

file = open(sys.argv[1], 'r')

lineno = -1

symbols = []
unsafe_symbols = [
    'malloc', 'free',
    'fopen', 'fread', 'fwrite', 'fclose',
    'exit']

ADDR = "(?P<addr>[0-9A-Fa-f]+)"
SYM_PLT = "<(?P<sym>.+)@plt>"
SYM_BASE = "<(?P<sym>.+)@@Base>"
SYM_BASE_OFFSET = "<(?P<sym>.+)@@Base\+0x(?P<offset>[0-9A-Fa-f]+)>"
BIN = "(?P<bin>[0-9A-Fa-f]{8})"
INST = "(?P<inst>.+)"
OPERANDS = "(?P<operands>.+)"
REG = "(?P<reg>[x|d][0-9]+)"
SPACE = "\s+"
TAB = "\t"

def sym(sym):
    if sym in unsafe_symbols:
        return "o_" + sym
    if sym not in symbols:
        symbols.append(sym)
    return sym

def process_insts(line, m):
    inst = m.group('inst')
    addr = int(m.group('addr'), 16)
    operands = m.group('operands')

    if inst in ['b', 'bl'] or inst.startswith('b.'):
        # 38a90 <kiss_fft_alloc@plt>
        mo = re.match(ADDR + SPACE + SYM_PLT, operands)
        if mo:
            print("L{:x}:\t{} {}".format(addr, inst, sym(mo.group('sym'))))
            return True
        # 577ac <kiss_fftr_alloc@@Base+0x184>
        mo = re.match(ADDR + SPACE + SYM_BASE_OFFSET, operands)
        if mo:
            print("L{:x}:\t{} L{} // <{}@@Base+0x{}>".format(addr, inst,
                mo.group('addr'), mo.group('sym'), mo.group('offset')))
            return True

        return False

    # tbnz    w23, #0, 576c4 <kiss_fftr_alloc@@Base+0x9c>
    if inst in ['tbnz']:
        mo = re.match("(?P<op1>.+), (?P<op2>.+), " + ADDR + SPACE + SYM_BASE_OFFSET, operands)
        if mo:
            print("L{:x}:\t{} {}, {}, L{} // <{}@@Base+0x{}>".format(addr, inst, mo.group('op1'), mo.group('op2'),
                mo.group('addr'), mo.group('sym'), mo.group('offset')))
            return True
    # cbz    x22, 576ec <kiss_fftr_alloc@@Base+0xc4>
    if inst in ['cbz', 'cbnz']:
        mo = re.match("(?P<op1>.+), " + ADDR + SPACE + SYM_BASE_OFFSET, operands)
        if mo:
            print("L{:x}:\t{} {}, L{} // <{}@@Base+0x{}>".format(addr, inst, mo.group('op1'),
                mo.group('addr'), mo.group('sym'), mo.group('offset')))
            return True

    # adrp    x9, 14a000 <av_sha_size@@Base+0x830>
    if m.group('inst') == 'adrp':
        mo = re.match(REG + ", " + ADDR + SPACE + SYM_BASE_OFFSET, operands)
        print("L{:x}:\t{} {}, ?{}".format(addr, inst, mo.group('reg'),
                mo.group('addr')))
        return True

    if '@' in m.group('operands'):
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
    # 0000000000000008 <syms@@Base>:
    m = re.match(ADDR + SPACE + SYM_BASE + ":", line)
    if m:
        print("{0}:".format(m.group('sym')))
        continue
    # 0000000000056d54 <kiss_fft_stride@@Base+0xb0>:
    m = re.match(ADDR + SPACE + SYM_BASE_OFFSET + ":", line)
    if m:
        #print("L{:x}:".format(int(m.group('addr'), 16)))
        continue

    #57628:    6db92beb     stp    d11, d10, [sp,#-112]!
    m = re.match(ADDR + ":" + SPACE + BIN + SPACE + INST + TAB + OPERANDS, line)

    if m:
        if process_insts(line, m):
            continue
    else:
    #577dc:    d65f03c0     ret
        m = re.match(ADDR + ":" + SPACE + BIN + SPACE + INST, line)
        if m and m.group('inst') == 'ret':
            print("L{:x}:\t{}".format(int(m.group('addr'), 16), m.group('inst')))
            continue

    sys.stderr.write("error at line #{0}\n".format(lineno))
    sys.stderr.write(line + "\n")
    break

sys.stderr.write("symbols:\n")
for sym in sorted(symbols):
    sys.stderr.write("  " + sym + "\n")
