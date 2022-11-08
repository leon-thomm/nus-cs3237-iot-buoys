import numpy
from sklearn.preprocessing import StandardScaler, MinMaxScaler

values = [-1, -0.566, 0.208, 1.026, 0.003, 0.365, -0.164]
out = []
# acc_x, acc_y
for i in range(2):
    val = abs(float(values[i]))
    if (val > 1):
        val = 1.0
    out.append(val/7)
# acc_z, acc_g (only supports from 0<g<2)
for i in range(2,4):
    val = abs(float(values[i]) - 1)
    if (val > 1):
        val = 1.0
    out.append(val/7)
# gyro values
for i in range(4, 7):
    val = abs(float(values[i]))
    if (val > 360):
        val = 360.0
    out.append(val/7)

def standardise(dataset):
    dataX = []
    for i in dataset:
        a = [i]
        dataX.append(a)
    dataX = numpy.array(dataX)
    
    scaler = StandardScaler()
    dataX = scaler.fit_transform(dataX)
    out = []
    for j in dataX:
        out.append(j[0])
    return out

# result = round(sum(out), 7)
# print(result)
# a1 = [0.35, 0.45, 0.35, 0.45, 0.35, 0.3, 0.3, 0.4, 0.4, 0.4]
# print(standardise(a1))
# a1 = [35.15, 35.25, 35.15, 35.25, 35.15, 35.2, 35.2, 35.2, 35.2, 35.2]
# print(standardise(a1))
# a2 = [26.1, 26, 26, 26, 26, 26, 26, 26, 26, 26]
# print(standardise(a2))
# a3 = [29.1, 29.2, 29.1, 29.3, 29.1, 29.2, 29.1, 29.0, 29.1, 29.1]
# print(standardise(a3))

def process_temp(at):
    out = []
    sum = 0
    for i in at:
        sum += i
    avg = sum/len(at)
    x = (avg-4.46) * 100
    x = x - 0.19
    #x = x/0.19
    #print(x)
    print(x * 100)

under = [4.3546724, 4.5567408, 4.491704, 4.4374247, 4.531048,  4.4096804, 4.3473077, 4.531853,  4.526977, 4.4306808]
over = [4.351223, 4.559252, 4.493246, 4.43139, 4.5351095, 4.4003735, 4.3492775,
  4.5357146, 4.5314913, 4.4330187]
ok = [4.3524585, 4.5583563, 4.4926887, 4.433568, 4.533655, 4.4037323, 4.34855654 ,4.5343323, 4.5298743, 4.4321713]
fukt = [
      4.351254940032959,
      4.559228897094727,
      4.493231296539307,
      4.4314470291137695,
      4.53507137298584,
      4.400461673736572,
      4.3492584228515625,
      4.535678386688232,
      4.531449317932129,
      4.432995796203613
    ]

process_temp(under)
process_temp(over)
process_temp(ok)
process_temp(fukt)