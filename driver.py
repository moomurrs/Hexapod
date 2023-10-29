from servo import Servo, servo2040
from leg import Leg
from pimoroni import Button
from calibration import r1_leg_cal
from hexapod import Hexapod

# Create the user button
user_sw = Button(servo2040.USER_SW)

# Create a servo on pin 0 using the custom calibration and confirmed it worked
#one = Servo(servo2040.SERVO_1, cal)

#r1:Leg = Leg(servo2040.SERVO_3, r1_leg_cal["coxa_cal"], 0,
#             servo2040.SERVO_2, r1_leg_cal["femur_cal"], 4,
#             servo2040.SERVO_1, r1_leg_cal["tibia_cal"], 0,
#             "R1")

#while True:    
#    r1.wait_joint("coxa", 1000)
#    r1.wait_joint("femur", 2000)
#    r1.wait_joint("tibia", 3000)

my_pod = Hexapod()

try:
    while True:
        my_pod.update(90, 0, "tripod")
    #pass
        
except KeyboardInterrupt:
    print("exception")