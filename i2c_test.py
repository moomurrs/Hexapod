from machine import I2C, Pin
from time import sleep_ms
i2c = I2C(0, scl=Pin(21), sda=Pin(20), freq=100_000)

print("device id: " + str(i2c.scan()))

# 1st value is garbage
i2c.readfrom(0x55, 2)

while True:
    mask = 0x000000000000FFFF
    # 4 bytes containing 2 int (each int is 2 bytes)
    hex_stream = i2c.readfrom(0x55, 8)
    # convert higher 2 bytes into own int
    rx = int.from_bytes(hex_stream, "big") >> (8 * 6)
    # convert lower 2 bytes into own int
    ry = (int.from_bytes(hex_stream, "big") >> (8 * 4)) & mask
    
    
    lx = (int.from_bytes(hex_stream, "big") >> (8 * 2)) & mask
    # convert lower 2 bytes into own int
    ly = int.from_bytes(hex_stream, "big") & mask
    
    
    print("rx: " + str(rx) + ", ry: " + str(ry) + " lx: " + str(lx) + ", ly: " + str(ly))
    sleep_ms(500)
