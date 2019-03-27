import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:groove_grid/data/groove_grid_apps.dart';
import 'dart:math';

class GrooveGridAppsState {
  //GrooveGridApp get runningApplication => GrooveGridApp.runningApplication;

//  static List<GrooveGridGame> uiTestGamesList = () {
//    List<GrooveGridGame> gamesList = [];
//    int numberOfGamesToBeAdded = 50;
//    for (int i = 0; i < numberOfGamesToBeAdded; ++numberOfGamesToBeAdded) {
//      gamesList.add(GrooveGridGame(
//        title: "2048-$i",
//        subtitle: "Can math really be fun?",
//        progress: Random().nextDouble(),
//      ));
//    }
//    print("uiTestGamesList length: ${gamesList.length}");
//
//    return [
//      GrooveGridGame(
//        title: "2048",
//        subtitle: "Can math really be fun?",
//        progress: 0.3,
//        startCommand: () async {
//          if (await FlutterBluetoothSerial.instance.isConnected) {
//            FlutterBluetoothSerial.instance.write('1');
//          }
//        },
//        stopCommand: () async {
//          if (await FlutterBluetoothSerial.instance.isConnected) {
//            FlutterBluetoothSerial.instance.write('q');
//          }
//        },
//      ),
//    ];
//  }();

  //static List<GrooveGridGame> uiTestGamesList = [];

  GrooveGridApp runningApplication;

  List<GrooveGridAnimation> animations;
  List<GrooveGridGame> games;

  GrooveGridAppsState({this.runningApplication, this.games, this.animations}) {
    if (animations == null) {
      animations = [
        GrooveGridAnimation(title: "Standard Animation"),
//    GrooveGridAnimation(title: "New Animation"),
//    GrooveGridAnimation(title: "Another New Animation"),
      ];
    }
    if (games == null) {
      games = [
        GrooveGridGame(
          title: "2048",
          subtitle: "Can math really be fun?",
          progress: 0.3,
          startCommand: () async {
            if (await FlutterBluetoothSerial.instance.isConnected) {
              FlutterBluetoothSerial.instance.write('1');
            }
          },
          stopCommand: () async {
            if (await FlutterBluetoothSerial.instance.isConnected) {
              FlutterBluetoothSerial.instance.write('q');
            }
          },
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
  }

  GrooveGridAppsState.from(GrooveGridAppsState state) {
    GrooveGridAppsState(
      runningApplication: state.runningApplication,
      games: List.from(state.games),
      animations: List.from(state.animations),
    );
  }

  @override
  bool operator ==(other) {
    // TODO: implement ==
    return other is GrooveGridAppsState &&
        runningApplication == other.runningApplication &&
        games == other.games &&
        animations == other.animations;
  }

  @override
  // TODO: implement hashCode
  int get hashCode =>
      runningApplication.hashCode ^ games.hashCode ^ animations.hashCode;
}
