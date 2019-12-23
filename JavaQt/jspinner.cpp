#include "jspinner.h"
#include "spinnernumbermodel.h"
#include "changeevent.h"
#include "changelistener.h"

JSpinner::JSpinner(QWidget *parent) : QSpinBox(parent)
{
 connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
 listeners = QVector<ChangeListener*>();

}

JSpinner::JSpinner(SpinnerModel* model, QWidget* parent)
{
 setModel(model);
 connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
 listeners = QVector<ChangeListener*>();

}

/*public*/ SpinnerModel* JSpinner::getModel()
{
 if(this->model == nullptr)
  model = new SpinnerNumberModel();
 return model;
}

/*public*/ void JSpinner::setModel(SpinnerModel *model)
{
 this->model = model;
 if(qobject_cast<SpinnerNumberModel*>(model))
 {
  SpinnerNumberModel* nm = (SpinnerNumberModel*)model;
  setValue(nm->getValue().toInt());
  setMinimum(nm->getMinimum().toInt());
  setMaximum(nm->getMaximum().toInt());
  setSingleStep(nm->getStepSize().toInt());
 }
}

/*public*/ bool JSpinner::isOpaque()
{
 return _opaque;
}
/*public*/ QColor JSpinner::getBackground()
{
 return palette().color(QWidget::backgroundRole());
}
/*public*/ void JSpinner::setBackground(QColor c)
{
 setStyleSheet(tr("QWidget{background-color: rgb(%1,%2,%3)").arg(c.red()).arg(c.green()).arg(c.blue()));
}
/*public*/ void JSpinner::setOpaque(bool b)
{
 _opaque = b;
}
/*public*/ QFont JSpinner::getFont()
{
 return font();
}

/*public*/ void JSpinner::setFont(QFont)
{

}

/*public*/ void JSpinner::onValueChanged(int)
{
 emit stateChange(new ChangeEvent(this));
 foreach (ChangeListener* l, listeners) {
  l->stateChanged(new ChangeEvent(this));
 }

}

/*public*/ void JSpinner::addChangeListener(ChangeListener* l)
{
 listeners.append(l);

}
/*public*/ void JSpinner::removeChangeListener(ChangeListener* l)
{
 listeners.removeOne(l);

}