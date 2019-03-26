import 'dart:math';

import 'package:groove_grid/bloc/bloc.dart';
import 'dart:async';
import 'package:groove_grid/bloc/groove_grid_app_event.dart';
import 'package:groove_grid/data/groove_grid_apps.dart';
import 'package:groove_grid/data/groove_grid_apps_state.dart';
import 'package:groove_grid/global_variables.dart';

class GrooveGridAppsBloc extends Bloc<GrooveGridAppEvent, GrooveGridAppsState> {
  @override
  GrooveGridAppsState get initialState => GlobalVariables.uiTest
      ? GrooveGridAppsState(games: uiTestGamesList)
      : GrooveGridAppsState();

//  @override
//  GrooveGridAppsState get initialState => GrooveGridAppsState();

  List<GrooveGridGame> uiTestGamesList;

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

    // I'm updating the state here, because initial drawing with uiTestGamesList = null
    // may have already occurred. Therefore the UI needs to be redrawn with the new state data
    state.games = uiTestGamesList;
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
