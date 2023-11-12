from machine import I2C, Pin
from time import sleep_ms
from tween import linear_interpolate
from servo import servo2040
i2c = I2C(0, scl=Pin(servo2040.SCL), sda=Pin(servo2040.SDA), freq=100_000)
from math import sqrt, sin, atan2, degrees, pi

print("device id: " + str(i2c.scan()))

# 1st value is garbage
i2c.readfrom(0x55, 2)

# raw controller axis value
rx_raw:int = 0
ry_raw:int = 0
lx_raw:int = 0
ly_raw:int = 0

# normalized controller axis value
rx:float = 0.0
ry:float = 0.0
lx:float = 0.0
ly:float = 0.0

# bitmask of various button groups
buttons:int = 0;
dpad:int = 0;
misc:int = 0;

while True:
    one_byte_mask = 0x0000000000000000000000FF
    two_byte_mask = 0x00000000000000000000FFFF
    
    # read 12 bytes in big endian
    # expected stream must look like this:
    # (high) FFFF FFFF FFFF FFFF  FFFF    FF    FF (low)
    #         rx   ry   lx   ly  button  dpad   misc
    hex_stream = i2c.readfrom(0x55, 12) # bytes object
    
    # begin reading the byte steam from low end to higher end
    
    # get lowest byte as misc bitmask, shift 1 byte
    misc = int.from_bytes(hex_stream, "big") & one_byte_mask
    
    dpad = (int.from_bytes(hex_stream, "big") >> 8) & one_byte_mask
    
    # get lowest 2 bytes as buttons bitmask
    buttons = (int.from_bytes(hex_stream, "big") >> (8 * 2)) & two_byte_mask
    
    ly_raw = buttons = (int.from_bytes(hex_stream, "big") >> (8 * 4)) & two_byte_mask
    
    lx_raw = buttons = (int.from_bytes(hex_stream, "big") >> (8 * 6)) & two_byte_mask
    
    ry_raw = buttons = ((int.from_bytes(hex_stream, "big") >> (8 * 8)) & two_byte_mask) - 508
    
    rx_raw = buttons = ((int.from_bytes(hex_stream, "big") >> (8 * 10)) & two_byte_mask) - 508
    
    
    # normalize controller value
    # PS4 controller has min value -508, max value 512
    if ry_raw <= 0:
        # north quads
        ry = linear_interpolate(ry_raw, 0, -508, 0.0, 1.0)
    else:
        # south quds
        ry = linear_interpolate(ry_raw, 0, 512, 0.0, -1.0)
    
    if rx_raw <= 0:
        # east quads
        rx = linear_interpolate(rx_raw, 0, -508, 0.0, -1.0)
    else:
        # west quads
        rx = linear_interpolate(rx_raw, 0, 512, 0.0, 1.0)
     
    #rx = linear_interpolate(rx_raw, 0, 1020, -1.0, 1.0) / 1.414214
    #ry = linear_interpolate(ry_raw, 0, 1020, 1.0, -1.0) / 1.414214
    
    r = sqrt(rx * rx + ry * ry)
    theta = atan2(ry, rx)
    
    if theta < 0:
        # theta is clockwise, we need counter-clockwise
        theta = pi + (pi + theta)
     
    
    #print("raw rx: " + str(rx_raw) + ", normalized rx: " + str(rx) + "raw ry: " + str(ry_raw) + ", normalized ry: " + str(ry) + ", button: " + str(button_raw))
    #print("raw rx: " + str(rx_raw) + " raw ry: " + str(ry_raw) + ", raw lx: " + str(lx_raw) + ", raw ly: " + str(ly_raw) + ", buttons: " + str(buttons) + ", dpad: " + str(dpad) + ", misc: " + str(misc))
    #print("raw rx: " + str(rx_raw) + " raw ry: " + str(ry_raw))
    #print("rx: " + str(rx) + ", ry: " + str(ry) + ", lx: " + str(lx) + ", ly: " + str(ly))
    #print("theta: " + str(degrees(theta)) + ", radius: " + str(r))
    #print("raw rx: " + str(rx_raw) + " raw ry: " + str(ry_raw) + " rx: " + str(rx) + ", ry: " + str(ry) + ", radius: " + str(r) + "theta: " + str(degrees(theta)) )
    print("radius: " + str(r) + "theta: " + str(degrees(theta)) + "mult: " + str(m))
    sleep_ms(50)

