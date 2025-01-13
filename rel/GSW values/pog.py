instruction = 0x805C281C
base = 0x805C2610
strings = [
  "13,5,000006dc:door_teresa_n\
13,5,00000990:door_teresa_s\
13,5,00000d8c:first_jin00\
13,5,00000ea0:push_evt\
13,5,00001358:evt_key_jin\
13,5,000013d0:evt_unlock_jin\
13,5,000014d8:tyuou_teresa_init\
13,5,00001564:tyuou_teresa_talk\
13,5,00001b80:oku_teresa_init\
13,5,00001bd4:temae_teresa_init\
13,5,000022e4:senteresa_evt\
13,5,00003664:jin_00_init_evt\
13,5,00004bb0:ranperu_toujou\
13,5,00005240:irekawatta_mario\
13,5,00005b00:ranperu_lastbattle\
13,5,000062dc:hontonosugata_mario\
13,5,000071ac:ranperu_init\
13,5,000071fc:nisemario_init\
13,5,000074d8:jin_04_jumpstand\
13,5,000077ac:jin_04_init_evt\
13,5,00007ef8:jin_06_init_evt\
13,5,00008634:evt_teresa_box_open\
13,5,00008f34:jin_07_init_evt\
13,5,00009398:evt_open\
13,5,00009614:oumu_talk\
13,5,0000979c:oumu_check\
13,5,00009c88:jin_08_init_evt\
13,5,0000adb4:evt_yuka\
13,5,0000adc8:jin_11_init_evt\
13,5,0000af00:evt_usu_kagemario_party_kill_jin"
]

text = "にせマリオ"
hex_data = text.encode('shift_jis')
literal_hex_string = ''.join(f'\\x{byte:02X}' for byte in hex_data)
c_literal = f'const char doopliss[] = "{literal_hex_string}";'

#print(c_literal)

split = strings[0].split(":")[1:]

def print_strings():
  for i, string in enumerate(split):
    print(f"extern int32_t {string[:-13] if i != (len(split) - 1) else string}[];")

print_strings()
print(f"\n{int((instruction - base) / 4)}")