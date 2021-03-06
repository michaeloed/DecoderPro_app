#include "scriptbuttonpanel.h"
#include "jfilechooser.h"
#include "loggerfactory.h"
#include <exceptions.h>
#include <QBoxLayout>
#include "file.h"

/**
 *
 * @author Randall Wood
 */
///*public*/ class ScriptButtonPanel extends JPanel {

/*private*/ /*final*/ /*static*/ Logger* ScriptButtonPanel::log = LoggerFactory::getLogger("ScriptButtonPanel");

/**
 * Creates new form ScriptButtonPanel
 *
 * @param chooser {@link JFileChooser} to use for Browse action
 * @param parent  parent {@link Component} within which this is contained
 */
/*public*/ ScriptButtonPanel::ScriptButtonPanel(JFileChooser* chooser, QWidget* parent) : QWidget(parent)
{
    initComponents();
    this->parent = parent;
    this->chooser = chooser;
}

/**
 * This method is called from within the constructor to initialize the form.
 * WARNING: Do NOT modify this code. The content of this method is always
 * regenerated by the Form Editor.
 */
//@SuppressWarnings("unchecked")
// <editor-fold defaultstate="collapsed" desc="Generated Code">
/*private*/ void ScriptButtonPanel::initComponents() {

    buttonName = new JTextField();
    nameLabel = new QLabel();
    scriptLabel = new QLabel();
    script = new JTextField();
    scriptButton = new QPushButton();

//    java.util.ResourceBundle bundle = java.util.ResourceBundle.getBundle("apps/startup/Bundle"); // NOI18N
    buttonName->setText(tr("")); // NOI18N

    nameLabel->setText(tr("Name:")); // NOI18N

    scriptLabel->setText(tr("Script:")); // NOI18N

    script->setText(tr("")); // NOI18N

    scriptButton->setText(tr("Browse...")); // NOI18N
//    scriptButton.addActionListener(new java.awt.event.ActionListener() {
//        /*public*/ void actionPerformed(java.awt.event.ActionEvent evt) {
//            scriptButtonActionPerformed(evt);
//        }
//    });
    connect(scriptButton, SIGNAL(clicked()), this, SLOT(scriptButtonActionPerformed(JActionEvent*)));
#if 0
    GroupLayout layout = new GroupLayout(this);
    this->setLayout(layout);
    layout.setHorizontalGroup(
        layout.createParallelGroup(GroupLayout.Alignment.LEADING)
        .addGroup(layout.createSequentialGroup()
            .addContainerGap()
            .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                .addComponent(nameLabel)
                .addComponent(scriptLabel))
            .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
            .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                .addComponent(script, GroupLayout.DEFAULT_SIZE, 220, Short.MAX_VALUE)
                .addComponent(buttonName))
            .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
            .addComponent(scriptButton)
            .addContainerGap())
    );
    layout.setVerticalGroup(
        layout.createParallelGroup(GroupLayout.Alignment.LEADING)
        .addGroup(layout.createSequentialGroup()
            .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                .addComponent(buttonName, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addComponent(nameLabel))
            .addPreferredGap(LayoutStyle.ComponentPlacement.UNRELATED)
            .addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                .addComponent(script, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addComponent(scriptLabel)
                .addComponent(scriptButton)))
    );
#endif
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(buttonName);
    buttonLayout->addWidget(nameLabel);
    QHBoxLayout* scriptLayout = new QHBoxLayout();
    scriptLayout->addWidget(script);
    scriptLayout->addWidget(scriptLabel);
    scriptLayout->addWidget(scriptButton);
    QVBoxLayout* thisLayout = new QVBoxLayout(this);
    thisLayout->addLayout(scriptLayout);
    thisLayout->addLayout(buttonLayout);
    this->adjustSize();

}// </editor-fold>

/*private*/ void ScriptButtonPanel::scriptButtonActionPerformed(JActionEvent* evt) {
    if (this->chooser->showOpenDialog(parent) == JFileChooser::APPROVE_OPTION) {
        try {
            this->script->setText(this->chooser->getSelectedFile()->getCanonicalPath());
        } catch (IOException ex) {
            log->error(tr("File %1 does not exist.").arg(this->chooser->getSelectedFile()->toString()));
        }
    }
}



/**
 * @return the name
 */
/*protected*/ QString ScriptButtonPanel::getButtonName() {
    return buttonName->text();
}

/**
 * @param name the name to set
 */
/*protected*/ void ScriptButtonPanel::setButtonName(QString name) {
    this->buttonName->setText(name);
}

/**
 * @return the script
 */
/*protected*/ QString ScriptButtonPanel::getScript() {
    return script->text();
}

/**
 * @param script the script to set
 */
/*protected*/ void ScriptButtonPanel::setScript(QString script) {
    this->script->setText(script);
}
