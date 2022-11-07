f1 = open("data.csv", "r")
f2 = open('data1.csv', "w")
f2.write("turbulence,light,temp,time\n")

for line in f1:
    if(line.startswith("acc")):
        continue
    values = line.split(",")
    moderated_values = []
    # get magnitude of accelerometer values (x, y, z, gforce)
    for i in range(4):
        val = abs(float(values[i]))
        if (val > 1):
            val = 1.0
        moderated_values.append(val * 1/7)
    # get magnitude of accelerometer values (roll, pitch, yaw)
    for i in range(4, 7):
        val = abs(float(values[i]))
        if (val > 180):
            val = 180.0
        moderated_values.append(val/180.0 * 1/7)
    result = sum(moderated_values)
    result = round(result, 7)

    f2.write(str(result) + ",")
    f2.write(values[7] + ",")
    f2.write(values[8] + ",")
    f2.write(values[9])

f1.close()
f2.close()