#ifndef LAYOUTXOVERVIEW_H
#define LAYOUTXOVERVIEW_H
#include "layoutturnoutview.h"
#include "layoutxover.h"

class LayoutXOverView : public LayoutTurnoutView
{
  Q_OBJECT
 public:
  /*public*/ LayoutXOverView(/*@Nonnull*/ LayoutXOver* xover);

};

#endif // LAYOUTXOVERVIEW_H
