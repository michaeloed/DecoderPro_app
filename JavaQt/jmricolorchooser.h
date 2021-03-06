#ifndef JMRICOLORCHOOSER_H
#define JMRICOLORCHOOSER_H

#include <QObject>
#include <QWidget>
#include "actionlistener.h"
#include "jcolorchooser.h"

class JColorChooser;
class JmriColorChooser : public QObject
{
 Q_OBJECT
public:
 explicit JmriColorChooser(QObject *parent = nullptr);
 static /*public*/ void addRecentColor(QColor color);
 static /*public*/ QList<QColor> getRecentColors();
 static /*public*/ QColor showDialog(QWidget *comp, QString dialogTitle, QColor currentColor);
 static /*public*/ JColorChooser* extendColorChooser(JColorChooser* chooser);
 QColor getForeground();

signals:

public slots:
private:
 static /*private*/ QList<QColor> recentColors;//= new ArrayList<>();
 static /*final*/ int COLOR_TAB_COUNT;// = 6;
 static QColor color;

};
#if 0
class JCCActionListener : public ActionListener
{
 Q_OBJECT
 JmriColorChooser* jmriColorChooser;
 JColorChooser* chooser;
public:
 JCCActionListener(JColorChooser* chooser, JmriColorChooser* jmriColorChooser)
 {
  this->jmriColorChooser = jmriColorChooser;
  this->chooser = chooser;
 }
public slots:
 void actionPerformed()
 {
  jmriColorChooser->color = chooser->getColor();
 }

};
#endif
#endif // JMRICOLORCHOOSER_H
