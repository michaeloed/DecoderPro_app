#ifndef SPROGTURNOUTMANAGER_H
#define SPROGTURNOUTMANAGER_H
#include "abstractturnoutmanager.h"
#include "sprogsystemconnectionmemo.h"

namespace Sprog
{
 class SprogTurnoutManager : public AbstractTurnoutManager
 {
  Q_OBJECT

 public:
  SprogTurnoutManager(SprogSystemConnectionMemo*, QObject* parent = 0);
  /*public*/ QString getSystemPrefix();
  /*public*/ Turnout* createNewTurnout(QString systemName, QString userName);
  /*public*/ int getBitFromSystemName(QString systemName);
  /*public*/ NameValidity validSystemNameFormat(QString systemName);
  /*public*/ bool allowMultipleAdditions(QString systemName);
  /*public*/ QString getEntryToolTip();
  QT_DEPRECATED
  static /*public*/ SprogTurnoutManager* instance();

 private:
  /*private*/ /*final*/ static Logger* log;// = LoggerFactory::getLogger("SprogTurnoutManager");
  SprogSystemConnectionMemo* _memo;// = null;

 };
}
#endif // SPROGTURNOUTMANAGER_H