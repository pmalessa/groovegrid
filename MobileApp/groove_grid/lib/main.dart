import 'package:flutter/material.dart';
import 'package:groove_grid/bloc/global_bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';

import 'package:groove_grid/bloc/bloc_provider.dart';
import 'package:groove_grid/ui/home_screen.dart';

void main() {
  print("App starting...");
  runApp(GrooveGridRemoteApp());
}

class GrooveGridRemoteApp extends StatelessWidget {
  // This widget is the root of your application.

  final GrooveGridAppsBloc appsBloc = GrooveGridAppsBloc();
  final GlobalBloc globalBloc = GlobalBloc();

  @override
  Widget build(BuildContext context) {
    return BlocProvider(
      bloc: globalBloc,
      child: MaterialApp(
        title: 'GrooveGrid',
        theme: ThemeData(
          // This is the theme of your application.
          //
          // Try running your application with "flutter run". You'll see the
          // application has a blue toolbar. Then, without quitting the app, try
          // changing the primarySwatch below to Colors.green and then invoke
          // "hot reload" (press "r" in the console where you ran "flutter run",
          // or simply save your changes to "hot reload" in a Flutter IDE).
          // Notice that the counter didn't reset back to zero; the application
          // is not restarted.

          //primarySwatch: Colors.red,
          brightness: Brightness.dark,
          primaryColor: Color(0xFF281D38),
          accentColor: Colors.yellow[700],
          canvasColor: Color(0xFF0F0025),
          cardColor: Color(0xFF21192C),
          //canvasColor: Color.fromRGBO(15, 0, 37, 1.0),
        ),
        home: HomeScreen(
          title: 'GrooveGrid',
        ),
      ),
    );
  }
}


