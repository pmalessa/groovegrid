import 'package:flutter/material.dart';




class GridBoxBorder extends Border {
  @override
  void paint(Canvas canvas, Rect rect,
      {TextDirection textDirection,
      BoxShape shape: BoxShape.rectangle,
      BorderRadius borderRadius}) {
    super.paint(
      canvas,
      rect,
      textDirection: textDirection ?? TextDirection.ltr,
      shape: shape,
      borderRadius: borderRadius,
    );
  }

  @override
  Path getOuterPath(Rect rect, {TextDirection textDirection}) {
    super.getOuterPath(rect, textDirection: textDirection ?? TextDirection.ltr);
  }

  @override
  Path getInnerPath(Rect rect, {TextDirection textDirection}) {
    super.getInnerPath(rect, textDirection: textDirection ?? TextDirection.ltr);
  }
}
