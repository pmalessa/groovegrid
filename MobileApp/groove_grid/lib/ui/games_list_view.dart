import 'package:flutter/material.dart';
import 'package:groove_grid/bloc/bloc_provider.dart';
import 'package:groove_grid/bloc/global_bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';
import 'package:groove_grid/data/controls.dart';
import 'package:groove_grid/data/groove_grid_apps.dart';
import 'package:groove_grid/data/groove_grid_apps_state.dart';
import 'package:groove_grid/ui/controls_view_builder.dart';
import 'package:groove_grid/ui/grid_card.dart';
import 'package:groove_grid/ui/grid_theme.dart';

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
        BlocProvider.of<GlobalBloc>(context).grooveGridAppsBloc;

    return StreamBuilder(
      initialData: _appsBloc.state,
      stream: _appsBloc.output,
      builder: (
        BuildContext context,
        AsyncSnapshot<GrooveGridAppsState> snapshot,
      ) {
        GrooveGridAppsState state = snapshot.data;
        return ListView.builder(
          itemCount: state.games.length,
          itemBuilder: (context, index) {
            var game = state.games[index];
            return GridAppListItem(
              title: game.title,
              onPressed: () {
                game.start().then((_) {
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) {
                      if (game.controls is SwipeControls) {
                        return ControlsViewBuilder(game);
                      }
                    }),
                  );
                });
              },
              subtitle: game.subtitle,
              icon: game.iconData ?? Icons.videogame_asset,
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

class GridAppListItem extends StatelessWidget {
  final bool highlight;
  final String title;
  final String subtitle;
  final IconData icon;
  final double progress;
  final VoidCallback onPressed;
  final Widget trailing;

  static const Gradient defaultGradient = LinearGradient(colors: [
    Color(0xFFBC247E),
    Color(0xFF6425C0),
  ]);

  GridAppListItem({
    this.highlight,
    this.onPressed,
    @required this.title,
    this.subtitle,
    this.icon,
    this.progress = null,
    this.trailing,
  });

  @override
  Widget build(BuildContext context) {
    // Setup Highlight Properties
    Color foregroundColor;
    Color mutedForegroundColor;
    Color backgroundColor;
    Color shadowColor;
    Gradient backgroundGradient;
    Border border;

    if (highlight) {
      if (GridTheme.of(context).highlightBehaviour == Highlight.foreground) {
        foregroundColor = Theme.of(context).accentColor;
        mutedForegroundColor = foregroundColor;
        backgroundColor = Theme.of(context).cardTheme.color;
      } else {
        foregroundColor = Theme.of(context).textTheme.subhead.color;
        mutedForegroundColor = foregroundColor;
        backgroundColor = Colors.pink;
        backgroundGradient =
            GridTheme.of(context).highlightGradient ?? defaultGradient;
        border = Border.all();
        shadowColor = GridTheme.of(context).highlightedShadowColor;
      }
    } else {
      foregroundColor = Theme.of(context).textTheme.subhead.color;
      mutedForegroundColor = Theme.of(context).hintColor;
      backgroundColor = Theme.of(context).cardTheme.color;
    }

    // Setup ListTile subtitle display
    Widget progressIndicator;
    Widget subtitleWidget;
    bool displayListTileSubtitle = false;
    List<Widget> subtitleItems = <Widget>[];

    if (progress != null) {
      progressIndicator = Expanded(
          flex: 1,
          child: Container(
            // tag: 'hero',
            child: LinearProgressIndicator(
              backgroundColor: Color.fromRGBO(209, 224, 224, 0.4),
              value: progress != null ? progress : 0.0,
              valueColor: AlwaysStoppedAnimation(foregroundColor),
            ),
          ));
      displayListTileSubtitle = true;

      subtitleItems.add(progressIndicator);
    }

    if (subtitle != null) {
      subtitleWidget = Expanded(
        flex: 4,
        child: Padding(
          padding: EdgeInsets.only(left: 10.0),
          child: Text(
            subtitle,
            style:
                Theme.of(context).textTheme.body1.apply(color: foregroundColor),
          ),
        ),
      );
      subtitleItems.add(subtitleWidget);
      displayListTileSubtitle = true;
    }

    return GridCard(
      margin: new EdgeInsets.symmetric(horizontal: 10.0, vertical: 6.0),
      color: backgroundColor,
      gradient: backgroundGradient,
      border: border,
      shadowColor: shadowColor,
      child: ListTile(
        contentPadding: EdgeInsets.symmetric(horizontal: 30.0, vertical: 0.0),
        leading: Container(
          padding: EdgeInsets.only(right: 12.0),
          decoration: new BoxDecoration(
            border: new Border(
              right: new BorderSide(
                width: 1.0,
                color: mutedForegroundColor,
              ),
            ),
          ),
          child: this.icon != null
              ? Icon(
                  this.icon,
                  color: mutedForegroundColor,
                )
              : Container(width: 0, height: 0),
        ),
        title: Text(
          title,
          style:
              Theme.of(context).textTheme.subhead.apply(color: foregroundColor),
        ),
        subtitle: displayListTileSubtitle
            ? Row(
                children: subtitleItems,
              )
            : null,
        trailing: trailing != null
            ? trailing
            : Icon(Icons.keyboard_arrow_right,
                color: mutedForegroundColor, size: 30.0),
        onTap: onPressed,
      ),
    );
  }
}
