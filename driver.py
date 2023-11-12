from servo import servo2040
from pimoroni import Button
from hexapod import Hexapod
from controller import Controller
from time import sleep_ms
from led import Led

# Create the user button
user_sw = Button(servo2040.USER_SW)

robot = Hexapod()
controller = Controller()
led = Led()

try:
    while not user_sw.raw():
        # update controller values
        # controller.update()
        # update hexapod legs
        #robot.update(controller.get_stick_angle("right"),
        #             controller.get_stick_strength("right"),
        #             "tripod")
        #sleep_ms(50)
        led.bounce(0, 255, 0, 0.7)
    #pass
        
except KeyboardInterrupt:
    print("exception")
    
led.turn_off()
