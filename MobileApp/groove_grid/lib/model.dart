import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:groove_grid/groove_grid_apps.dart';


class GrooveGridAppsState {
  List<GrooveGridAnimation> animations = [
    GrooveGridAnimation(title: "Standard Animation"),
//    GrooveGridAnimation(title: "New Animation"),
//    GrooveGridAnimation(title: "Another New Animation"),
  ];

  List<GrooveGridGame> games = [
    GrooveGridGame(
      title: "2048",
      subtitle: "Can math really be fun?",
      progress: 0.3,
      startCommand: () => FlutterBluetoothSerial.instance.write('1'),
      stopCommand: () => FlutterBluetoothSerial.instance.write('q'),
    ),
//    GrooveGridGame(
//      title: "Invisible",
//      subtitle: "Sneak yourself to victory",
//      progress: 0.8,
//      startCommand: () {
//        print("Start command called on Invisible Game");
//      },
//    ),
//    GrooveGridGame(
//        title: "Whack-A-Mole",
//        subtitle: "Can you whack 'em all?",
//        progress: 0.5),
  ];
}