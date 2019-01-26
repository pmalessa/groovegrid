import 'package:bloc/bloc.dart';
import 'dart:async';
import 'package:groove_grid/groove_grid_app_event.dart';
import 'package:groove_grid/model.dart';

class GrooveGridAppsBloc extends Bloc<GrooveGridAppEvent, GrooveGridAppsState> {
  @override
  // TODO: implement initialState
  GrooveGridAppsState get initialState => GrooveGridAppsState();

  @override
  Stream<GrooveGridAppsState> mapEventToState(
    GrooveGridAppsState currentState,
    GrooveGridAppEvent event,
  ) async* {
    // TODO: implement mapEventToState
    yield currentState;
  }
}
