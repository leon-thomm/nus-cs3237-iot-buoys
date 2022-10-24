import 'package:flutter/material.dart';

const textInputDecoration = InputDecoration(
    fillColor: Color(0xFFE0E0E0),
    filled: true,
    enabledBorder: OutlineInputBorder(
        borderSide: BorderSide(color: Color(0xFFE0E0E0), width: 2.0)),
    focusedBorder: OutlineInputBorder(
        borderSide: BorderSide(color: Colors.pink, width: 2.0)));

class FieldValidators {
  static String? doubleValidator(String? val) {
    if (val == null) {
      return "Cannot be empty!";
    }
    return double.tryParse(val) != null ? null : "Please use valid number!";
  }
}

class MqttForm extends StatefulWidget {
  const MqttForm({Key? key}) : super(key: key);

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

  onPressed() {

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
        TextFormField(
          controller: x,
          decoration: textInputDecoration.copyWith(hintText: 'x'),
          keyboardType: TextInputType.number,
          validator: FieldValidators.doubleValidator,
        ),
        TextFormField(
          controller: y,
          decoration: textInputDecoration.copyWith(hintText: 'y'),
          keyboardType: TextInputType.number,
          validator: FieldValidators.doubleValidator,
        ),
        TextFormField(
          controller: z,
          decoration: textInputDecoration.copyWith(hintText: 'z'),
          keyboardType: TextInputType.number,
          validator: FieldValidators.doubleValidator,
        ), TextButton(onPressed: onPressed, child: const Text('Create packet'))
      ],
    ));
  }
}
