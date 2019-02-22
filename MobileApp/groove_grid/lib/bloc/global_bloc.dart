//import 'package:bloc/bloc.dart';
import 'package:groove_grid/bloc/bloc.dart';
import 'package:groove_grid/bloc/connection_bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';
import 'package:tuple/tuple.dart';

class GlobalEvent {

}

/// Handles global business logic for the entire app
/// and holds references to other [Bloc] objects for easy access
class GlobalBloc extends Bloc<GlobalEvent, void> {

  GrooveGridAppsBloc _grooveGridAppsBloc;
  ConnectionBloc _connectionBloc;

  ConnectionBloc get connectionBloc => _connectionBloc;
  GrooveGridAppsBloc get grooveGridAppsBloc => _grooveGridAppsBloc;

  GlobalBloc(): super() {
    _grooveGridAppsBloc = GrooveGridAppsBloc();
    _connectionBloc = ConnectionBloc();
  }

  @override
  void get initialState => null;

  @override
  Future<Tuple2<void, Set<Sink>>> mapEventToState(GlobalEvent event) {
    return null;
  }

}