#ifndef ADDNEWBEANPANEL_H
#define ADDNEWBEANPANEL_H
#include "jmripanel.h"

class ActionListener;
class QCheckBox;
class QPushButton;
class QLabel;
class JTextField;
class AddNewBeanPanel : public JmriPanel
{
 Q_OBJECT
public:
 //explicit AddNewBeanPanel(QWidget *parent = 0);
 /*public*/ AddNewBeanPanel(JTextField* sys, JTextField* userName, JTextField* endRange, QCheckBox* addRange, QCheckBox* autoSystem,
         QString addButtonLabel, ActionListener* listener, QWidget *parent = 0);

signals:

public slots:
private:
 QPushButton* ok;
 JTextField* sysName;
 QLabel* sysNameLabel;// = new JLabel(rb.getString("LabelSystemName"));
 QLabel* userNameLabel;// = new JLabel(rb.getString("LabelUserName"));

 JTextField* _endRange;
 QCheckBox* _range;
 QCheckBox* _autoSys;
 QLabel* finishLabel;// = new JLabel(rb.getString("LabelNumberToAdd"));
private slots:
 /*private*/ void autoSystemName();
 /*private*/ void rangeState();

};

#endif // ADDNEWBEANPANEL_H
