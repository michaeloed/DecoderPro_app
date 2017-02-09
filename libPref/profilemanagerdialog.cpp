#include "profilemanagerdialog.h"
#include "jframe.h"
#include "profilemanager.h"
#include "../libPr3/profile.h"
#include <QCloseEvent>
#include <QTimer>
#include <QListWidget>
#include <QLabel>
#include "QFileDialog"
#include <QBoxLayout>
#include "fileutil.h"
#include "rosterentry.h"
#include "profilelistmodel.h"
#include "system.h"
#include "addprofiledialog.h"
#include "jfilechooser.h"

//ProfileManagerDialog::ProfileManagerDialog(QWidget *parent) :
//    JDialog(parent)
//{
//}
/**
 * Display a list of {@link Profile}s that can be selected to start a JMRI
 * application.
 * <p>
 * This dialog is designed to be displayed while an application is starting. If
 * the last profile used for the application can be found, this dialog will
 * automatically start the application with that profile after 10 seconds unless
 * the user intervenes.
 *
 * @author rhwood
 */
// /*public*/ class ProfileManagerDialog extends JDialog {

    /**
     *
     */
//	/*private*/ static final long serialVersionUID = 8335767552519729376L;

/**
 * Creates new form ProfileManagerDialog
 *
 * @param parent {@inheritDoc}
 * @param modal {@inheritDoc}
 */
/*public*/ProfileManagerDialog::ProfileManagerDialog(JFrame* parent, bool modal)
    : JDialog(parent, modal)
{
 log = new Logger("ProfileManagerDialog");
 // super(parent, modal);
 initComponents();
#if 0
ProfileManager::defaultManager()->addPropertyChangeListener(ProfileManager::ACTIVE_PROFILE, new PropertyChangeListener(); {
        @Override
        /*public*/ void propertyChange(PropertyChangeEvent* evt) {
            profiles.setSelectedValue(ProfileManager::defaultManager().getActiveProfile(), true);
            profiles.repaint();
        }
    });
    ProfileManager::defaultManager()->addPropertyChangeListener(Profile::NAME, new PropertyChangeListener() {

        @Override
        /*public*/ void propertyChange(PropertyChangeEvent* evt) {
            if (evt.getSource().getClass().equals(Profile.class) && evt.getPropertyName().equals(Profile.NAME)) {
                profileNameChanged(((Profile) evt.getSource()));
            }
        }
    });
#endif
    connect(ProfileManager::defaultManager(), SIGNAL(indexedPropertyChange(IndexedPropertyChangeEvent*)), this, SLOT(propertyChange(IndexedPropertyChangeEvent*)));
//    this.jScrollPane1.getVerticalScrollBar().addAdjustmentListener(new AdjustmentListener() {

//        @Override
//        /*public*/ void adjustmentValueChanged(AdjustmentEvent e) {
//            profilesValueChanged(NULL);
//        }
//    });
}

/**
 * This method is called from within the constructor to initialize the form.
 * WARNING: Do NOT modify this code. The content of this method is always
 * regenerated by the Form Editor.
 */
//@SuppressWarnings("unchecked")
// <editor-fold defaultstate="collapsed" desc="Generated Code">
/*private*/ void ProfileManagerDialog::initComponents()
{
 QList<Profile*> list = ProfileManager::defaultManager()->getProfiles();
 listLabel = new QLabel();
 //jScrollPane1 = new QScrollArea();
 profiles = new QListView();
 btnSelect = new QPushButton();
 btnCreate = new QPushButton();
 btnUseExisting = new QPushButton();
 countDownLbl = new QLabel();

 setDefaultCloseOperation(JFrame::DISPOSE_ON_CLOSE);
//    ResourceBundle bundle = ResourceBundle.getBundle("jmri/profile/Bundle"); // NOI18N
 setTitle(tr("Set Active Profile")); // NOI18N
 setMinimumSize( QSize(310, 110));
//    addMouseListener(new MouseAdapter() {
//        /*public*/ void mousePressed(MouseEvent evt) {
//            formMousePressed(evt);
//        }
//    });

//    addWindowListener(new WindowAdapter() {
//        /*public*/ void windowOpened(WindowEvent evt) {
//            formWindowOpened(evt);
//        }
//        /*public*/ void windowClosing(WindowEvent evt) {
//            formWindowClosing(evt);
//        }
//    });

 listLabel->setText(tr("Use Configuration Profile:")); // NOI18N

 profiles->setModel(new ProfileListModel());
//    profiles.setSelectedValue(ProfileManager::defaultManager().getActiveProfile(), true);
 profiles->setSelectionMode(QAbstractItemView::SingleSelection);
 profiles->setToolTip(tr("Select the profile to load JMRI with.")); // NOI18N
//    profiles.setNextFocusableComponent(btnSelect);
//    profiles.addListSelectionListener(new ListSelectionListener() {
//        /*public*/ void valueChanged(ListSelectionEvent evt) {
//            profilesValueChanged(evt);
//        }
//    });
 connect(profiles, SIGNAL(clicked(QModelIndex)), this, SLOT(profilesValueChanged(QModelIndex)));
//    profiles.addKeyListener(new KeyAdapter() {
//        /*public*/ void keyPressed(KeyEvent evt) {
//            profilesKeyPressed(evt);
//        }
//    });

//    jScrollPane1.setViewportView(profiles);
//    profiles.getAccessibleContext().setAccessibleName(tr("ProfileManagerDialog.profiles.AccessibleContext.accessibleName")); // NOI18N

 btnSelect->setText(tr("OK")); // NOI18N
//    btnSelect.addActionListener(new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent evt) {
//            btnSelectActionPerformed(evt);
//        }
//    });
 connect(btnSelect, SIGNAL(clicked()), this, SLOT(btnSelectActionPerformed()));

 btnCreate->setText(tr("New...")); // NOI18N
 btnCreate->setToolTip(tr("Create a new profile.")); // NOI18N
//    btnCreate.addActionListener(new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent evt) {
//            btnCreateActionPerformed(evt);
//        }
//    });
 connect(btnCreate, SIGNAL(clicked()), this, SLOT(btnCreateActionPerformed()));

 btnUseExisting->setText(tr("Add Existing...")); // NOI18N
//    btnUseExisting.addActionListener(new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent evt) {
//            btnUseExistingActionPerformed(evt);
//        }
//    });
 connect(btnUseExisting, SIGNAL(clicked()), this, SLOT(btnUseExistingActionPerformed()));

 countDownLbl->setText(tr("10")); // NOI18N
 countDownLbl->setToolTip(tr("The selected profile will be automatically used after 10 seconds.")); // NOI18N
#if 0
    GroupLayout layout = new GroupLayout(getContentPane());
    getContentPane().setLayout(layout);
    layout.setHorizontalGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
        .addGroup(layout.createSequentialGroup()
            .addContainerGap()
            .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                .addComponent(listLabel)
                .addComponent(countDownLbl))
            .addPreferredGap(LayoutStyle.ComponentPlacement.UNRELATED)
            .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                    .addGap(0, 0, Short.MAX_VALUE)
                    .addComponent(btnUseExisting)
                    .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                    .addComponent(btnCreate)
                    .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                    .addComponent(btnSelect))
                .addComponent(jScrollPane1))
            .addContainerGap())
    );
    layout.setVerticalGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
        .addGroup(layout.createSequentialGroup()
            .addContainerGap()
            .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                .addComponent(jScrollPane1, GroupLayout.DEFAULT_SIZE, 168, Short.MAX_VALUE)
                .addComponent(listLabel))
            .addPreferredGap(LayoutStyle.ComponentPlacement.UNRELATED)
            .addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                .addComponent(btnSelect)
                .addComponent(btnCreate)
                .addComponent(btnUseExisting)
                .addComponent(countDownLbl))
            .addContainerGap())
    );
#endif
 ((ProfileListModel*)profiles->model())->reset();
 log->debug(tr("list item count: %1").arg(((ProfileListModel*)profiles->model())->rowCount(QModelIndex())));

 QVBoxLayout* thisLayout = new QVBoxLayout;
 setLayout(thisLayout);
 QHBoxLayout* listLayout = new QHBoxLayout;
 listLayout->addWidget(listLabel);
 listLayout->addWidget(profiles);
 thisLayout->addLayout(listLayout);
 QHBoxLayout* buttonLayout = new QHBoxLayout;
 buttonLayout->addWidget(btnSelect);
 buttonLayout->addWidget(btnCreate);
 buttonLayout->addWidget(btnUseExisting);
 buttonLayout->addWidget(countDownLbl);
 thisLayout->addLayout(buttonLayout);
//    listLabel.getAccessibleContext().setAccessibleName(tr("ProfileManagerDialog->listLabel.text")); // NOI18N

 adjustSize();
}// </editor-fold>

void ProfileManagerDialog::keyPressEvent(QKeyEvent *evt)
{
 profilesKeyPressed(evt);
}

/*private*/ void ProfileManagerDialog::btnSelectActionPerformed(ActionEvent* /*evt*/)
{
 timer->stop();
 countDown = -1;
 countDownLbl->setVisible(false);
 //if (profiles->currentItem() != NULL)
 accept();
 if(profiles->currentIndex() != QModelIndex())
 {
  QModelIndex m = profiles->currentIndex();
  QString s = m.data().toString();
  Profile* p = ProfileManager::getDefault()->getProfiles().at(m.row());
  ProfileManager::defaultManager()->setActiveProfile(p);
  //dispose();
  close();
 }
}

/*private*/ void ProfileManagerDialog::btnCreateActionPerformed(ActionEvent* /*evt*/) {
 timer->stop();
 countDownLbl->setVisible(false);
 AddProfileDialog* apd = new AddProfileDialog(this, true, false);
 apd->setLocationRelativeTo(this);
 apd->setVisible(true);
}

/*private*/ void ProfileManagerDialog::btnUseExistingActionPerformed(ActionEvent* /*evt*/)
{
 timer->stop();
 countDownLbl->setVisible(false);
 JFileChooser* chooser = new JFileChooser(FileUtil::getPreferencesPath());
 chooser->setFileSelectionMode(JFileChooser::FILES_AND_DIRECTORIES);
 //chooser->setFileFilter(new ProfileFileFilter());
 chooser->setFileFilter("JMRI Configuration Profiles (*.*)");
 //chooser.setFileView(new ProfileFileView());
 // TODO: Use NetBeans OpenDialog if its availble
 if (chooser->showOpenDialog(this) == JFileChooser::APPROVE_OPTION)
 {
//        try {
 //QString selectedFile = QFileDialog::getOpenFileName(this,tr("Select profile"),FileUtil::getPreferencesPath());
 Profile* p = new Profile(chooser->getSelectedFile());
 ProfileManager::defaultManager()->addProfile(p);
// profiles->setSelectedValue(p, true);
//        } catch (IOException ex) {
//            log->warn("{} is not a profile directory", chooser.getSelectedFile());
//            // TODO: Display error dialog - selected file is not a profile directory
//        }
 }
}
void ProfileManagerDialog::showEvent(QShowEvent *e)
{
 formWindowOpened(e);
}

/*private*/ void ProfileManagerDialog::formWindowOpened(QShowEvent* /*evt*/)
{
 countDown = 10; // TODO: make this a profileManager property
//    timer = new Timer(1000, new ActionListener() {
//        @Override
//        /*public*/ void actionPerformed(ActionEvent e) {
//            if (countDown > 0) {
//                countDown--;
//                countDownLbl.setText(Integer.toString(countDown));
//            } else {
//                setVisible(false);
//                ProfileManager::defaultManager().setActiveProfile((Profile) profiles.getSelectedValue());
//                log->info("Automatically starting with profile " + ProfileManager::defaultManager().getActiveProfile().getId() + " after timeout.");
//                timer.stop();
//                countDown = -1;
//                dispose();
//            }
//        }
//    });
 timer = new QTimer();
 timer->setInterval(1000); // one sec
 timer->setSingleShot(false);
 connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

 //if (profiles.getModel().getSize() > 0 && NULL != ProfileManager::defaultManager()->getActiveProfile())
 if(profiles->model()->rowCount() > 0
         && (ProfileManager::defaultManager()->getActiveProfile() != NULL))
 {
  timer->start();
 }
 else
 {
  countDownLbl->setVisible(false);
  btnSelect->setEnabled(false);
 }
}
void ProfileManagerDialog::timeout()
{
 if (countDown > 0)
 {
  countDown--;
  countDownLbl->setText(QString::number(countDown));
 }
 else
 {
  setVisible(false);
  QModelIndex m = profiles->currentIndex();
  ProfileManager::defaultManager()->setActiveProfile(ProfileManager::defaultManager()->getProfiles().at(m.row()));
  if(ProfileManager::defaultManager()->getActiveProfile() != NULL)
   log->info("Automatically starting with profile " + ProfileManager::defaultManager()->getActiveProfile()->getId() + " after timeout.");
  timer->stop();
  countDown = -1;
  //dispose();
  close();
 }
}

/**
 * Get the active profile or display a dialog to prompt the user for it.
 *
 * @param f - The {@link java.awt.Frame} to display the dialog over.
 * @return the active or selected {@link Profile}
 * @throws IOException
 * @see ProfileManager#getStartingProfile()
 */
/*public*/ /*static*/ Profile* ProfileManagerDialog::getStartingProfile(JFrame* f) throw (IOException)
{
 if (ProfileManager::getStartingProfile() == NULL
  || (System::getProperty(ProfileManager::SYSTEM_PROPERTY) == NULL
            && !ProfileManager::defaultManager()->isAutoStartActiveProfile()))
 {
  ProfileManagerDialog* pmd = new ProfileManagerDialog(f, true);
  //pmd->setLocationRelativeTo(f);
  //pmd->setVisible(true);
  pmd->exec();
  ProfileManager::defaultManager()->saveActiveProfile();
 }
 return ProfileManager::defaultManager()->getActiveProfile();
}

/*private*/ void ProfileManagerDialog::profileNameChanged(Profile* p)
{
//    try {
 p->save();
 log->info("Saving profile "+ p->getId());
//    } catch (IOException ex) {
//        log->error("Unable to save renamed profile: {}", ex.getMessage());
//        JOptionPane.showMessageDialog(this,
//                Bundle.getMessage("ProfileManagerDialog->errorRenamingProfile"),
//                Bundle.getMessage("ProfileManagerDialog->errorRenamingProfileTitle"),
//                JOptionPane.ERROR_MESSAGE);
//    }
}

/*private*/ void ProfileManagerDialog::profilesValueChanged(QModelIndex)
{
 timer->stop();
 countDownLbl->setVisible(false);
 btnSelect->setEnabled(true);
}

/*private*/ void ProfileManagerDialog::mousePressEvent(QMouseEvent* /*evt*/) {
    this->profilesValueChanged(QModelIndex());
}

/*private*/ void ProfileManagerDialog::profilesKeyPressed(QKeyEvent* evt)
{
 if (evt->key() == Qt::Key_Enter) {
     this->btnSelect->animateClick();
 }
}

/*private*/ void ProfileManagerDialog::closeEvent(QCloseEvent* /*evt*/)
{
 if (countDown != -1)
 {
//        System.exit(255);
 }
}
/*public*/ void ProfileManagerDialog::propertyChange(IndexedPropertyChangeEvent* evt) {
 // TODO: profiles->setSelectedValue(ProfileManager::defaultManager()->getActiveProfile(), true);
 //profiles.repaint();
 if (QString(evt->getSource()->metaObject()->className()) ==("Profile") && evt->getPropertyName()==(Profile::NAME))
 {
  profileNameChanged(((Profile*) evt->getSource()));
 }
}

