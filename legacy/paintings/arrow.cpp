/***************************************************************************
                                arrow.cpp
                               -----------
    begin                : Sun Nov 23 2003
    copyright            : (C) 2003 by Michael Margraf
    email                : michael.margraf@alumni.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*!
  \class Arrow
  \brief The Arrow class implements the arrow painting
*/

#include "arrowdialog.h"
#include "schematic_doc.h" // BUG
#include "misc.h"
#include <cmath>

#include <QPolygon>
#include <QPainter>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include "qucs_globals.h"
#include "painting.h"

#include <QPen>

namespace{

class Arrow : public Painting  {
public:
  Arrow();
 ~Arrow();

  void paint(ViewPainter*);
  void paintScheme(SchematicDoc*);
  void getCenter(int&, int&);
  void setCenter(int, int, bool relative=false);

  Element* newOne() { return new Arrow(*this); }
  Element* clone() const { return new Arrow(*this); }

  static Element* info(QString&, char* &, bool getNewOne=false);
  bool load(const QString&);
  QString save();
  QString saveCpp();
  QString saveJSON();
//  void MouseMoving(SchematicDoc*, int, int, int, int, SchematicDoc*, int, int, bool);
  bool MousePressing();
  bool getSelected(float, float, float);
  void Bounding(int&, int&, int&, int&);
  bool resizeTouched(float, float, float);
  void MouseResizeMoving(int, int, SchematicDoc*);

  void rotate();
  void mirrorX();
  void mirrorY();
  bool Dialog();

  void calcArrowHead();

  QPen   Pen;
  int    Style;
  double Height, Width;  // size of the arrow head
  double Length, beta;
  int    xp1, yp1, xp2, yp2;   // coordinates to paint the arrow head
};


Arrow::Arrow() : Painting()
{
  Name = "Arrow ";
  Pen = QPen(QColor());
  x1 = x2 = 0;
  y1 = y2 = 0;

  Height = 20.0;
  Width  =  8.0;
  Style  = 0;   // arrow head not filled
  beta   = atan2(double(Width), double(Height));
  Length = sqrt(Width*Width + Height*Height);
}

Arrow::~Arrow()
{
}

Arrow D;
Dispatcher<Painting>::INSTALL p(&painting_dispatcher, "Arrow", &D);
//Module::INSTALL pp("paintings", &D);
// --------------------------------------------------------------------------
void Arrow::paint(ViewPainter *p)
{
	 auto cx=0;
     auto cy=0;

  QPolygon Points;
  int x1_, y1_, x2_, y2_, x3_, y3_;
#if 0
  if(isSelected()) {
    p->setPen(QPen(Qt::darkGray,Pen.width()+5));
    p->drawLine(cx, cy, cx+x2, cy+y2);
    p->drawLine(cx+x2, cy+y2, cx+xp1, cy+yp1);
    p->drawLine(cx+x2, cy+y2, cx+xp2, cy+yp2);
    if(Style == 0) {   // arrow head with two lines ?
      p->setPen(QPen(Qt::white, Pen.width(), Pen.style()));
      p->drawLine(cx, cy, cx+x2, cy+y2);
      p->setPen(QPen(Qt::white, Pen.width(), Qt::SolidLine));
      p->drawLine(cx+x2, cy+y2, cx+xp1, cy+yp1);
      p->drawLine(cx+x2, cy+y2, cx+xp2, cy+yp2);
    }
    else {   // filled arrow head
      p->drawLine(cx+xp1, cy+yp1, cx+xp2, cy+yp2);
      p->setPen(QPen(Qt::white, Pen.width(), Pen.style()));
      p->drawLine(cx, cy, cx+x2, cy+y2);

      p->setPen(QPen(Qt::white, Pen.width(), Qt::SolidLine));
      p->setBrush(Qt::white);
      p->map(cx+xp1, cy+yp1, x1_, y1_);
      p->map(cx+x2, cy+y2, x2_, y2_);
      p->map(cx+xp2, cy+yp2, x3_, y3_);
      Points.setPoints(3, x1_, y1_, x2_, y2_, x3_, y3_);
      p->drawConvexPolygon(Points);
      p->setBrush(Qt::NoBrush); // no filling for next paintings
    }

    p->setPen(QPen(Qt::darkRed,2));
    p->drawResizeRect(cx, cy);  // markers for changing the size
    p->drawResizeRect(cx+x2, cy+y2);
    return;
  }
#endif
  p->setPen(Pen);
  p->drawLine(cx, cy, cx+x2, cy+y2);
  p->setPen(QPen(Pen.color(), Pen.width(), Qt::SolidLine));
  if(Style == 0) {   // arrow head with two lines ?
    p->drawLine(cx+x2, cy+y2, cx+xp1, cy+yp1);
    p->drawLine(cx+x2, cy+y2, cx+xp2, cy+yp2);
  }
  else {   // filled arrow head
    p->setBrush(Pen.color());
    p->map(cx+xp1, cy+yp1, x1_, y1_);
    p->map(cx+x2, cy+y2, x2_, y2_);
    p->map(cx+xp2, cy+yp2, x3_, y3_);
    Points.setPoints(3, x1_, y1_, x2_, y2_, x3_, y3_);
    p->drawConvexPolygon(Points);
    p->setBrush(Qt::NoBrush); // no filling for next paintings
  }
}

// --------------------------------------------------------------------------
void Arrow::paintScheme(SchematicDoc *p)
{
	 auto cx=Element::cx();
     auto cy=Element::cy();

  p->PostPaintEvent(_Line, cx, cy, cx+x2, cy+y2,0,0,false);
  p->PostPaintEvent(_Line, cx+x2, cy+y2, cx+xp1, cy+yp1,0,0,false);
  p->PostPaintEvent(_Line, cx+x2, cy+y2, cx+xp2, cy+yp2,0,0,false);
}

// --------------------------------------------------------------------------
void Arrow::getCenter(int& x, int &y)
{
	 auto cx=Element::cx();
     auto cy=Element::cy();

  x = cx+(x2>>1);
  y = cy+(y2>>1);
}

// --------------------------------------------------------------------------
// Sets the center of the painting to x/y.
void Arrow::setCenter(int x, int y, bool relative)
{
	 auto cx=Element::cx();
     auto cy=Element::cy();

  if(relative) { cx += x;  cy += y; }
  else { cx = x-(x2>>1);  cy = y-(y2>>1); }
}

// --------------------------------------------------------------------------
#if 0
Element* Arrow::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Arrow");
  BitmapFile = (char *) "arrow";

  if(getNewOne)  return new Arrow();
  return 0;
}
#endif

// --------------------------------------------------------------------------
bool Arrow::load(const QString& s)
{
  bool ok;

  QString n;
  n  = s.section(' ',1,1);    // cx
  int cx = n.toInt(&ok);
  if(!ok) return false;

  n  = s.section(' ',2,2);    // cy
  int cy = n.toInt(&ok);
  if(!ok) return false;

  setPosition(pos_t(cx, cy));

  n  = s.section(' ',3,3);    // x2
  x2 = n.toInt(&ok);
  if(!ok) return false;

  n  = s.section(' ',4,4);    // y2
  y2 = n.toInt(&ok);
  if(!ok) return false;

  n  = s.section(' ',5,5);    // height
  Height = n.toDouble(&ok);
  if(!ok) return false;

  n  = s.section(' ',6,6);    // width
  Width = n.toDouble(&ok);
  if(!ok) return false;

  n  = s.section(' ',7,7);    // color
  QColor co;
  co.setNamedColor(n);
  Pen.setColor(co);
  if(!Pen.color().isValid()) return false;

  n  = s.section(' ',8,8);    // thickness
  Pen.setWidth(n.toInt(&ok));
  if(!ok) return false;

  n  = s.section(' ',9,9);    // line style
  Pen.setStyle((Qt::PenStyle)n.toInt(&ok));
  if(!ok) return false;

  n  = s.section(' ',10,10);    // arrow style
  if(!n.isEmpty()) {            // backward compatible
    Style = n.toInt(&ok);
    if(!ok) return false;
  }

  beta   = atan2(double(Width), double(Height));
  Length = sqrt(Width*Width + Height*Height);
  calcArrowHead();
  return true;
}

// --------------------------------------------------------------------------
QString Arrow::save()
{
	 auto cx=Element::cx();
     auto cy=Element::cy();

  QString s = Name+QString::number(cx)+" "+QString::number(cy)+" ";
  s += QString::number(x2)+" "+QString::number(y2)+" ";
  s += QString::number(int(Height))+" "+QString::number(int(Width))+" ";
  s += Pen.color().name()+" "+QString::number(Pen.width())+" ";
  s += QString::number(Pen.style()) + " " + QString::number(Style);
  return s;
}

// --------------------------------------------------------------------------
#if 0
QString Arrow::saveCpp()
{
	 auto cx=Element::cx();
     auto cy=Element::cy();

  // arrow not allowed in symbols, thus we use line here
  QString s =
    QString ("new Line (%1, %2, %3, %4, QPen (QColor (\"%5\"), %6, %7))").
    arg(cx+x1).arg(cy+y1).arg(cx+x2).arg(cy+y2).
    arg(Pen.color().name()).arg(Pen.width()).arg(toPenString(Pen.style()));
  s = "Lines.append (" + s + ");";
  return s;
}

QString Arrow::saveJSON()
{
	 auto cx=Element::cx();
     auto cy=Element::cy();

  // arrow not allowed in symbols, thus we use line here
  QString s =
    QString("{\"type\" : \"arrow\", "
       "\"x1\" : %1, \"y1\" : %2, \"x2\" : %3, \"y2\" : %4, "
       "\"color\" : \"%5\", \"thick\" : %6, \"style\" : \"%7\"},").
       arg(cx+x1).arg(cy+y1).arg(cx+x2).arg(cy+y2).
       arg(Pen.color().name()).arg(Pen.width()).arg(toPenString(Pen.style()));
  return s;
}
#endif

// --------------------------------------------------------------------------
// Checks if the resize area was clicked.
bool Arrow::resizeTouched(float fX, float fY, float len)
{
	 auto cx=Element::cx();
     auto cy=Element::cy();

  float fCX = float(cx),fCY = float(cy);
  if(fX < fCX+len) if(fX > fCX-len) if(fY < fCY+len) if(fY > fCY-len) {
    State = 1;
    return true;
  }

  fCX += float(x2);
  fCY += float(y2);
  if(fX < fCX+len) if(fX > fCX-len) if(fY < fCY+len) if(fY > fCY-len) {
    State = 2;
    return true;
  }

  State = 0;
  return false;
}

// --------------------------------------------------------------------------
// Mouse move action during resize.
void Arrow::MouseResizeMoving(int x, int y, SchematicDoc *p)
{
	 auto cx=Element::cx();
     auto cy=Element::cy();

  paintScheme(p);  // erase old painting
  if(State == 1) { x2 += cx-x; y2 += cy-y; cx = x; cy = y; } // moving shaft
  else { x2 = x-cx;  y2 = y-cy; }  // moving head

  calcArrowHead();
  paintScheme(p);  // paint new painting
}

// --------------------------------------------------------------------------
void Arrow::calcArrowHead()
{
  double phi  = atan2(double(y2), double(x2));

  double w = beta+phi;
  xp1 = x2-int(Length*cos(w));
  yp1 = y2-int(Length*sin(w));

  w = phi-beta;
  xp2 = x2-int(Length*cos(w));
  yp2 = y2-int(Length*sin(w));
}

// --------------------------------------------------------------------------
// fx/fy are the precise coordinates, gx/gy are the coordinates set on grid.
// x/y are coordinates without scaling.
#if 0
void Arrow::MouseMoving(
	SchematicDoc *paintScale, int, int, int gx, int gy,
	SchematicDoc *p, int x, int y, bool drawn)
{
	incomplete();
  if(State > 0) {
    if(State > 1) {
      calcArrowHead();
      paintScheme(paintScale);  // erase old painting
    }
    State++;
    x2 = gx-cx;
    y2 = gy-cy;
    calcArrowHead();
    paintScheme(paintScale);  // paint new painting
  }
  else { cx = gx; cy = gy; }


// FIXME #warning  p->setPen(Qt::SolidLine);
  if(drawn) {
    p->PostPaintEvent(_Line, x1+25, y1, x1+13, y1+12,0,0,true);  // erase old cursor symbol
    p->PostPaintEvent(_Line, x1+18, y1+2, x1+25, y1,0,0,true);
    p->PostPaintEvent(_Line, x1+23, y1+7, x1+25, y1,0,0,true);
  }
  x1 = x;
  y1 = y;
  p->PostPaintEvent(_Line, x1+25, y1, x1+13, y1+12,0,0,true);  // paint new cursor symbol
  p->PostPaintEvent(_Line, x1+18, y1+2, x1+25, y1,0,0,true);
  p->PostPaintEvent(_Line, x1+23, y1+7, x1+25, y1,0,0,true);
}
#endif

// --------------------------------------------------------------------------
bool Arrow::MousePressing()
{
  State++;
  if(State > 2) {
    x1 = y1 = 0;
    State = 0;

    calcArrowHead();
    return true;    // painting is ready
  }
  return false;
}

// --------------------------------------------------------------------------
// Checks if the coordinates x/y point to the painting.
// 5 is the precision the user must point onto the painting.
bool Arrow::getSelected(float fX, float fY, float w)
{
	 auto cx=Element::cx();
     auto cy=Element::cy();

  float A, xn, yn;
  // first check if coordinates match the arrow body
  fX -= float(cx);
  fY -= float(cy);

  if(fX < -w) {
    if(fX < float(x2)-w)  // is point between x coordinates ?
      goto Head1;
  }
  else {
    if(fX > w)
      if(fX > float(x2)+w)
        goto Head1;
  }

  if(fY < -w) {
    if(fY < float(y2)-w)   // is point between y coordinates ?
      goto Head1;
  }
  else {
    if(fY > w)
      if(fY > float(y2)+w)
        goto Head1;
  }

  A  = float(x2)*fY - fX*float(y2); // calculate the rectangle area spanned
  A *= A;               // avoid the need for square root

  if(A <= w*w*float(x2*x2 + y2*y2))
    return true;     // x/y lies on the graph line

Head1:    // check if coordinates match the first arrow head line
  xn = float(xp1-x2);  fX -= float(x2);
  yn = float(yp1-y2);  fY -= float(y2);

  if(fX < -w) {
    if(fX < xn-w)    // is point between x coordinates ?
      goto Head2;
  }
  else {
    if(fX > w)
      if(fX > xn+w)
        goto Head2;
  }

  if(fY < -w) {
    if(fY < yn-w)    // is point between y coordinates ?
      goto Head2;
  }
  else
    if(fY > w)
      if(fY > yn+w)
        goto Head2;

  A  = xn*fY - fX*yn;   // calculate the rectangle area spanned
  A *= A;               // avoid the need for square root

  if(A <= w*w*(xn*xn + yn*yn))
    return true;     // x/y lies on the arrow head

Head2:    // check if coordinates match the second arrow head line
  xn = float(xp2-x2);
  yn = float(yp2-y2);

  if(fX < -w) {
    if(fX < xn-w)   // is point between x coordinates ?
      return false;
  }
  else
    if(fX > w)
      if(fX > xn+w)
        return false;

  if(fY < -w) {
    if(fY < yn-w)   // is point between y coordinates ?
      return false;
  }
  else
    if(fY > w)
      if(fY > yn+w)
        return false;

  A  = xn*fY - fX*yn;   // calculate the rectangle area spanned
  A *= A;               // avoid the need for square root

  if(A <= w*w*(xn*xn + yn*yn))
    return true;     // x/y lies on the arrow head

  return false;
}

// --------------------------------------------------------------------------
void Arrow::Bounding(int& _x1, int& _y1, int& _x2, int& _y2)
{
	 auto cx=Element::cx();
     auto cy=Element::cy();

  if(x2 < 0) { _x1 = cx+x2; _x2 = cx; }
  else { _x1 = cx; _x2 = cx+x2; }

  if(y2 < 0) { _y1 = cy+y2; _y2 = cy; }
  else { _y1 = cy; _y2 = cy+y2; }
}

// --------------------------------------------------------------------------
// Rotates around the center.
void Arrow::rotate()
{
  //_cx += (x2>>1) - (y2>>1);
  //_cy += (x2>>1) + (y2>>1);

  int tmp = x2;
  x2  =  y2;
  y2  = -tmp;

  tmp =  xp1;
  xp1 =  yp1;
  yp1 = -tmp;

  tmp =  xp2;
  xp2 =  yp2;
  yp2 = -tmp;
}

// --------------------------------------------------------------------------
// Mirrors about center line.
void Arrow::mirrorX()
{
  yp1 = -yp1;
  yp2 = -yp2;
  // _cy +=  y2;   // change cy after the other changes !
  y2  = -y2;
}

// --------------------------------------------------------------------------
// Mirrors about center line.
void Arrow::mirrorY()
{
  xp1 = -xp1;
  xp2 = -xp2;
  // _cx +=  x2;   // change cx after the other changes !
  x2  = -x2;
}

// --------------------------------------------------------------------------
// Calls the property dialog for the painting and changes them accordingly.
// If there were changes, it returns 'true'.
bool Arrow::Dialog()
{
  bool changed = false;

  ArrowDialog *d = new ArrowDialog();
  d->HeadWidth->setText(QString::number(Width));
  d->HeadLength->setText(QString::number(Height));
  misc::setPickerColor(d->ColorButt, Pen.color());
  d->LineWidth->setText(QString::number(Pen.width()));
  d->SetComboBox(Pen.style());
  d->ArrowStyleBox->setCurrentIndex(Style);

  if(d->exec() == QDialog::Rejected) {
    delete d;
    return false;
  }

  if(Width != d->HeadWidth->text().toDouble()) {
    Width = d->HeadWidth->text().toDouble();
    changed = true;
  }
  if(Height != d->HeadLength->text().toDouble()) {
    Height = d->HeadLength->text().toDouble();
    changed = true;
  }
  /// \todo deduplicate
  QColor penColor = misc::getWidgetBackgroundColor(d->ColorButt);
  if(Pen.color() != penColor) {
    Pen.setColor(penColor);
    changed = true;
  }
  if(Pen.width() != d->LineWidth->text().toInt()) {
    Pen.setWidth(d->LineWidth->text().toInt());
    changed = true;
  }
  if(Pen.style() != d->LineStyle) {
    Pen.setStyle(d->LineStyle);
    changed = true;
  }
  if(Style != d->ArrowStyleBox->currentIndex()) {
    Style = d->ArrowStyleBox->currentIndex();
    changed = true;
  }

  beta   = atan2(double(Width), double(Height));
  Length = sqrt(Width*Width + Height*Height);
  calcArrowHead();

  delete d;
  return changed;
}

} // namespace
