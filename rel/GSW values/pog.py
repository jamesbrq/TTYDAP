instruction = 0x805F6AA8
base = 0x805F6838
strings = [
  ""
]

text = "キノシコワ"
hex_data = text.encode('shift_jis')
literal_hex_string = ''.join(f'\\x{byte:02X}' for byte in hex_data)
c_literal = f'const char jolene[] = "{literal_hex_string}";'

print(c_literal)

split = strings[0].split(":")[1:]

def print_strings():
  for i, string in enumerate(split):
    print(f"extern int32_t {string[:-13] if i != (len(split) - 1) else string}[];")

print_strings()
print(f"\n{int((instruction - base) / 4)}")