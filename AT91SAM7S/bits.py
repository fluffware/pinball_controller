
def build_bits(value):
    bits = 0
    s = 0;
    while(s < 24):
        if (value & 1) != 0:
            bits |= 6 << s;
        else:
            bits |= 4 << s;
        value >>= 1
        s += 3
    return bits


for v in range(0,8):
    print(("0x%02x"%(build_bits(v)&0xff)),end=",")
print()
    
for v in range(0,4):
    print("0x%02x"%((build_bits(v*8)&0xff00)>>8),end=",")
print()

for v in range(0,8):
    print("0x%02x"%((build_bits(v*32)&0xff0000)>>16),end=",")
print()
