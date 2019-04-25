import 'dart:math';

import 'package:groove_grid/bloc/bloc.dart';
import 'dart:async';
import 'package:groove_grid/bloc/groove_grid_app_event.dart';
import 'package:groove_grid/data/groove_grid_apps.dart';
import 'package:groove_grid/data/groove_grid_apps_state.dart';
import 'package:groove_grid/global_variables.dart';
import 'package:groove_grid/services/bluetooth_service.dart';

class GrooveGridAppsBloc extends Bloc<GrooveGridAppEvent, GrooveGridAppsState> {

  BluetoothService bluetooth = BluetoothService();

  @override
  GrooveGridAppsState get initialState => GlobalVariables.uiTest
      ? GrooveGridAppsState(games: uiTestGamesList, animations: uiTestAnimations)
      : GrooveGridAppsState(games: games, animations: animations);

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
      startCommand: () async {
        if (await BluetoothService().isConnected) {
          BluetoothService().write('1');
        }
      },
      stopCommand: () async {
        if (await BluetoothService().isConnected) {
          BluetoothService().write('q');
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

  List<GrooveGridGame> uiTestGamesList;
  List<GrooveGridAnimation> uiTestAnimations;

  GrooveGridAppsBloc() {
    setupStreams();
    if (GlobalVariables.uiTest) setupUITestVariables();
  }

  void setupUITestVariables() {
    uiTestGamesList = [];
    int numberOfGamesToBeAdded = 50;
    for (int i = 0; i < numberOfGamesToBeAdded; ++i) {
      uiTestGamesList.add(GrooveGridGame(
        title: "2048-$i",
        subtitle: "Can math really be fun?",
        progress: Random().nextDouble(),
      ));
    }

    uiTestAnimations = [];
    int numberOfAnimationsToBeAdded = 20;
    for (int i = 0; i < numberOfAnimationsToBeAdded; ++i) {
      uiTestAnimations.add(GrooveGridAnimation(title: "Standard Animation $i"));
    }

    // I'm updating the state here, because initial drawing with uiTestGamesList = null
    // may have already occurred. Therefore the UI needs to be redrawn with the new state data
    state.games = uiTestGamesList;
    state.animations = uiTestAnimations;
    outputSink.add(state);
  }

  void setupStreams() {
    GrooveGridApp.onRunningApplicationChanged()
        .listen((GrooveGridApp runningApp) {
      print("Running App Changed! Received in BLoC Class");
      dispatch(RunningAppChanged(runningApp));
    });
  }

  @override
  Future<GrooveGridAppsState> mapEventToState(GrooveGridAppEvent event) async {
    if (event is RunningAppChanged) {
      print("Changing State...");
      state.runningApplication = event.runningApp;
      //bool connected = await FlutterBluetoothSerial.instance.isConnected;
    }
    return state;
  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();
  }
}
