//import 'package:bloc/bloc.dart';
import 'package:groove_grid/bloc/bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';

class GlobalEvent {

}

/// Handles global business logic for the entire app
/// and holds references to other [Bloc] objects for easy access
class GlobalBloc extends Bloc<GlobalEvent, void> {

  GrooveGridAppsBloc _grooveGridAppsBloc;

  GlobalBloc(): super() {
    _grooveGridAppsBloc = GrooveGridAppsBloc();
  }

  @override
  // TODO: implement initialState
  void get initialState => null;

  @override
  Future<List<Sink>> mapEventToState(GlobalEvent event) {
    // TODO: implement mapEventToState
    return null;
  }

  GrooveGridAppsBloc get grooveGridAppsBloc => _grooveGridAppsBloc;

}