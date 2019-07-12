import 'package:flutter/material.dart';
import 'package:groove_grid/bloc/global_bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';
import 'package:groove_grid/data/groove_grid_apps.dart';
import 'package:groove_grid/bloc/bloc_provider.dart';
import 'package:groove_grid/data/groove_grid_apps_state.dart';
import 'package:groove_grid/ui/games_list_view.dart';


class AnimationsListView extends StatefulWidget {
  AnimationsListView({@required this.animations});

  final List<GrooveGridAnimation> animations;

  @override
  _AnimationsListViewState createState() => _AnimationsListViewState();
}

class _AnimationsListViewState extends State<AnimationsListView> {
  @override
  Widget build(BuildContext context) {
    final GrooveGridAppsBloc _appsBloc =
        BlocProvider.of<GlobalBloc>(context).grooveGridAppsBloc;

    return StreamBuilder(
      initialData: _appsBloc.state,
      stream: _appsBloc.output,
      builder:
          (BuildContext context, AsyncSnapshot<GrooveGridAppsState> snapshot) {
        GrooveGridAppsState state = snapshot.data;
        return ListView.builder(
          itemCount: state.animations.length,
          itemBuilder: (context, index) {
            GrooveGridAnimation animation = state.animations[index];

            return GridAppListItem(
              title: animation.title,
              onPressed: () {
                animation.start();
              },
              highlight: animation == state.runningApplication,
              icon: Icons.bubble_chart,
            );
          },
        );
      },
    );
  }
}
