import paho.mqtt.client as mqtt

MQTT_IP = "localhost"

def on_connect(client, userdata, flags, rc):
    try:
        client.subscribe("test/#")
        #now = datetime.now().strftime(TIME_FORMAT)
        print("[!] Connected, code = %s" % str(rc))
    except:
        print("[-] Error connecting")

def on_message(client, userdata, msg):
    print(msg.payload.decode("utf-8"))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set("cs3237", "thisisag00dp4ssw0rd")

print("Connecting")
client.connect(MQTT_IP, 1883, 60)
client.loop_forever()