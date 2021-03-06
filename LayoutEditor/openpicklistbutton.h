#ifndef OPENPICKLISTBUTTON_H
#define OPENPICKLISTBUTTON_H
#include "namedbean.h"
#include "jframe.h"
#include <QPushButton>
#include "picklistmodel.h"

class OpenPickListButton : public NamedBean
{
public:
 OpenPickListButton(QStringList blurbLines, PickListModel/*<T>*/* model, QWidget* parent= nullptr);
 /*public*/ JPanel* getButtonPanel();
 /*public*/ void closePickList();
 /*public*/ QString getSystemName()const {}

private:
 /*private*/ JPanel* _buttonPanel = nullptr;
 /*private*/ JFrame* _pickFrame = nullptr;
 /*private*/ QPushButton* _openPicklistButton;
 QStringList _blurbLines;
 PickListModel/*<T>*/* _model = nullptr;
 QWidget* _parent;
 /*private*/ JPanel* makePickListPanel();
 void openPickList();

};

#endif // OPENPICKLISTBUTTON_H
