from flask import Flask, render_template
from pymongo import MongoClient
import socket
from collections import deque 

MONGO_IP = "mongodb://cs3237:thisisag00dp4ssw0rd@" + socket.gethostbyname('mongo') + ":27017/cs3237"
#MONGO_IP = "mongodb://cs3237:thisisag00dp4ssw0rd@localhost:27017/cs3237"
mongo_client = MongoClient(MONGO_IP)
db = mongo_client['cs3237']

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