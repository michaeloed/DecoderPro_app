#include "abstractmonframe.h"
#include "instancemanager.h"
#include "defaultusermessagepreferences.h"
#include "jtogglebutton.h"
#include "jtextarea.h"
#include "jtextfield.h"
#include <QCheckBox>
#include <QPushButton>
#include "defaultusermessagepreferences.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFileDialog>
#include "fileutil.h"

//AbstractMonFrame::AbstractMonFrame(QWidget *parent) :
//    JmriJFrame(parent)
//{
//}
/**
 * Abstract base class for Frames displaying communications monitor information
 * @author	Bob Jacobsen   Copyright (C) 2001, 2003
 * @version	$Revision: 22530 $
 */
// /*public*/ abstract class AbstractMonFrame extends JmriJFrame  {

// template functions to fill in

/**
 * Initialize the data source.
 * <P>
 * This is invoked at the end of the GUI initialization phase.
 * Subclass implementations should connect to their data source here.
 */

// the subclass also needs a dispose() method to close any specific communications; call super.dispose()
//@Override
/*public*/ void AbstractMonFrame::dispose() {
    p->setSimplePreferenceState(timeStampCheck, timeCheckBox->isChecked());
    p->setSimplePreferenceState(rawDataCheck, rawCheckBox->isChecked());
    p->setSimplePreferenceState(alwaysOnTopCheck, alwaysOnTopCheckBox.isChecked());
    p->setSimplePreferenceState(autoScrollCheck, !autoScrollCheckBox->isChecked());
    JmriJFrame::dispose();
}
// you'll also have to add the message(Foo) members to handle info to be logged.
// these should call nextLine(String line, String raw) with their updates
/*static*/ /*private*/ int AbstractMonFrame::MAX_LINES = 500 ;


/*public*/ AbstractMonFrame::AbstractMonFrame(QWidget *parent) : JmriJFrame(parent){
    //super();
    self = this;
    rawDataCheck = QString(this->metaObject()->className())+".RawData";
    timeStampCheck = QString(this->metaObject()->className())+".TimeStamp";
     alwaysOnTopCheck = QString(this->metaObject()->className())+".alwaysOnTop";
    autoScrollCheck = QString(this->metaObject()->className())+".AutoScroll";
    // member declarations
    clearButton = new QPushButton();
    freezeButton = new JToggleButton();
    jScrollPane1 = new QScrollArea();
    monTextPane = new JTextArea();
    startLogButton = new QPushButton ();
    stopLogButton = new QPushButton ();
    rawCheckBox = new QCheckBox ();
    timeCheckBox = new QCheckBox ();
    alwaysOnTopCheckBox = new QCheckBox ();
    autoScrollCheckBox = new QCheckBox ();
    openFileChooserButton = new QPushButton ();
    entryField = new JTextField();
    enterButton = new QPushButton ();
    newline = /*System.getProperty("line.separator");*/ "\n";
    log = new Logger("AbstractMonFrame");
    logStream = NULL;
    df = new SimpleDateFormat("HH:mm:ss.SSS");
    linesBuffer =  QString();
    logFileChooser = new QFileDialog(this, tr("Select Log File"), FileUtil::getUserFilesPath());
}

/*public*/ void AbstractMonFrame::initComponents() throw (Exception)
{

    p = (DefaultUserMessagePreferences*) InstanceManager::getDefault("UserPreferencesManager");
    // the following code sets the frame's initial state

    clearButton->setText("Clear screen");
    clearButton->setVisible(true);
    clearButton->setToolTip("Clear monitoring history");

    freezeButton->setText("Freeze screen");
    freezeButton->setVisible(true);
    freezeButton->setToolTip("Stop display scrolling");

    enterButton->setText("Add Message");
    enterButton->setVisible(true);
    enterButton->setToolTip("Add a text message to the log");

    monTextPane->setVisible(true);
    monTextPane->setToolTip("Command and reply monitoring information appears here");
    monTextPane->setEditable(false);

    // Add document listener to scroll to end when modified if required
    monTextPane->getDocument().addDocumentListener(new DocumentListener();
//    {

//        // References to the JTextArea and QCheckBox
//        // of this instantiation
//        JTextArea* ta = monTextPane;
//        QCheckBox*  chk = autoScrollCheckBox;

//       // @Override
//        /*public*/ void insertUpdate(DocumentEvent* e) {
//            doAutoScroll(ta, chk->isChecked());
//        }

//       //@Override
//        /*public*/ void removeUpdate(DocumentEvent* e) {
//            doAutoScroll(ta, chk->isChecked());
//        }

//        //@Override
//        /*public*/ void changedUpdate(DocumentEvent e) {
//            doAutoScroll(ta, chk->isChecked());
//        }
//    });

    entryField->setToolTip("Enter text here, then click button to include it in log");

    // fix a width for current character set
    JTextField* t = new JTextField(80);
    int x = jScrollPane1.getPreferredSize().width+t.getPreferredSize().width;
    int y = jScrollPane1.getPreferredSize().height+10*t.getPreferredSize().height;

    jScrollPane1->getViewport().add(monTextPane);
    jScrollPane1->setPreferredSize(new Dimension(x, y));
    jScrollPane1->setVisible(true);

    startLogButton->setText("Start logging");
    startLogButton->setVisible(true);
    startLogButton->setToolTip("start logging to file");

    stopLogButton->setText("Stop logging");
    stopLogButton->setVisible(true);
    stopLogButton->setToolTip("Stop logging to file");

    rawCheckBox->setText("Show raw data");
    rawCheckBox->setVisible(true);
    rawCheckBox->setToolTip("If checked, show the raw traffic in hex");
    rawCheckBox->setChecked(p->getSimplePreferenceState(rawDataCheck));

    timeCheckBox->setText("Show timestamps");
    timeCheckBox->setVisible(true);
    timeCheckBox->setToolTip("If checked, show timestamps before each message");
    timeCheckBox->setChecked(p->getSimplePreferenceState(timeStampCheck));

    alwaysOnTopCheckBox->setText("Window always on Top");
    alwaysOnTopCheckBox->setVisible(true);
    alwaysOnTopCheckBox->setToolTip("If checked, this window be always be displayed in front of any other window");
    alwaysOnTopCheckBox->setChecked(p->getSimplePreferenceState(alwaysOnTopCheck));
    setAlwaysOnTop(alwaysOnTopCheckBox->isChecked());

    autoScrollCheckBox->setText("Auto scroll");
    autoScrollCheckBox->setVisible(true);
    autoScrollCheckBox->setToolTip("If checked, always scroll to the latest log entry");
    autoScrollCheckBox->setChecked(!p->getSimplePreferenceState(autoScrollCheck));

    openFileChooserButton->setText("Choose log file");
    openFileChooserButton->setVisible(true);
    openFileChooserButton->setToolTip("Click here to select a new output log file");

    setTitle(title());
//    getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));
 QWidget* centralWidget = new QWidget;
 QVBoxLayout* contentPaneLayout = new QVBoxLayout;
 centralWidget->setLayout(contentPaneLayout);
    // add items to GUI
    contentPaneLayout->addWidget(jScrollPane1);

//    QWidget* paneA = new QWidget();
//    paneA.setLayout(new BoxLayout(paneA, BoxLayout.Y_AXIS));
    QVBoxLayout* paneALayout = new QVBoxLayout;

//    QWidget* pane1 = new QWidget();
//    pane1.setLayout(new BoxLayout(pane1, BoxLayout.X_AXIS));
    QHBoxLayout* pane1Layout = new QHBoxLayout;
    pane1Layout->addWidget(clearButton);
    pane1Layout->addWidget(freezeButton);
    pane1Layout->addWidget(rawCheckBox);
    pane1Layout->addWidget(timeCheckBox);
    pane1Layout->addWidget(alwaysOnTopCheckBox);
    paneALayout->addLayout(pane1Layout);

//    QWidget* pane2 = new QWidget();
//    pane2.setLayout(new BoxLayout(pane2, BoxLayout.X_AXIS));
    QHBoxLayout* pane2Layout = new QHBoxLayout;
    pane2Layout->addWidget(openFileChooserButton);
    pane2Layout->addWidget(startLogButton);
    pane2Layout->addWidget(stopLogButton);
    paneALayout->addLayout(pane2Layout);

//    QWidget* pane3 = new QWidget();
//    pane3.setLayout(new BoxLayout(pane3, BoxLayout.X_AXIS));
    QHBoxLayout* pane3Layout = new QHBoxLayout;
    pane3Layout->addWidget(enterButton);
    pane3Layout->addWidget(entryField);
    paneALayout->addLayout(pane3Layout);

    contentPaneLayout->addLayout(paneALayout);

    // connect actions to buttons
//    clearButton.addActionListener(new ActionListener() {
//        @Override
//        /*public*/ void actionPerformed(ActionEvent e) {
//            clearButtonActionPerformed(e);
//        }
//    });
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearButtonActionPerformed()));
//    startLogButton.addActionListener(new ActionListener() {
//        @Override
//        /*public*/ void actionPerformed(ActionEvent e) {
//            startLogButtonActionPerformed(e);
//        }
//    });
    connect(startLogButton, SIGNAL(clicked()),this, SLOT(startLogButtonActionPerformed()));
//    stopLogButton.addActionListener(new ActionListener() {
//        @Override
//        /*public*/ void actionPerformed(ActionEvent e) {
//            stopLogButtonActionPerformed(e);
//        }
//    });
    connect(stopLogButton, SIGNAL(clicked()), this, SLOT(stopLogButtonActionPerformed()));
//    openFileChooserButton.addActionListener(new ActionListener() {
//        @Override
//        /*public*/ void actionPerformed(ActionEvent e) {
//            openFileChooserButtonActionPerformed(e);
//        }
//    });
    connect(openFileChooserButton, SIGNAL(clicked()), this, SLOT(openFileChooserButtonActionPerformed()));
//    enterButton.addActionListener(new ActionListener() {
//        @Override
//        /*public*/ void actionPerformed(ActionEvent e) {
//            enterButtonActionPerformed(e);
//        }
//    });
    connect(enterButton, SIGNAL(clicked()), this, SLOT(enterButtonActionPerformed()));

//    alwaysOnTopCheckBox.addActionListener(new ActionListener() {
//        @Override
//        /*public*/ void actionPerformed(ActionEvent e) {
//            setAlwaysOnTop(alwaysOnTopCheckBox.isChecked());
//        }
//    });
    connect(alwaysOnTopCheckBox, SIGNAL(clicked()), this, SLOT(ONalwaysOnTopCheckBox()));

//    autoScrollCheckBox.addActionListener(new ActionListener() {
//        @Override
//        /*public*/ void actionPerformed(ActionEvent e) {
//            doAutoScroll(monTextPane, autoScrollCheckBox.isChecked());
//        }
//    });
    connect(autoScrollCheckBox, SIGNAL(clicked()), this, SLOT(OnAutoScrollCheckBox()));

 // set file chooser to a default
 logFileChooser.setSelectedFile(new File("monitorLog.txt"));

 // connect to data source
 init();

 // add help menu to window
 addHelpMenu();

 // prevent button areas from expanding
 pack();
 paneALayout->setMaximumSize(paneALayout->sizeHint());
 pack();
}

/**
 * Define help menu for this window.
 * <p>
 * By default, provides a generic help page
 * that covers general features.  Specific
 * implementations can override this to
 * show their own help page if desired.
 */
/*protected*/ void AbstractMonFrame::addHelpMenu() {
    addHelpMenu("package.jmri.jmrix.AbstractMonFrame", true);
}

/*public*/ void AbstractMonFrame::nextLine(QString line, QString raw) {
    // handle display of traffic
    // line is the traffic in 'normal form', raw is the "raw form"
    // Both should be one or more well-formed lines, e.g. end with \n
    //StringBuffer sb = new StringBuffer(120);
    QString sb;

    // display the timestamp if requested
    if ( timeCheckBox->isChecked() ) {
        sb.append(df.format(new Date())).append( ": " ) ;
    }

    // display the raw data if requested
    if ( rawCheckBox.isChecked() ) {
        sb.append( '[' ).append(raw).append( "]  " );
    }

    // display decoded data
    sb.append(line);
    synchronized( self )
    {
        linesBuffer.append( sb.toString() );
    }

    // if not frozen, display it in the Swing thread
    if (!freezeButton.isChecked()) {
        Runnable r = new Runnable() {
            /*public*/ void run() {
                synchronized( self )
                {
                    monTextPane.append( linesBuffer.toString() );
                    int LineCount = monTextPane.getLineCount() ;
                    if( LineCount > MAX_LINES )
                    {
                        LineCount -= MAX_LINES ;
                        try {
                            int offset = monTextPane.getLineStartOffset(LineCount);
                            monTextPane.getDocument().remove(0, offset ) ;
                        }
                        catch (BadLocationException ex) {
                        }
                    }
                    linesBuffer.setLength(0) ;
                }
            }
        };
        javax.swing.SwingUtilities.invokeLater(r);
    }

    // if requested, log to a file.

    if (logStream != NULL) {
        synchronized (logStream) {
            String logLine = sb.toString();
            if (!newline.equals("\n")) {
                // have to massage the line-ends
                int i = 0;
                int lim = sb.length();
                StringBuffer out = new StringBuffer(sb.length()+10);  // arbitrary guess at space
                for ( i = 0; i<lim; i++) {
                    if (sb.charAt(i) == '\n')
                        out.append(newline);
                    else
                        out.append(sb.charAt(i));
                }
                logLine = out.toString();
            }
            logStream.print(logLine);
        }
    }
}


/*public*/ /*synchronized*/ void AbstractMonFrame::clearButtonActionPerformed(ActionEvent* e) {
    // clear the monitoring history
    synchronized( linesBuffer )
    {
        linesBuffer.setLength(0);
        monTextPane.setText("");
    }
}

/*public*/ /*synchronized*/ void AbstractMonFrame::startLogButtonActionPerformed(ActionEvent* e) {
    // start logging by creating the stream
    if ( logStream==NULL) {  // successive clicks don't restart the file
        // start logging
        try {
            logStream = new PrintStream (new FileOutputStream(logFileChooser.getSelectedFile()));
        } catch (Exception ex) {
            log.error("exception "+ex);
        }
    }
}

/*public*/ /*synchronized*/ void AbstractMonFrame::stopLogButtonActionPerformed(ActionEvent* e) {
    // stop logging by removing the stream
    if (logStream!=NULL) {
        synchronized (logStream) {
            logStream.flush();
            logStream.close();
        }
        logStream = NULL;
    }
}

/*public*/ void AbstractMonFrame::openFileChooserButtonActionPerformed(ActionEvent*e) {
    // start at current file, show dialog
    int retVal = logFileChooser.showSaveDialog(this);

    // handle selection or cancel
    if (retVal == JFileChooser.APPROVE_OPTION) {
        boolean loggingNow = (logStream != NULL);
        stopLogButtonActionPerformed(e);  // stop before changing file
        //File file = logFileChooser.getSelectedFile();
        // if we were currently logging, start the new file
        if (loggingNow) startLogButtonActionPerformed(e);
    }
}

/*public*/ void AbstractMonFrame::enterButtonActionPerformed(ActionEvent* e) {
    nextLine(entryField->text()+"\n", "");
}

/*public*/ /*synchronized*/ QString getFrameText()
{
    return linesBuffer.toString();
}

/**
 * Method to position caret at end of JTextArea ta when
 * scroll true.
 * @param ta Reference to JTextArea
 * @param scroll True to move to end
 */
/*private*/ void AbstractMonFrame::doAutoScroll(/*final*/ JTextArea* ta, /*final*/ bool scroll) {
    SwingUtilities.invokeLater(new Runnable() {
        //@Override
        /*public*/ void run() {
            int len = ta.getText().length();
            if (scroll) {
                ta.setCaretPosition(len);
            } else if (ta.getCaretPosition()==len && len>0) {
                ta.setCaretPosition(len-1);
            }
        }
    });
}

//private static final org.apache.log4j.Logger log = org.apache.log4j.Logger.getLogger(AbstractMonFrame.class.getName());
//}
