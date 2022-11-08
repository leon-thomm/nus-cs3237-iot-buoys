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
        model.fit(trainX, trainY, epochs=20, batch_size=1, verbose=2)
        model.save(name)
    return model

def load_dataset(idx, model_name):
    ## Read base data
    df = pd.read_csv("data-aggregated.csv", usecols=[idx], engine="python")
    ds = df.values.astype('float32')
    ## Scale data
    scaler = StandardScaler()
    ds = scaler.fit_transform(ds)
    ## Create dataset
    trainX_ds, trainY_ds = create_dataset(ds, 10)
    trainX_ds = numpy.reshape(trainX_ds, (trainX_ds.shape[0], 1, trainX_ds.shape[1]))
    ## Train
    model = build_model(model_name, trainX_ds, trainY_ds)
    return model

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

model_turbulence = load_dataset(0, "lstm-turbulence.hd5")
model_light = load_dataset(1, "lstm-light.hd5")
model_temp = load_dataset(2, "lstm-temp.hd5")

# Predict Turbulence
# pred1 = [0.10, 0.11, 0.141, 0.192, 0.17, 0.18, 0.15, 0.18, 0.1, 0.1245]
# out1 = model_turbulence.predict([[pred1]])
# print(out1)

# pred2 = [-0.48, 1.4, -0.48, 1.4, -0.48, -1.46, -1.46, 0.48, 0.48, 0.48]
# out2 = model_turbulence.predict([[pred2]])
# print(out2)

# Predict light
# pred3 = [0.4, 0.2, 0.3, 0.2, 0.3, 0.4, 0.3, 0.3, 0.4, 0.4]
# out3 = model_light.predict([[pred3]])
# print(out3)

# pred4 = [0.81, 0.80, 0.82, 0.81, 0.80, 0.81, 0.82, 0.80, 0.82, 0.7]
# out4 = model_light.predict([[pred4]])
# print(out4)
# pred4 = standardise(pred4)
# out4 = model_light.predict([[pred4]])
# print(out4)

# pred5 = [26.1, 26, 26, 26, 26, 26, 26, 26, 26, 26]
# out5 = model_temp.predict([[pred5]])
# print(out5)

# pred6 = [35.15, 35.25, 35.15, 35.25, 35.15, 35.2, 35.2, 35.2, 35.2, 35.2]
# out6 = model_temp.predict([[pred6]])
# print(out6)

# pred7 = [30, 30, 30, 30, 30, 30, 30, 30, 30, 30]
# out7 = model_temp.predict([[pred7]])
# print(out7)