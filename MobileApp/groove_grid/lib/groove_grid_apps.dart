import 'dart:async';
import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:groove_grid/bloc/bloc_provider.dart';
import 'package:groove_grid/bloc/global_bloc.dart';
import 'package:groove_grid/bloc/groove_grid_app_event.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:groove_grid/model.dart';
import 'controls.dart';

abstract class GrooveGridApp {
  static GrooveGridApp _runningApplication;

  static GrooveGridApp get runningApplication => _runningApplication;
  static set runningApplication(newValue) {
    _runningApplication = newValue;
    streamController.add(newValue);
  }

  static StreamController<GrooveGridApp> streamController =
      StreamController<GrooveGridApp>.broadcast();

  static Stream<GrooveGridApp> onRunningApplicationChanged() {
    return streamController.stream;
  }

  GrooveGridApp({
    @required this.title,
    this.iconData,
    this.startCommand,
    this.stopCommand,
    this.controlsView,
  }) {
    if (startCommand == null)
      startCommand = () => print("Default Start Command on $title");
    if (stopCommand == null)
      stopCommand = () => print("Default Stop Command on $title");
  }

  //bool hasControls = false;
  bool get hasControls {
    return controlsView != null;
  }

  String title;
  IconData iconData;
  VoidCallback startCommand;
  VoidCallback stopCommand;
  Widget controlsView;

  Future start() async {
    if (this != runningApplication) {
      if (runningApplication == null) {
        return Future(() {
          startCommand();
          runningApplication = this;
        });
      } else {
        return runningApplication._stopWithoutResettingRunningApp().then((_) {
          startCommand();
          runningApplication = this;
        });
      }
    }
  }

  Future _stopWithoutResettingRunningApp() async {
    if (this == runningApplication) {
      return Future(() {
        stopCommand();
      });
    }
  }

  Future stop() async {
//    if (this == runningApplication) {
//      return Future(() {
//        stopCommand();
//        runningApplication = null;
//      });
//    }
    return _stopWithoutResettingRunningApp().then((_) {
      runningApplication = null;
    });
  }

  @override
  bool operator ==(other) {
    // TODO: implement ==
    return other is GrooveGridApp && title == other.title;
  }

  @override
  // TODO: implement hashCode
  int get hashCode => title.hashCode;
}

class GrooveGridAnimation extends GrooveGridApp {
  GrooveGridAnimation({
    @required String title,
    IconData iconData,
    VoidCallback startCommand,
    VoidCallback stopCommand,
    bool hasControls,
  }) : super(
          title: title,
          iconData: iconData,
          startCommand: startCommand,
          stopCommand: stopCommand,
        );
}

class GrooveGridGame extends GrooveGridApp {
  GrooveGridGame({
    @required String title,
    IconData iconData,
    VoidCallback startCommand,
    VoidCallback stopCommand,
    this.subtitle,
    this.progress,
  }) : super(
          title: title,
          iconData: iconData,
          startCommand: startCommand,
          stopCommand: stopCommand,
        ) {
    if (controlsView == null) controlsView = SwipeControlsView(title: title);
    if (iconData == null) iconData = Icons.videogame_asset;
  }

  String subtitle = "";
  double progress = 0.0;
  Widget controlsView;
  bool isRunning = false;

  @override
  bool operator ==(other) {
    // TODO: implement ==
    return other is GrooveGridGame && title == other.title;
  }

  @override
  // TODO: implement hashCode
  int get hashCode => title.hashCode;
}

class AnimationsListView extends StatefulWidget {
  AnimationsListView({@required this.animations});

  final List<GrooveGridAnimation> animations;

  @override
  _AnimationsListViewState createState() => _AnimationsListViewState();
}

class _AnimationsListViewState extends State<AnimationsListView> {
  @override
  Widget build(BuildContext context) {

    final GrooveGridAppsBloc _appsBloc = GrooveBlocProvider.of<GlobalBloc>(context).grooveGridAppsBloc;

    ListTile makeListTile({@required String title, bool highlight}) => ListTile(
          contentPadding: EdgeInsets.symmetric(horizontal: 20.0, vertical: 0.0),
          leading: Container(
            padding: EdgeInsets.only(right: 12.0),
            decoration: new BoxDecoration(
                border: new Border(
                    right: new BorderSide(
                        width: 1.0, color: Theme.of(context).hintColor))),
            child: Icon(Icons.bubble_chart, color: Theme.of(context).hintColor),
          ),
          title: Text(
            title,
            style: highlight
                ? Theme.of(context)
                    .textTheme
                    .subhead
                    .apply(color: Theme.of(context).accentColor)
                : Theme.of(context)
                    .textTheme
                    .subhead, //TextStyle(color: Theme.of(context).text, fontWeight: FontWeight.bold),
          ),
          // subtitle: Text("Intermediate", style: TextStyle(color: Colors.white)),
          trailing: Icon(Icons.more_vert,
              color: Theme.of(context).hintColor, size: 25.0),
          onTap: null,
        );

    Card makeCard(
            {@required String title, VoidCallback onPressed, bool highlight}) =>
        Card(
          elevation: 8.0,
          margin: new EdgeInsets.symmetric(horizontal: 10.0, vertical: 6.0),
          child: FlatButton(
            onPressed: onPressed,
            child: makeListTile(
                title: title, highlight: highlight != null ? highlight : false),
          ),
        );

    return BlocBuilder(
      bloc: _appsBloc,
      builder: (BuildContext context, GrooveGridAppsState state) {
        return ListView.builder(
          itemCount: state.animations.length,
          itemBuilder: (context, index) {
            GrooveGridAnimation animation = state.animations[index];
            return makeCard(
              title: animation.title,
              onPressed: () {
                animation.start();
//            bool isCurrentlyRunning = animation == GrooveGridApp.runningApplication;
//            print("This is the currently running application: $isCurrentlyRunning");
//            print("Current running Application: ${GrooveGridApp.runningApplication}");
              },
              highlight: animation == state.runningApplication,
            );
          },
        );
      },
    );
  }
}

class GamesListView extends StatefulWidget {
  GamesListView({@required this.games});

  final List<GrooveGridGame> games;

  @override
  _GamesListViewState createState() => _GamesListViewState();
}

class _GamesListViewState extends State<GamesListView> {
  @override
  Widget build(BuildContext context) {
    final GrooveGridAppsBloc _appsBloc =
        GrooveBlocProvider.of<GlobalBloc>(context).grooveGridAppsBloc;

    ListTile makeListTile(
            {@required String title,
            String subtitle,
            IconData icon,
            double progress,
            @required bool highlight}) =>
        ListTile(
          contentPadding: EdgeInsets.symmetric(horizontal: 20.0, vertical: 0.0),
          leading: Container(
            padding: EdgeInsets.only(right: 12.0),
            decoration: new BoxDecoration(
              border: new Border(
                right: new BorderSide(
                  width: 1.0,
                  color: highlight
                      ? Theme.of(context).accentColor
                      : Theme.of(context).hintColor,
                ),
              ),
            ),
            child: Icon(
              Icons.videogame_asset,
              color: highlight
                  ? Theme.of(context).accentColor
                  : Theme.of(context).hintColor,
            ),
          ),
          title: Text(
            title,
            style: highlight
                ? Theme.of(context)
                    .textTheme
                    .subhead
                    .apply(color: Theme.of(context).accentColor)
                : Theme.of(context)
                    .textTheme
                    .subhead, //TextStyle(color: Theme.of(context).text, fontWeight: FontWeight.bold),
          ),
          // subtitle: Text("Intermediate", style: TextStyle(color: Colors.white)),

          subtitle: Row(
            children: <Widget>[
              Expanded(
                  flex: 1,
                  child: Container(
                    // tag: 'hero',
                    child: LinearProgressIndicator(
                        backgroundColor: Color.fromRGBO(209, 224, 224, 0.4),
                        value: progress != null ? progress : 0.0,
                        valueColor: AlwaysStoppedAnimation(Colors.green)),
                  )),
              Expanded(
                flex: 4,
                child: Padding(
                    padding: EdgeInsets.only(left: 10.0),
                    child: Text(subtitle != null ? subtitle : "Undefined",
                        style: Theme.of(context).textTheme.body1)),
              )
            ],
          ),
          trailing: Icon(Icons.keyboard_arrow_right,
              color: Theme.of(context).hintColor, size: 30.0),
          onTap: null,
        );

    Card makeCard(
            {@required String title,
            @required VoidCallback onPressed,
            String subtitle,
            IconData icon,
            double progress,
            bool highlight}) =>
        Card(
          elevation: 8.0,
          margin: new EdgeInsets.symmetric(horizontal: 10.0, vertical: 6.0),
          child: FlatButton(
            onPressed: onPressed,
            child: makeListTile(
              title: title,
              subtitle: subtitle,
              icon: icon,
              progress: progress,
              highlight: highlight,
            ),
          ),
        );

    return BlocBuilder<GrooveGridAppEvent, GrooveGridAppsState>(
      bloc: _appsBloc,
      builder: (
        BuildContext context,
        GrooveGridAppsState state,
      ) {
        return ListView.builder(
          itemCount: state.games.length,
          itemBuilder: (context, index) {
            var game = state.games[index];
            return makeCard(
              title: game.title,
              onPressed: () {
                game.start().then((_) {
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => game.controlsView),
                  );
                });
//            Navigator.push(
//              context,
//              MaterialPageRoute(builder: (context) => game.controlsView),
//            ).then((_) => game.start());
              },
              subtitle: game.subtitle,
              icon: game.iconData,
              progress: game.progress,
              highlight:
                  game == GrooveGridApp.runningApplication ? true : false,
            );
          },
        );
      },
    );
  }
}