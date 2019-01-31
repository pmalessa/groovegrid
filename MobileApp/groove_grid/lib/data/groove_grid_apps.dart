import 'dart:async';
import 'package:flutter/material.dart';
import 'package:groove_grid/ui/swipe_controls.dart';

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


