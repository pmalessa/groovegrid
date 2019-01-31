
import 'package:flutter/material.dart';
import 'package:groove_grid/data/controls.dart';
import 'package:groove_grid/data/groove_grid_apps.dart';
import 'package:groove_grid/ui/swipe_controls_view.dart';

class ControlsView extends StatelessWidget {

  ControlsView(this.app): assert(app.hasControls);

  final GrooveGridApp app;

  @override
  Widget build(BuildContext context) {

    if (app.controls is SwipeControls) {
      return SwipeControlsView(title: app.title,);
    }

    return null;
  }
}
