import 'package:groove_grid/data/groove_grid_apps.dart';

class GrooveGridAppsState {

  GrooveGridApp runningApplication;

  List<GrooveGridAnimation> animations;
  List<GrooveGridGame> games;

  GrooveGridAppsState({this.runningApplication, this.games, this.animations}) {
    if (animations == null) {
      animations = [];
    }
    if (games == null) {
      games = [];
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
