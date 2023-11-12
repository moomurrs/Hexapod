from time import sleep_ms
from plasma import WS2812
from pimoroni import Button
from servo import servo2040
from wait import Timer

class Led:
    def __init__(self):
        # Create the LED bar, using PIO 1 and State Machine 0
        self.led_bar:WS2812 = WS2812(servo2040.NUM_LEDS, 1, 0, servo2040.LED_DATA)

        # Create the user button
        self.user_sw:Button = Button(servo2040.USER_SW)

        # Start updating the LED bar
        self.led_bar.start()
        
        self.timer:Timer = Timer(100)
        
        # used to bounce led
        self.i:int = 0
        self.direction:int = 1
    
    def bounce(self, r:int, g:int, b:int):
        if self.timer.is_timer_expired():
            # turn off current led
            self.led_bar.set_rgb(self.i, 0, 0, 0)
            # increment to next led
            self.i = (self.i + self.direction) % 6
            # turn on next led
            self.led_bar.set_rgb(self.i, r, g, b)
            # change direction to bounce
            if self.i == 5:
                self.direction = -1
            elif self.i == 0:
                self.direction = 1
    
    def turn_off(self):
        self.led_bar.clear()
    
    
