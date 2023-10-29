from math import degrees, radians, sin, cos, pi
from time import ticks_ms, sleep_ms
from pimoroni import Button
from servo import Servo, servo2040
from leg import Leg
from calibration import r1_leg_cal
from tween import linear_interpolate
from wait import is_timer_expired, reset_timer

user_sw = Button(servo2040.USER_SW)


#l1:Leg = Leg(servo2040.SERVO_1, servo2040.SERVO_2, servo2040.SERVO_3, "L1")
#l2:Leg = Leg(servo2040.SERVO_4, servo2040.SERVO_5, servo2040.SERVO_6, "L2")

#legs:list = [l1, l2]
#l1.enable_leg(True)
#l2.enable_leg(True)


class Hexapod:
    # static members
    
    
    #r1_progress:float = 0.0
    #r1_increment:float = 0.1
    timer:dict = {"start_time" : 0, "duration" : 0, "in_progress" : False}
    
    def __init__(self):
        self.r1:Leg = Leg(servo2040.SERVO_3, r1_leg_cal["coxa_cal"], 0,
                            servo2040.SERVO_2, r1_leg_cal["femur_cal"], 4,
                            servo2040.SERVO_1, r1_leg_cal["tibia_cal"], 0,
                            "R1")
    
    def update(self, directional_angle:int, strength:float, gait_name:str):
        if gait_name is "tripod":
            self.r1.update_leg(directional_angle, strength)
            
            sleep_ms(100)
    
    def move_leg_xyz(self, leg_name:str, x:int, y:int, z:int):
        if leg_name is "R1":
            self.r1.move_to_xyz(x, y, z)
        else:
            print("bad input")
            
    
            
            
                