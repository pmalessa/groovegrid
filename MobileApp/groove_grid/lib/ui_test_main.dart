import 'package:flutter/material.dart';
import 'package:groove_grid/ui/app.dart';
import 'package:groove_grid/global_variables.dart';



void main() {
  print("App starting...");

  GlobalVariables.uiTest = true;

  runApp(GrooveGridRemoteApp());
}