import pandas as pd
import numpy
import os
import matplotlib.pyplot as plt
from tensorflow.keras.layers import LSTM, Dense
from tensorflow.keras.models import Sequential, load_model
from tensorflow.keras.callbacks import ModelCheckpoint
from sklearn.preprocessing import StandardScaler, MinMaxScaler

MODEL_NAME = "lstm-split.hd5"
LOOKBACK = 10
FORECASE = 10
TRAIN_PER = 0.7

def create_dataset(data, lookback):
    dataX, dataY = [],[]
    for i in range(len(data) - lookback - 1):
        a = data[i:(i+lookback)]
        dataX.append(a)
        dataY.append(data[i+lookback])
    return numpy.array(dataX), numpy.array(dataY)

def build_model(name, trainX, trainY):
    if os.path.exists(name):
        model = load_model(name)
    else:
        model = Sequential()
        model.add(LSTM(64, input_shape=(1,10)))
        model.add(Dense(10))
        model.compile(loss='mean_squared_error', optimizer='adam')
        model.fit(trainX, trainY, epochs=16, batch_size=1, verbose=2)
        model.save(name)
    return model

## Read base data
df = pd.read_csv("data1.csv", usecols=[0], engine="python")
data_turbulence = df.values.astype('float32')
## Scale data
# scaler = MinMaxScaler()
# data_turbulence = scaler.fit_transform(data_turbulence)
## Create dataset
trainX_turbulence, trainY_turbulence = create_dataset(data_turbulence, 10)
trainX_turbulence = numpy.reshape(trainX_turbulence, (trainX_turbulence.shape[0], 1, trainX_turbulence.shape[1]))
## Train
model = build_model(MODEL_NAME, trainX_turbulence, trainY_turbulence)

## Predict
pred_normal = [0.40, 0.41, 0.441, 0.392, 0.37, 0.38, 0.35, 0.38, 0.4, 0.3245]
pred0 = model.predict([[pred_normal]])
print(pred0)

pred_bad = [0.90, 0.91, 0.941, 0.892, 0.97, 0.98, 0.95, 0.88, 0.9, 0.8245]
pred1 = model.predict([[pred_bad]])
print(pred1)