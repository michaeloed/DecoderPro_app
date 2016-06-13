#ifndef VARIABLETABLEMODEL_H
#define VARIABLETABLEMODEL_H
#include <QVector>
#include <QStringList>
#include <QLabel>
#include "cvtablemodel.h"
#include "indexedcvtablemodel.h"
#include "variablevalue.h"
#include <QtXml>
#include "exceptions.h"
#include "qualifieradder.h"

class ActionEvent;
class LIBPR3SHARED_EXPORT VariableTableModel : public AbstractTableModel
{
    Q_OBJECT
public:
    explicit VariableTableModel(QObject *parent = 0);
/*public*/ VariableTableModel(QLabel* status, QStringList h, CvTableModel* cvModel, IndexedCvTableModel* iCvModel, QObject *parent = 0);
    /*public*/ int rowCount(const QModelIndex &parent) const;
    /*public*/ int columnCount(const QModelIndex &parent) const;
    /*public*/ QString getColumnName(int col);
    /*public*/ QString getColumnClass(int col);
    /*public*/ bool isCellEditable(int row, int col);
    /*public*/ VariableValue* getVariable(int row);
    /*public*/ QString getLabel(int row);
    /*public*/ QString getItem(int row);
    /*public*/ QString getCvName(int row);
    /*public*/ QString getValString(int row);
    /*public*/ void setIntValue(int row, int val);
    /*public*/ void setState(int row, int val);
    /*public*/ int getState(int row) ;
    /*public*/ QWidget* getRep(int row, QString format);
    /*public*/ QVariant data(const QModelIndex &index, int role) const;
    /*public*/ bool setData(const QModelIndex &index, const QVariant &value, int role);
    /*public*/ void setRow(int row, QDomElement e);
    /*public*/ QString piCv();
    /*public*/ QString siCv();
    /*public*/ int setIndxRow(int row, QDomElement e, QString productID);
    void reportBogus() ;
    /*public*/ void setConstant(QDomElement e) ;
    /*public*/ void newDecVariableValue(QString name, QString CV, QString mask,
                                    bool readOnly, bool infoOnly, bool writeOnly, bool opsOnly);
    /*public*/ void actionPerformed(ActionEvent* e);
    /*public*/ void read(int i);
    /*public*/ void write(int i);
    /*public*/ void configDone() ;
    /*public*/ bool fileDirty() ;
    /*public*/ void setFileDirty(bool b);
    /*public*/ bool decoderDirty();
    /*public*/ VariableValue* findVar(QString name);
    /*public*/ int findVarIndex(QString name);
    /*public*/ void dispose();
    /*public*/  QVector<CvValue*> allIndxCvVector();
    /*public*/  QMap<QString, CvValue*>* allIndxCvMap();
    /**
     * Gives access to the programmer used to reach these Indexed CVs,
     * so you can check on mode, capabilities, etc.
     * @return Programmer object for the Indexed CVs
     */
    /*public*/ Programmer* getProgrammer() ;
    /*public*/ void setProgrammer(Programmer* p) ;
signals:

public slots:
    /*public*/ void propertyChange(PropertyChangeEvent* e);
private:
    /*private*/ QStringList headers;// = NULL;

    /*private*/ QVector<VariableValue*>* rowVector;// = new Vector<VariableValue>();  // vector of Variable items
    /*private*/ CvTableModel* _cvModel;// = null;          // reference to external table model
    /*private*/ IndexedCvTableModel* _indxCvModel;// = NULL;
    /*private*/ QVector<QPushButton*>* _writeButtons;// = new QVector<QPushButton*>();
    /*private*/ QVector<QPushButton*>* _readButtons;// = new QVector<QPushButton*>();
    /*private*/ QLabel* _status;// = NULL;
    /*private*/ QString _piCv;// = -1;
    /*private*/ QString _siCv;// = -1;
    /*private*/ bool _fileDirty;
 protected:
    /*protected*/ void processModifierElements(QDomElement e, VariableValue* v);
    /*protected*/ VariableValue* createIndexedVariableFromElement(QDomElement e, int row, QString name, QString comment, QString cvName, bool readOnly, bool infoOnly, bool writeOnly, bool opsOnly, QString cv, QString mask, QString item, QString productID) throw (NumberFormatException) ;
    /*protected*/ bool setDefaultValue(QDomElement e, VariableValue* v);
    /*protected*/ VariableValue* processCompositeVal(QDomElement child, QString name, QString comment, bool readOnly, bool infoOnly, bool writeOnly, bool opsOnly, QString CV, QString mask, QString item);
    /*protected*/ VariableValue* processDecVal(QDomElement child, QString name, QString comment, bool readOnly, bool infoOnly, bool writeOnly, bool opsOnly, QString CV, QString mask, QString item) throw (NumberFormatException);
    /*protected*/ VariableValue* processEnumVal(QDomElement child, QString name, QString comment, bool readOnly, bool infoOnly, bool writeOnly, bool opsOnly, QString CV, QString mask, QString item) throw (NumberFormatException);
    /*protected*/ VariableValue* processHexVal(QDomElement child, QString name, QString comment, bool readOnly, bool infoOnly, bool writeOnly, bool opsOnly, QString CV, QString mask, QString item) throw (NumberFormatException) ;
    /*protected*/ VariableValue* processIEnumVal(QDomElement child, int row, QString name, QString comment, QString cvName, bool readOnly, bool infoOnly, bool writeOnly, bool opsOnly, QString cv, QString mask, QString item, QString productID) throw (NumberFormatException);
    /*protected*/ VariableValue* processIndexedPairVal(QDomElement child, int row, bool readOnly, bool infoOnly, bool writeOnly, QString name, QString comment, QString cvName, bool opsOnly, QString cv, QString mask, QString item) throw (NumberFormatException) ;
    /*protected*/ VariableValue* processIndexedVal(QDomElement child, int row, QString name, QString comment, QString cvName, bool readOnly, bool infoOnly, bool writeOnly, bool opsOnly, QString cv, QString mask, QString item) throw (NumberFormatException) ;
    /*protected*/ VariableValue* processLongAddressVal(QString CV, bool readOnly, bool infoOnly, bool writeOnly, QString name, QString comment, bool opsOnly, QString mask, QString item) ;
    /*protected*/ VariableValue* processShortAddressVal(QString name, QString comment, bool readOnly, bool infoOnly, bool writeOnly, bool opsOnly, QString CV, QString mask, QString item, QDomElement child) ;
    /*protected*/ VariableValue* processSpeedTableVal(QDomElement child, QString CV, bool readOnly, bool infoOnly, bool writeOnly, QString name, QString comment, bool opsOnly, QString mask, QString item) throw (NumberFormatException) ;
    /*protected*/ VariableValue* processSplitVal(QDomElement child, QString CV, bool readOnly, bool infoOnly, bool writeOnly, QString name, QString comment, bool opsOnly, QString mask, QString item) throw (NumberFormatException) ;
    /*protected*/ void setButtonsReadWrite(bool readOnly, bool infoOnly, bool writeOnly, QPushButton* bw, QPushButton* br, int row);
    /*protected*/ void setToolTip(QDomElement e, VariableValue* v);
 Logger* log;
};
class VTQualifierAdder : public QualifierAdder
{
    Q_OBJECT
    VariableValue* v;
public:
    VTQualifierAdder(VariableValue* v);
protected:
   /*protected*/ Qualifier* createQualifier(VariableValue* var, QString relation, QString value);
   /*protected*/ void addListener(PropertyChangeListener* qc) ;
};

#endif // VARIABLETABLEMODEL_H
