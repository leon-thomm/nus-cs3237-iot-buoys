from flask import Flask, render_template
from pymongo import MongoClient
import socket
from collections import deque 
from tensorflow.keras.models import load_model

#MONGO_IP = "mongodb://cs3237:thisisag00dp4ssw0rd@" + socket.gethostbyname('mongo') + ":27017/cs3237"
MONGO_IP = "mongodb://cs3237:thisisag00dp4ssw0rd@localhost:27017/cs3237"
mongo_client = MongoClient(MONGO_IP)
db = mongo_client['cs3237']
LIGHT_MODEL_NAME = 'lstm-light.hd5'
TEMP_MODEL_NAME = 'lstm-temp.hd5'
TURBULENCE_MODEL_NAME = 'lstm-turbulence.hd5'
light_model = load_model(LIGHT_MODEL_NAME)
temp_model = load_model(TEMP_MODEL_NAME)
turbulence_model = load_model(TURBULENCE_MODEL_NAME)

app = Flask(__name__,
            static_url_path='', 
            static_folder='./static',
            template_folder='./templates')

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/mongo")
def mongo():
    return render_template("mongo.html")

@app.route("/mongo/update")
def updates():
    result = {
        "time": deque([]),
        "temp": deque([]),
        "light": deque([]),
        "acc_x": deque([]),
        "acc_y": deque([]),
        "acc_z": deque([]),
        "acc_g": deque([]),
        "gyro_pitch": deque([]),
        "gyro_yaw": deque([]),
        "gyro_roll": deque([])
    }

    cur = db.abc.find().sort("time", -1).limit(30)
    for record in cur:
        result["time"].appendleft(record.get("time"))
        result["temp"].appendleft(record.get("temp"))
        result["light"].appendleft(record.get("light"))
        result["acc_x"].appendleft(record.get("acc")[0])
        result["acc_y"].appendleft(record.get("acc")[1])
        result["acc_z"].appendleft(record.get("acc")[2])
        result["acc_g"].appendleft(record.get("acc")[3])
        result["gyro_pitch"].appendleft(record.get("gyro")[0])
        result["gyro_yaw"].appendleft(record.get("gyro")[1])
        result["gyro_roll"].appendleft(record.get("gyro")[2])
    
    for key in result.keys():
        temp = result.get(key)
        result[key] = list(temp)
    return result

@app.route("/mongo/lastTenAggregate")
def fetchLastTen():
    lastTen = {
        "time": deque([]),
        "temp": deque([]),
        "light" :deque([]),
        "turbulence": deque([])
    }

    def scale_axis(data):
        data = abs(float(data))
        return 1.0 if data > 1 else (data / 7)

    def scale_degree(data):
        data = abs(float(data))
        return 1.0 if data > 180 else (data / 1260.0)

    res = db.abc.find().sort("time", -1).limit(10)
    for record in res:
        lastTen["time"].appendleft(record.get("time"))
        lastTen["temp"].appendleft(record.get("temp"))
        lastTen["light"].appendleft(record.get("light"))

        acc_x = scale_axis(record.get("acc")[0])
        acc_y = scale_axis(record.get("acc")[1])
        acc_z = scale_axis(record.get("acc")[2])
        acc_g = scale_axis(record.get("acc")[3])        
        gyro_pitch = scale_degree(record.get("gyro")[0])
        gyro_yaw = scale_degree(record.get("gyro")[1])
        gyro_roll = scale_degree(record.get("gyro")[2])
        agg = acc_x + acc_y + acc_z + acc_g + gyro_pitch + gyro_yaw + gyro_roll
        turbulence = round(agg, 7)
        lastTen["turbulence"].appendleft(turbulence)

    for key in lastTen.keys():
        tmp = lastTen.get(key)
        lastTen[key] = list(tmp)

    tempTen = lastTen["temp"]
    lightTen = lastTen["light"]
    turbulenceTen = lastTen["turbulence"]
    print("10temps = ", tempTen)
    print("10light = ", lightTen)
    print("10turbu = ", turbulenceTen)

    predTemp = temp_model.predict([[tempTen]])
    predLight = light_model.predict([[lightTen]])
    predTurbulence = turbulence_model.predict([[turbulenceTen]])

    return { "temp": predTemp.tolist(), "light": predLight.tolist(), "turbulence": predTurbulence.tolist() }
        