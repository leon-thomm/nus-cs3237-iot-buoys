import 'package:mqtt_client/mqtt_server_client.dart';

class MQTTClient {
  late MqttServerClient client;
  String mqttAddress = '104.248.98.70'; // IP of the Mqtt Server
  int mqttPort = 1883; // Port of the Mqtt Server
  static String usr = "cs3237"; //
  static String pwd = "thisisag00dp4ssw0rd"; //
  static int sendThreshold =
      10; // always sending #sendThreshold data collection to the mqtt server (might safe power)
  String topic = 'topic/test'; // Topic to send the data to
  String statusMessageMqtt =
      "Not Started"; // Possible Status: Not Started, Running, Error

  MQTTClient(
      String mqttAddress, int mqttPort, String topic, statusMessageMqtt) {
    client = MqttServerClient.withPort(mqttAddress, 'iot_device', mqttPort);
  }
}
