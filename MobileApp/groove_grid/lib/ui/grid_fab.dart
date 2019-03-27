import 'package:flutter/material.dart';
import 'package:groove_grid/ui/diamond_border.dart';
import 'package:groove_grid/ui/grid_theme.dart';

class GridFAB extends StatefulWidget {
  final VoidCallback onPressed;
  final double size;

  GridFAB({this.onPressed, this.size = 75});

  @override
  _GridFABState createState() => _GridFABState();
}

class _GridFABState extends State<GridFAB> {
  @override
  Widget build(BuildContext context) {
    return SizedBox(
        width: widget.size,
        height: widget.size,
        child: FloatingActionButton(
          onPressed: widget.onPressed,
          //gradient: GridTheme.of(context).highlightGradient,
          shape: DiamondBorder(),
          //backgroundColor: Colors.transparent,
          child: Container(
            width: widget.size,
            height: widget.size,
            decoration: ShapeDecoration(
              gradient: GridTheme.of(context).highlightGradient,
              shape: DiamondBorder(),
            ),
            child: Icon(Icons.play_arrow, color: Theme.of(context).textTheme.subhead.color,),
          ),
        ));
  }
}
