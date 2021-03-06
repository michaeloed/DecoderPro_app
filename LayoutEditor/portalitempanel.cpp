#include "portalitempanel.h"
#include "flowlayout.h"
#include "controlpaneleditor.h"

/**
 * ItemPanel for for PortalIcons. Since this class has been introduced after
 * users may have customized the defaultPanelIcons, the default family,
 * "Standard" is added by overriding the initIconFamiliesPanel method.
 *
* @author Pete Cressman Copyright (c) 2013
 */
///*public*/ /*abstract*/ class PortalItemPanel extends FamilyItemPanel {

/**
 * Constructor types with multiple families and multiple icon families.
 */
/*public*/ PortalItemPanel::PortalItemPanel(DisplayFrame* parentFrame, QString type, QString family, Editor *editor, QWidget* parent)
 : FamilyItemPanel(parentFrame, type, family, editor, parent)
{
    //super(parentFrame, type, family, editor);
 setObjectName("PortalItemPanel");
}

/**
 * Init for creation _bottom1Panel and _bottom2Panel alternate visibility in
 * bottomPanel depending on whether icon families exist. They are made first
 * because they are referenced in initIconFamiliesPanel(). Subclasses will
 * insert other panels.
 */
//@Override
/*public*/ void PortalItemPanel::init()
{
 if (!_initialized)
 {
  FamilyItemPanel::init();
  _suppressDragging = true;
  thisLayout->addWidget(makeChangeDefaultIconsPanel());
 }
}

/*private*/ QWidget* PortalItemPanel::makeChangeDefaultIconsPanel() {
    JPanel* panel = new JPanel();
    FlowLayout* panelLayout;
    panel->setLayout(panelLayout =new FlowLayout());
    QPushButton* setDefaultsButton = new QPushButton(tr("Set Default Icons"));
//    setDefaultsButton.addActionListener(new ActionListener() {
//        @Override
//        /*public*/ void actionPerformed(ActionEvent a) {
    connect(setDefaultsButton, &QPushButton::clicked, [=]{
     setDefaults();
//        }
    });
    setDefaultsButton->setToolTip(tr("Change the Default Portal Icons to the selected icon family."));
    panelLayout->addWidget(setDefaultsButton);
    return panel;
}

/*private*/ void PortalItemPanel::setDefaults() {
    QMap<QString, NamedIcon*>* map = getIconMap();
    ((ControlPanelEditor*) _editor)->setDefaultPortalIcons(PositionableIcon::cloneMap(map, NULL));
}

//@Override
/*protected*/ void PortalItemPanel::makeDndIconPanel(QMap<QString, NamedIcon*>* /*iconMap*/, QString /*displayKey*/) {
}
