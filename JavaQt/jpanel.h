#ifndef JPANEL_H
#define JPANEL_H
#include <QFrame>
#include "border.h"
#include "flowlayout.h"

class JPanel : public QFrame
{
 Q_OBJECT
public:
 JPanel(QWidget* parent = nullptr);
 JPanel(QLayout* layout, QWidget* parent=nullptr);
 JPanel(FlowLayout* layout, QWidget* parent=nullptr);

 /*public*/ void setBorder(Border* border);
 /*public*/ virtual QString getTitle() {return "";}
 /*public*/ void setBackground(QColor color);
 /*public*/ void removeAll();

// void paintEvent(QPaintEvent*);
// void virtual paintComponent(QPainter*);
};
#endif // JPANEL_H
