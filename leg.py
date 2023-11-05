from servo import Servo, servo2040
from math import atan2, cos, sin, acos, sqrt, degrees, radians, pi, trunc
from time import sleep_ms, ticks_ms
from wait import is_timer_expired, reset_timer
from tween import linear_interpolate
class Leg:
    
    # duration (ms) to complete a single stroke (either power or return stroke)
    period:int = 750
    
    # static segment length (mm)
    coxa_length:int = 27
    femur_length:int = 43
    tibia_length:int = 89
    
    x_max:int = 40
    x_min:int = -40
    y_max:int = 140
    y_center:int = 100
    y_min:int = 60
    z_ground:int = 60
    
    # base increment must be divisible by 6 and 12
    base_increment:float = 1 / 12 
     
    # initialize servos
    def __init__(self,
                 coxa_port:servo2040, coxa_cal, coxa_offset,
                 femur_port:servo2040, femur_cal, femur_offset,
                 tibia_port:servo2040, tibia_cal, tibia_offset,
                 id:str):
        
        self.coxa_port:Servo = Servo(coxa_port, coxa_cal)
        self.femur_port:Servo = Servo(femur_port, femur_cal)
        self.tibia_port:Servo = Servo(tibia_port, tibia_cal)
        self.id:str = id
        
        self.coxa_offset:int = coxa_offset
        self.femur_offset:int = femur_offset
        self.tibia_offset:int = tibia_offset
        
        
        # angle in rads
        self.coxa_angle:float = 0.0
        self.femur_angle:float = 0.0
        self.tibia_angle:float = 0.0
        
        self.leg_timer = {"start_time" : 0, "duration" : 0, "in_progress" : False}
        
        # each leg tracks its progress along its path
        self.cycle_progress:float = 0.0
        self.cycle_increment:float = Leg.base_increment
        self.power_stroke_multiplier:int = 1
        self.return_stroke_multiplier:int = 1
        
        
        # the legs are positioned around the body at different angles
        # its offset angle (degrees) must be considered before calculating inverse kinematics
        # R1:  45, R2:   0, R3: 315
        # L1: 135, L2: 180, L3: 225
        self.leg_angle_offset:int = 0
        if self.id is "R1":
            self.leg_angle_offset = 45
        elif self.id is "R2":
            self.leg_angle_offset = 0
        elif self.id is "R3":
            self.leg_angle_offset = 315
        elif self.id is "L1":
            self.leg_angle_offset = 135
        elif self.id is "L2":
            self.leg_angle_offset = 180
        elif self.id is "L3":
            self.leg_angle_offset = 225
        else:
            print("ERROR: bad leg name")
    
    # enable/disable leg servos
    def enable_leg(self, is_enable:bool):
        if is_enable:
            self.coxa_port.enable()
            self.femur_port.enable()
            self.tibia_port.enable()
            print("enabled: " + self.id)
        else:
            if self.coxa_port.is_enabled():
                self.coxa_port.disable()
            if self.femur_port.is_enabled():
                self.femur_port.disable()
            if self.tibia_port.is_enabled():
                self.tibia_port.disable()
                
            print("disabled: " + self.id)
    
    
    def change_leg_timings(self, cycle:float, is_power_stroke:bool, power_mult:int, return_mult:int):
        self.cycle_progress = cycle
        
        if power_mult < 1 or return_mult < 1:
            print("WARNING: power/return multiplier cannot be less than 0, using default")
            return
        
        self.power_stroke_multiplier = power_mult
        self.return_stroke_multiplier = return_mult
        
        #self.cycle_increment = increment
        if is_power_stroke:
            # positive increment is power stroke
            self.cycle_increment = Leg.base_increment / self.power_stroke_multiplier
        else:
            # negative increment is return stroke
            self.cycle_increment = -Leg.base_increment / self.return_stroke_multiplier
        
        
    
    # mm units for xyz
    def move_to_xyz(self, x:int, y:int, z:int):
        target_coxa:int
        target_femur:int
        target_tibia:int
        
        target_coxa, target_femur, target_tibia = self.solve_ir(x, y, z)
        #print("ir: coxa angle " + str(target_coxa))
        #print("ir: femur angle " + str(target_femur))
        #print("ir: tibia angle " + str(target_tibia))
        self.coxa_value(target_coxa)
        self.femur_value(target_femur)
        self.tibia_value(target_tibia)
    
    def solve_ir(self, x:int, y:int, z:int):
        # joints angles in degrees to solve
        target_coxa:int = 0
        target_femur:int = 0
        target_tibia:int = 0
        
        # distance between end of tibia to femur rotation
        # hypotenuse exists in 3D space, must consider x,y,z
        # hypotenuse doesn't include coxa length, must remove that length from y
        hypotenuse:int = sqrt(pow(z, 2) + pow(y - self.coxa_length, 2) + pow(x, 2))
        
        
        ####### SOLVING FEMUR ANGLE #######
        
        # angle (in rads) between hypotenuse to femur right angle
        # used to derive femur angle
        epsilon:float = atan2(z, y - self.coxa_length)
        
        # angle (int rads) between hypotenuse to femur length
        D:float = acos((pow(hypotenuse, 2) + pow(self.femur_length, 2) - pow(self.tibia_length, 2))
                       /
                       (2 * hypotenuse * self.femur_length))
        
        # solved femur angle in degrees
        target_femur = round(degrees(D - epsilon))
        
        
        ####### SOLVING TIBIA ANGLE #######
        
        target_tibia = round(degrees(acos((pow(self.femur_length, 2) + pow(self.tibia_length, 2) - pow(hypotenuse, 2))
                            /
                            (2 * self.femur_length * self.tibia_length))))
        
        
        ####### SOLVING COXA ANGLE #######
        
        # solved coxa angle in degrees
        target_coxa = round(degrees(atan2(x, y)))
        return target_coxa, target_femur, target_tibia
    
    
    # test functions that move only in x, y, or z axis
    # used to verify legs can move in a straight line
    def move_x(self, min:int, max:int) -> None:
        for i in range(99):
            for x in range(max, min - 1, -1):
                self.move_to_xyz(x, 100, 60)
                sleep_ms(10)
            for x in range(min, max + 1):
                self.move_to_xyz(x, 100, 60)
                sleep_ms(10)
    
    def move_y(self) -> None:
        for i in range(99):
            for y in range(90, 141):
                self.move_to_xyz(0, y, 40)
                sleep_ms(10)
            for y in range(140, 90, -1):
                self.move_to_xyz(0, y, 40)
                sleep_ms(10)
        
    def move_y2(self) -> None:
        y = 90
        for i in range(99):
            while True:
                if self.wait_leg(10) and y < 141:
                    # ready for new location
                    self.move_to_xyz(0, y, 40)
                    #print(y)
                    y += 1
                elif y is 141:
                    reset_timer(self.leg_timer)
                    break
            # retract in y
            while True:
                if self.wait_leg(10) and y > 90:
                    # ready for new location
                    y -= 1
                    self.move_to_xyz(0, y, 40)
                    #print(y)
                    
                elif y is 90:
                    reset_timer(self.leg_timer)
                    break
        
    def move_z(self) -> None:
        for i in range(99):
            for z in range(0, 71):
                self.move_to_xyz(0, 120, z)
                sleep_ms(10)
            for z in range(71, 0, -1):
                self.move_to_xyz(0, 120, z)
                sleep_ms(10)
    
    def move_circle(self) -> None:
        while True:
            # move in a circle, 80mm in diameter
            self.move_to_xyz(40 * sin(ticks_ms() / 500), 100 + 40 * cos(ticks_ms() / 500), 60)
            sleep_ms(10)
    # power stroke:  [0, 1)
    # return stroke: [1, 0)
    def increment_progress(self):
        if self.cycle_progress > 0.9999 :
            # switch to return stroke, from power stroke
            self.cycle_increment = -Leg.base_increment / self.return_stroke_multiplier
        elif self.cycle_progress < 0.0001:
            # switch to power stroke speed, from return stroke
            self.cycle_increment = Leg.base_increment / self.power_stroke_multiplier
         
        self.cycle_progress = self.cycle_progress + self.cycle_increment
        
        if self.cycle_progress > 0.999:
            self.cycle_progress = 1.0
        elif self.cycle_progress < 0.001:
            self.cycle_progress = 0.0
        
        print("progress: " + str(self.cycle_progress))
    
    def update_leg(self, directional_angle:int, stroke_length:int):
        #print("incoming progress: " + f'{self.cycle_progress:.8f}')
        trig_radius:float = round(linear_interpolate(self.cycle_progress, 0, 1, 1, -1), 2)
        #self.r1_progress = round(self.r1_progress + 0.1, 2) % 1.1
        #print("progress: " + str(current_progress))
        direction_rad:float = radians(directional_angle - self.leg_angle_offset)
        
        
        z:int = 60
        
        x:int = round(trig_radius * self.x_max * sin(direction_rad))
        y:int = round(self.y_center + (trig_radius * self.x_max * cos(direction_rad)))
        
        if self.cycle_increment < 0:
            #cos_input = linear_interpolate(self.r1_progress, 0, 1, 0, pi)
            #z = 60 - abs(round((20 * abs(cos(cos_input)))) - 20)
            sin_input = linear_interpolate(self.cycle_progress, 0, 1, 0, pi)
            z = 60 - round(20 * sin(sin_input))
        
        #print("xyz: " + str(x) + ", "  + str(y) + ", " + str(z))
        #self.move_to_xyz(x, y, z)
        self.increment_progress()
        
    
    def coxa_rawvalue(self, raw_angle:int) -> None:
        self.coxa_port.value(raw_angle)
    
    def femur_rawvalue(self, raw_angle:int) -> None:
        self.femur_port.value(-raw_angle)
            
    def tibia_rawvalue(self, raw_angle:int) -> None:
        self.tibia_port.value(raw_angle)
    
    def coxa_value(self, raw_angle:int) -> None:
        servo_range:int = 70
        adjusted_angle = raw_angle + self.coxa_offset
        if adjusted_angle <= servo_range and adjusted_angle >= -servo_range:
            self.coxa_port.value(adjusted_angle)
        else:
            print("WARNING: coxa input angle out of range")
    
    def femur_value(self, raw_angle:int) -> None:
        servo_range:int = 80
        adjusted_angle = raw_angle + self.femur_offset
        if adjusted_angle <= servo_range and adjusted_angle >= -servo_range:
            self.femur_port.value(-adjusted_angle)
        else:
            print("WARNING: femur input angle out of range")
            
    def tibia_value(self, raw_angle:int) -> None:
        tibia_limit:int = 45
        adjusted_angle = raw_angle + self.tibia_offset
        if adjusted_angle <= 180 and adjusted_angle >= tibia_limit:
            self.tibia_port.value(adjusted_angle)
        else:
            print("WARNING: tibia input angle out of range")
    
    def to_straight(self) -> None:
        self.coxa_value(0)
        self.femur_value(0)
        self.tibia_value(180)
    
    def to_right_angle(self) -> None:
        self.coxa_value(0)
        self.femur_value(0)
        self.tibia_value(90)
    
    def wait_leg(self, wait_time:int) -> bool:
        return is_timer_expired(wait_time, self.leg_timer)
    
    def wait_joint(self, joint_name:str, wait_time:int) -> None:
        if joint_name is "coxa":
            if is_timer_expired(wait_time, self.coxa_timer):
                # ready to assign new target
                print("coxa expired")
        elif joint_name is "femur":
            if is_timer_expired(wait_time, self.femur_timer):
                # ready to assign new target
                print("femur expired")
        elif joint_name is "tibia":
            if is_timer_expired(wait_time, self.tibia_timer):
                # ready to assign new target
                print("tibia expired")
        else:
            print("ERROR: wait joint name mismatch")


def ease_joint(current_time:int, start_time:int, end_time:int, start_angle:int, end_angle:int) -> None:
    new_servo_target = easeout_sine_tween(current_time, start_angle, end_time, start_angle, end_angle)

def easeout_sine_tween(input, in_min, in_max, start_angle, end_angle) -> float:
    
    # TODO:
    # [in_min, in_max] -> [0, pi/2] -> [out_min, out_max]
    
    out_min:float = 0.0
    out_max:float = 1.0
    if input < in_min:
        print("WARNING: sine tween min clamped")
        return out_min
    elif input > in_max:
        print("WARNING: sine tween max clamped")
        return out_max
    else:
        # map start/end movement time to [0, pi/2] using current time
        time_to_sine_input_interpolate:float = linear_interpolate(input, in_min, in_max, out_min, out_max)
        
        # create sine output
        sine_output_current = sin(time_to_sine_input_interpolate)
        
        # map sine output [0, 1] to servo start/end to get where servo should be at this time (keyframe)
        servo_current_target = linear_interpolate(sine_output_current, out_min, out_max, start_angle, end_angle)
        
        return None

def linear_tween(input, in_min, in_max) -> float:
    out_min:float = 0.0
    out_max:float = 1.0
    
    return linear_interpolate(input, in_min, in_max, out_min, out_max)
