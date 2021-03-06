#include "optionaction.h"
#include "optionframe.h"

namespace Operations
{
/**
 * Swing action to load the options frame.
 *
 * @author Bob Jacobsen Copyright (C) 2001
 * @author Daniel Boudreau Copyright (C) 2010
 * @version $Revision: 28746 $
 */
//public class OptionAction extends AbstractAction {

 /**
  *
  */
 //private static final long serialVersionUID = 7507020443490655869L;

 /*public*/ OptionAction::OptionAction(QString s, QObject* parent)
 : AbstractAction(s, parent)
 {
     //super(s);
  f = NULL;
 connect(this, SIGNAL(triggered()), this, SLOT(actionPerformed()));
 }


 /*public*/ void OptionAction::actionPerformed(ActionEvent* /*e*/) {
     if (f == NULL || !f->isVisible()) {
         f = new OptionFrame();
         f->initComponents();
     }
     //f.setExtendedState(Frame.NORMAL);
     f->setVisible(true); // this also brings the frame into focus
 }
}
