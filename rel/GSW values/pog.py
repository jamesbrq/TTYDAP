instruction = 0x805DF6CC
base = 0x805DF594

offset = int((instruction - base) / 4)
rel = 0x0001488c

skip = 3

text = "キノシコワ"
hex_data = text.encode('shift_jis')
literal_hex_string = ''.join(f'\\x{byte:02X}' for byte in hex_data)
c_literal = f'const char jolene[] = "{literal_hex_string}";'

#print(c_literal)
print(f"\n{int(offset)}")
print(f"\n0x" + f"{format(rel + ((offset + skip) * 4), 'x')}".upper())