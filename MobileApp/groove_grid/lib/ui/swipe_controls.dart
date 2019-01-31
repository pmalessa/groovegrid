import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

class SwipeControlsView extends StatefulWidget {
  SwipeControlsView({Key key, @required this.title}) : super(key: key);

  final String title;

  @override
  _SwipeControlsViewState createState() => _SwipeControlsViewState();
}

class _SwipeControlsViewState extends State<SwipeControlsView> {
  String _labelText = "Swipe to move 2048 tiles in the corresponding direction";

  void _setLabel(String text) {
    setState(() {
      _labelText = text;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
      ),
      body: GestureDetector(
        behavior: HitTestBehavior.translucent,
        onTap: () => print("Tap Gesture recognized!"),
        onHorizontalDragStart: (DragStartDetails details) async {
          print("Horizontal Drag Started!");
          print(details);
        },
        onHorizontalDragEnd: (DragEndDetails details) async {
          var velocity = details.primaryVelocity;
          print("Horitiontal Gesture recognized with speed $velocity");
          if (velocity < -2) {
            _setLabel("Left swipe detected!");
            FlutterBluetoothSerial.instance.write('l');
          } else if (velocity > 2) {
            _setLabel("Right swipe detected!");
            FlutterBluetoothSerial.instance.write('r');
          } else {
            _setLabel("Horizontal swipe detected!");
          }
        },
        onHorizontalDragCancel: () => (print("Vertical Drag cancelled!")),
        onVerticalDragEnd: (DragEndDetails details) async {
          var velocity = details.primaryVelocity;
          print("Vertical Gesture recognized with speed $velocity");
          if (velocity < -2) {
            _setLabel("Up swipe detected!");
            FlutterBluetoothSerial.instance.write('u');
          } else if (velocity > 2) {
            _setLabel("Down swipe detected!");
            FlutterBluetoothSerial.instance.write('d');
          } else {
            _setLabel("Vertical swipe detected!");
          }
        },
        child: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              Text(_labelText),
            ],
          ),
        ),
      ),
    );
  }
}
