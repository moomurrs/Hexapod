from machine import I2C, Pin
from time import sleep_ms
from tween import linear_interpolate
i2c = I2C(0, scl=Pin(21), sda=Pin(20), freq=100_000)

print("device id: " + str(i2c.scan()))

# 1st value is garbage
i2c.readfrom(0x55, 2)

# raw controller value
rx_raw:int = 0
ry_raw:int = 0
lx_raw:int = 0
ly_raw:int = 0

# normalized controller value
rx:float = 0.0
ry:float = 0.0
lx:float = 0.0
ly:float = 0.0

while True:
    mask = 0x000000000000FFFF
    # 4 bytes containing 2 int (each int is 2 bytes)
    hex_stream = i2c.readfrom(0x55, 8)
    # convert higher 2 bytes into own int
    rx_raw = int.from_bytes(hex_stream, "big") >> (8 * 6)
    # convert lower 2 bytes into own int
    ry_raw = (int.from_bytes(hex_stream, "big") >> (8 * 4)) & mask
    
    
    lx_raw = (int.from_bytes(hex_stream, "big") >> (8 * 2)) & mask
    # convert lower 2 bytes into own int
    ly_raw = int.from_bytes(hex_stream, "big") & mask
    
    # normalize controller value
    # PS4 controller has min value -508, max value 512
    if ry_raw < 0:
        ry = linear_interpolate(ry_raw, 0, -508, 0.0, 1.0)
    else:
        ry = linear_interpolate(ry_raw, 0, 512, 0.0, 1.0)
    
    if rx_raw < 0:
        rx = linear_interpolate(rx_raw, 0, -508, 0.0, 1.0)
    else:
        rx = linear_interpolate(rx_raw, 0, 512, 0.0, 1.0)
    
    print("raw rx: " + str(rx_raw) + ", normalized rx: " + str(rx) + "raw ry: " + str(ry_raw) + ", normalized ry: " + str(ry))
    #print("rx: " + str(rx) + ", ry: " + str(ry) + " lx: " + str(lx) + ", ly: " + str(ly))
    sleep_ms(500)
