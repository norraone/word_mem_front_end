#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>

class CustomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomWidget(QWidget *parent = nullptr);
    ~CustomWidget();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    // Add your private members here
};

#endif // CUSTOMWIDGET_H
