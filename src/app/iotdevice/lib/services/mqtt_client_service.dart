import 'package:flutter/foundation.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

class MqttClientService with ChangeNotifier {
  static MqttServerClient client =
      MqttServerClient.withPort(mqttAddress, 'iot_device', mqttPort);
  static String mqttAddress = '104.248.98.70'; // IP of the Mqtt Server
  static int mqttPort = 1883; // Port of the Mqtt Server
  static String usr = "cs3237"; //
  static String pwd = "thisisag00dp4ssw0rd"; //
  static int sendThreshold =
      10; // always sending #sendThreshold data collection to the mqtt server (might safe power)
  static String topic = 'test/test'; // Topic to send the data to
  static String statusMessageMqtt =
      "Not Started"; // Possible Status: Not Started, Running, Error

  MqttServerClient get getClient {
    return client;
  }

  void updateAddress(String newAddress) {
    List<String> parts = newAddress.split(".");

    bool isValid = parts.length == 4 &&
        !parts.any((element) {
          int? y = int.tryParse(element);
          bool isInvalid = y == null || y > 255 || y < 0;

          return isInvalid;
        });

    if (isValid) {
      mqttAddress = newAddress;
      client = MqttServerClient.withPort(mqttAddress, 'iot_device', mqttPort);
      notifyListeners();
    }
  }

  void updatePort(int newPort) {
    mqttPort = newPort;
    client = MqttServerClient.withPort(mqttAddress, 'iot_device', newPort);
    notifyListeners();
  }

  int publish(String data) {
    final builder = MqttClientPayloadBuilder();
    builder.addString(data);
    int result =
        client.publishMessage(topic, MqttQos.exactlyOnce, builder.payload!);

    if (kDebugMode) {
      print(result);
      print(builder.payload!);
    }
    return result;
  }

  void connectMqttClient() async {
    client.onConnected = connected;
    client.onDisconnected = disconnected;
    client.autoReconnect = true; // enabling auto reconnect

    final connMessage = MqttConnectMessage()
        .authenticateAs(usr, pwd)
        .withWillTopic('willtopic')
        .withWillMessage('Will message')
        .startClean()
        .withWillQos(MqttQos.atLeastOnce);

    client.keepAlivePeriod = 60;

    client.connectionMessage = connMessage;

    try {
      await client.connect();
      if (kDebugMode) {
        print("Connected to Mqtt Server");
      }
    } catch (e) {
      if (kDebugMode) {
        print('Error Connecting');
      }
      error();
    }
  }

  void disconnect() {
    client.disconnect();
  }

  void error() {
    if (kDebugMode) {
      print("Error Occured while connecting to Mqtt Server");
    }

    statusMessageMqtt = "Error";
  }

  void connected() {
    if (kDebugMode) {
      print("Connected");
    }

    statusMessageMqtt = "Running";
  }

  void disconnected() {
    if (kDebugMode) {
      print("Disconnected");
    }

    statusMessageMqtt = "Not Started";
  }
}
