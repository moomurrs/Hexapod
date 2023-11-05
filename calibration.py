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

# R2 coxa port 6
coxa_r2_cal = Calibration()
coxa_r2_cal.apply_three_pairs(515, 1440, 2305, -90, 0, 90)
# R2 femur port 5
femur_r2_cal = Calibration()
femur_r2_cal.apply_three_pairs(650, 1590, 2470, -90, 0, 90)
# R2 tibia port 4
tibia_r2_cal = Calibration()
tibia_r2_cal.apply_three_pairs(530, 1450, 2350, 0, 90, 180)
# R2 cal dictionary
r2_leg_cal:dict = {"coxa_cal" : coxa_r2_cal, "femur_cal" : femur_r2_cal, "tibia_cal" : tibia_r2_cal}

# R3 coxa port 9
coxa_r3_cal = Calibration()
coxa_r3_cal.apply_three_pairs(530, 1455, 2360, -90, 0, 90)
# R3 femur port 8
femur_r3_cal = Calibration()
femur_r3_cal.apply_three_pairs(650, 1550, 2460, -90, 0, 90)
# R3 tibia port 7
tibia_r3_cal = Calibration()
tibia_r3_cal.apply_three_pairs(630, 1630, 2570, 0, 90, 180)
# R3 cal dictionary
r3_leg_cal:dict = {"coxa_cal" : coxa_r3_cal, "femur_cal" : femur_r3_cal, "tibia_cal" : tibia_r3_cal}

# L1 coxa port 12
coxa_l1_cal = Calibration()
coxa_l1_cal.apply_three_pairs(560, 1495, 2400, -90, 0, 90)
# L1 femur port 11
femur_l1_cal = Calibration()
femur_l1_cal.apply_three_pairs(620, 1550, 2450, -90, 0, 90)
# L1 tibia port 10
tibia_l1_cal = Calibration()
tibia_l1_cal.apply_three_pairs(600, 1510, 2440, 0, 90, 180)
# L1 cal dictionary
l1_leg_cal:dict = {"coxa_cal" : coxa_l1_cal, "femur_cal" : femur_l1_cal, "tibia_cal" : tibia_l1_cal}

# L2 coxa port 15
coxa_l2_cal = Calibration()
coxa_l2_cal.apply_three_pairs(615, 1560, 2430, -90, 0, 90)
# L2 femur port 14
femur_l2_cal = Calibration()
femur_l2_cal.apply_three_pairs(565, 1475, 2375, -90, 0, 90)
# L2 tibia port 13
tibia_l2_cal = Calibration()
tibia_l2_cal.apply_three_pairs(660, 1610, 2500, 0, 90, 180)
# L2 cal dictionary
l2_leg_cal:dict = {"coxa_cal" : coxa_l2_cal, "femur_cal" : femur_l2_cal, "tibia_cal" : tibia_l2_cal}

# L3 coxa port 18
coxa_l3_cal = Calibration()
coxa_l3_cal.apply_three_pairs(600, 1520, 2425, -90, 0, 90)
# L3 femur port 17
femur_l3_cal = Calibration()
femur_l3_cal.apply_three_pairs(630, 1540, 2450, -90, 0, 90)
# L3 tibia port 16
tibia_l3_cal = Calibration() 
tibia_l3_cal.apply_three_pairs(500, 1425, 2350, 0, 90, 180)
# L3 cal dictionary
l3_leg_cal:dict = {"coxa_cal" : coxa_l3_cal, "femur_cal" : femur_l3_cal, "tibia_cal" : tibia_l3_cal}
