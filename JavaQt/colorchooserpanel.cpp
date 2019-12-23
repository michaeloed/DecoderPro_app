#include "colorchooserpanel.h"
#include "jlabel.h"
#include "colorpanel.h"
#include "gridbagconstraints.h"
#include "gridbaglayout.h"
#include "colormodel.h"
#include "jtextfield.h"
#include "diagramcomponent.h"
#include "imageicon.h"
#include "propertychangeevent.h"
#include <QVariant>
#include "colorselectionmodel.h"
#include <QColor>

// /*final*/ class ColorChooserPanel extends AbstractColorChooserPanel implements PropertyChangeListener {
/*private*/ /*static*/ /*final*/ int ColorChooserPanel::MASK = 0xFF000000;


ColorChooserPanel::ColorChooserPanel(ColorModel* model) {
    this->model = model;
    this->panel = new ColorPanel(this->model);
    this->slider = new DiagramComponent(this->panel, false);
    this->diagram = new DiagramComponent(this->panel, true);
    this->text = new JTextField();//JFormattedTextField();
    this->label = new JLabel("", nullptr, Qt::AlignRight);
    //ValueFormatter.init(6, true, this->text);
}

//@Override
/*public*/ void ColorChooserPanel::setEnabled(bool enabled) {
    AbstractColorChooserPanel::setEnabled(enabled);
    setEnabled(this, enabled);
}

/*private*/ /*static*/ void ColorChooserPanel::setEnabled(/*Container*/QWidget* container, bool enabled) {
#if 0
    for (Component component : container.getComponents())
    {
        component.setEnabled(enabled);
        if (component instanceof Container) {
            setEnabled((Container) component, enabled);
        }
    }
#endif
}

//@Override
/*public*/ void ColorChooserPanel::updateChooser() {
    QColor color = getColorFromModel();
    if (color.isValid()) {
        this->panel->setColor(color);
        this->text-> setText(QString::number(color.value()));//setValue( color.value());//Integer.valueOf(color.getRGB()));
        this->slider->repaint();
        this->diagram->repaint();
    }
}

//@Override
/*protected*/ void ColorChooserPanel::buildChooser() {
//    if (0 == getComponentCount())
    {
     GridBagLayout* g;
        setLayout(g = new GridBagLayout());

        GridBagConstraints gbc = GridBagConstraints();

        gbc.gridx = 3;
        gbc.gridwidth = 2;
        gbc.weighty = 1.0;
        gbc.anchor = GridBagConstraints::NORTH;
        gbc.fill = GridBagConstraints::HORIZONTAL;
        gbc.insets->top = 10;
        gbc.insets->right = 10;
        g->addWidget(this->panel, gbc);

        gbc.gridwidth = 1;
        gbc.weightx = 1.0;
        gbc.weighty = 0.0;
        gbc.anchor = GridBagConstraints::CENTER;
        gbc.insets->right = 5;
        gbc.insets->bottom = 10;
        g->addWidget(this->label, gbc);

        gbc.gridx = 4;
        gbc.weightx = 0.0;
        gbc.insets->right = 10;
        g->addWidget(this->text, gbc);

        gbc.gridx = 2;
        gbc.gridheight = 2;
        gbc.anchor = GridBagConstraints::NORTH;
        gbc.ipadx = this->text->sizeHint().height();
        gbc.ipady = sizeHint().height();
        g->addWidget(this->slider, gbc);

        gbc.gridx = 1;
        gbc.insets->left = 10;
        gbc.ipadx = gbc.ipady;
        g->addWidget(this->diagram, gbc);

        this->label->setLabelFor(this->text);
//        this->text->addPropertyChangeListener("value", this); // NON-NLS: the property name
//        this->slider.setBorder(this->text.getBorder());
//        this->diagram.setBorder(this->text.getBorder());

        setInheritsPopupMenu((JComponent*)this, true); // CR:4966112
    }
    QString label = this->model->getText((Component*)this, "HexCode"); // NON-NLS: suffix
    bool visible = label != "";
//    this->text->setVisible(visible);
//    this->text.getAccessibleContext().setAccessibleDescription(label);
//    this->label->setVisible(visible);
    if (visible) {
//        this->label->setText(label);
        int mnemonic = this->model->getInteger((Component*)this, "HexCodeMnemonic"); // NON-NLS: suffix
        if (mnemonic > 0) {
            this->label->setDisplayedMnemonic(mnemonic);
            mnemonic = this->model->getInteger((Component*)this, "HexCodeMnemonicIndex"); // NON-NLS: suffix
            if (mnemonic >= 0) {
//                this->label->setDisplayedMnemonicIndex(mnemonic);
            }
        }
    }
    this->panel->buildPanel();
}

//@Override
/*public*/ QString ColorChooserPanel::getDisplayName() {
    return this->model->getText((Component*)this, "Name"); // NON-NLS: suffix
}

//@Override
/*public*/ int ColorChooserPanel::getMnemonic() {
    return this->model->getInteger((Component*)this, "Mnemonic"); // NON-NLS: suffix
}

//@Override
/*public*/ int ColorChooserPanel::getDisplayedMnemonicIndex() {
    return this->model->getInteger((Component*)this, "DisplayedMnemonicIndex"); // NON-NLS: suffix
}

//@Override
/*public*/ QIcon ColorChooserPanel::getSmallDisplayIcon() {
    return QIcon();
}

//@Override
/*public*/ QIcon ColorChooserPanel::getLargeDisplayIcon() {
    return QIcon();
}

/*public*/ void ColorChooserPanel::propertyChange(PropertyChangeEvent* event) {
    ColorSelectionModel* model = getColorSelectionModel();
    if (model != nullptr) {
        QVariant object = event->getNewValue();
        //if (object instanceof Integer)
        if(object.canConvert(QMetaType::Int))
        {
            int value = MASK & model->getSelectedColor().rgb() |  object.toInt();
            model->setSelectedColor(QColor());       //QColor(value, true));
        }
    }
    this->text->selectAll();
}

/**
 * Allows to show context popup for all components recursively.
 *
 * @param component  the root component of the tree
 * @param value      whether or not the popup menu is inherited
 */
/*private*/ /*static*/ void ColorChooserPanel::setInheritsPopupMenu(JComponent* component, bool value) {
    component->setInheritsPopupMenu(value);
#if 0
    for (Object object : component.getComponents()) {
        if (object instanceof JComponent) {
            setInheritsPopupMenu((JComponent) object, value);
        }
    }
#endif
}
