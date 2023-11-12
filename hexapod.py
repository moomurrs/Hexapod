from math import degrees, radians, sin, cos, pi
from time import ticks_ms, sleep_ms
from pimoroni import Button
from servo import Servo, servo2040
from leg import Leg
from calibration import r1_leg_cal, r2_leg_cal, r3_leg_cal, l1_leg_cal, l2_leg_cal, l3_leg_cal
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
    r1_progress:float = 0.0
    r1_increment:float = 0.1
    timer:dict = {"start_time" : 0, "duration" : 0, "in_progress" : False}
    # initial gait is always empty, request a new gait on instande
    current_gait:str = None
    
    def __init__(self):
        self.r1:Leg = Leg(servo2040.SERVO_3, r1_leg_cal["coxa_cal"], 0,
                            servo2040.SERVO_2, r1_leg_cal["femur_cal"], 4,
                            servo2040.SERVO_1, r1_leg_cal["tibia_cal"], 0,
                            "R1")
        self.r2:Leg = Leg(servo2040.SERVO_6, r2_leg_cal["coxa_cal"], 0,
                            servo2040.SERVO_5, r2_leg_cal["femur_cal"], 0,
                            servo2040.SERVO_4, r2_leg_cal["tibia_cal"], 0,
                            "R2")
        self.r3:Leg = Leg(servo2040.SERVO_9, r3_leg_cal["coxa_cal"], 0,
                            servo2040.SERVO_8, r3_leg_cal["femur_cal"], 0,
                            servo2040.SERVO_7, r3_leg_cal["tibia_cal"], 0,
                            "R3")
        self.l1:Leg = Leg(servo2040.SERVO_12, l1_leg_cal["coxa_cal"], 0,
                            servo2040.SERVO_11, l1_leg_cal["femur_cal"], -4,
                            servo2040.SERVO_10, l1_leg_cal["tibia_cal"], 0,
                            "L1")
        self.l2:Leg = Leg(servo2040.SERVO_15, l2_leg_cal["coxa_cal"], 0,
                            servo2040.SERVO_14, l2_leg_cal["femur_cal"], 0,
                            servo2040.SERVO_13, l2_leg_cal["tibia_cal"], 0,
                            "L2")
        self.l3:Leg = Leg(servo2040.SERVO_18, l3_leg_cal["coxa_cal"], 0,
                            servo2040.SERVO_17, l3_leg_cal["femur_cal"], 0,
                            servo2040.SERVO_16, l3_leg_cal["tibia_cal"], 0,
                            "L3")
    
    def update(self, directional_angle:int, strength:float, gait_name:str):
        if gait_name is "tripod":
            if gait_name == self.current_gait:
                # continue tripod gait as normal
                #self.r1.update_leg(directional_angle, strength)
                #self.r2.update_leg(directional_angle, strength)
                self.r3.update_leg(directional_angle, strength)
                #self.l1.update_leg(directional_angle, strength)
                #self.l2.update_leg(directional_angle, strength)
                #self.l3.update_leg(directional_angle, strength)
                sleep_ms(10)
            else:
                # new gait request, set legs to tripod gait offset
                self.current_gait = "tripod"
                
                # group 1
                self.r1.change_leg_timings(0.0, # position leg at start of power stroke
                                           True, # specify power stroke increment
                                           1,   # indicate how long power stroke should be (5x long)
                                           1)   # indicate how long return stroke should be
                self.l2.change_leg_timings(0.0, True, 1, 1)
                self.r3.change_leg_timings(0.0, True, 1, 1)
                
                # group 2
                self.l1.change_leg_timings(1.0, False, 1, 1)
                self.r2.change_leg_timings(1.0, False, 1, 1)
                self.l3.change_leg_timings(1.0, False, 1, 1)
        
        if gait_name is "wave":
            if gait_name == self.current_gait:
                # continue wave gait as normal
                self.r1.update_leg(directional_angle, strength)
                self.r2.update_leg(directional_angle, strength)
                self.r3.update_leg(directional_angle, strength)
                self.l1.update_leg(directional_angle, strength)
                self.l2.update_leg(directional_angle, strength)
                self.l3.update_leg(directional_angle, strength)
                
                sleep_ms(10)
            else:
                # new gait request, set legs to tripod gait offset
                self.current_gait = "wave"
                
                self.r1.change_leg_timings(0, True, 5, 1)
                self.r2.change_leg_timings(1 / 5, True, 5, 1)
                self.r3.change_leg_timings(2 / 5, True, 5, 1)
                
                self.l1.change_leg_timings(3 / 5, True, 5, 1)
                self.l2.change_leg_timings(4 / 5, True, 5, 1)
                self.l3.change_leg_timings(5 / 5, True, 5, 1)
    
    def move_leg_xyz(self, leg_name:str, x:int, y:int, z:int):
        if leg_name is "R1":
            self.r1.move_to_xyz(x, y, z)
        elif leg_name is "R2":
            self.r2.move_to_xyz(x, y, z)
        elif leg_name is "R3":
            self.r3.move_to_xyz(x, y, z)
        elif leg_name is "L1":
            self.l1.move_to_xyz(x, y, z)
        elif leg_name is "L2":
            self.l2.move_to_xyz(x, y, z)
        elif leg_name is "L3":
            self.l3.move_to_xyz(x, y, z)
        else:
            print("bad input")

