from servo import Calibration, Servo, servo2040


# Create an empty calibration
cal = Calibration()

# Give it a range of -45 to 45 degrees, corresponding to pulses of 1000 and 2000 microseconds
#cal.apply_two_pairs(500, 2600, -90, 90)
#cal.apply_two_pairs(500, 2600, -90, 90)
cal.apply_three_pairs(600, 1520, 2410, -90, 0, 90)
#cal.apply_two_pairs(650, 2470, -90, 90)

# Turn on the lower and upper limits, so the servo can't go beyond 45 degrees
cal.limit_to_calibration(True, True)

# Create a servo on pin 0 using the custom calibration and confirmed it worked
one = Servo(servo2040.SERVO_1, cal)
#two = Servo(servo2040.SERVO_2, cal)
#three = Servo(servo2040.SERVO_3, cal)
#four = Servo(servo2040.SERVO_18, cal)
print("Custom Servo:", one.calibration(), end="\n\n")

def value(v):
    global one
    if(v >= -90 and v <= 90):
        one.value(v)

def pulse(p):
    global one
    if(p >= 500 and p <= 2600):
        one.pulse(p)

def min():
    global one#, two, three, four
    one.to_min()
    #two.to_min()
    #three.to_min()
    #four.to_min()

def mid():
    global one#, two, three, four
    one.to_mid()
    #two.to_mid()
    #three.to_mid()
    #four.to_mid()

def max():
    global one#, two, three, four
    one.to_max()
    #two.to_max()
    #three.to_max()
    #four.to_max()