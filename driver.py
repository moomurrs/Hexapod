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

# state variables
last_status:bool = False
gait_controller:str = "tripod"

# called on valid controller values to blink the LEDs green and more servos
def progress_forward(gait:str):
    global led, robot, controller
    if gait == "tripod":
        led.bounce(0, 255, 0)
    elif gait == "wave":
        led.bounce(0, 0, 255)
    elif gait == "ripple":
        led.bounce(0, 255, 255)
    # update hexapod legs
    robot.update(controller.get_stick_angle("right"),
                 controller.get_stick_strength("right"),
                 gait)

try:
    while not user_sw.raw():
        # update controller values
        status:bool = controller.update()
        #print(controller.value["buttons"])
        if status == True:
            # good new controller value
            if controller.value["dpad"] == Controller.LEFT_DPAD:
                # use tripod gait
                gait_controller = "tripod"
                print("using tripod")
            if controller.value["dpad"] == Controller.UP_DPAD:
                # use ripple gait
                gait_controller = "ripple"
                print("using ripple")
            if controller.value["dpad"] == Controller.RIGHT_DPAD:
                # use wave gait
                gait_controller = "wave"
                print("using wave")
            
            progress_forward(gait_controller)
            last_status = True
        elif status == False:
            # bad controller value (disconnected?)
            led.flash_all(255, 0, 0)
            last_status = False
        elif status == None:
            # poll time hasn't expired, used last status instead
            # housekeeping to keep LEDs/servos working
            if last_status == True:
                # continue blinking led, moving servos while waiting for next
                # controller values
                progress_forward(gait_controller)
            else:
                # continue flashing
                led.flash_all(255, 0, 0)
            
        #sleep_ms(100)
        
        
except KeyboardInterrupt:
    print("exception")
    
led.turn_off()
