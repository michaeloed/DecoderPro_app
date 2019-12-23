#ifndef JCOMBOBOX_H
#define JCOMBOBOX_H
#include <qcombobox.h>
#include "jcomponent.h"

class EventObject;
class JComboBox : public QComboBox, public JComponent
{
 Q_OBJECT
 Q_INTERFACES(JComponent)
public:
 JComboBox(QWidget* parent = nullptr);
 JComboBox(QStringList list, QWidget* parent = nullptr);

 bool isOpaque();
 QColor getBackground();
 void setBackground(QColor);
 void setOpaque(bool);
 QFont getFont();
 void setFont(QFont);
 QObject* jself() {return (QObject*)this;}

signals:
 void itemStateChanged(EventObject* e);
private:
bool _opaque = false;
private slots:
 void currentIndexChanged(int);
};

#endif // JCOMBOBOX_H