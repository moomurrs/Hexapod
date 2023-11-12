from machine import I2C, Pin
from tween import linear_interpolate
from servo import servo2040
from math import sqrt, atan2, degrees, pi
from wait import Timer
from time import ticks_ms, ticks_diff

class Controller:
    i2c = I2C(0, scl=Pin(servo2040.SCL), sda=Pin(servo2040.SDA), freq=100_000)
    def __init__(self):
        print("device id: " + str(Controller.i2c.scan()))
        # button and analog stick values dictionary
        self.value:dict = {"rx" : 0.0,
                           "ry" : 0.0,
                           "lx" : 0.0, # not used yet
                           "ly" : 0.0, # not used yet
                           "buttons": 0, # not used yet
                           "dpad" : 0, # not used yet
                           "misc" : 0 } # not used yet
        # right/left analog stick in polar coordinates
        self.right_stick:dict = {"radius" : 0.0,
                                 "theta" : 0.0}
        self.left_stick:dict = {"radius" : 0.0,
                                 "theta" : 0.0}
        self.timer = Timer(50)
        
        # first i2c request is garbage, throw away in constructor
        Controller.i2c.readfrom(0x55, 8)
    
    # update member variables by querying esp32 over i2c
    def update(self):
        if not self.timer.is_timer_expired():
            return
        # raw controller value
        rx_raw:int = 0
        ry_raw:int = 0
        lx_raw:int = 0
        ly_raw:int = 0
        one_byte_mask = 0x0000000000000000000000FF
        two_byte_mask = 0x00000000000000000000FFFF
        
        # read 12 bytes in big endian
        # expected stream must look like this:
        # (high) FFFF FFFF FFFF FFFF  FFFF    FF    FF (low)
        #         rx   ry   lx   ly  button  dpad   misc
        hex_stream = Controller.i2c.readfrom(0x55, 12) # bytes object
        
        # begin reading the byte steam from low end to higher end
        
        # get lowest byte as misc bitmask, shift 1 byte
        self.value["misc"] = int.from_bytes(hex_stream, "big") & one_byte_mask
        
        self.value["dpad"] = (int.from_bytes(hex_stream, "big") >> 8) & one_byte_mask
        
        # get lowest 2 bytes as buttons bitmask
        self.value["buttonsd"] = (int.from_bytes(hex_stream, "big") >> (8 * 2)) & two_byte_mask
        
        ly_raw = (int.from_bytes(hex_stream, "big") >> (8 * 4)) & two_byte_mask
        
        lx_raw = (int.from_bytes(hex_stream, "big") >> (8 * 6)) & two_byte_mask
        
        ry_raw = ((int.from_bytes(hex_stream, "big") >> (8 * 8)) & two_byte_mask) - 508
        
        rx_raw = ((int.from_bytes(hex_stream, "big") >> (8 * 10)) & two_byte_mask) - 508
        
        # normalize controller value
        # PS4 controller has min value -508, max value 512
        if ry_raw <= 0:
            # north quads
            self.value["ry"] = linear_interpolate(ry_raw, 0, -508, 0.0, 1.0)
        else:
            # south quads
            self.value["ry"] = linear_interpolate(ry_raw, 0, 512, 0.0, -1.0)
        
        if rx_raw <= 0:
            # east quads
            self.value["rx"] = linear_interpolate(rx_raw, 0, -508, 0.0, -1.0)
        else:
            # west quads
            self.value["rx"] = linear_interpolate(rx_raw, 0, 512, 0.0, 1.0)
        
        self.right_stick["radius"] = sqrt(pow(self.value["rx"], 2) + pow(self.value["ry"], 2))
        
        # clamp radius to 1
        if self.right_stick["radius"] > 1.0:
            self.right_stick["radius"] = 1.0
        
        self.right_stick["theta"] = degrees(atan2(self.value["ry"], self.value["rx"]))
        
        if self.right_stick["theta"] < 0:
            # theta is clockwise, we need counter-clockwise
            self.right_stick["theta"] = 180 + (180 + self.right_stick["theta"])
    
        
        #print("raw rx: " + str(rx_raw) + " normalized rx: " + str(self.value["rx"]) + ", raw ry: " + str(ry_raw) + " normalized ry: " + str(self.value["ry"]))
        #print("theta: " + str(self.get_stick_angle("right")) + ", radius: " + str(self.get_stick_strength("right")))
    
    def get_stick_angle(self, stick:str) -> float:
        if stick == "right":
            return self.right_stick["theta"]
        elif stick == "left":
            return self.left_stick["theta"]
        else:
            print("WARNING: bad stick name")
            return 0.0
    
    def get_stick_strength(self, stick:str) -> float:
        if stick == "right":
            return self.right_stick["radius"]
        elif stick == "left":
            return self.left_stick["radius"]
        else:
            print("WARNING: bad stick name")
            return 0.0
