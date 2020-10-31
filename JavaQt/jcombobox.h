#ifndef JCOMBOBOX_H
#define JCOMBOBOX_H
#include <qcombobox.h>
#include "jcomponent.h"
#include "changelistener.h"
#include "focuslistener.h"
#include "itemlistener.h"

class EventObject;
class JComboBox : public QComboBox, public JComponent
{
 Q_OBJECT
 Q_INTERFACES(JComponent)
public:
 JComboBox(QWidget* parent = nullptr);
 JComboBox(QStringList list, QWidget* parent = nullptr);
 ~JComboBox() {}
 JComboBox(const JComboBox&) : QComboBox() {}

 bool isOpaque();
 /*public*/ QColor getForeground();
 QColor getBackground();
 void setBackground(QColor);
 void setOpaque(bool);
 QFont getFont();
 void setFont(QFont);
 QObject* jself() {return (QObject*)this;}
 /*public*/ void setBorder(Border* border) override {this->_border = border;}
 /*public*/ Border* getBorder() {return _border;}
 /*public*/ void addChangeListener(ChangeListener* l);
 /*public*/ void removeChangeListener(ChangeListener* l);
 /*public*/ void addFocusListener(FocusListener *l);
 /*public*/ void removeFocusListener(FocusListener* l);
 /*public*/ void addItemListener(ItemListener* listener);
 /*public*/ void setEnabled(bool b) override {QComboBox::setEnabled(b);}

signals:
 void itemStateChanged(ItemEvent* e);
 /*public*/ void focusGained(FocusEvent* fe);
 /*public*/ void focusLost(FocusEvent* fe);

 public slots:
 void currentIndexChanged(int);

private:
bool _opaque = false;
Border* _border = nullptr;
/*private*/ void focusInEvent(QFocusEvent* e);
/*private*/ void focusOutEvent(QFocusEvent* e);

private slots:
 //void on_selected();
};
Q_DECLARE_METATYPE(JComboBox)
#endif // JCOMBOBOX_H
