import 'package:flutter/material.dart';
import 'package:iotdevice/models/data_class.dart';

const textInputDecoration = InputDecoration(
    filled: true,
    enabledBorder: OutlineInputBorder(
        borderSide: BorderSide(color: Color(0xFFE0E0E0), width: 2.0)),
    focusedBorder: OutlineInputBorder(
        borderSide: BorderSide(color: Colors.blue, width: 2.0)));

class FieldValidators {
  static String? doubleValidator(String? val) {
    if (val == null) {
      return "Cannot be empty!";
    }
    return double.tryParse(val) != null ? null : "Please use valid number!";
  }
}

class MqttForm extends StatefulWidget {
  const MqttForm({Key? key, required this.onSend, required this.onAdd})
      : super(key: key);

  final ValueSetter<DataClass> onSend;
  final ValueSetter<DataClass> onAdd;

  @override
  State<StatefulWidget> createState() {
    return _MqttFormState();
  }
}

class _MqttFormState extends State<MqttForm> {
  final _formkey = GlobalKey<FormState>();

  TextEditingController time = TextEditingController();
  TextEditingController temp = TextEditingController();
  TextEditingController light = TextEditingController();
  TextEditingController x = TextEditingController();
  TextEditingController y = TextEditingController();
  TextEditingController z = TextEditingController();
  TextEditingController gRes = TextEditingController();
  TextEditingController roll = TextEditingController();
  TextEditingController pitch = TextEditingController();
  TextEditingController yaw = TextEditingController();

  DataClass onPressed() {
    DataClass datum = DataClass([
      double.parse(x.text),
      double.parse(y.text),
      double.parse(z.text),
      double.parse(gRes.text)
    ], [
      double.parse(roll.text),
      double.parse(pitch.text),
      double.parse(yaw.text)
    ], double.parse(temp.text), double.parse(light.text), int.parse(time.text));

    return datum;
  }

  void sendPacket() {
    DataClass data = onPressed();
    widget.onSend(data);
  }

  void addToBuffer() {
    DataClass data = onPressed();
    widget.onAdd(data);
  }

  @override
  Widget build(BuildContext context) {
    return Form(
        child: Column(
      key: _formkey,
      children: <Widget>[
        TextFormField(
          controller: time,
          decoration: textInputDecoration.copyWith(hintText: 'Time'),
          keyboardType: TextInputType.number,
          validator: FieldValidators.doubleValidator,
        ),
        TextFormField(
          controller: temp,
          decoration: textInputDecoration.copyWith(hintText: 'temp'),
          keyboardType: TextInputType.number,
          validator: FieldValidators.doubleValidator,
        ),
        TextFormField(
          controller: light,
          decoration: textInputDecoration.copyWith(hintText: 'light'),
          keyboardType: TextInputType.number,
          validator: FieldValidators.doubleValidator,
        ),
        Row(
          children: [
            Expanded(child: TextFormField(
              controller: x,
              decoration: textInputDecoration.copyWith(hintText: 'x'),
              keyboardType: TextInputType.number,
              validator: FieldValidators.doubleValidator,
            )),
            Expanded(child:TextFormField(
              controller: y,
              decoration: textInputDecoration.copyWith(hintText: 'y'),
              keyboardType: TextInputType.number,
              validator: FieldValidators.doubleValidator,
            )),
            Expanded(child:TextFormField(
              controller: z,
              decoration: textInputDecoration.copyWith(hintText: 'z'),
              keyboardType: TextInputType.number,
              validator: FieldValidators.doubleValidator,
            )),
            Expanded(child: TextFormField(
              controller: gRes,
              decoration: textInputDecoration.copyWith(hintText: 'g_res'),
              keyboardType: TextInputType.number,
              validator: FieldValidators.doubleValidator,
            ))
          ],
        ),
        Row(children: [
          Expanded(child:TextFormField(
            controller: roll,
            decoration: textInputDecoration.copyWith(hintText: 'roll'),
            keyboardType: TextInputType.number,
            validator: FieldValidators.doubleValidator,
          )),
          Expanded(
            child: TextFormField(
              controller: pitch,
              decoration: textInputDecoration.copyWith(hintText: 'pitch'),
              keyboardType: TextInputType.number,
              validator: FieldValidators.doubleValidator,
            ),
          ),
          Expanded(
            child: TextFormField(
              controller: yaw,
              decoration: textInputDecoration.copyWith(hintText: 'yaw'),
              keyboardType: TextInputType.number,
              validator: FieldValidators.doubleValidator,
            ),
          )
        ]),
        TextButton(
            onPressed: addToBuffer, child: const Text('Add packet to buffer')),
        TextButton(onPressed: sendPacket, child: const Text('Send packet'))
      ],
    ));
  }
}
