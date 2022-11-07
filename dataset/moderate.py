def moderate():
    f1 = open("data.csv", "r")
    f2 = open('data-moderated.csv', "w")
    header = "acc_x,acc_y,acc_z,acc_g,gyro_pitch,gyro_yaw,gyro_roll,light,temp,time\n"
    f2.write(header)
    for line in f1:
        if(line.startswith("acc")):
            continue
        values = line.split(",")
        # acc_x, acc_y
        for i in range(2):
            val = abs(float(values[i]))
            if (val > 1):
                val = 1.0
            f2.write(str(val) + ",")
        # acc_z, acc_g (only supports from 0<g<2)
        for i in range(2,4):
            val = abs(float(values[i]) - 1)
            if (val > 1):
                val = 1.0
            f2.write(str(val) + ",")
        # gyro values
        for i in range(4, 7):
            val = abs(float(values[i]))
            if (val > 360):
                val = 360.0
            f2.write(str(val/360.0) + ",")
        # light
        val = float(values[7])
        if (val > 1):
            val = 1.0
        f2.write(str(val) + ",")
        # temp
        f2.write(values[8] + ",")
        # time
        f2.write(values[9])
    f1.close()
    f2.close()

def aggregate():
    f1 = open("data-moderated.csv", "r")
    f2 = open("data-aggregated.csv", "w")
    f2.write("turbulence,light,temp,time\n")

    for line in f1:
        if(line.startswith("acc")):
            continue
        values = line.split(",")
        moderated_values = []
        for i in range(7):
            val = abs(float(values[i]))
            if (val > 1):
                val = 1.0
            moderated_values.append(val* 1/7)
        result = round(sum(moderated_values), 7)
        f2.write(str(result) + ",")
        f2.write(values[7] + ",")
        f2.write(values[8] + ",")
        f2.write(values[9])
    f1.close()
    f2.close()

#moderate()
aggregate()