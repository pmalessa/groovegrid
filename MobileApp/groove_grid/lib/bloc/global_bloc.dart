//import 'package:bloc/bloc.dart';
import 'package:groove_grid/bloc/bloc.dart';
import 'package:groove_grid/bloc/connection_bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';
import 'package:groove_grid/bloc/message_bloc.dart';

class GlobalEvent {

}

/// Handles global business logic for the entire app
/// and holds references to other [Bloc] objects for easy access
class GlobalBloc extends Bloc<GlobalEvent, void> {

  GrooveGridAppsBloc _grooveGridAppsBloc;
  ConnectionBloc _connectionBloc;
  MessageBloc _messageBloc;

  ConnectionBloc get connectionBloc => _connectionBloc;
  GrooveGridAppsBloc get grooveGridAppsBloc => _grooveGridAppsBloc;
  MessageBloc get messageBloc => _messageBloc;

  GlobalBloc(): super() {
    _grooveGridAppsBloc = GrooveGridAppsBloc();
    _connectionBloc = ConnectionBloc();
    _messageBloc = MessageBloc();
  }

  @override
  void get initialState => null;

  @override
  Future<void> mapEventToState(GlobalEvent event) {
    return null;
  }

}