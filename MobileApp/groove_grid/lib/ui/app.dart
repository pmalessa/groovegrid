import 'package:flutter/material.dart';
import 'package:groove_grid/bloc/global_bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';

import 'package:groove_grid/bloc/bloc_provider.dart';
import 'package:groove_grid/ui/home_screen.dart';
import 'package:groove_grid/ui/grid_theme.dart';

class GrooveGridRemoteApp extends StatelessWidget {
  // This widget is the root of your application.

  //final GrooveGridAppsBloc appsBloc = GrooveGridAppsBloc();
  final GlobalBloc globalBloc = GlobalBloc();

  final GridThemeData connectedGridTheme = GridThemeData(
    shadowColor: Color(0x80831255),
    highlightedShadowColor: Color(0xFF831255),
    highlightBehaviour: Highlight.background,
    highlightGradient: LinearGradient(colors: [
      Color(0xFFBC247E),
      Color(0xFF6425C0),
    ]),
  );

  final ThemeData connectedTheme = ThemeData(
    //primarySwatch: Colors.red,
    brightness: Brightness.dark,
    primaryColor: Color(0xFF281D38),
    accentColor: Colors.yellow[700],
    canvasColor: Color(0xFF0F0025),
    cardColor: Color(0xFF21192C),
    cardTheme: CardTheme(
      elevation: 0.0,
    ),
    //canvasColor: Color.fromRGBO(15, 0, 37, 1.0),
  );

  @override
  Widget build(BuildContext context) {
    return GridTheme(
      data: connectedGridTheme,
      child: BlocProvider(
        bloc: globalBloc,
        child: MaterialApp(
          title: 'GrooveGrid',
          theme: connectedTheme,
          home: HomeScreen(
            title: 'GrooveGrid',
          ),
        ),
      ),
    );
  }
}