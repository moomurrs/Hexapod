from time import ticks_ms, ticks_diff
def is_timer_expired(wait_time:int, timer_dict:dict) -> bool:
    if timer_dict["in_progress"] is True:
        # timer is in progress
        current_time = ticks_ms()
        elapsed_time = ticks_diff(current_time, timer_dict["start_time"])
        
        if elapsed_time > timer_dict["duration"]:
            # timer expired, turn off
            timer_dict["in_progress"] = False
            timer_dict["start_time"] = 0
            timer_dict["duration"] = 0
            return True
        else:
            # timer hasn't expired
            return False
    else:
        # timer expired, reset
        current_time = ticks_ms()
        timer_dict["start_time"] = current_time
        timer_dict["duration"] = wait_time
        timer_dict["in_progress"] = True
        return False
def reset_timer(timer_dict:dict) -> None:
    timer_dict["in_progress"] = False
    timer_dict["start_time"] = 0
    timer_dict["duration"] = 0
