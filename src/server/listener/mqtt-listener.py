from sqlite3 import Timestamp
import paho.mqtt.client as mqtt
from pymongo import MongoClient
from datetime import datetime
import socket
import json

TIME_FORMAT = "%H:%M:%S"
MQTT_IP = socket.gethostbyname('mosquitto')
MONGO_IP = "mongodb://cs3237:thisisag00dp4ssw0rd@" + socket.gethostbyname('mongo') + ":27017/cs3237"
print(MQTT_IP)
print(MONGO_IP)

def on_connect(client, userdata, flags, rc):
    try:
        client.subscribe("test/#")
        now = datetime.now().strftime(TIME_FORMAT)
        print("%s [!] Connected, code = %s" % (now, str(rc)))
    except Exception as e:
        print("%s [-] Error connecting: %s" % (str(now), e))

def on_message(client, userdata, msg):
    doc = msg.topic.split("/")[1]
    data = msg.payload.decode("utf-8")
    now = datetime.now().strftime(TIME_FORMAT)
    print("%s [+] Incoming = %s..." % (now, data[:16]))
    try:
        dict = json.loads(data)
        for record in dict:
            result = db[doc].insert_one(record)

    except json.JSONDecodeError:
        print("%s [-] JSON Decode Error \n     Payload = %s", (now, data))
    except Exception as e:
        print("%s [-] MongoDB Error: %s", (now, e))

mongo_client = MongoClient(MONGO_IP)
db = mongo_client['cs3237']

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set("cs3237", "thisisag00dp4ssw0rd")

print("Connecting")
client.connect(MQTT_IP, 1883, 60)
client.loop_forever()