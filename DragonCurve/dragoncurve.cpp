#include "dragoncurve.h"

#include <QPainter>

DragonCurve::DragonCurve(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

DragonCurve::~DragonCurve()
{

}

enum direction {
  up, down, left, right
};

void DragonCurve::paintEvent(QPaintEvent *)
{
  //static const int step = 50/curve_.iterations;
  static const int step = 1;
  QPainter p(this);
  //p.setRenderHint(QPainter::Antialiasing);
  p.fillRect(rect(), Qt::white);

  p.setPen(Qt::black);
  QPoint cur(rect().center());
  p.drawPoint(cur);
  cur.ry() -= step;
  //p.drawLine(rect().center(), cur);
  p.drawPoint(cur);
  direction dir = up;

  for (size_t i = 0; i != curve_.elements; ++i) {
    QPoint dst = cur;
    bool r_turn = curve_[i];
    switch (dir)
    {
    case up:
      if (r_turn) {
        dir = right;
        dst.rx() += step;
      }
      else {
        dir = left;
        dst.rx() -= step;
      }
      break;

    case down:
      if (r_turn) {
        dir = left;
        dst.rx() -= step;
      }
      else {
        dir = right;
        dst.rx() += step;
      }
      break;

    case left:
      if (r_turn) {
        dir = up;
        dst.ry() -= step;
      }
      else {
        dir = down;
        dst.ry() += step;
      }
      break;

    case right:
      if (r_turn) {
        dir = down;
        dst.ry() += step;
      }
      else {
        dir = up;
        dst.ry() -= step;
      }
      break;
    }

    //p.drawLine(cur, dst);
    p.drawPoint(dst);
    cur = dst;
  }
}

