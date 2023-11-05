from servo import Servo, servo2040
from leg import Leg
from pimoroni import Button
from hexapod import Hexapod

# Create the user button
user_sw = Button(servo2040.USER_SW)

my_pod = Hexapod()

try:
    #while True:
    #    my_pod.update(45, 0, "tripod")
    pass
        
except KeyboardInterrupt:
    print("exception")
