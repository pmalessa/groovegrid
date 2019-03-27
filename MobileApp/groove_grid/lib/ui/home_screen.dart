import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:groove_grid/bloc/global_bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';

import 'package:groove_grid/bloc/bloc_provider.dart';
import 'package:groove_grid/data/controls.dart';
import 'package:groove_grid/ui/bluetooth_settings_screen.dart';
import 'package:groove_grid/data/groove_grid_apps.dart';

import 'package:groove_grid/data/groove_grid_apps_state.dart';
import 'package:groove_grid/ui/animations_list_view.dart';
import 'package:groove_grid/ui/controls_view_builder.dart';
import 'package:groove_grid/ui/diamond_border.dart';
import 'package:groove_grid/ui/games_list_view.dart';
import 'package:groove_grid/ui/grid_box_border.dart';
import 'package:groove_grid/ui/grid_fab.dart';
import 'package:groove_grid/ui/grid_list_view.dart';
import 'package:groove_grid/ui/grid_navigation_bar.dart';
import 'package:groove_grid/ui/grid_theme.dart';

class HomeScreen extends StatefulWidget {
  HomeScreen({
    Key key,
    this.title,
    this.bottomNavBarHeight = 60.0,
    this.topAppBarHeight = 60.0,
  }) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  final double bottomNavBarHeight;
  final double topAppBarHeight;

  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  GamesListView gamesView;
  AnimationsListView animationsView;
  ScrollController scrollController;

  final List tabsGenerator = [
    (appsBloc, {double endWhiteSpace, ScrollController controller}) =>
        AppsListView(
          apps: appsBloc.state.animations,
          appType: GrooveGridAnimation,
          controller: controller,
          endWhiteSpace: endWhiteSpace,
        ),
    (appsBloc, {double endWhiteSpace, ScrollController controller}) =>
        AppsListView(
          apps: appsBloc.state.games,
          appType: GrooveGridGame,
          controller: controller,
          endWhiteSpace: endWhiteSpace,
        ),
  ];

  int tabIndex = 0;

  _HomeScreenState() {
    //setupStreams();
  }

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    scrollController = ScrollController();
  }

  void setupStreams() {
    GrooveGridApp.onRunningApplicationChanged()
        .listen((GrooveGridApp runningApp) {
      print("Running App changed to $runningApp");
      // Only redraw to remove the controls floating action button
      // When an app that has controls starts, the controls view is opened,
      // so this view doesn't need to refresh
      if (!runningApp.hasControls) {
        setState(() {});
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    final GrooveGridAppsBloc _appsBloc =
        BlocProvider.of<GlobalBloc>(context).grooveGridAppsBloc;

    Widget currentTab = tabsGenerator[tabIndex](_appsBloc);
    Widget backgroundTab = tabsGenerator[tabIndex](_appsBloc,
        endWhiteSpace: widget.bottomNavBarHeight, controller: scrollController);

    // This method is rerun every time setState is called, for instance as done
    // by the _incrementCounter method above.
    //
    // The Flutter framework has been optimized to make rerunning build methods
    // fast, so that you can just rebuild anything that needs updating rather
    // than having to individually change instances of widgets.
    return Stack(
      children: <Widget>[
        Column(children: <Widget>[
          Container(
            height: widget.topAppBarHeight,
          ),
          Expanded(
            child: backgroundTab,
          )
        ]),
        Scaffold(
          backgroundColor: Colors.transparent,
          appBar: PreferredSize(
            preferredSize: Size.fromHeight(widget.topAppBarHeight),
            child: AppBar(
              leading: IconButton(
                icon: Icon(Icons.settings_bluetooth),
                onPressed: () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                        builder: (context) => BluetoothSettingsView()),
                  );
                },
              ),
              // Here we take the value from the MyHomePage object that was created by
              // the App.build method, and use it to set our appbar title.
              title: Text(widget.title),
            ),
          ),
          body: Stack(
            children: <Widget>[
              Container(
                color: Theme.of(context).canvasColor,
              ),
              NotificationListener<ScrollNotification>(
                onNotification: (scrollNotification) {
                  scrollController.jumpTo(scrollNotification.metrics.pixels);
                },
                child: currentTab,
              ),
            ],
          ),

          bottomNavigationBar: GridNavigationBar(
            currentIndex: tabIndex,
            height: widget.bottomNavBarHeight,
            shape: AutomaticNotchedShape(
              RoundedRectangleBorder(),
              DiamondBorder(),
            ),
            notchMargin: 30.0,
            color: Theme.of(context).hintColor,
            items: <GridNavigationBarItem>[
              GridNavigationBarItem(
                icon: Icons.bubble_chart,
                title: "Animations",
              ),
              GridNavigationBarItem(
                icon: Icons.videogame_asset,
                title: "Games",
              ),
            ],
            onTap: (int index) {
              if (index != tabIndex) {
                setState(() {
                  tabIndex = index;
                });
              }
            },
          ),


          floatingActionButtonLocation:
              FloatingActionButtonLocation.centerDocked,
          floatingActionButton: StreamBuilder(
            initialData: _appsBloc.state,
            stream: _appsBloc.output,
            builder: (BuildContext context,
                AsyncSnapshot<GrooveGridAppsState> snapshot) {
              GrooveGridAppsState state = snapshot.data;
              return state.runningApplication == null
                  ? Container(
                      width: 0,
                      height: 0,
                    )
                  : !state.runningApplication.hasControls
                      ? Container(
                          width: 0,
                          height: 0,
                        )
                      : GridFAB(
                          onPressed: () {
                            state.runningApplication.start().then((_) {
                              Navigator.push(
                                context,
                                MaterialPageRoute(builder: (context) {
                                  if (state.runningApplication.controls
                                      is SwipeControls) {
                                    return ControlsViewBuilder(
                                        state.runningApplication);
                                  }
                                }),
                              );
                            });
                          },
                        );
            },
          ),
        ),
      ],
    );
  }
}
