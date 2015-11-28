#ifndef DRAGONCURVE_H
#define DRAGONCURVE_H

#include <QtWidgets/QMainWindow>
#include <assert.h>
#include "ui_dragoncurve.h"

#include <f:\projs\Visual Studio 2008\Projects\libs\boost\math\common_factor_rt.hpp>

template <int iterations>
class curve {
public:
  static const size_t iterations = iterations;
  static const size_t elements = (1 << iterations) - 1;

  bool operator[](size_t pos) const {
    assert(pos < elements);
    if (pos % 2 == 0) {
      return (pos / 2) % 2;
    }
    else {
      return (pos / boost::math::gcd(pos + 1, elements + 1)) % 4;
    }
  }
};

class DragonCurve : public QMainWindow
{
    Q_OBJECT

public:
    DragonCurve(QWidget *parent = 0);
    ~DragonCurve();

private:
    Ui::DragonCurveClass ui;
    void paintEvent(QPaintEvent *);

    curve<18> curve_;
};

#endif // DRAGONCURVE_H
