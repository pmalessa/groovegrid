import 'package:groove_grid/data/groove_grid_apps.dart';

class GrooveGridAppEvent {

}

class RunningAppChanged extends GrooveGridAppEvent {
  GrooveGridApp runningApp;

  RunningAppChanged(this.runningApp): super();
}