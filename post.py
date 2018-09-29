import os
import sys
import re

file = sys.argv[1]
results = os.popen("cat " + file + " | grep adrp -A 5")

tmp = {}
replaces = []

LABEL = "L(?P<label>[0-9A-Fa-f]+):"
REG = "(?P<reg>[x|d|q][0-9]+)"
REG2 = "(?P<reg2>[x|d|q][0-9]+)"
ADDR = "(?P<addr>[0-9A-Fa-f]+)"
OFFSET = "#(?P<offset>[0-9A-Fa-f]+)"
OFFSET_HEX = "#0x(?P<offset>[0-9A-Fa-f]+)"

while True:
    line = results.readline()
    if not line:
        break
    if line.startswith('--'):
        tmp = {}
        continue
    # L583bc:    adrp x8, ?14a000
    mo = re.match(LABEL + "\tadrp " + REG + ", \?" + ADDR, line)
    if mo:
        tmp[mo.group('reg')] = mo.groupdict()
        continue
    # L56e40:    add x9, x9, #0x8d8
    mo = re.match(LABEL + "\tadd " + REG + ", " + REG2 + ", " + OFFSET_HEX, line)
    if mo:
        reg = mo.group('reg2')
        if reg in tmp:
            local = {}
            addr = "LC_{:x}".format(int(tmp[reg]['addr'], 16) + int(mo.group('offset'), 16))
            s = "/^L{}:/s/?{}/{}/".format(tmp[reg]['label'], tmp[reg]['addr'], addr)
            replaces.append(s)
            s = "/^L{}:/s/\#0x{}/\#:lo12:{}/".format(mo.group('label'), mo.group('offset'), addr)
            replaces.append(s)
            del tmp[reg]
    # L57fc0:    ldr x23, [x23,#2912]
    # L57ff8:    ldr q0, [x8,#2224]
    # L5814c:    ldr d0, [x8,#2368]
    # L58494:    ldr x8, [x8,#2912]
    mo = re.match(LABEL + "\tldr " + REG + ", \[" + REG2 + "," + OFFSET, line)
    if mo:
        reg = mo.group('reg2')
        if reg in tmp:
            local = {}
            addr = "LC_{:x}".format(int(tmp[reg]['addr'], 16) + int(mo.group('offset')))
            s = "/^L{}:/s/?{}/{}/".format(tmp[reg]['label'], tmp[reg]['addr'], addr)
            replaces.append(s)
            s = "/^L{}:/s/\#{}/\#:lo12:{}/".format(mo.group('label'), mo.group('offset'), addr)
            replaces.append(s)
            del tmp[reg]

for s in replaces:
    cmd = "sed -i '' {} {}".format(s, file)
    os.system("sed -i '' {} {}".format(s, file))

os.system("cat {} >> {}".format("post." + file, file))
