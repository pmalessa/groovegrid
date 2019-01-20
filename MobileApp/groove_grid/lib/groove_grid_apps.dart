import 'dart:async';
import 'package:flutter/material.dart';
import 'controls.dart';

abstract class GrooveGridApp {
  static GrooveGridApp runningApplication;
  GrooveGridApp({
    @required this.title,
    this.iconData,
    this.startCommand,
    this.stopCommand,
  }) {
    if (startCommand == null)
      startCommand = () => print("Default Start Command on $title");
    if (stopCommand == null)
      stopCommand = () => print("Default Stop Command on $title");
  }

  String title;
  IconData iconData;
  VoidCallback startCommand;
  VoidCallback stopCommand;

  Future start() async {
    if (this != runningApplication) {
      if (runningApplication == null) {
        return Future(() {
          startCommand();
          runningApplication = this;
        });
      } else {
        return runningApplication.stop().then((_) {
          startCommand();
          runningApplication = this;
        });
      }
    }
  }

  Future stop() async {
    if (this == runningApplication) {
      return Future(() {
        stopCommand();
        runningApplication = null;
      });
    }
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

class AnimationsListView extends StatefulWidget {
  @override
  _AnimationsListViewState createState() => _AnimationsListViewState();
}

class _AnimationsListViewState extends State<AnimationsListView> {
  @override
  Widget build(BuildContext context) {
    ListTile makeListTile(String title) => ListTile(
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
        style: Theme.of(context)
            .textTheme
            .subhead, //TextStyle(color: Theme.of(context).text, fontWeight: FontWeight.bold),
      ),
      // subtitle: Text("Intermediate", style: TextStyle(color: Colors.white)),
      trailing: Icon(Icons.more_vert,
          color: Theme.of(context).hintColor, size: 25.0),
      onTap: null,
    );

    Card makeCard(String title) => Card(
      elevation: 8.0,
      margin: new EdgeInsets.symmetric(horizontal: 10.0, vertical: 6.0),
      child: Container(
        decoration: BoxDecoration(color: Theme.of(context).cardColor),
        child: makeListTile(title),
      ),
    );

    return ListView.builder(
      itemCount: _animations.length,
      itemBuilder: (context, index) {
        GrooveGridAnimation animation = _animations[index];
        return makeCard(animation.title);
      },
    );
  }

  List<GrooveGridAnimation> _animations = [
    GrooveGridAnimation(title: "Standard Animation"),
    GrooveGridAnimation(title: "New Animation"),
    GrooveGridAnimation(title: "Another New Animation"),
  ];
}

class GrooveGridAnimation extends GrooveGridApp {
  GrooveGridAnimation({@required String title}): super(title: title);
}

class GamesListView extends StatefulWidget {
  @override
  _GamesListViewState createState() => _GamesListViewState();
}

class _GamesListViewState extends State<GamesListView> {
  @override
  Widget build(BuildContext context) {
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

    return ListView.builder(
      itemCount: _games.length,
      itemBuilder: (context, index) {
        var game = _games[index];
        return makeCard(
          title: game.title,
          onPressed: () {
            game.start();
            Navigator.push(
              context,
              MaterialPageRoute(builder: (context) => game.controlsView),
            );
          },
          subtitle: game.subtitle,
          icon: game.iconData,
          progress: game.progress,
          highlight: game == GrooveGridGame.currentGame ? true : false,
        );
      },
    );
  }

  List<GrooveGridGame> _games = [
    GrooveGridGame(
        title: "2048", subtitle: "Can math really be fun?", progress: 0.3),
    GrooveGridGame(
      title: "Invisible",
      subtitle: "Sneak yourself to victory",
      progress: 0.8,
      startCommand: () {
        print("Start command called on Invisible Game");
      },
    ),
    GrooveGridGame(
        title: "Whack-A-Mole",
        subtitle: "Can you whack 'em all?",
        progress: 0.5),
  ];
}

class GrooveGridGame {
  static GrooveGridGame currentGame;

  GrooveGridGame({
    @required this.title,
    this.subtitle,
    this.progress,
    this.iconData,
    this.startCommand,
    this.stopCommand,
  }) {
    if (controlsView == null) controlsView = SwipeControlsView(title: title);
    if (startCommand == null)
      startCommand = () => print("Default Start Command on $title");
    if (stopCommand == null)
      stopCommand = () => print("Default Stop Command on $title");
  }

  String title;
  String subtitle = "";
  double progress = 0.0;
  IconData iconData = Icons.videogame_asset;
  Widget controlsView;
  VoidCallback startCommand;
  VoidCallback stopCommand;
  bool isRunning = false;

  Future start() async {
    if (this != currentGame) {
      if (currentGame == null) {
        return Future(() {
          startCommand();
          currentGame = this;
        });
      } else {
        return currentGame.stop().then((_) {
          startCommand();
          currentGame = this;
        });
      }
    }
  }

  Future stop() async {
    if (this == currentGame) {
      return Future(() {
        stopCommand();
        currentGame = null;
      });
    }
  }

  @override
  bool operator ==(other) {
    // TODO: implement ==
    return other is GrooveGridGame && title == other.title;
  }

  @override
  // TODO: implement hashCode
  int get hashCode => title.hashCode;
}