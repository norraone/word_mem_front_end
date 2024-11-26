#include "customwidget.h"
#include <QPainter>
#include <QPaintEvent>

CustomWidget::CustomWidget(QWidget *parent)
    : QWidget(parent)
{
    // Set size policy
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // Set minimum size
    setMinimumSize(100, 100);
}

CustomWidget::~CustomWidget()
{
}

void CustomWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Example: Draw a filled rectangle
    painter.fillRect(rect(), Qt::white);
    painter.setPen(Qt::black);
    painter.drawRect(rect().adjusted(0, 0, -1, -1));
}
