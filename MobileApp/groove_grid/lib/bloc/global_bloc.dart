import 'package:bloc/bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';

class GlobalEvent {

}

class GlobalBloc extends Bloc<GlobalEvent, void> {

  GrooveGridAppsBloc _grooveGridAppsBloc;

  GlobalBloc() {
    _grooveGridAppsBloc = GrooveGridAppsBloc();
  }

  @override
  // TODO: implement initialState
  void get initialState => null;

  @override
  Stream<void> mapEventToState(void currentState, GlobalEvent event) {
    // TODO: implement mapEventToState
    return null;
  }

  GrooveGridAppsBloc get grooveGridAppsBloc => _grooveGridAppsBloc;

}