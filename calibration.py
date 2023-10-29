from servo import Calibration
# R1 coxa port 3
coxa_r1_cal = Calibration()
coxa_r1_cal.apply_three_pairs(625, 1525, 2425, -90, 0, 90)
# R1 femur port 2
femur_r1_cal = Calibration()
femur_r1_cal.apply_two_pairs(500, 2320, -90, 90)
# R1 tibia port 1
tibia_r1_cal = Calibration()
tibia_r1_cal.apply_three_pairs(680, 1590, 2470, 0, 90, 180)
# R1 cal dictionary
r1_leg_cal:dict = {"coxa_cal" : coxa_r1_cal, "femur_cal" : femur_r1_cal, "tibia_cal" : tibia_r1_cal}