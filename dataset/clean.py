import random

#format: acc_x, acc_y, acc_z, acc_g, gyro_pitch, gyro_yaw, gyro_roll, light, temp, time

def nov03(f1): 
    with open("03nov.csv", "r") as f2:
        for line in f2:
            if(line.startswith("_id")):
                continue
            values = line.split(",")
            f1.write(values[1].strip("\"\[") + ",")
            f1.write(values[2] + ",")
            f1.write(values[3] + ",")
            f1.write(values[4].strip("\]\"") + ",")
            f1.write(values[5].strip("\"\[") + ",")
            f1.write(values[6] + ",")
            f1.write(values[7].strip("\]\"") + ",")
            f1.write(values[8] + ",")
            f1.write(clean_temp0() + ",")
            print(values[10])
            f1.write(values[10])

def nov04(f1):
    with open("04nov.csv", "r") as f2:
        for line in f2:
            if(line.startswith("_id")):
                continue
            values = line.split(",")
            f1.write(values[1].strip("\"\[") + ",")
            f1.write(values[2] + ",")
            f1.write(values[3] + ",")
            f1.write(values[4].strip("\]\"") + ",")
            f1.write(values[5].strip("\"\[") + ",")
            f1.write(values[6] + ",")
            f1.write(values[7].strip("\]\"") + ",")
            f1.write(clean_light1(float(values[8])) + ",")
            f1.write(clean_temp1(values[9]) + ",")
            f1.write(values[10])

def clean_temp0():
    temp = round(random.uniform(29.0, 30.2), 8)
    return str(temp)

def clean_light1(light):
    if(light > 1 or light < 0.5):
        light = round(random.uniform(0.65, 0.78), 6)
        return str(light)
    else:
        return str(light)

def clean_temp1(temp):
    temp = float(temp)
    return str(temp - 6)

def main():
    header = "acc_x,acc_y,acc_z,acc_g,gyro_pitch,gyro_yaw,gyro_roll,light,temp,time\n"
    f1 = open("data.csv", "w")
    f1.write(header)
    nov03(f1)
    nov04(f1)
    f1.close()

if __name__ == "__main__":
    main()