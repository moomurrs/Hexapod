from time import sleep_ms
from plasma import WS2812
from pimoroni import Button
from servo import servo2040

# Create the LED bar, using PIO 1 and State Machine 0
led_bar = WS2812(servo2040.NUM_LEDS, 1, 0, servo2040.LED_DATA)

# Create the user button
user_sw = Button(servo2040.USER_SW)

# Start updating the LED bar
led_bar.start()

def bouncing_led():
    BRIGHTNESS:float = 0.7
    interval:int = 100
    color = .3
    for i in range(0, 6, 1):
        led_bar.set_hsv((i - 1) % 6, 113, 0.0, 0.0)
        led_bar.set_hsv(i, color, 1.0, BRIGHTNESS)
        if i is 0 or i is 5:
            sleep_ms(interval // 2)
        else:
            sleep_ms(interval)
        
    for i in range(5, -1, -1):
        led_bar.set_hsv((i + 1) % 6, 113, 0.0, 0.0)
        led_bar.set_hsv(i, color, 1.0, BRIGHTNESS)
        if i is 0 or i is 5:
            sleep_ms(interval // 2)
        else:
            sleep_ms(interval)

# Make rainbows until the user button is pressed
try:
    while not user_sw.raw():
        bouncing_led()
        
except KeyboardInterrupt:
    print("exception")

# Turn off the LED bar
led_bar.clear()