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

class AppsListView extends StatelessWidget {
  AppsListView({@required this.apps, @required this.appType});

  final List<GrooveGridApp> apps;
  final Type appType;

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
          itemCount: appType == GrooveGridGame
              ? state.games.length
              : appType == GrooveGridAnimation ? state.animations.length : null,
          itemBuilder: (context, index) {
            var app;

            if (appType == GrooveGridAnimation) {
              app = state.animations[index];

              return GridAppListItem(
                title: app.title,
                onPressed: app.start,
                icon: app.iconData ?? Icons.bubble_chart,
                highlight: app == state.runningApplication,
              );
            }
            else if (appType == GrooveGridGame) {
              app = state.games[index];

              return GridAppListItem(
                title: app.title,
                onPressed: () {
                  app.start().then((_) {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) {
                        if (app.controls is SwipeControls) {
                          return ControlsViewBuilder(app);
                        }
                      }),
                    );
                  });
                },
                subtitle: app.subtitle,
                icon: app.iconData ?? Icons.videogame_asset,
                progress: app.progress,
                highlight: app == state.runningApplication,
              );
            }
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
