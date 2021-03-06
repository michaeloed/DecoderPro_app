#include "printroutesaction.h"
#include "logger.h"
#include "hardcopywriter.h"
#include "control.h"
#include "routemanager.h"
#include "route.h"

namespace Operations
{
 /**
  * Action to print all of the routes used in operations.
  *
  * @author Daniel Boudreau Copyright (C) 2012
  * @version $Revision: 17977 $
  */
 ///*public*/ class PrintRoutesAction extends PrintRouteAction {

 /**
  *
  */
 //private static final long serialVersionUID = 6083754676592916495L;
 /*private*/ /*static*/ /*final*/ char PrintRoutesAction::FORM_FEED = '\f';

 /*public*/ PrintRoutesAction::PrintRoutesAction(QString actionName, bool preview, QObject* parent)
  : PrintRouteAction(actionName, preview, NULL,parent)
 {
 log = new Logger("PrintRoutesAction");
 mFrame = new JFrame();
     //super(actionName, preview, null);
 //connect(this, SIGNAL(triggered(bool)), this, SLOT(actionPerformed())); not necessary, done by PrintRouteAction
 }

 /*public*/ void PrintRoutesAction::actionPerformed(ActionEvent* /*e*/)
 {
  log->debug("Print all routes");
  // obtain a HardcopyWriter to do this
  HardcopyWriter* writer = NULL;
  //try {
      writer = new HardcopyWriter(mFrame, tr(
              "Print all routes"), Control::reportFontSize, .5, .5, .5, .5,
              isPreview);
//     } catch (HardcopyWriter.PrintCanceledException ex) {
//         log.debug("Print cancelled");
//         return;
//     }
  QList<Route*> routes = RouteManager::instance()->getRoutesByNameList();
  for (int i = 0; i < routes.size(); i++) {
      Route* route = routes.at(i);
      //try {
          writer->write(route->getName() + NEW_LINE);
          printRoute(writer, route);
          if (i != routes.size() - 1) {
              writer->write(QString(FORM_FEED));
          }
//         } catch (IOException e1) {
//             log.error("Exception in print routes");
//         }
  }
  // and force completion of the printing
  writer->close();
 }
}
