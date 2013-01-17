#ifndef LONPOSWINDOW_H
#define LONPOSWINDOW_H

#include <QMainWindow>

namespace Ui {
class LonposWindow;
}

class LonposWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit LonposWindow(QWidget *parent = 0);
    ~LonposWindow();
    
private:
    Ui::LonposWindow *ui;
};

#endif // LONPOSWINDOW_H
