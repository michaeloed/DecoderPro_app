#ifndef LOCATIONMANAGER_H
#define LOCATIONMANAGER_H

#include <QObject>
#include <QHash>
#include "logger.h"
#include <QtXml>
#include "appslib_global.h"

class Reporter;
class JComboBox;
class PropertyChangeSupport;
namespace Operations
{
 class Track;
 class Location;
 class APPSLIBSHARED_EXPORT LocationManager : public QObject
 {
  Q_OBJECT
 public:
  explicit LocationManager(QObject *parent = 0);
  /*public*/ static /*final*/ QString LISTLENGTH_CHANGED_PROPERTY;// = "locationsListLength"; // NOI18N
  QT_DEPRECATED /*public*/ static /*synchronized*/ LocationManager* instance();
  /*public*/ void dispose();
  /*public*/ int getNumberOfLocations() ;
  /*public*/ Location* getLocationByName(QString name);
  /*public*/ Location* getLocationById(QString id);
  /*public*/ void load(QDomElement root);
  /*public*/ void store(QDomElement root, QDomDocument doc);
  /*public*/ QList<Location*> getLocationsByIdList();
  /*public*/ QList<Location*> getList();
  /*public*/ void _register(Location* location);
  PropertyChangeSupport* pcs;// = new java.beans.PropertyChangeSupport(this);
  /*public*/ Location* getLocationByReporter(Reporter* r);
  /*public*/ Location* newLocation(QString name);
  /*public*/ JComboBox* getComboBox();
  /*public*/ void updateComboBox(JComboBox *box);
  /*public*/ QList<Location*> getLocationsByNameList();
  /*public*/ void deregister(Location* location);
  /*public*/ void replaceLoad(QString type, QString oldLoadName,QString newLoadName);
  /*public*/ int getMaxLocationNameLength() ;
  /*public*/ int getMaxTrackNameLength();
  /*public*/ int getMaxLocationAndTrackNameLength();
  /*public*/ QList<Track*> getTracks(QString type);
  /*public*/ QList<Track*> getTracksByMoves(QString type);
  /*public*/ void resetMoves();
  Q_INVOKABLE/*public*/ void initialize();

 signals:

 public slots:
 private:
  /*private*/ static LocationManager* _instance;// = null;
  /*private*/ int _id;// = 0;
  Logger* log;
  /*private*/ void calculateMaxNameLengths();

 protected:
  /*protected*/ QHash<QString, Location*> _locationHashTable;// = new Hashtable<String, Location>();
  /*protected*/ void setDirtyAndFirePropertyChange(QString p, QVariant old, QVariant n);
  /*protected*/ int _maxLocationNameLength;// = 0;
  /*protected*/ int _maxTrackNameLength;// = 0;
  /*protected*/ int _maxLocationAndTrackNameLength;// = 0;

 };
}
#endif // LOCATIONMANAGER_H
