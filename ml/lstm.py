import pandas as pd
import numpy
import math
import os
import matplotlib.pyplot as plt
from tensorflow.keras.layers import LSTM, Dense
from tensorflow.keras.models import Sequential, load_model
from tensorflow.keras.callbacks import ModelCheckpoint
from sklearn.preprocessing import StandardScaler

MODEL_NAME = "lstm.hd5"
LOOKBACK = 10
FORECAST = 10
TRAIN_PER = 0.7

def create_dataset(data, lookback):
    dataX, dataY = [],[]
    for i in range(len(data) - lookback - 1):
        a = data[i:(i+lookback)]
        dataX.append(a)
        dataY.append(data[i+lookback])
    return numpy.array(dataX), numpy.array(dataY)
    
def build_model(name, trainX, trainY, testX, testY):
    if os.path.exists(name):
        model = load_model(name)
    else:
        model = Sequential()
        model.add(LSTM(128, input_shape=(3,10)))
        model.add(Dense(10))
        model.compile(loss='mean_squared_error', optimizer='adam')
        model.fit(trainX, trainY, epochs=20, batch_size=1, verbose=2)
        model.save(name)
    return model

def create_dataset1(ds_turbulence, ds_light, ds_temp, length, lookback=10):
    dataX, dataY = [], []
    for i in range(length - lookback - 1):
        current_batchX = []
        current_batchY = []

        a = ds_turbulence[i:(i+lookback)]
        current_batchX.append(a)
        current_batchY.append(ds_turbulence[i+lookback])
        b = ds_light[i:(i+lookback)]
        current_batchX.append(b)
        current_batchY.append(ds_light[i+lookback])
        c= ds_temp[i:(i+lookback)]
        current_batchX.append(c)
        current_batchY.append(ds_temp[i+lookback])

        dataX.append(current_batchX)
        dataY.append(current_batchY)
    return numpy.array(dataX), numpy.array(dataY)

def unshape_temp(ds_temp):
    temp = []
    for i in ds_temp:
        temp.append(i[0])
    return temp

### Read from csv
# df = pd.read_csv('data1.csv', usecols = [2], engine = "python")
# data = df.values
# data = data.astype('float32')
# scaler = MinMaxScaler(feature_range=(0,1))
# data = scaler.fit_transform(data)

# ### Split data into training and testing set
# train_size = int(len(data) * 0.7)
# test_size = len(data) - train_size
# train, test  = data[:train_size], data[train_size:]

# trainX, trainY = create_dataset(data, 10)
# testX, testY = create_dataset(data, 10)

# trainX = numpy.reshape(trainX, (trainX.shape[0], 1, trainX.shape[1]))
# testX = numpy.reshape(testX, (testX.shape[0], 1, testX.shape[1]))

#model = build_model(MODEL_NAME, trainX, trainY, testX, testY)

### READ DATA FROM CSV
scaler = StandardScaler()
df1 = pd.read_csv('data1.csv', usecols = [0,1,2], engine="python")
length = len(df1)
### SCALE TEMPERATURE TO STDEV
ds_temp = scaler.fit_transform(df1['temp'].values.reshape(-1, 1))
ds_temp = unshape_temp(ds_temp)

### SPLIT DATA
dataX, dataY = create_dataset1(df1['turbulence'].values, df1['light'].values, ds_temp, length)
train_size = int(length * TRAIN_PER)
test_size = length - train_size
trainX = dataX[:train_size]
trainY = dataY[:train_size]

print(trainX.shape, trainY.shape)
print(trainY)
#model = build_model(MODEL_NAME, trainX, trainY, None, None)



# df1 = pd.read_csv('predict.csv', engine="python")
# predictData = df1.values.astype("float32")
# predictData = scaler.fit_transform(predictData)
# predictData = numpy.reshape(predictData, (predictData.shape[0], 1, predictData.shape[1]))
# print(predictData)
# prediction0 = model.predict(predictData)
# print(prediction0)
# print(scaler.inverse_transform(prediction0))

# i = [35.0, 36.0, 37.0, 38.0, 38.1, 38.3, 37.9, 38.0, 39.0, 40.0]
# j = pd.DataFrame([20.0, 21.0, 20.0, 22.0, 15.3, 22.2, 20.9, 21.0, 20.3, 21.0]).values
# i.reshape(-1,1)
# i = scaler.fit_transform(i)
# print(i)
# j = scaler.fit_transform(j)
# iPredict = model.predict([[i]])
# print("> High temp test actual:\n", i[0][0])
# print("> High temp test predict:\n", iPredict)
# jPredict = model.predict([[j]])
# print("> Low temp test actual:\n", j[0][0])
# print("> Low temp test predict:\n", jPredict)
