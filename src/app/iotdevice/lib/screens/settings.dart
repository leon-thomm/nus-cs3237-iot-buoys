import 'package:flutter/cupertino.dart';

class Settings extends StatelessWidget {
  const Settings({super.key});

  static String mqttAddress = '104.248.98.70'; // IP of the Mqtt Server
  static int mqttPort = 1883; // Port of the Mqtt Server
  static String usr = "cs3237"; //
  static String pwd = "thisisag00dp4ssw0rd"; //
  static int sendThreshold =
      10; // always sending #sendThreshold data collection to the mqtt server (might safe power)
  static String defaultTopic = 'topic/test'; // Topic to send the data to

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    throw UnimplementedError();
  }

}