#include "trainbycartypeaction.h"
#include "trainbycartypeframe.h"

namespace Operations
{
/**
 * Swing action to create and register a TrainByCarTypeFrame.
 *
 * @author Bob Jacobsen Copyright (C) 2001
 * @author Daniel Boudreau Copyright (C) 2010
 * @version $Revision: 28746 $
 */
 ///*public*/ class TrainByCarTypeAction extends AbstractAction {

 /**
  *
  */
 //private static final long serialVersionUID = -5831631159830861734L;

 /*public*/ TrainByCarTypeAction::TrainByCarTypeAction(QString s, Train* train, QObject* parent)
    : AbstractAction(s, parent)
 {
     //super(s);
     _train = train;
     connect(this, SIGNAL(triggered(bool)), this, SLOT(actionPerformed()));
 }


 /*public*/ void TrainByCarTypeAction::actionPerformed(ActionEvent* e) {
     // create frame
     TrainByCarTypeFrame* f = new TrainByCarTypeFrame();
     f->initComponents(_train);
 }
}
