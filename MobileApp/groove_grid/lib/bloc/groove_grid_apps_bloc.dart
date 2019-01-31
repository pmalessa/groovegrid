import 'package:groove_grid/bloc/bloc.dart';
import 'dart:async';
import 'package:groove_grid/bloc/groove_grid_app_event.dart';
import 'package:groove_grid/data/groove_grid_apps.dart';
import 'package:groove_grid/data/model.dart';

class GrooveGridAppsBloc extends Bloc<GrooveGridAppEvent, GrooveGridAppsState> {
  @override
  GrooveGridAppsState get initialState => GrooveGridAppsState();

  GrooveGridAppsBloc() {
    setupStreams();
  }

  void setupStreams() {
    GrooveGridApp.onRunningApplicationChanged().listen((GrooveGridApp runningApp) {
      print("Running App Changed! Received in BLoC Class");
      dispatch(RunningAppChanged(runningApp));
    });
  }

  @override
  Future<List<Sink>> mapEventToState(GrooveGridAppEvent event) async {
    // TODO: implement mapEventToState
    List<Sink> sinks = [];
    if (event is RunningAppChanged) {
      print("Changing State...");
      state.runningApplication = event.runningApp;
      sinks.add(outputSink);
      //bool connected = await FlutterBluetoothSerial.instance.isConnected;
    }
    return sinks;
  }

//  @override
//  Stream<GrooveGridAppsState> mapEventToState(
//    GrooveGridAppsState currentState,
//    GrooveGridAppEvent event,
//  ) async* {
//    // TODO: implement mapEventToState
//
//    if (event is RunningAppChanged) {
//      print("Changing State...");
//      yield GrooveGridAppsState()..runningApplication = event.runningApp;
//    }
//
//
//  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();
  }
}
