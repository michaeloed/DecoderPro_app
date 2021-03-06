#ifndef TRAINSBYCARTYPEACTION_H
#define TRAINSBYCARTYPEACTION_H

#include "abstractaction.h"
namespace Operations
{
 class TrainsByCarTypeFrame;
 class TrainsByCarTypeAction : public AbstractAction
 {
  Q_OBJECT
 public:
  TrainsByCarTypeAction(QObject* parent);
  TrainsByCarTypeAction(QString s, QObject* parent);
 public slots:
  /*public*/ void actionPerformed(ActionEvent* e = 0);

 private:
  TrainsByCarTypeFrame* f;// = null;

 };
}
#endif // TRAINSBYCARTYPEACTION_H
