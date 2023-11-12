# standalone function for uniform interpolation (similiar to map() on Arduino)
def linear_interpolate(input, in_min, in_max, out_min, out_max) -> float:
    # valid input is within in_min & in_max range
    if in_min > in_max:
        # inverse interpolation
        if input > in_min:
            print("WARNING: linear tween min clamped")
            return out_min
        elif input < in_max:
            print("WARNING: linear tween max clamped")
            return out_max
    else:
        # normal interpolation
        if input < in_min:
            print("WARNING: linear tween min clamped")
            return out_min
        elif input > in_max:
            print("WARNING: linear tween max clamped")
            return out_max
    
    # input validated, interpolate
    return (((input - in_min) * (out_max - out_min)) / ((in_max - in_min)) + out_min)

