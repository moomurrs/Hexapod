from time import ticks_ms, ticks_diff

class Timer:
    
    def __init__(self, wait_time_ms:int):
        current_time = ticks_ms()
        # start the timer as soon as its instantiated
        self.timer:dict = {"start_time" : current_time,
                           "time_to_wait" : wait_time_ms,
                           "in_progress" : False}

    def is_timer_expired(self) -> bool:
        if self.timer["in_progress"] is True:
            # timer is in progress
            # calculate elapsed time
            current_time = ticks_ms()
            elapsed_time = ticks_diff(current_time, self.timer["start_time"])
            
            if elapsed_time > self.timer["time_to_wait"]:
                # timer expired, turn off
                self.timer["in_progress"] = False
                self.timer["start_time"] = 0
                return True
            else:
                # timer hasn't expired
                return False
        else:
            # timer expired, reset
            self.reset_timer()
            return False
    def reset_timer(self) -> None:
        current_time = ticks_ms()
        self.timer["start_time"] = current_time
        self.timer["in_progress"] = True

