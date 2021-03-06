#ifndef LOCOIODIALOG_H
#define LOCOIODIALOG_H

#include <QDialog>
#include "locoio.h"
#include "pr3adapter.h"
#include "locoiodata.h"
#include "saverestoredialog.h"
#include <QLineEdit>
#include "inputwidget.h"
#include "outputwidget.h"
#include "registerswidget.h"
#include "inportdefinitionwidget.h"
#include "addressportdefinitionwidget.h"
#include "outportdefinitionwidget.h"
#include "extraopcodewidget.h"
#include "extraopcodedefinitionwidget.h"
#include "servowidget.h"
#include "servoregisterswidget.h"
#include <QRegExpValidator>
#include "boosterinputwidget.h"
#include "boosteroutputwidget.h"
#include "boosterpanel.h"
#include "sql.h"
#include "locoio_global.h"
#include "addressvalidator.h"

namespace Ui {
class LocoIODialog;
}



class UserPreferencesManager;
class LocoIOModules;
class LocoIOAddress;
class LOCOIOSHARED_EXPORT LocoIODialog : public QDialog
{
 Q_OBJECT
    
public:
 explicit LocoIODialog(LocoIOAddress* address, LnTrafficController* tc, bool bHex, Sql* sql, QWidget *parent = 0);
 ~LocoIODialog();
 void newAddress();
 void setData(LocoIOData* data);
 LocoIOData* Data();
 void setAddress(int unitAddress, int unitSubAddress);
 void retranslate_Controls();
 LocoIOAddress* address();

public slots:
 //void onIOComplete(QList<int>* vals);
 void setName(int, QString);

private:
 Ui::LocoIODialog *ui;
 //LocoBufferAdapter* locobufferadapter;
 LnTrafficController* tc;
 LocoIOData* data;
// int unitAddr;
// int unitSubAddr;
 LocoIOAddress * _address;
 Logger* log;
 QStringList* msg;
 bool bDirty;
 void enableUi(bool bEnable);
 QString moduleName;
 SaveRestoreDialog* saveDlg;
 int decodeText(QLineEdit* ctl);
 void setupTable();
 //void setupCbAddr();
 QStringList labels;
 QStringList servoLabels;
 QStringList boosterLabels;
 ServoWidget* servoWidget;
 ServoRegistersWidget* servoRegistersWidget;
 bool bLoadCB;
 bool bLoading;
 AddressValidator* rv;
 QRegExp rxHex;
 QRegExp rxDec;
 qint32 val;
 bool bIsServo;
 bool bIsBooster;
 bool bReadingAll;
 bool bWritingAll;
 bool checkDirty();
 int currPort;
 Sql* sql;
 int cfg;
 LocoIOModules* locoIOModules;
 UserPreferencesManager* p;
 QString displayHexCheck;// = this.getClass().getName()+".displayHexCheck";
 QString allowEdits;


public slots:
 void on_btnWriteAll_clicked();

signals:
 void LnStatusChange(QString ss);
 void displayFormat(bool bHex);
 void allowEdit(bool bAllow);
 void specialPorts();
 void retranslateControls();

private slots:
 void on_btnProbe_clicked();
 void on_btnReadAll_clicked();
 void onAddrChanged(const QString & position);
 void onPropertyChange(PropertyChangeEvent*);
 void on_btnCapture_clicked();
 void on_btnHide_clicked();
 void on_btnChangeAddress_clicked();
 void on_btnSetDefaults_clicked();
 void onMessageReceived(LocoNetMessage* m, bool bOut);
 void setDirty(bool bDirty);
 void on_btnSave_clicked();
 void on_btnRestore_clicked();
 //void on_tableWidget_cellClicked(int row, int column);
 void onConfigRead(int cfg, LocoIOData* data);
//    void on_edUnitAddr_editingFinished();
//    void on_edSubAddr_editingFinished();
 void on_chkExtra1_toggled(bool bChecked);
 void on_chkExtra2_toggled(bool bChecked);
 void on_chkHex_toggled(bool bChecked);
 void on_chkAllowEdit_toggled(bool bChecked);
 //void on_chkServoIO_toggled(bool bChecked);
 void on_chk4PosServo_toggled(bool bChecked);
 void onProbeCompleted(QList<LocoIOAddress*> list);
 void on_chkSpecialPorts_toggled(bool bChecked);
// void on_cbAddr_currentIndexChanged(int index);
 void on_edNewAddr_editTextChanged(QString text);
 void on_edNewAddr_editingFinished();
 void onIOComplete(QList<int>* vals);
 void onIOCompleteX2(QList<int>* vals);
 void onIOCompleteServo();
 void onIOAborted();
 void onIOCompleted7D();
 void onReadWriteAllComplete();
 //void on_chkBooster_toggled(bool bChecked);
 void on_rbFixCodeForPBs_toggled(bool bChecked);
 void customContextMenuRequested(QPoint);
 void tableCustomContextMenuRequested(QPoint);
 void onChangeModuleType(int);
 void on_chkInputStatus_toggled(bool bChecked);
 void onInputStatusRbChanged(int i);
 void onUpdateComments();
 void onOptionByte(int, int);
};

#endif // LOCOIODIALOG_H
