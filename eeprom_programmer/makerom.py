rom = bytearray([0xea] * 32768)  # fill entire ROM with NOPs

# rom[0x7ffc] = 0x00
# rom[0x7ffd] = 0x80

with open("rom.bin", "wb") as f:
    f.write(rom)

print("rom.bin created")