#include "editor.h"
#include <QMessageBox>
#include "positionablelabel.h"
#include "memoryicon.h"
#include "memoryiconcoordinateedit.h"
#include "rosterentry.h"
#include "locoicon.h"
#include "sensoricon.h"
#include "positionablepopuputil.h"
#include "rosterentryselectorpanel.h"
#include "analogclock2display.h"
#include "lighticon.h"
#include "abstractsensor.h"
#include "light.h"
#include "inputdialog.h"
#include "panelmenu.h"
#include "configxmlmanager.h"
#include "lcdclockframe.h"
#include "nixieclockframe.h"
#include "signalheadicon.h"
#include "signalmasticon.h"
#include "signalmast.h"
#include "abstractsignalhead.h"
#include "abstractlight.h"
#include "abstractsignalmast.h"
#include "layouteditor.h"
#include "multisensoricon.h"
#include "locoicon.h"
#include "positionablepopuputil.h"
#include "positionableicon.h"
#include "decoratorpanel.h"
#include "memoryinputicon.h"
#include <QGraphicsProxyWidget>
#include "memoryspinnericon.h"
#include "memorycomboicon.h"
#include "jtextfield.h"
#include "borderlayout.h"
#include <QMenuBar>
#include "directorysearcher.h"
#include "imageindexeditor.h"
#include "positionablejcomponent.h"
#include "picklistmodel.h"
#include "turnouticon.h"
#include "slipiconadder.h"
#include "slipturnouticon.h"
#include "iconadder.h"
//#include "rpspositionicon.h"
#include "indicatortrackicon.h"
#include "indicatorturnouticon.h"
#include "positionablerectangle.h"
#include "positionableroundrect.h"
#include "positionablecircle.h"
#include "positionableellipse.h"
#include "trainicon.h"
#include "metatypes.h"


//Editor::Editor(QWidget *parent) :
//    JmriJFrame(parent)
//{
//}
/**
 * This is the Model and a Controller for panel editor Views.
 * (Panel Editor, Layout Editor or any subsequent editors)
 * The Model is simply a list of Positionable objects added to a
 * "target panel". Control of the display attributes of the
 * Positionable objects is done here.  However, control of mouse
 * events is passed to the editor views, so control is also
 * done by the editor views.
 * <P>
 * The "contents" List keeps track of all the objects added to the target
 * frame for later manipulation.  This class only locates and moves
 * "target panel" items, and does not control their appearance - that
 * is left for the editor views.
 * <P>
 * The Editor has tri-state "flags" to control the display of
 * Positionable object attributes globally - i.e. "on" or "off" for
 * all - or as a third state, permits the display control "locally"
 * by corresponding flags in each Positionable object
 * <P>
 * The title of the target and the editor panel are kept
 * consistent via the {#setTitle} method.
 *
 * <p>
 * Mouse events are initially handled here, rather than in the
 * individual displayed objects, so that selection boxes for
 * moving multiple objects can be provided.
 *
 * <p>
 * This class also implements an effective ToolTipManager replacement,
 * because the standard Swing one can't deal with the coordinate
 * changes used to zoom a panel.  It works by controlling the contents
 * of the _tooltip instance variable, and triggering repaint of the
 * target window when the tooltip changes.  The window painting then
 * explicitly draws the tooltip for the underlying object.
 *
 * @author  Bob Jacobsen  Copyright: Copyright (c) 2002, 2003, 2007
 * @author  Dennis Miller 2004
 * @author  Howard G. Penny Copyright: Copyright (c) 2005
 * @author  Matthew Harris Copyright: Copyright (c) 2009
 * @author  Pete Cressman Copyright: Copyright (c) 2009, 2010, 2011
 * @version			$Revision 1.0 $
 *
 */

//abstract /*public*/ class Editor extends JmriJFrame implements MouseListener, MouseMotionListener,
//                                ActionListener, KeyListener {


//    /*public*/ static const ResourceBundle rbean = ResourceBundle.getBundle("jmri.NamedBeanBundle");

//    /*public*/ static const QString POSITIONABLE_FLAVOR = java.awt.datatransfer.DataFlavor.javaJVMLocalObjectMimeType +
//               ";class=jmri.jmrit.display.Positionable";


//    /*private*/ bool _showCoordinates = true;


/*static*/ const int Editor::DELTA = 20;
/*public*/ /*static*/ /*final*/ QString Editor::POSITIONABLE_FLAVOR = /*java.awt.datatransfer.DataFlavor.javaJVMLocalObjectMimeType */ QString("text/plain")+
               ";class=jmri.jmrit.display.Positionable";

/*public*/ Editor::Editor(QWidget* parent) : JmriJFrame( parent)
{
 commonInit();
}
void Editor::commonInit()
{
 //commonInit();
 log = new Logger("Editor");
 _urlMap = new QMap<QString, QString>();
 _debug = false;
 _loadFailed = false;
 _positionable = true;
 _controlLayout = true;
 _showHidden = true;
 _showTooltip = true;
 _globalSetsLocal = true;    // pre 2.9.6 behavior
 _useGlobalFlag = false;     // pre 2.9.6 behavior
 _ignore = false;
 _contents = new QVector<Positionable*>();
 _scrollState = SCROLL_NONE;
 _editable = true;
 _selectRect = QRect();
 _selectRectItemGroup = NULL;
 _highlightcomponent = QRect();
 _dragging = false;
 _selectionGroup = QList<Positionable*>();  // items gathered inside fence

 xLoc = 0;     // x coord of selected Positionable
 yLoc = 0;     // y coord of selected Positionable
 defaultBackgroundColor = QColor(Qt::lightGray);
 _pastePending = false;
 showCloseInfoMessage = true;	//display info message when closing panel
 _paintScale = 1.0;   // scale for _targetPanel drawing
 _targetFrame = (JFrame*)this;
 _highlightColor =  QColor(204, 207, 88);
 _selectGroupColor =  QColor(204, 207, 88);
 _selectRectColor = QColor(Qt::red);
 _selectRectStroke = Qt::DashLine;
 _selectRectItemGroup = new QGraphicsItemGroup();
 editPanel = NULL;
 frameLocationX = 0;
 frameLocationY = 0;
 _iconEditorFrame = new QHash <QString, JFrameItem*>();
 _spinCols = new SpinnerNumberModel(3,1,100,1);
  panelMenuIsVisible = true;
}

/*public*/ Editor::Editor(QString name, bool saveSize, bool savePosition, QWidget* parent) : JmriJFrame(parent)
{
 //super(name, saveSize, savePosition);
 //setName(name);
 this->name = name;
 this->saveSize = saveSize;
 this->savePosition = savePosition;
 commonInit();

 _debug = log->isDebugEnabled();
 _defaultToolTip = /*new ToolTip(NULL, 0, 0)*/ "";
 setVisible(false);
}

/*public*/ Editor::Editor(QString name, QWidget* parent) : JmriJFrame( parent)
{
 commonInit();
 _debug = false;
 _loadFailed = false;

 showCloseInfoMessage = true;	//display info message when closing panel

 //this(name, true, true);
 this->name = name;
 this->saveSize = true;
 this->savePosition = true;
}

/*public*/ void Editor::loadFailed()
{
 _loadFailed = true;
}
/**
*
*/
/*public*/ NamedIcon* Editor::loadFailed(QString /*msg*/, QString url)
{
 if (_debug) log->debug("loadFailed _ignore= "+_ignore);
 QString goodUrl = _urlMap->value(url);
 if (goodUrl!=NULL)
 {
  return NamedIcon::getIconByName(goodUrl);
 }
 if (_ignore)
 {
  _loadFailed = true;
  return new NamedIcon(url, url);
 }
 _newIcon = NULL;
 _delete = false;
// TODO:
// new UrlErrorDialog(msg, url);

if (_delete)
{
 if (_debug) log->debug("loadFailed _delete= "+_delete);
  return NULL;
}
if (_newIcon==NULL)
{
 _loadFailed = true;
 _newIcon =new NamedIcon(url, url);
}
if (_debug) log->debug("loadFailed icon NULL= "+(_newIcon==NULL));
 return _newIcon;
}


/*public*/ void Editor::disposeLoadData()
{
 _urlMap = NULL;
}

/*public*/ bool Editor::loadOK()
{
 return !_loadFailed;
}

/*public*/ QList <Positionable*> Editor::getContents()
{
  return _contents->toList();
}

/*public*/ void Editor::setDefaultToolTip(QString dtt)
{
 _defaultToolTip = dtt;
}

/***************** setting the main panel and frame ****************/
#if 1
/**
* An Editor may or may not choose to use 'this' as its frame or
* the interior class 'TargetPane' for its targetPanel
*/
/*protected*/ void Editor::setTargetPanel(QGraphicsScene* targetPanel, JmriJFrame* frame) {
    if (targetPanel == NULL){
       // _targetPanel = new TargetPane();
        _targetPanel = editScene = new EditScene();
    } else {
        _targetPanel = targetPanel;
    }
    if (frame == NULL) {
        _targetFrame = this;
    } else {
        _targetFrame = frame;
    }
//    _targetFrame->setDefaultCloseOperation(javax.swing.WindowConstants.HIDE_ON_CLOSE);
//    _panelScrollPane = new JScrollPane(_targetPanel);
//    Container contentPane = _targetFrame.getContentPane();
//    contentPane->layout()->addWidget(_panelScrollPane);

    //_targetFrame->centralWidget()->layout()->addWidget(_targetPanel);
    if(_targetFrame->centralWidget() == NULL)
    {
        QWidget* centralWidget = new QWidget();
        centralWidget->setLayout(new QVBoxLayout());
    }
    editPanel = new QGraphicsView(_targetFrame->centralWidget());
    _targetFrame->centralWidget()->layout()->addWidget(editPanel);
    editPanel->setMouseTracking(true);
    editPanel->setRenderHint(QPainter::Antialiasing);
    editPanel->setScene(_targetPanel);
    setBackgroundColor(defaultBackgroundColor);

//    _targetFrame.addWindowListener(new java.awt.event.WindowAdapter() {
//        /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//            targetWindowClosingEvent(e);
//        }
//    });
//    _targetPanel.addMouseListener(this);
//    _targetPanel.addMouseMotionListener(this);
//    _targetPanel.setFocusable(true);
//    _targetPanel.addKeyListener(this);
    _targetFrame->pack();
}
#endif
/*protected*/ void Editor::setTargetPanelSize(int w, int h) {
        if (_debug) log->debug("setTargetPanelSize now w="+QString::number(w)+", h="+QString::number(h));
    //_targetPanel->setSize(w, h);
    _targetPanel->setSceneRect(QRectF(0,0,w,h));
    //_targetPanel.invalidate();
    editPanel->resize(w,h);
}

/*protected*/ QSizeF Editor::getTargetPanelSize() {
    return _targetPanel->sceneRect().size();
}

/*public*/  QGraphicsScene* Editor::getTargetPanel() {
    return _targetPanel;
}
///*protected*/ const JScrollPane* Editor::getPanelScrollPane() {
//    return _panelScrollPane;
//}

/*public*/  JFrame* Editor::getTargetFrame() {
    return _targetFrame;
}

/*public*/ QColor Editor::getBackgroundColor()
{
 QBrush b = editPanel->backgroundBrush();
 if(b == Qt::NoBrush)
  return QColor(Qt::white);
 QColor c = b.color();
 return c;
}

/*public*/ void Editor::setBackgroundColor(QColor color)
{
 editPanel->setBackgroundBrush(QBrush(color, Qt::SolidPattern));
}

/*public*/ void Editor::clearBackgroundColor(){
//    TargetPane tmp = (TargetPane) _targetPanel;
//    tmp.clearBackgroundColor();
}

/**
 * Get/Set scale for TargetPane drawing
 */
/*public*/ /*const*/ double Editor::getPaintScale() {
    return _paintScale;
}
/*protected*/ /*const*/ void Editor::setPaintScale(double newScale) {
    double ratio = newScale/_paintScale;
    _paintScale = newScale;
    //setScrollbarScale(ratio);
}

/*protected*/ void Editor::setToolTip(QString tt) {
//    if (tt==NULL) {
//        _tooltip = NULL;
//        if (_tooltipTimer != NULL) {
//            _tooltipTimer.stop();
//            _tooltipTimer = NULL;
//        }

//    } else if (_tooltip==NULL && _tooltipTimer==NULL) {
//        _tooltipTimer = new ToolTipTimer(TOOLTIPSHOWDELAY, this, tt);
//        _tooltipTimer.setRepeats(false);
//        _tooltipTimer.start();
//    }
 _toolTip = tt;
}

#if 0
/**
* Wait TOOLTIPSHOWDELAY then show tooltip.  Wait TOOLTIPDISMISSDELAY and disaappear
*/
/*public*/ void Editor::actionPerformed(ActionEvent event) {
    //if (_debug) log->debug("_tooltipTimer actionPerformed: Timer on= "+(_tooltipTimer!=NULL));
    if (_tooltipTimer!=NULL) {
        _tooltip = _tooltipTimer.getTooltip();
        _tooltipTimer.stop();
    }
    if (_tooltip!=NULL) {
        _tooltipTimer = new ToolTipTimer(TOOLTIPDISMISSDELAY, this, NULL);
        _tooltipTimer.setRepeats(false);
        _tooltipTimer.start();
    } else {
        _tooltipTimer = NULL;
    }
    _targetPanel.repaint();
}




/*private*/ void setScrollbarScale(double ratio) {
    Dimension dim = _targetPanel.getSize();
    //Dimension dim = _targetPanel.getPreferredSize();
    //Dimension dim = _targetPanel.getMaximumSize();
    int tpWidth = (int)((dim.width)*ratio);
    int tpHeight = (int)((dim.height)*ratio);
    _targetPanel.setSize(tpWidth,tpHeight);
    if (_debug) log->debug("setScrollbarScale: ratio= "+ratio+", tpWidth= "+tpWidth+", tpHeight= "+tpHeight);
    // compute new scroll bar positions in order to keep image centered
    JScrollBar horScroll = _panelScrollPane.getHorizontalScrollBar();
    JScrollBar vertScroll = _panelScrollPane.getVerticalScrollBar();
    int hScroll = horScroll.getVisibleAmount()/2;
    hScroll = (int)((horScroll.getValue() + hScroll) * ratio) - hScroll;
    int vScroll = vertScroll.getVisibleAmount()/2;
    vScroll = (int)((vertScroll.getValue() + vScroll) * ratio) - vScroll;
    // set scrollbars maximum range (otherwise setValue may fail);
    horScroll.setMaximum((int)((horScroll.getMaximum())*ratio));
    vertScroll.setMaximum((int)((vertScroll.getMaximum())*ratio));
    // set scroll bar positions
    horScroll.setValue(hScroll);
    vertScroll.setValue(vScroll);
    repaint();
}
#endif
/************************ Options setup **********************/
/**
 *  Control whether target panel items are editable.
 *  Does this by invoke the {@link Positionable#setEditable(bool)} function of
 *  each item on the target panel. This also controls the relevant pop-up menu items
 *  (which are the primary way that items are edited).
 * @param state true for editable.
 */
/*public*/ void Editor::setAllEditable(bool state)
{
 _editable = state;
 for (int i = 0; i<_contents->size(); i++)
 {
  ((PositionableLabel*)_contents->at(i))->setEditable(state);
 }
 if (!_editable)
 {
  _highlightcomponent = QRect();
  _selectionGroup = QList<Positionable*>();
 }
}

// accessor routines for persistent information
/*public*/ bool Editor::isEditable() {
    return _editable;
}

/**
* Set which flag should be used, global or local for Positioning
* and Control of individual items.  Items call getFlag() to return
* the appropriate flag it should use.
*/
/*public*/ void Editor::setUseGlobalFlag(bool set) {
    _useGlobalFlag = set;
}
/*public*/ bool Editor::useGlobalFlag() {
    return _useGlobalFlag;
}

/**
* @param localFlag is the current setting of the item
*/
/*public*/ bool Editor::getFlag(int whichOption, bool localFlag)
{
 //if (_debug)  log->debug("getFlag Option= "+whichOption+", _useGlobalFlag="+_useGlobalFlag+" localFlag="+localFlag);
 if (_useGlobalFlag)
 {
  switch (whichOption)
  {
  case OPTION_POSITION:
   return _positionable;
  case OPTION_CONTROLS:
   return _controlLayout;
  case OPTION_HIDDEN:
   return _showHidden;
  case OPTION_TOOLTIP:
        return _showTooltip;
//                case OPTION_COORDS:
//                    return _showCoordinates;
  }
 }
 return localFlag;
}
/**
* Does global flag sets Positionable and Control for individual items
*/
/*public*/ void Editor::setGlobalSetsLocalFlag(bool set)
{
 _globalSetsLocal = set;
}

/**
 *  Control whether panel items are positionable.
 *  Markers are always positionable.
 * @param state true for all items positionable.
 */
/*public*/ void Editor::setAllPositionable(bool state)
{
 _positionable = state;
 if (_globalSetsLocal)
 {
  for (int i = 0; i<_contents->size(); i++)
  {
   Positionable* p = _contents->at(i);
   // don't allow backgrounds to be set positionable by global flag
   if (!state || ((PositionableLabel*)p)->getDisplayLevel()!=BKG)
   {
    ((PositionableLabel*)p)->setPositionable(state);
   }
  }
 }
}
/*public*/ bool Editor::allPositionable()
{
    return _positionable;
}

/**
 *  Control whether target panel items are controlling layout items.
 *  Does this by invoke the {@link Positionable#setControlling} function of
 *  each item on the target panel. This also controls the relevant pop-up menu items.
 * @param state true for controlling.
 */
/*public*/ void Editor::setAllControlling(bool state)
{
 _controlLayout = state;
 if (_globalSetsLocal)
 {
  for (int i = 0; i<_contents->size(); i++)
  {
   _contents->at(i)->setControlling(state);
  }
 }
}
/*public*/ bool Editor::allControlling() {
    return _controlLayout;
}

/**
 *  Control whether target panel hidden items are visible or not.
 *  Does this by invoke the {@link Positionable#setHidden} function of
 *  each item on the target panel.
 * @param state true for Visible.
 */
/*public*/ void Editor::setShowHidden(bool state)
{
    _showHidden = state;
    if (_showHidden) {
        for (int i = 0; i<_contents->size(); i++) {
            _contents->at(i)->setVisible(true);
        }
    } else {
        for (int i = 0; i<_contents->size(); i++) {
            _contents->at(i)->showHidden();
        }
    }
}

/*public*/ bool Editor::showHidden() {
    return _showHidden;
}

/*public*/ void Editor::setAllShowTooltip(bool state) {
    _showTooltip = state;
    for (int i = 0; i<_contents->size(); i++) {
        _contents->at(i)->setShowTooltip(state);
    }
}
/*public*/ bool Editor::showTooltip() {
    return _showTooltip;
}


/**
 *  Control whether target panel items will show their
 *  coordinates in their popup memu.
 * @param state true for show coodinates.
 */
/*public*/ void Editor::setShowCoordinates(bool state)
{
 _showCoordinates = state;
 for (int i = 0; i<_contents->size(); i++)
 {
  _contents->at(i)->setViewCoordinates(state);
 }
}
/*public*/ bool Editor::showCoordinates() {
    return _showCoordinates;
}


/**
 *  Control whether target panel shows a menu
 * @param state true for controlling.
 */
/*public*/ void Editor::setPanelMenu(bool state)
{
 _targetFrame->menuBar()->setVisible(state);
//    validate();
}
/**
 * Hide or show menus on the target panel.
 *
 * @param state
 * @since 3.9.5
 */
/*public*/ void Editor::setPanelMenuVisible(bool state)
{
    this->panelMenuIsVisible = state;
    //if (!GraphicsEnvironment.isHeadless() && this._targetFrame != null)
    {
     _targetFrame->menuBar()->setVisible(state);
     //this.revalidate();
     this->repaint();
    }
}

/**
 * Is the menu on the target panel shown?
 *
 * @return true if menu is visible
 * @since 3.9.5
 */
/*public*/ bool Editor::isPanelMenuVisible() {
    //if (!GraphicsEnvironment.isHeadless() && this._targetFrame != null)
    {
        this->panelMenuIsVisible = _targetFrame->menuBar()->isVisible();
    }
    return this->panelMenuIsVisible;
}
#if 0
/*protected*/ void setScroll(int state) {
    if (_debug) log->debug("setScroll "+state);
    switch (state) {
        case SCROLL_NONE:
            _panelScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
            _panelScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
            break;
        case SCROLL_BOTH:
            _panelScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
            _panelScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
            break;
        case SCROLL_HORIZONTAL:
            _panelScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
            _panelScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
            break;
        case SCROLL_VERTICAL:
            _panelScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
            _panelScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
            break;
    }
    _scrollState = state;
}

/*public*/ void setScroll(QString strState) {
    int state = SCROLL_BOTH;
    if (strState==IgnoreCase("none") || strState==IgnoreCase("no")) state = SCROLL_NONE;
    else if (strState==("horizontal")) state = SCROLL_HORIZONTAL;
    else if (strState==("vertical")) state = SCROLL_VERTICAL;
    if (_debug) log->debug("setScroll: strState= "+strState+", state= "+state);
    setScroll(state);
}


/*public*/ QString getScrollable() {
    QString value = "";
    switch (_scrollState) {
        case SCROLL_NONE:
            value = "none";
            break;
        case SCROLL_BOTH:
            value = "both";
            break;
        case SCROLL_HORIZONTAL:
            value = "horizontal";
            break;
        case SCROLL_VERTICAL:
            value = "vertical";
            break;
    }
    return value;
}
/************************* end Options setup ***********************/
#endif
void Editor::closeEvent(QCloseEvent */*e*/)
{
 targetWindowClosing(true);
}

/**
 * Handle closing the target window.
 *
 * The target window has been requested to close, don't delete it at this
 * time. Deletion must be accomplished via the Delete this panel menu item.
 *
 * @param save True if user should be reminded to save the panel
 */
/*protected*/ void Editor::targetWindowClosing(bool save)
{
 //this.setVisible(false);   // doesn't remove the editor!
 // display info message on panel close
 if (showCloseInfoMessage)
 {
  QString name = "Panel";
  QString message = NULL;
  if(save)
  {
//   message = Bundle.getMessage("Reminder1")+" "+Bundle.getMessage("Reminder2")+
//                            "\n"+Bundle.getMessage("Reminder3");
   message = tr("This panel has been changed.  ") + tr("Please remember to save it to disk.") + tr("( Select 'Save Panels...' in the Panels menu or the File menu of any panel.)");
   QMessageBox::question(this, tr("Reminder"), message);
  }
  else
  {
   //message = Bundle.getMessage("PanelCloseQuestion") +"\n" + Bundle.getMessage("PanelCloseHelp");
   if(this->window() != NULL)
    name = this->window()->windowTitle();
   message = tr("Do you want to hide or delete \"%1\"?\nUse Panels->Show Panel to display hidden panels.").arg(name);
  }
   QMessageBox* msg = new QMessageBox(QMessageBox::Question,tr("Reminder"), message,QMessageBox::NoButton, _targetFrame);
  QPushButton* hideButton = new QPushButton(tr("HidePanel"));
  msg->addButton(hideButton,QMessageBox::YesRole);
  QPushButton* deletePanelButton = new QPushButton(tr("Delete Panel"));
  msg->addButton(deletePanelButton, QMessageBox::NoRole);
  QPushButton* dontShowButton = new QPushButton(tr("Don't show this again"));
  msg->addButton(dontShowButton, QMessageBox::ActionRole);
  msg->exec();
  if(msg->clickedButton() == hideButton)
  {
   _targetFrame->setVisible(false);   // doesn't remove the editor!
   PanelMenu::instance()->updateEditorPanel(this);
  }
  else if(msg->clickedButton() == deletePanelButton)
  {
   if (deletePanel() )
   { // disposes everything
    dispose(true);
   }
  }
  else if(msg->clickedButton() == dontShowButton)
  {
   showCloseInfoMessage = false;
   _targetFrame->setVisible(false);   // doesn't remove the editor!
   PanelMenu::instance()->updateEditorPanel(this);
  }
  else
  {
   _targetFrame->setVisible(false);   // doesn't remove the editor!
   PanelMenu::instance()->updateEditorPanel(this);
  }
 }
}
/*protected*/ void Editor::setSecondSelectionGroup(QList<Positionable*> list) {
    _secondSelectionGroup = list;
}

/*protected*/ Editor* Editor::changeView(QString className)
{
 setCursor(Qt::WaitCursor);
JFrame* frame = getTargetFrame();
//QColor background = getBackgroundColor();
//    try {
 Editor* ed = (Editor*)Class::forName(className); //.newInstance();
 if(ed == NULL)
 {
  log->error(tr("Failed to load editor %1").arg(className));
  return NULL;
 }

 ed->setName(getName());
 ed->init(getName());

 ed->_contents = _contents;
 for (int i = 0; i<_contents->size(); i++)
 {
  Positionable* p = _contents->at(i);
  p->setEditor(ed);
  ed->addToTarget(p);
  if (_debug) log->debug("changeView: "+p->getNameString()+" addToTarget class= " +QString(p->metaObject()->className()));
 }
 ed->setAllEditable(isEditable());
 ed->setAllPositionable(allPositionable());
 //ed.setShowCoordinates(showCoordinates());
 ed->setAllShowTooltip(showTooltip());
 ed->setAllControlling(allControlling());
 ed->setShowHidden(isVisible());
 ed->setPanelMenu(frame->menuBar()->isVisible());
//        ed->setScroll(getScrollable());
 ed->setTitle();
 //ed->setBackgroundColor(getBackgroundColor());
//        ed->getTargetFrame()->setLocation(frame->getLocation());
 ed->getTargetFrame()->resize(frame->size());
 ed->resize(size());
 //ed->adjustSize();
 ed->setVisible(true);
 PanelMenu::instance()->addEditorPanel(ed);
 dispose(false);
 setCursor(Qt::ArrowCursor);
 return ed;
//    } catch (ClassNotFoundException cnfe) {
//        log->error("changeView exception "+cnfe.toQString());
//    } catch (InstantiationException ie) {
//        log->error("changeView exception "+ie.toQString());
//    } catch (IllegalAccessException iae) {
//        log->error("changeView exception "+iae.toQString());
//    }

 //return NULL;
}


/************************* Popup Item Methods ***********************/
/**
* These methods are to be called from the editor view's showPopUp method
*/
/**
* Add a checkbox to lock the position of the Positionable item
*/
/*public*/ void Editor::setPositionableMenu(Positionable* p, QMenu* popup)
{
 //JCheckBoxMenuItem lockItem = new JCheckBoxMenuItem(Bundle.getMessage("LockPosition"));
 //lockItem.setSelected(!p.isPositionable());
 PositionableLabel* pl = qobject_cast<PositionableLabel*>(p);
 if(pl == NULL) return;
 QAction* lockItemAction = new QAction(tr("Lock Position"), this);
 lockItemAction->setCheckable(true);
 lockItemAction->setChecked(!((PositionableLabel*)p)->isPositionable());
 connect(lockItemAction,SIGNAL(toggled(bool)), this, SLOT(On_lockItemAction_toggled(bool)));
 currItem = pl;
//    lockItem.addActionListener(new ActionListener(){
//        Positionable comp;
//        JCheckBoxMenuItem checkBox;
//        /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
//            comp.setPositionable(!checkBox.isSelected());
//            setSelectionsPositionable(!checkBox.isSelected(), comp);
//        }
//        ActionListener init(Positionable pos, JCheckBoxMenuItem cb) {
//            comp = pos;
//            checkBox = cb;
//            return this;
//        }
//    }.init(p, lockItem));
 popup->addAction(lockItemAction);
}
void Editor::On_lockItemAction_toggled(bool enabled) // SLOT[]
{
 PositionableLabel* pl = currItem;
 //pl->setPositionable(enabled);
 setSelectionsPositionable(enabled, pl);
}

/**
* Display the X & Y coordinates of the Positionable item and provide a
* dialog memu item to edit them.
*/
/*public*/ bool Editor::setShowCoordinatesMenu(Positionable* p, QMenu* popup)
{
    //if (showCoordinates()) {
 QMenu* edit = new QMenu(tr("Edit Location"));
 if ((/*p instanceof MemoryIcon*/qobject_cast<MemoryIcon*>(p)!= NULL) && (((MemoryIcon*)p)->getPopupUtility()->getFixedWidth()==0))
 {
  MemoryIcon* pm = (MemoryIcon*) p;
  edit->addAction(new QAction("x= " + QString("%1").arg(pm->getOriginalX()),this));
  edit->addAction(new QAction("y= " + QString("%1").arg(pm->getOriginalY()), this));

  edit->addAction(((MemoryIconCoordinateEdit*) pm->cEdit)->getCoordinateEditAction(pm,this));
 }
 else
 {
  edit->addAction(new QAction("x= " + QString("%1").arg(p->getX()),this));
  edit->addAction(new QAction("y= " + QString("%1").arg(p->getY()),this));
  CoordinateEdit* ce = new CoordinateEdit();
  edit->addAction(CoordinateEdit::getCoordinateEditAction(p,ce));
 }
 popup->addMenu(edit);
 return true;
    //}
    //return false;
}

/**
* Offer actions to align the selected Positionable items either
* Horizontally (at avearage y coord) or Vertically (at avearage x coord).
*/
/*public*/ bool Editor::setShowAlignmentMenu(Positionable* p, QMenu* popup)
{
    if (showAlignPopup(p))
    {
     /*JMenu*/QMenu* edit = new /*JMenu*/QMenu(tr("EditAlignment"));

   //     edit.add(new AbstractAction(rb.getQString("AlignX")) {
   //         /*public*/ void actionPerformed(ActionEvent e) {
   //             alignSelection(true);
   //         }
   //     });
     QAction* alignXAction = new QAction("AlignX",this);
     popup->addAction(alignXAction);
   //     edit.add(new AbstractAction(rb.getQString("AlignY")) {
   //         /*public*/ void actionPerformed(ActionEvent e) {
   //             alignSelection(false);
   //         }
   //     });
     QAction* alignYAction = new QAction("AlignY",this);
     popup->addAction(alignYAction);

     popup->addMenu(edit);
     return true;
    }
    return false;}

/**
* Display display 'z' level of the Positionable item and provide a
* dialog memu item to edit it.
*/
/*public*/ void Editor::setDisplayLevelMenu(Positionable* p, QMenu* popup)
{
 QMenu* edit = new QMenu(tr("Edit Level"));
 PositionableLabel* ps = qobject_cast<PositionableLabel*>(p);
 if(ps != NULL)
  edit->addAction(new QAction(tr("level= ") + QString("%1").arg(ps->getDisplayLevel()),this));
 CoordinateEdit* coordinateEdit = new CoordinateEdit();
 edit->addAction(CoordinateEdit::getLevelEditAction(p, coordinateEdit));
 popup->addMenu(edit);
}

/**
* Add a checkbox to set visibility of the Positionable item
*/
/*public*/ void Editor::setHiddenMenu(Positionable* p, QMenu* popup)
{
 if(qobject_cast<PositionableLabel*>(p)!= NULL)
 {
    PositionableLabel* pl = qobject_cast<PositionableLabel*>(p);
    Q_ASSERT(pl != NULL);
   //  if (p.getDisplayLevel() == BKG || p instanceof PositionableShape) {
   //      return;
   //  }
   //  JCheckBoxMenuItem hideItem = new JCheckBoxMenuItem(Bundle.getMessage("SetHidden"));
   //  hideItem.setSelected(p->isHidden());
   //  hideItem.addActionListener(new ActionListener(){
   //      Positionable comp;
   //      JCheckBoxMenuItem checkBox;
   //      /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
   //          comp.setHidden(checkBox.isSelected());
   //          setSelectionsHidden(checkBox.isSelected(), comp);
   //      }
   //      ActionListener init(Positionable pos, JCheckBoxMenuItem cb) {
   //          comp = pos;
   //          checkBox = cb;
   //          return this;
   //      }
   //  }.init(p, hideItem));
   //  popup.add(hideItem);
     QAction* hiddenAction = new QAction(tr("Hide when not editing"),this);
     hiddenAction->setCheckable(true);
     hiddenAction->setChecked(pl->isHidden());
     connect(hiddenAction, SIGNAL(toggled(bool)), this, SLOT(On_actionHidden_toggled(bool)));
     popup->addAction(hiddenAction);
     saveP = p;
 }
}
void Editor::On_actionHidden_toggled(bool bState) // [slot]
{
 Positionable* comp =saveP;
 //comp.setHidden(checkBox.isSelected());
 setSelectionsHidden(bState, comp);
}
#if 1
/**
* Add a checkbox to display a tooltip for the Positionable item and
* if showable, provide a dialog menu to edit it.
*/
/*public*/ void Editor::setShowTooltipMenu(Positionable* p, QMenu* popup)
{
 if (p->getDisplayLevel() == BKG) {
     return;
 }
 QMenu* edit = new QMenu(tr("Edit Tooltip"));
 QAction* showTooltipItem = new QAction(tr("Show Tooltip"), this);
 showTooltipItem->setCheckable(true);
 showTooltipItem->setChecked(p->showTooltip());
// showTooltipItem.addActionListener(new ActionListener(){
//     Positionable comp;
//     JCheckBoxMenuItem checkBox;
//     /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
//         comp.setShowTooltip(checkBox.isSelected());
//     }
//     ActionListener init(Positionable pos, JCheckBoxMenuItem cb) {
//         comp = pos;
//         checkBox = cb;
//         return this;
//     }
// }.init(p, showTooltipItem));
 ShowToolTipActionListener* showToolTipActionListener = new ShowToolTipActionListener( p,  p->showTooltip(), this);
 connect(showTooltipItem, SIGNAL(triggered()), showToolTipActionListener, SLOT(actionPerformed()));

 edit->addAction(showTooltipItem);
 edit->addAction(CoordinateEdit::getTooltipEditAction(p, this));
 popup->addMenu(edit);
}

ShowToolTipActionListener::ShowToolTipActionListener(Positionable *pos, bool isChecked, Editor *editor)
{
 this->comp = pos;
 this->isChecked = isChecked;
 this->editor = editor;
}
void ShowToolTipActionListener::actionPerformed(ActionEvent *e)
{
 comp->setShowTooltip(!isChecked);
}

#endif
/**
* Add an action to remove the Positionable item.
*/
/*public*/ void Editor::setRemoveMenu(Positionable* p, QMenu* popup)
{
    //  popup.add(new AbstractAction(Bundle.getMessage("Remove")) {
    //      Positionable comp;
    //      /*public*/ void actionPerformed(ActionEvent e) {
    //          comp.remove();
    //          removeSelections(comp);
    //      }
    //      AbstractAction init(Positionable pos) {
    //          comp = pos;
    //          return this;
    //      }
    //  }.init(p));
     //currComp = p;
    saveP = p;
     QAction* removeMenuAction = new QAction("Remove", this);
     connect(removeMenuAction, SIGNAL(triggered()), this, SLOT(On_removeMenuAction_triggered()));
     popup->addAction(removeMenuAction);
}
void Editor::On_removeMenuAction_triggered()
{
 saveP->remove();
 removeSelections(saveP);
}

/************************* End Popup Methods ***********************/
/******************** Marker Menu ****************************/

/*protected*/ void Editor::locoMarkerFromRoster()
{
    QDialog* locoRosterFrame = new QDialog(this);
    locoRosterFrame->resize(200, 50);
    formLayout = new QFormLayout(locoRosterFrame);
    //locoRosterFrame.getContentPane().setLayout(new FlowLayout());
    locoRosterFrame->setWindowTitle(tr("Loco From Roster"));
    QLabel* mtext = new QLabel();
    mtext->setText(tr("Select Loco")+":");
    //locoRosterFrame.getContentPane().add(mtext);
    formLayout->addWidget(mtext);
    rosterBox = new  RosterEntrySelectorPanel(locoRosterFrame);
    rosterBox->setVisible(true);
    formLayout->addWidget(rosterBox);
   // RosterEntryComboBox* reCombo = new RosterEntryComboBox(Roster::instance(), Roster::instance()->getDefaultRosterGroup(), "", "", "", "", "", "", "", locoRosterFrame);
   // reCombo->setEnabled(true);
    //reCombo->setCurrentIndex(0);
   // formLayout->addWidget(reCombo);
   //             qDebug() << tr("comboBox now contains %1 items").arg(reCombo->count());

    //formLayout->activate();
   // rosterBox.addPropertyChangeListener("selectedRosterEntries", new PropertyChangeListener() {

   //     @Override
   //     /*public*/ void propertyChange(PropertyChangeEvent pce) {
   //         if (rosterBox.getSelectedRosterEntries().length != 0) {
   //             selectLoco(rosterBox.getSelectedRosterEntries()[0]);
   //         }
   //     }
   // });
    //connect(rosterBox, SIGNAL(propertyChange(QString,QObject*,QObject*)), this, SLOT(On_rosterBoxSelectionChanged(QString,QObject*,QObject*)));
    connect(rosterBox, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));

    //locoRosterFrame.getContentPane().add(rosterBox);
   // locoRosterFrame.addWindowListener(new java.awt.event.WindowAdapter() {
   //         /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
   //             locoRosterFrame.dispose();
   // }
   //     });
   // locoRosterFrame.pack();
    locoRosterFrame->setVisible(true);
    //rosterBox->setEnabled(true);
}
void Editor::On_rosterBoxSelectionChanged(QString propertyName,QObject* /*o*/,QObject* n) // SLOT[]
{
 if(propertyName == "selectedRosterEntries")
 {
  LocoIcon* icon = selectLoco(((QList<RosterEntry*>*)n)->at(0));
  icon->setLocation(200,100);
  addToTarget((Positionable*)icon);
 }
}

/*protected*/ LocoIcon* Editor::selectLoco(QString rosterEntryTitle)
{
 if (""==(rosterEntryTitle))
        return NULL;
    return selectLoco(Roster::instance()->entryFromTitle(rosterEntryTitle));
}

/*protected*/ LocoIcon* Editor::selectLoco(RosterEntry* entry) {
    LocoIcon* l = NULL;
    if (entry==NULL) {
        return NULL;
    }
    // try getting road number, else use DCC address
    QString rn = entry->getRoadNumber();
    if ((rn.isNull()) || rn.isEmpty())
        rn = entry->getDccAddress();
    if (rn != NULL){
        l = addLocoIcon(rn);
        l->setRosterEntry(entry);
    }
    return l;
}

/*protected*/ void Editor::locoMarkerFromInput()
{
#if 0 // done, see below.
    const JmriJFrame locoFrame = new JmriJFrame();
    locoFrame.getContentPane().setLayout(new FlowLayout());
    locoFrame.setTitle(Bundle.getMessage("EnterLocoMarker"));
    javax.swing.JLabel textId = new javax.swing.JLabel();
    textId.setText(Bundle.getMessage("LocoID")+":");
    locoFrame.getContentPane().add(textId);
    const javax.swing.JTextField locoId = new javax.swing.JTextField(7);
    locoFrame.getContentPane().add(locoId);
    locoId.setText("");
    locoId.setToolTipText(Bundle.getMessage("EnterLocoID"));
    javax.swing.JButton okay = new javax.swing.JButton();
    okay.setText(Bundle.getMessage("OK"));
    okay.addActionListener(new java.awt.event.ActionListener() {
        /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
            QString nameID = locoId.getText();
            if ( (nameID!=NULL) && !(nameID.trim()==("")) ) {
                addLocoIcon(nameID.trim());
            }
            else {
                JOptionPane.showMessageDialog(locoFrame,Bundle.getMessage("ErrorEnterLocoID"),
                                Bundle.getMessage("errorTitle"),JOptionPane.ERROR_MESSAGE);
            }
        }
    });
    locoFrame.getContentPane().add(okay);
    locoFrame.addWindowListener(new java.awt.event.WindowAdapter() {
            /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
                locoFrame.dispose();
            }
        });
    locoFrame.pack();
    if(_targetFrame != NULL)
        locoFrame.setLocation(_targetFrame.getLocation());
    locoFrame.setVisible(true);
#endif
    InputDialog* dlg = new InputDialog("Enter loco id", "0", NULL, this);
    if(dlg->exec() == QDialog::Accepted)
    {
     QString nameID= dlg->value();
     if(!nameID.isNull())
     {
      LocoIcon* icon = addLocoIcon(nameID.trimmed());
      icon->setLocation(200,100);
      addToTarget((Positionable*)icon);
     }
    }
    //paintTargetPanel(editScene);
}

/**
 * Remove marker icons from panel
 */
/*protected*/ void Editor::removeMarkers()
{
 if (_debug) log->debug("Remove markers");
 for (int i=_contents->size()-1; i>=0; i--)
 {
  Positionable* il = _contents->at(i);
  //if (il instanceof LocoIcon)
  if(qobject_cast<LocoIcon*>(il)!= NULL)
  {
   ((LocoIcon*)il)->remove();
  }
 }
}

/************************* End Marker Menu Methods ***********************/

/************** Adding content to the panel ***********************/

/*public*/ PositionableLabel* Editor::setUpBackground(QString name)
{
    NamedIcon* icon = NamedIcon::getIconByName(name);
    PositionableLabel* l = new PositionableLabel(icon, this);
    l->setPopupUtility(NULL);        // no text
    l->setPositionable(false);
    l->setShowTooltip(false);
    l->setSize(icon->getIconWidth(), icon->getIconHeight());
    l->setDisplayLevel(BKG);
    l->setLocation(getNextBackgroundLeft(),0);
    putItem((Positionable*)l);
    return l;
}


/*protected*/ PositionableLabel* Editor::addLabel(QString text)
{
 PositionableLabel* l = addLabel(text.trimmed());
 setDirty(true);
 l->setForeground(defaultTextColor);
 l->setLocation(dLoc.x(),dLoc.y());
 return l;
}

/**
 * Determine right side x of furthest right background
 */
/*private*/ int Editor::getNextBackgroundLeft()
{
 int left = 0;
 // place to right of background images, if any
 for (int i=0; i<_contents->size(); i++)
 {
  Positionable* p = _contents->at(i);
  //if (p instanceof PositionableLabel)
  if(qobject_cast<PositionableLabel*>(p)!= NULL)
  {
   PositionableLabel* l = (PositionableLabel*)p;
   if (l->isBackground())
   {
    int test = l->getX() + l->maxWidth();
    if (test>left) left = test;
   }
  }
 }
 return left;
}

/* Positionable has set a new level.  Editor must change it in the target panel.
*/
/*public*/ void Editor::displayLevelChange(Positionable* l){
//    removeFromTarget(l);
//    addToTarget(l);
// if(l->_itemGroup != NULL)
//  l->updateScene();
}

/*public*/ TrainIcon* Editor::addTrainIcon (QString name){
    TrainIcon* l = new TrainIcon(this);
    putLocoIcon(l, name);
    return l;
}

/*public*/ LocoIcon* Editor::addLocoIcon (QString name)
{
 LocoIcon* l = new LocoIcon(this);
 l->setTooltip(name);
 putLocoIcon(l, name);
 l->setPositionable(true);
 addToTarget((Positionable*)l);
 return l;
}

/*public*/ void Editor::putLocoIcon(LocoIcon* l, QString name) {
 l->setText(name);
 l->setHorizontalTextPosition(JLabel::CENTER);
 l->setSize(l->getPreferredSize().width(), l->getPreferredSize().height());
 l->setEditable(isEditable());    // match popup mode to editor mode
 putItem((Positionable*)l);
}

/*public*/ void Editor::putItem(Positionable* l)
{
 PositionableLabel* pl = qobject_cast<PositionableLabel*>(l);
 //l->invalidate();
 if(pl != NULL)
 {
  pl->setPositionable(true);
  //pl->setVisible(true);

  if (pl->getTooltip().isEmpty())
  {
   pl->setTooltip(_defaultToolTip);
   //pl->setTooltip();
  }
 }
 addToTarget(l);
 //  if (!_contents->append((Positionable*)l))
 //  {
 //      log->error("Unable to add "+l->getNameString()+" to _contents");
 //  }
 _contents->append(l);
 if(pl != NULL)
 //if( log->isDebugEnabled())
  log->debug(tr("putItem ")+pl->getNameString()+" to _contents. level= "+QString("%1").arg(pl->getDisplayLevel()));
}

/*protected*/ void Editor::addToTarget(Positionable* l)
{
 bool bAdded = false;
#if 0
 PositionableLabel* pl = qobject_cast<PositionableLabel*> (l);

 PositionableJComponent* pj = qobject_cast<PositionableJComponent*>(l);
 if(pj!=NULL)
  bAdded = pj->updateScene();
 if(pj != NULL)
 {
  if(pj->_itemGroup != NULL && pj->_itemGroup->scene() != NULL)
  {
   //Q_ASSERT(pl->_itemGroup->scene()!=0);
   editScene->removeItem(pl->_itemGroup);
  }
  pj->_itemGroup = new QGraphicsItemGroup();
  if(qobject_cast<PositionableRoundRect*>(l)!= NULL)
  {
   PositionableRoundRect* ps = (PositionableRoundRect*)l;
   QGraphicsRoundRectItem* rr = new QGraphicsRoundRectItem(0, 0, ps->_width, ps->_height, ps->_radius, ps->_radius);
   rr->setBrush(QBrush(ps->_fillColor));
   rr->setPen(QPen(QBrush(ps->_lineColor),ps->_lineWidth));
   ps->_itemGroup = new QGraphicsItemGroup();
   ps->_itemGroup->addToGroup(rr);
   int x = ps->getX();
   int y = ps->getY();
   ps->_itemGroup->setPos(x,y);
   ps->_itemGroup->setZValue(ps->getDisplayLevel());
   editScene->addItem(ps->_itemGroup);
  }
  else
  if(qobject_cast<PositionableRectangle*>(l)!= NULL)
  {
   PositionableRectangle* ps = (PositionableRectangle*)l;
   QGraphicsRectItem* rr = new QGraphicsRectItem(QRectF(0, 0, ps->_width, ps->_height));
   rr->setBrush(QBrush(ps->_fillColor));
   rr->setPen(QPen(QBrush(ps->_lineColor),ps->_lineWidth));
   ps->_itemGroup = new QGraphicsItemGroup();
   ps->_itemGroup->addToGroup(rr);
   int x = ps->getX();
   int y = ps->getY();
   ps->_itemGroup->setPos(x,y);
   ps->_itemGroup->setZValue(ps->getDisplayLevel());
   editScene->addItem(ps->_itemGroup);
  }
  else

   if(qobject_cast<PositionableCircle*>(l)!= NULL)
  {
   PositionableCircle* ps = (PositionableCircle*)l;
   QGraphicsEllipseItem* rr = new QGraphicsEllipseItem(QRectF(0, 0, ps->_radius, ps->_radius));
   rr->setBrush(QBrush(ps->_fillColor));
   rr->setPen(QPen(QBrush(ps->_lineColor),ps->_lineWidth));
   ps->_itemGroup = new QGraphicsItemGroup();
   ps->_itemGroup->addToGroup(rr);
   int x = ps->getX();
   int y = ps->getY();
   ps->_itemGroup->setPos(x,y);
   ps->_itemGroup->setZValue(ps->getDisplayLevel());
   editScene->addItem(ps->_itemGroup);
  }
  else if(qobject_cast<PositionableEllipse*>(l)!= NULL)
  {
   PositionableEllipse* ps = (PositionableEllipse*)l;
   QGraphicsEllipseItem* rr = new QGraphicsEllipseItem(QRectF(0, 0, ps->_width, ps->_height));
   rr->setBrush(QBrush(ps->_fillColor));
   rr->setPen(QPen(QBrush(ps->_lineColor),ps->_lineWidth));
   ps->_itemGroup = new QGraphicsItemGroup();
   ps->_itemGroup->addToGroup(rr);
   int x = ps->getX();
   int y = ps->getY();
   ps->_itemGroup->setPos(x,y);
   ps->_itemGroup->setZValue(ps->getDisplayLevel());
   editScene->addItem(ps->_itemGroup);
  }
  return;
 }
 if(pl == NULL)
     return;
 if(pl != NULL)
 {
 if(pl->_itemGroup != NULL && pl->_itemGroup->scene() != NULL)
 {
  //Q_ASSERT(pl->_itemGroup->scene()!=0);
  editScene->removeItem(pl->_itemGroup);
 }
 pl->_itemGroup = new QGraphicsItemGroup();
 if(pl->isIcon())
 {
  QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(pl->getIcon()->getOriginalImage()));

  pl->_itemGroup->addToGroup(item);
  editScene->addItem(pl->_itemGroup);
  item->setPos(pl->getLocation());
  item->setToolTip(pl->getTooltip());
 }
 else
 {
  QGraphicsTextItem* item = new QGraphicsTextItem(pl->getUnRotatedText());
  pl->_itemGroup->addToGroup(item);
  editScene->addItem(pl->_itemGroup);
  item->setPos(pl->getLocation());
  item->setToolTip(pl->getTooltip());
 }
 if(pl->getDegrees() != 0)
  pl->_itemGroup->setRotation(pl->_itemGroup->rotation()+ pl->getDegrees());
 }
 editScene->addItem(pl->_itemGroup);
#else
// if(pl->_itemGroup != NULL && pl->_itemGroup->scene() != NULL)
// {
//  Q_ASSERT(pl->_itemGroup->scene()!=0);
//  editScene->removeItem(pl->_itemGroup);
//  if(qobject_cast<MemoryInputIcon*>(pl) == NULL && qobject_cast<MemorySpinnerIcon*>(pl) == NULL && qobject_cast<MemoryComboIcon*>(pl) == NULL)
//   pl->_itemGroup = new QGraphicsItemGroup();
// }
// else
//  pl->_itemGroup = new QGraphicsItemGroup();
 //pl->_itemGroup->setZValue(pl->getDisplayLevel());
#if 0
 if(!pl->isIcon())
 {
  //color = defaultTextColor;

//   if(pl->item != NULL)
//   {
//    g2->removeItem(pl->item);
//    pl->item = NULL;
//   }
  if(pl->getUnRotatedText()=="crap")
  {
      log->debug("crap found");
  }
  QGraphicsTextItem*  item = new QGraphicsTextItem(pl->getUnRotatedText());
  item->setFont(pl->getFont());
  item->setPos(pl->getX(), pl->getY());
  item->setDefaultTextColor(pl->getForeground());
  //if(pl->isOpaque())
  {
   QRectF ir = item->boundingRect();
   int margin = pl->getPopupUtility()->getMargin();
   QRectF r = QRectF(ir.x()-margin,ir.y()-margin,ir.right()+margin*2,ir.bottom()+margin*2);
   QGraphicsRectItem* rItem = new QGraphicsRectItem(r);
   if(pl->isOpaque())
    rItem->setBrush( QBrush(pl->getBackground()));
   if(pl->getPopupUtility()->getBorderSize() == 0)
    rItem->setPen(Qt::NoPen);
   else
    rItem->setPen(QPen(QBrush(pl->getPopupUtility()->getBorderColor()),pl->getPopupUtility()->getBorderSize()));
   rItem->setPos(pl->getX(), pl->getY());

   if(pl->_itemGroup == NULL) pl->_itemGroup = new QGraphicsItemGroup;
   pl->_itemGroup->addToGroup(rItem);
  }
  if(pl->showTooltip()) item->setToolTip(pl->getTooltip());
  pl->_itemGroup->addToGroup(item);
  editScene->addItem(pl->_itemGroup);
  bAdded = true;
  if(pl->getDegrees() != 0)
   //pl->item->rotate(pl->getDegrees());
  {
   QPointF center = item->boundingRect().center();
   pl->_itemGroup->setTransformOriginPoint(center);
   pl->_itemGroup->setRotation(pl->_itemGroup->rotation()+ pl->getDegrees());
  }
 }
 else
 {
  QString state = "BeanStateUnknown";

  QPixmap pixmap;
  if(qobject_cast<SensorIcon*>(l) != NULL)
  {
   int iState = 0;
   if(((SensorIcon*)l)->getSensor() != NULL)
    iState=((AbstractSensor*)((SensorIcon*)l)->getSensor())->getKnownState();
   state = ((SensorIcon*)l)->_state2nameMap->value(iState);
//    if(pl->item != NULL)
//    {
//     g2->removeItem(pl->item);
//     pl->item = NULL;
//    }
//    if(((SensorIcon*)l)->_state2nameMap->contains(istate))
//     state = ((SensorIcon*)l)->_state2nameMap->value(istate);
   pixmap = QPixmap::fromImage(((SensorIcon*)l)->getIcon(iState)->getImage());
   if(pixmap.isNull())
   {
    log->warn("Sensor icon invalid pixmap, state = "+ state);
   }
   QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
   item->setPos(pl->getX(), pl->getY());
   if(pl->showTooltip()) item->setToolTip(pl->getTooltip());
   pl->_itemGroup->addToGroup(item);
   editScene->addItem(pl->_itemGroup);
   bAdded = true;
  }
  else
  if(qobject_cast<AnalogClock2Display*>(l) != NULL)
  {
   ((AnalogClock2Display*)l)->update();
   bAdded = true;
  }
  else
  if(qobject_cast<LcdClockFrame*>(l) != NULL)
  {
   ((LcdClockFrame*)l)->update();
  }
  else
  if(qobject_cast<NixieClockFrame*>(l) != NULL)
  {
   ((NixieClockFrame*)l)->update();
  }
  else
  if(qobject_cast<LocoIcon*>(l) != NULL)
  {
   if(pl->isIcon())
   {
    pixmap = QPixmap::fromImage(((JLabel*)l)->getIcon()->getOriginalImage());
    if(pixmap.isNull())
     log->debug(QString("LocoIcon null pixmap: %1").arg(((JLabel*)l)->getIcon()->getURL()));
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
    item->setPos(pl->getX(), pl->getY());
    if(pl->showTooltip()) item->setToolTip(pl->getTooltip());
    if(pl->getDegrees() != 0)
    {
     QPointF center = item->boundingRect().center();
     pl->_itemGroup->setTransformOriginPoint(center);
     item->setRotation(item->rotation()+ pl->getDegrees());
    }
    pl->_itemGroup->addToGroup( item);
    //g2->addItem(pl->item);
   }
   else
   {
    QGraphicsTextItem* item = new QGraphicsTextItem(pl->getUnRotatedText());
    item->setPos(pl->getX(), pl->getY());
    if(pl->showTooltip()) item->setToolTip(pl->getTooltip());
    pl->_itemGroup->addToGroup(item);
    if(pl->getDegrees() != 0)
    {
     QPointF center = item->boundingRect().center();
     pl->_itemGroup->setTransformOriginPoint(center);
     item->setRotation(item->rotation()+ pl->getDegrees());
    }
   }
   editScene->addItem(pl->_itemGroup);
   bAdded = true;

  }

  else
  if(qobject_cast<MemoryIcon*>(l) != NULL)
  {
   MemoryIcon* m = (MemoryIcon*)l;
//    if(m->item != NULL)
//    {
//     g2->removeItem(m->item);
//     m->item = NULL;
//    }
   if(m->isIcon())
   {
    pixmap = QPixmap::fromImage(((PositionableLabel*)l)->getIcon()->getImage());
    //QGraphicsPixmapItem* item = g2->addPixmap(pixmap);
            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap, 0);
    item->setPos(pl->getX(), pl->getY());
    if(pl->showTooltip()) item->setToolTip(pl->getTooltip());
    if(pixmap.isNull())
     qDebug() << "No pixmap";
    if(m->_itemGroup== NULL)
      m->_itemGroup = new QGraphicsItemGroup;
    m->_itemGroup->addToGroup(item);
   }
   else
   {

    //m->item = g2->addText("????");
//     m->_item = new QGraphicsTextItem("????");
//     m->_item->setPos(l->getX(), l->getY());
    QGraphicsTextItem* item = new QGraphicsTextItem("????");
    m->_itemGroup->addToGroup(item);
    if(pl->showTooltip()) item->setToolTip(pl->getTooltip());
    item->setPos(pl->getX(),pl->getY());
    if(m->getDegrees() != 0)
    {
     //m->item->rotate(l->getDegrees());
     QPointF center = item->boundingRect().center();
     pl->_itemGroup->setTransformOriginPoint(center);
     item->setRotation(item->rotation()+ m->getDegrees());
    }
   }
   editScene->addItem(m->_itemGroup);
   bAdded = true;

  }
  else
  if(qobject_cast<LightIcon*>(l)!= NULL)
  {
    if(((LightIcon*)l)->getLight() != NULL)
    {
     int iState = ((AbstractLight*)((LightIcon*)l)->getLight())->getState();
     pixmap = QPixmap::fromImage(((LightIcon*)l)->getIcon(iState)->getImage());
     //   QGraphicsPixmapItem* item = g2->addPixmap(pixmap);
     QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap,0);
     item->setPos(((LightIcon*)l)->getX(),((LightIcon*)l)->getY());
     if(pl->showTooltip()) item->setToolTip(((LightIcon*)l)->getTooltip());
     if(pixmap.isNull())
      qDebug() << "No pixmap";
     //   else
     //    qDebug() << tr("pixmap h = %1, w = %2").arg(pixmap.height()).arg(pixmap.width());
     pl->_itemGroup->addToGroup(item);
     editScene->addItem(pl->_itemGroup);
     bAdded = true;
    }
    else
     log->debug(tr("Light icon '%1' missing light").arg(((LightIcon*)l)->getName()));
  }

  else
  if(qobject_cast<SignalHeadIcon*>(l)!= NULL && ((SignalHeadIcon*)l)->getSignalHead() != NULL)
  {
    //int iState = ((SignalHeadIcon*)l)->getSignalHead()->getState();
    QString appearance;
    if(((SignalHeadIcon*)l)->getSignalHead() != NULL)
     appearance = ((AbstractSignalHead*)((SignalHeadIcon*)l)->getSignalHead())->getAppearanceName();
    else
        Q_ASSERT(false);
    pixmap = QPixmap::fromImage(((SignalHeadIcon*)l)->getIcon(appearance)->getImage());
    //   QGraphicsPixmapItem* item = g2->addPixmap(pixmap);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap,0);
    item->setPos(((SignalHeadIcon*)l)->getX(),((SignalHeadIcon*)l)->getY());
    if(pl->showTooltip()) item->setToolTip(((SignalHeadIcon*)l)->getTooltip());
    if(pixmap.isNull())
     qDebug() << "No pixmap";
    //   else
    //    qDebug() << tr("pixmap h = %1, w = %2").arg(pixmap.height()).arg(pixmap.width());
    if(pl->getDegrees() != 0)
    {
     QPointF center = item->boundingRect().center();
     pl->_itemGroup->setTransformOriginPoint(center);
     pl->_itemGroup->setRotation(item->rotation()+ pl->getDegrees());
    }
    pl->_itemGroup->addToGroup(item);
    editScene->addItem(pl->_itemGroup);
    bAdded = true;
  }
  else
  if(qobject_cast<SignalMastIcon*>(l)!= NULL)
  {
   //int iState = ((SignalMastIcon*)l)->getSignalMast()->getState();
   if(((SignalMastIcon*)l)->getSignalMast() != NULL)
   {
    QString aspect = ((AbstractSignalMast*)((SignalMastIcon*)l)->getSignalMast())->getAspect();
    pixmap = QPixmap::fromImage(((SignalMastIcon*)l)->getIcon(aspect)->getImage());
    //   QGraphicsPixmapItem* item = g2->addPixmap(pixmap);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap,0);
    item->setPos(((SignalMastIcon*)l)->getX(),((SignalMastIcon*)l)->getY());
    if(pl->showTooltip()) item->setToolTip(((SignalMastIcon*)l)->getTooltip());
    if(pixmap.isNull())
     qDebug() << "No pixmap";
    //   else
    //    qDebug() << tr("pixmap h = %1, w = %2").arg(pixmap.height()).arg(pixmap.width());
    if(pl->getDegrees() != 0)
     item->setRotation(item->rotation()+ pl->getDegrees());
    pl->_itemGroup->addToGroup(item);
    editScene->addItem(pl->_itemGroup);
    bAdded = true;
   }
   else
    log->debug("SignalMastIcon not added");
  }
  else
  if(qobject_cast<MultiSensorIcon*>(l)!= NULL)
  {
    pixmap = QPixmap::fromImage(((MultiSensorIcon*)l)->getIcon()->getImage());
    //   QGraphicsPixmapItem* item = g2->addPixmap(pixmap);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap,0);
    item->setPos(((MultiSensorIcon*)l)->getX(),((MultiSensorIcon*)l)->getY());
    if(pl->showTooltip()) item->setToolTip(((MultiSensorIcon*)l)->getTooltip());
    if(pixmap.isNull())
     qDebug() << "No pixmap";
    //   else
    //    qDebug() << tr("pixmap h = %1, w = %2").arg(pixmap.height()).arg(pixmap.width());
    if(pl->getDegrees() != 0)
     item->setRotation(item->rotation()+ pl->getDegrees());
    pl->_itemGroup->addToGroup(item);
    editScene->addItem(pl->_itemGroup);
    bAdded = true;
  }
  else if(qobject_cast<MemoryInputIcon*>(l)!= NULL)
  {
   MemoryInputIcon* icon = (MemoryInputIcon*)l;
//   if(icon->_itemGroup != NULL)
//   {
//    //icon->_itemGroup->addToGroup(new QGraphicsTextItem("MemoryInputItem"));
//    int x = icon->getX();
//    int y = icon->getY();
//    icon->_itemGroup->setPos(x,y);
//    editScene->addItem(icon->_itemGroup);
//   }
//   else
   {
//    icon->_itemGroup = new QGraphicsItemGroup();
//    icon-> widget = new MyGraphicsProxyWidget();
//    QLineEdit* edit = new QLineEdit("???");
//    edit->resize(icon->_textBox->size());
//    icon->widget->setWidget(edit);
       icon->widget->setFlag(QGraphicsItem::ItemIsFocusable,true) ;
       icon->widget->setFlag(QGraphicsItem::ItemIsMovable, true);
       icon->widget->setFlag(QGraphicsItem::ItemIsSelectable, true);
       icon->_textBox->setEnabled(true);
       if(icon->_itemGroup == NULL) icon->_itemGroup = new QGraphicsItemGroup;
       icon->_itemGroup->addToGroup(icon->widget);
       icon->_itemGroup->setZValue(icon->getDisplayLevel());
     int x = icon->getX();
     int y = icon->getY();
     icon->_itemGroup->setPos(x,y);
     editScene->addItem(icon->_itemGroup);
     bAdded = true;
   }
  }
  else if(qobject_cast<MemorySpinnerIcon*>(l)!= NULL)
  {
   MemorySpinnerIcon* icon = (MemorySpinnerIcon*)l;
//   if(icon->_itemGroup != NULL)
//   {
//    icon->_itemGroup->addToGroup(new QGraphicsTextItem("MemorySpinnerItem"));
//    icon->_itemGroup->setPos(icon->getX(), icon->getY());
//    editScene->addItem(icon->_itemGroup);
//   }
//   else
   {
       //icon->_itemGroup = new QGraphicsItemGroup();
   //    icon->widget = new MyGraphicsProxyWidget();
   //    QSpinBox* edit = new QSpinBox();
   //    edit->resize(icon->spinner->size());
   //    icon->widget->setWidget(edit);
       icon->widget->setFlag(QGraphicsItem::ItemIsFocusable,true) ;
       if(icon->_itemGroup == NULL) icon->_itemGroup = new QGraphicsItemGroup;
        icon->_itemGroup->addToGroup(icon->widget);
        icon->_itemGroup->setZValue(icon->getDisplayLevel());
        int x = icon->getX();
        int y = icon->getY();
        icon->_itemGroup->setPos(x,y);
        editScene->addItem(icon->_itemGroup);
        bAdded = true;
   }

  }
  else if(qobject_cast<MemoryComboIcon*>(l)!= NULL)
  {
   MemoryComboIcon* icon = (MemoryComboIcon*)l;
//   if(icon->_itemGroup != NULL)
//   {
//    icon->_itemGroup->addToGroup(new QGraphicsTextItem("MemoryComboItem"));
//    icon->_itemGroup->setPos(icon->getX(), icon->getY());
//    editScene->addItem(icon->_itemGroup);
//   }
//   else
   {
       //icon->_itemGroup = new QGraphicsItemGroup();
   //    icon-> widget = new MyGraphicsProxyWidget();
   //    QComboBox* edit = new QComboBox();
   //    edit->resize(icon->_comboBox->size());
   //    icon->widget->setWidget(edit);
    if(icon->_itemGroup == NULL) icon->_itemGroup = new QGraphicsItemGroup;
        icon->_itemGroup->addToGroup(icon->widget);
        icon->widget->setFlag(QGraphicsItem::ItemIsFocusable,true) ;
        icon->_itemGroup->setZValue(icon->getDisplayLevel());
        int x = icon->getX();
        int y = icon->getY();
        icon->_itemGroup->setPos(x,y);
        editScene->addItem(icon->_itemGroup);
        bAdded = true;
   }

  }
  if(((PositionableLabel*)l)->getIcon(/*state*/)!=NULL)
  {
   pixmap = QPixmap::fromImage(((PositionableLabel*)l)->getIcon(/*state*/)->getImage());
//   QGraphicsPixmapItem* item = g2->addPixmap(pixmap);
   QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap,0);
  item->setPos(pl->getX(), pl->getY());
  if(pl->showTooltip()) item->setToolTip(pl->getTooltip());
  if(pixmap.isNull())
   qDebug() << "No pixmap";
//   else
//    qDebug() << tr("pixmap h = %1, w = %2").arg(pixmap.height()).arg(pixmap.width());
   pl->_itemGroup->addToGroup(item);
   editScene->addItem(pl->_itemGroup);
   bAdded = true;
   //if(pl->getDegrees() != 0)
   {
    //l->item->rotate(l->getDegrees());
    QRectF bnd = pl->_itemGroup->boundingRect();
    QPointF center = bnd.center();
    pl->_itemGroup->setTransformOriginPoint(center);
    pl->_itemGroup->setRotation(item->rotation()+ pl->getDegrees()+pl->getIcon()->getRotation());
    pl->currRotation = pl->getIcon()->getRotation();
   }
  }
  else
#endif
   if(l != NULL)
   {
    bAdded = l->updateScene();
    if(l->_itemGroup != NULL && l->_itemGroup->parentObject() != NULL)
     log->debug(tr("Item already has a parent %1 ").arg(l->getNameString()));
    editScene->addItem(l->_itemGroup);
   }
   else
    log->error("updateScene failed!");

  if(!bAdded)
      log->error("add failed " + QString(l->metaObject()->className()));
//  QRectF r = ((PositionableLabel*)l)->getBounds();
//  editScene->invalidate(r,QGraphicsScene::ItemLayer);
// }
#endif
}
/************** Icon editors for adding content ************/

/*static*/ const /*public*/ QStringList Editor::ICON_EDITORS = QStringList() <<
"Sensor"<< "RightTurnout"<< "LeftTurnout"<< "SlipTOEditor"<< "SignalHead"<< "SignalMast"<< "Memory"<< "Light"<< "Reporter"<< "Background"<< "MultiSensor"<< "Icon"<< "Text";
#if 0
/**
* @param name Icon editor's name
*/
/*public*/ JFrameItem getIconFrame(QString name) {
    JFrameItem frame = _iconEditorFrame.get(name);
    if (frame==NULL) {
        if ("Sensor"==(name)) {
            addSensorEditor();
        } else if ("RightTurnout"==(name)) {
            addRightTOEditor();
        } else if ("LeftTurnout"==(name)) {
            addLeftTOEditor();
        } else if ("SlipTOEditor"==(name)) {
            addSlipTOEditor();
        } else if ("SignalHead"==(name)) {
            addSignalHeadEditor();
        } else if ("SignalMast"==(name)) {
            addSignalMastEditor();
        } else if ("Memory"==(name)) {
            addMemoryEditor();
        } else if ("Reporter"==(name)) {
            addReporterEditor();
        } else if ("Light"==(name)) {
            addLightEditor();
        } else if ("Background"==(name)) {
            addBackgroundEditor();
        } else if ("MultiSensor"==(name)) {
            addMultiSensorEditor();
        } else if ("Icon"==(name)) {
            addIconEditor();
        } else if ("Text"==(name)) {
            addTextEditor();
        } else {
//                log->error("No such Icon Editor \""+name+"\"");
            return NULL;
        }
        // frame added in the above switch
        frame = _iconEditorFrame.get(name);

        if (frame==NULL) { // addTextEditor does not create a usable frame
            return NULL;
        }
        //frame.setLocationRelativeTo(this);
        frame.setLocation(frameLocationX, frameLocationY);
        frameLocationX += DELTA;
        frameLocationY += DELTA;
    }
    frame.setVisible(true);
    return frame;
}
#endif
/*public*/ IconAdder* Editor::getIconEditor(QString name) {
    return _iconEditorFrame->value(name)->getEditor();
}

/**
 * Add a label to the target
 */
/*protected*/ void Editor::addTextEditor()
{
 //QString newLabel = JOptionPane.showInputDialog(this, Bundle.getMessage("PromptNewLabel"));
 QString newLabel;
 InputDialog* dlg = new InputDialog(tr("Enter Text, then press OK"), "");
//    if (newLabel==NULL) return;  // canceled
 if(dlg->exec() == QDialog::Accepted)
 {
  newLabel = dlg->value();
  PositionableLabel* l = addLabel(newLabel);
  // always allow new items to be moved
  l->setPositionable(true);
 }
}

/*protected*/ void Editor::addRightTOEditor()
{
 IconAdder* editor = new IconAdder("RightTurnout");
 editor->setIcon(3, "TurnoutStateClosed",
        ":/resources/icons/smallschematics/tracksegments/os-righthand-west-closed.gif");
 editor->setIcon(2, "TurnoutStateThrown",
        ":/resources/icons/smallschematics/tracksegments/os-righthand-west-thrown.gif");
 editor->setIcon(0, "BeanStateInconsistent",
        ":/resources/icons/smallschematics/tracksegments/os-righthand-west-error.gif");
 editor->setIcon(1, "BeanStateUnknown",
        "resources/icons/smallschematics/tracksegments/os-righthand-west-unknown.gif");

 JFrameItem* frame = makeAddIconFrame("RightTurnout", true, true, editor);
 frame->resize(400,800);
 //frame->setMinimumHeight(600);
 _iconEditorFrame->insert("RightTurnout", frame);
 editor->setPickList(PickListModel::turnoutPickModelInstance());

//    ActionListener addIconAction = new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent a) {
//            addTurnoutR();
//        }
//    };
 AddRightTOActionListener* addIconAction = new AddRightTOActionListener(this);
 editor->makeIconPanel(true);
 editor->complete(addIconAction, true, true, false);
 frame->addHelpMenu("package.jmri.jmrit.display.IconAdder", true);
 frame->pack();
 frame->show();
}

/*protected*/ void Editor::addLeftTOEditor()
{
 IconAdder* editor = new IconAdder("LeftTurnout");
 editor->setIcon(3, "TurnoutStateClosed",
        ":/resources/icons/smallschematics/tracksegments/os-lefthand-east-closed.gif");
 editor->setIcon(2, "TurnoutStateThrown",
        ":/resources/icons/smallschematics/tracksegments/os-lefthand-east-thrown.gif");
 editor->setIcon(0, "BeanStateInconsistent",
        ":/resources/icons/smallschematics/tracksegments/os-lefthand-east-error.gif");
 editor->setIcon(1, "BeanStateUnknown",
        ":/resources/icons/smallschematics/tracksegments/os-lefthand-east-unknown.gif");

 JFrameItem* frame = makeAddIconFrame("LeftTurnout", true, true, editor);
 frame->resize(400,800);
 //frame->setMinimumHeight(600);
 _iconEditorFrame->insert("LeftTurnout", frame);
 editor->setPickList(PickListModel::turnoutPickModelInstance());

//    ActionListener addIconAction = new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent a) {
//            addTurnoutL();
//        }
//    };
 AddLeftTOActionListener* addIconAction = new AddLeftTOActionListener(this);
 editor->makeIconPanel(true);
 editor->complete(addIconAction, true, true, false);
 frame->addHelpMenu("package.jmri.jmrit.display.IconAdder", true);
 frame->pack();
 frame->show();
}

/*protected*/ void Editor::addSlipTOEditor()
{
    SlipIconAdder* editor = new SlipIconAdder("SlipTOEditor");
    editor->setIcon(3, "LowerWestToUpperEast",
        ":/resources/icons/smallschematics/tracksegments/os-slip-lower-west-upper-east.gif");
    editor->setIcon(2, "UpperWestToLowerEast",
        ":/resources/icons/smallschematics/tracksegments/os-slip-upper-west-lower-east.gif");
    editor->setIcon(4, "LowerWestToLowerEast",
        "resources/icons/smallschematics/tracksegments/os-slip-lower-west-lower-east.gif");
    editor->setIcon(5, "UpperWestToUpperEast",
        "resources/icons/smallschematics/tracksegments/os-slip-upper-west-upper-east.gif");
    editor->setIcon(0, "BeanStateInconsistent",
        "resources/icons/smallschematics/tracksegments/os-slip-error-full.gif");
    editor->setIcon(1, "BeanStateUnknown",
        "resources/icons/smallschematics/tracksegments/os-slip-unknown-full.gif");
    editor->setTurnoutType(SlipTurnoutIcon::DOUBLESLIP);
    JFrameItem* frame = makeAddIconFrame("SlipTOEditor", true, true, editor);
    frame->resize(400,800);
    //frame->setMinimumHeight(600);
    QFont f = frame->font();
    f.setPointSize(8);
    frame->setFont(f);
    _iconEditorFrame->insert("SlipTOEditor", frame);
    editor->setPickList(PickListModel::turnoutPickModelInstance());

//    ActionListener addIconAction = new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent a) {
//            addSlip();
//        }
//    };
    AddSlipActionListener* addIconAction = new AddSlipActionListener(this);
    editor->makeIconPanel(true);
    editor->complete(addIconAction, true, true, false);
    frame->addHelpMenu("package.jmri.jmrit.display.SlipTurnoutIcon", true);
    frame->pack();
    frame->show();
}

/*protected*/ void Editor::addSensorEditor()
{
 IconAdder* editor = new IconAdder("Sensor");
 editor->setIcon(3, "SensorStateActive",
     ":/resources/icons/smallschematics/tracksegments/circuit-occupied.gif");
 editor->setIcon(2, "SensorStateInactive",
     ":/resources/icons/smallschematics/tracksegments/circuit-empty.gif");
 editor->setIcon(0, "BeanStateInconsistent",
     ":/resources/icons/smallschematics/tracksegments/circuit-error.gif");
 editor->setIcon(1, "BeanStateUnknown",
     ":/resources/icons/smallschematics/tracksegments/circuit-error.gif");

 JFrameItem* frame = makeAddIconFrame("Sensor", true, true, editor);
 frame->resize(550,850);
 frame->setMinimumHeight(600);
 frame->layout()->setSpacing(0);
 _iconEditorFrame->insert("Sensor", frame);
 editor->setPickList(PickListModel::sensorPickModelInstance());

//    ActionListener addIconAction = new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent a) {
//            putSensor();
//        }
//    };
 AddPanelIconActionListener* addIconAction = new AddPanelIconActionListener(this);
 editor->makeIconPanel(true);
 editor->complete(addIconAction, true, true, false);
 frame->addHelpMenu("package.jmri.jmrit.display.IconAdder", true);
 frame->pack();
 frame->show();
}
AddPanelIconActionListener::AddPanelIconActionListener(Editor *parent)
{
 this->parent = parent;
}
void AddPanelIconActionListener::actionPerformed(ActionEvent *)
{
 parent->putSensor();
}

/*protected*/ IconAdder* Editor::getSignalHeadEditor()
{
 IconAdder* editor = new IconAdder("SignalHead");
 editor->setIcon(0, ("Red"),
        ":/resources/icons/smallschematics/searchlights/left-red-marker.gif");
 editor->setIcon(1, ("Yellow"),
        ":/resources/icons/smallschematics/searchlights/left-yellow-marker.gif");
 editor->setIcon(2, ("Green"),
        ":/resources/icons/smallschematics/searchlights/left-green-marker.gif");
 editor->setIcon(3, ("Dark"),
        ":/resources/icons/smallschematics/searchlights/left-dark-marker.gif");
 editor->setIcon(4, ("Held"),
        ":/resources/icons/smallschematics/searchlights/left-held-marker.gif");
 editor->setIcon(5, ("Lunar"),
        "resources/icons/smallschematics/searchlights/left-lunar-marker.gif");
 editor->setIcon(6, ("Flashing Red"),
        ":/resources/icons/smallschematics/searchlights/left-flashred-marker.gif");
 editor->setIcon(7, ("Flashing Yellow"),
        ":/resources/icons/smallschematics/searchlights/left-flashyellow-marker.gif");
 editor->setIcon(8, ("Flashing Green"),
        ":/resources/icons/smallschematics/searchlights/left-flashgreen-marker.gif");
 editor->setIcon(9, ("Flashing Lunar"),
        ":/resources/icons/smallschematics/searchlights/left-flashlunar-marker.gif");
 return editor;
}

/*protected*/ void Editor::addSignalHeadEditor()
{
 IconAdder* editor = getSignalHeadEditor();
 JFrameItem* frame = makeAddIconFrame("SignalHead", true, true, editor);
 frame->resize(400,800);
 //frame->setMinimumHeight(600);
 _iconEditorFrame->insert("SignalHead", frame);
 editor->setPickList(PickListModel::signalHeadPickModelInstance());

//    ActionListener addIconAction = new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent a) {
//            putSignalHead();
//        }
//    };
 SignalHeadActionListener* addIconAction = new SignalHeadActionListener(this);
 editor->makeIconPanel(true);
 editor->complete(addIconAction, true, false, false);
 frame->addHelpMenu("package.jmri.jmrit.display.IconAdder", true);
 frame->pack();
 frame->show();
}


/*protected*/ void Editor::addSignalMastEditor()
{
 IconAdder* editor = new IconAdder("SignalMast");

 JFrameItem* frame = makeAddIconFrame("SignalMast", true, true, editor);
 frame->resize(400,800);
 //frame->setMinimumHeight(600);
 _iconEditorFrame->insert("SignalMast", frame);
 editor->setPickList(PickListModel::signalMastPickModelInstance());

//    ActionListener addIconAction = new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent a) {
//            putSignalMast();
//        }
//    };
 SignalMastActionListener* addIconAction = new SignalMastActionListener(this);
 editor->makeIconPanel(true);
 editor->complete(addIconAction, true, false, false);
 frame->addHelpMenu("package.jmri.jmrit.display.IconAdder", true);
 frame->pack();
 frame->show();
}

#if 1
/*protected*/ void Editor::addMemoryEditor()
{
// IconAdder* editor = new IconAdder("Memory") {
//            JButton bSpin = new JButton(Bundle.getMessage("AddSpinner"));
//            JButton bBox = new JButton(Bundle.getMessage("AddInputBox"));
//            JSpinner spinner = new JSpinner(_spinCols);
//            /*protected*/ void addAdditionalButtons(JPanel p) {
//                bSpin.addActionListener( new ActionListener() {
//                    /*public*/ void actionPerformed(ActionEvent a) {
//                    addMemorySpinner();
//                    }
//                });
//                JPanel p1 = new JPanel();
//                //p1.setLayout(new BoxLayout(p1, BoxLayout.X_AXIS));
//                bBox.addActionListener( new ActionListener() {
//                    /*public*/ void actionPerformed(ActionEvent a) {
//                    addMemoryInputBox();
//                    }
//                });
//                ((JSpinner.DefaultEditor)spinner.getEditor()).getTextField().setColumns(2);
//                spinner.setMaximumSize(spinner.getPreferredSize());
//                JPanel p2 = new JPanel();
//                p2.add(new JLabel(Bundle.getMessage("NumColsLabel")));
//                p2.add(spinner);
//                p1.add(p2);
//                p1.add(bBox);
//                p.add(p1);
//                p1 = new JPanel();
//                p1.add(bSpin);
//                p.add(p1);
//            }

//            /*public*/ void valueChanged(ListSelectionEvent e) {
//                super.valueChanged(e);
//                bSpin.setEnabled(addIconIsEnabled());
//                bBox.setEnabled(addIconIsEnabled());
//            }
//    };
//    ActionListener addIconAction = new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent a) {
//            putMemory();
//        }
//    };
    MemoryActionListener* addIconAction = new MemoryActionListener(this);
    IconAdder* editor = new MemoryIconAdder(this);
    JFrameItem* frame = makeAddIconFrame("Memory", true, true, editor);
    _iconEditorFrame->insert("Memory", frame);
    editor->setPickList(PickListModel::memoryPickModelInstance());
    editor->makeIconPanel(true);
    editor->complete(addIconAction, false, true, false);
    frame->addHelpMenu("package.jmri.jmrit.display.IconAdder", true);
    frame->pack();
    frame->show();
}
#endif
Editor::MemoryIconAdder::MemoryIconAdder(Editor *parent)
{
    this->editor = parent;
    bSpin = new QPushButton(tr("Add Spinner"));
    bBox = new QPushButton(tr("Add InputBox"));
    spinner = new QSpinBox(/*_spinCols*/);
    spinner->setMaximum(/*_spinCols->getMaximum()*/100);
    spinner->setMinimum(/*_spinCols->getMinimum()*/1);
    spinner->setSingleStep(/*_spinCols->getStepSize()*/1);
    spinner->setValue(/*_spinCols->getValue()*/3);

}
void Editor::MemoryIconAdder::addAdditionalButtons(QWidget* p)
{
    QVBoxLayout* pLayout = (QVBoxLayout*)p->layout();
    connect(bSpin, SIGNAL(clicked(bool)), editor, SLOT(addMemorySpinner()));
    QWidget* p1 = new QWidget();
    p1->setContentsMargins(0,0,0,0);
    QHBoxLayout* p1Layout;
    p1->setLayout(p1Layout = new QHBoxLayout(p1/*, BoxLayout.X_AXIS*/));
//          bBox.addActionListener( new ActionListener() {
//              /*public*/ void actionPerformed(ActionEvent a) {
//              addMemoryInputBox();
//              }
//          });
    connect(bBox, SIGNAL(clicked()), editor, SLOT(addMemoryInputBox()));
//          ((JSpinner.DefaultEditor)spinner.getEditor()).getTextField().setColumns(2);
    spinner->setMaximumSize(spinner->sizeHint());
    QWidget* p2 = new QWidget();
    QHBoxLayout* p2Layout;
    p2->setContentsMargins(0,0,0,0);
    p2->setLayout(p2Layout = new QHBoxLayout);
    p2->layout()->addWidget(new QLabel(tr("Col Width")));
    p2->layout()->addWidget(spinner);
    p2->layout()->addWidget(bBox);
    p1Layout->addWidget(p2,0,Qt::AlignHCenter);
    pLayout->addWidget(p1);
    p1 = new QWidget();
    p1->setContentsMargins(0,0,0,0);
    p1->setLayout(new QVBoxLayout(p1/*, BoxLayout.X_AXIS*/));

    p1->layout()->addWidget(bSpin);
    ((QBoxLayout*)pLayout)->addWidget(p1,0, Qt::AlignHCenter);

}
void Editor::MemoryIconAdder::valueChanged(ListSelectionEvent* e )
{
 IconAdder::valueChanged(e);
 bSpin->setEnabled(addIconIsEnabled());
}
/*protected*/ void Editor::addReporterEditor()
{
 IconAdder* editor = new IconAdder("Reporter");
//    ActionListener addIconAction = new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent a) {
//            addReporter();
//        }
//    };
 AddReporterActionListener* addIconAction = new AddReporterActionListener(this);
 JFrameItem* frame = makeAddIconFrame("Reporter", true, true, editor);
 frame->resize(400,800);
 //frame->setMinimumHeight(600);
 _iconEditorFrame->insert("Reporter", frame);
    editor->setPickList(PickListModel::reporterPickModelInstance());
    editor->makeIconPanel(true);
    editor->complete(addIconAction, false, true, false);
    frame->addHelpMenu("package.jmri.jmrit.display.IconAdder", true);
    frame->pack();
    frame->show();
}

/*protected*/ void Editor::addLightEditor()
{
 IconAdder* editor = new IconAdder("Light");
 editor->setIcon(3, "LightStateOff",
        ":/resources/icons/smallschematics/tracksegments/os-lefthand-east-closed.gif");
 editor->setIcon(2, "LightStateOn",
        ":/resources/icons/smallschematics/tracksegments/os-lefthand-east-thrown.gif");
 editor->setIcon(0, "BeanStateInconsistent",
        ":/resources/icons/smallschematics/tracksegments/os-lefthand-east-error.gif");
 editor->setIcon(1, "BeanStateUnknown",
        ":/resources/icons/smallschematics/tracksegments/os-lefthand-east-unknown.gif");

 JFrameItem* frame = makeAddIconFrame("Light", true, true, editor);
 _iconEditorFrame->insert("Light", frame);
 editor->setPickList(PickListModel::lightPickModelInstance());

//    ActionListener addIconAction = new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent a) {
//            addLight();
//        }
//    };
 AddLightActionListener* addIconAction= new AddLightActionListener(this);
 editor->makeIconPanel(true);
 editor->complete(addIconAction, true, true, false);
 frame->addHelpMenu("package.jmri.jmrit.display.IconAdder", true);
 frame->pack();
 frame->show();
}

/*protected*/ void Editor::addBackgroundEditor()
{
 IconAdder* editor = new IconAdder("Background");
 editor->setIcon(0, "background","resources/PanelPro.gif");

 JFrameItem* frame = makeAddIconFrame("Background", true, false, editor);
 frame->resize(400,800);
 //frame->setMinimumHeight(600);
 _iconEditorFrame->insert("Background", frame);

//    ActionListener addIconAction = new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent a) {
//            putBackground();
//        }
//    };
 AddBackgoundActionListener* addIconAction = new AddBackgoundActionListener(this);
 editor->makeIconPanel(true);
 editor->complete(addIconAction, true, false, false);
 frame->addHelpMenu("package.jmri.jmrit.display.IconAdder", true);
 frame->pack();
 frame->show();
}

/*protected*/ JFrameItem* Editor::addMultiSensorEditor()
{
 MultiSensorIconAdder* editor = new MultiSensorIconAdder("MultiSensor");
 editor->setIcon(0, "BeanStateInconsistent",
                              "program:resources/icons/USS/plate/levers/l-inconsistent.gif");
 editor->setIcon(1, "BeanStateUnknown",
                              "program:resources/icons/USS/plate/levers/l-unknown.gif");
 editor->setIcon(2, "SensorStateInactive",
                              "program:resources/icons/USS/plate/levers/l-inactive.gif");
 editor->setIcon(3, "MultiSensorPosition 0",
                              "program:resources/icons/USS/plate/levers/l-left.gif");
 editor->setIcon(4, "MultiSensorPosition 1",
                              "program:resources/icons/USS/plate/levers/l-vertical.gif");
 editor->setIcon(5, "MultiSensorPosition 2",
                              "program:resources/icons/USS/plate/levers/l-right.gif");

 JFrameItem* frame = makeAddIconFrame("MultiSensor", true, false, editor);
 frame->resize(400,800);
 //frame->setMinimumHeight(600);
 _iconEditorFrame->insert("MultiSensor", frame);
 frame->addHelpMenu("package.jmri.jmrit.display.MultiSensorIconAdder", true);

 editor->setPickList(PickListModel::sensorPickModelInstance());

//    ActionListener addIconAction = new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent a) {
//            addMultiSensor();
//        }
//    };
 MultisensorIconActionListener* addIconAction = new MultisensorIconActionListener(this);
 editor->makeIconPanel(true);
    editor->complete(addIconAction, true, true, false);
 frame->pack();
 frame->show();
 return frame;
}

/*protected*/ void Editor::addIconEditor()
{
 IconAdder* editor = new IconAdder("Icon");
 editor->setIcon(0, "plainIcon",":/resources/icons/smallschematics/tracksegments/block.gif");
 JFrameItem* frame = makeAddIconFrame("Icon", true, false, editor);
 //frame->resize(350,400);
 //frame->setMinimumHeight(300);
 _iconEditorFrame->insert("Icon", frame);

//    ActionListener addIconAction = new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent a) {
//            putIcon();
//        }
//    };
 AddIconActionListener* addIconAction = new AddIconActionListener(this);
 editor->makeIconPanel(true);
 editor->complete(addIconAction, true, false, false);
 frame->addHelpMenu("package.jmri.jmrit.display.IconAdder", true);
 frame->pack();
 frame->show();
}

/**************** add content items from Icon Editors ********************/
/**
 * Add a sensor indicator to the target
 */
/*protected*/ SensorIcon* Editor::putSensor()
{
 SensorIcon* l = new SensorIcon(new NamedIcon("resources/icons/smallschematics/tracksegments/circuit-error.gif",
                        "resources/icons/smallschematics/tracksegments/circuit-error.gif"), this);
 IconAdder* editor = getIconEditor("Sensor");
 QHash <QString, NamedIcon*>* map = ((IconAdder*)editor)->getIconMap();
 QStringListIterator e( map->keys());
 while (e.hasNext())
 {
  QString key = e.next();
  l->setIcon(key, map->value(key));
 }
//        l.setActiveIcon(editor.getIcon("SensorStateActive"));
//        l.setInactiveIcon(editor.getIcon("SensorStateInactive"));
//        l.setInconsistentIcon(editor.getIcon("BeanStateInconsistent"));
//        l.setUnknownIcon(editor.getIcon("BeanStateUnknown"));
 NamedBean* b = editor->getTableSelection();
 if (b!=NULL)
 {
  l->setSensor(b->getDisplayName());
 }
 l->setDisplayLevel(Editor::SENSORS);
 setNextLocation(l);
 putItem(l);
 return l;
}

/**
 * Add a turnout indicator to the target
 */
void Editor::addTurnoutR() {
    IconAdder* editor = getIconEditor("RightTurnout");
    addTurnout(editor);
}

void Editor::addTurnoutL() {
    IconAdder* editor = getIconEditor("LeftTurnout");
    addTurnout(editor);
}

/*protected*/ TurnoutIcon*  Editor::addTurnout(IconAdder* editor)
{
 TurnoutIcon* l = new TurnoutIcon(this);
 l->setTurnout(editor->getTableSelection()->getDisplayName());
 QHash <QString, NamedIcon*>* map = ((IconAdder*)editor)->getIconMap();
 QStringListIterator e(map->keys());
 while (e.hasNext())
 {
  QString key = e.next();
  l->setIcon(key, map->value(key));
 }
 l->setDisplayLevel(TURNOUTS);
 //setNextLocation(l);
 l->setLocation(_lastX, _lastY);
 l->setVisible(true);
 putItem(l);
 return l;
}

void Editor::addSlip()
{
 SlipIconAdder* editor = (SlipIconAdder*)getIconEditor("SlipTOEditor");
 SlipTurnoutIcon* l = new SlipTurnoutIcon(this);
 l->setSingleSlipRoute(editor->getSingleSlipRoute());
 switch(editor->getTurnoutType())
 {
 case SlipTurnoutIcon::DOUBLESLIP :
    l->setLowerWestToUpperEastIcon(((IconAdder*)editor)->getIcon("LowerWestToUpperEast"));
    l->setUpperWestToLowerEastIcon(((IconAdder*)editor)->getIcon("UpperWestToLowerEast"));
    l->setLowerWestToLowerEastIcon(((IconAdder*)editor)->getIcon("LowerWestToLowerEast"));
    l->setUpperWestToUpperEastIcon(((IconAdder*)editor)->getIcon("UpperWestToUpperEast"));
    break;
 case SlipTurnoutIcon::SINGLESLIP:
    l->setLowerWestToUpperEastIcon(((IconAdder*)editor)->getIcon("LowerWestToUpperEast"));
    l->setUpperWestToLowerEastIcon(((IconAdder*)editor)->getIcon("UpperWestToLowerEast"));
    l->setLowerWestToLowerEastIcon(((IconAdder*)editor)->getIcon("Slip"));
    l->setSingleSlipRoute(editor->getSingleSlipRoute());
    break;
 case SlipTurnoutIcon::THREEWAY:
    l->setLowerWestToUpperEastIcon(((IconAdder*)editor)->getIcon("Upper"));
    l->setUpperWestToLowerEastIcon(((IconAdder*)editor)->getIcon("Middle"));
    l->setLowerWestToLowerEastIcon(((IconAdder*)editor)->getIcon("Lower"));
    l->setSingleSlipRoute(editor->getSingleSlipRoute());
    break;
 case SlipTurnoutIcon::SCISSOR: //Scissor is the same as a Double for icon storing.
    l->setLowerWestToUpperEastIcon(((IconAdder*)editor)->getIcon("LowerWestToUpperEast"));
    l->setUpperWestToLowerEastIcon(((IconAdder*)editor)->getIcon("UpperWestToLowerEast"));
    l->setLowerWestToLowerEastIcon(((IconAdder*)editor)->getIcon("LowerWestToLowerEast"));
    l->setUpperWestToUpperEastIcon(((IconAdder*)editor)->getIcon("UpperWestToUpperEast"));
    break;
    }

    if((editor->getTurnoutType()==SlipTurnoutIcon::SCISSOR)&&(!editor->getSingleSlipRoute()))
    {
     l->setTurnout(editor->getTurnout("lowerwest")->getName(), SlipTurnoutIcon::LOWERWEST);
     l->setTurnout(editor->getTurnout("lowereast")->getName(), SlipTurnoutIcon::LOWEREAST);
    }
    l->setInconsistentIcon(((IconAdder*)editor)->getIcon("BeanStateInconsistent"));
    l->setUnknownIcon(((IconAdder*)editor)->getIcon("BeanStateUnknown"));
    l->setTurnoutType(editor->getTurnoutType());
    l->setTurnout(editor->getTurnout("west")->getName(), SlipTurnoutIcon::WEST);
    l->setTurnout(editor->getTurnout("east")->getName(), SlipTurnoutIcon::EAST);
    l->setDisplayLevel(TURNOUTS);
    //setNextLocation(l);
    l->setLocation(_lastX, _lastY);
    l->setVisible(true);
    putItem(l);
}

/**
 * Add a signal head to the target
 */
/*protected*/ SignalHeadIcon* Editor::putSignalHead()
{
 SignalHeadIcon* l = new SignalHeadIcon(this);
 IconAdder* editor = getIconEditor("SignalHead");
 l->setSignalHead(editor->getTableSelection()->getDisplayName());
 QHash <QString, NamedIcon*>* map = editor->getIconMap();
 QStringListIterator e(map->keys());
 while (e.hasNext())
 {
  QString key = e.next();
  l->setIcon(key, map->value(key));
 }
 l->setDisplayLevel(SIGNALS);
 //setNextLocation(l);
 l->setLocation(_lastX, _lastY);
 putItem(l);
 return l;
}

/**
 * Add a signal mast to the target
 */
/*protected*/ SignalMastIcon* Editor::putSignalMast()
{
 SignalMastIcon* l = new SignalMastIcon(this);
 IconAdder* editor = _iconEditorFrame->value("SignalMast")->getEditor();
 l->setSignalMast(editor->getTableSelection()->getDisplayName());
 l->setDisplayLevel(SIGNALS);
 //   setNextLocation(l);
 l->setLocation(_lastX, _lastY);
 putItem(l);
 return l;
}

/*protected*/ MemoryIcon* Editor::putMemory()
{
 MemoryIcon* l = new MemoryIcon(new NamedIcon(":/resources/icons/misc/X-red.gif",
                        "resources/icons/misc/X-red.gif"), this);
 IconAdder* memoryIconEditor = getIconEditor("Memory");
 l->setMemory(memoryIconEditor->getTableSelection()->getDisplayName());
 l->setSize(l->getPreferredSize().width(), l->getPreferredSize().height());
 l->setDisplayLevel(MEMORIES);
 //setNextLocation(l);
 l->setLocation(_lastX, _lastY);
 putItem(l);
 return l;
}

/*protected*/ MemorySpinnerIcon* Editor::addMemorySpinner()
{
 MemorySpinnerIcon* l = new MemorySpinnerIcon(this);
 IconAdder* memoryIconEditor = getIconEditor("Memory");
 l->setMemory(memoryIconEditor->getTableSelection()->getDisplayName());
 l->setSize(l->getPreferredSize().width(), l->getPreferredSize().height());
 l->setDisplayLevel(MEMORIES);
 //setNextLocation(l);
 l->setLocation(_lastX, _lastY);
 putItem(l);
 return l;
}

/*protected*/ MemoryInputIcon* Editor::addMemoryInputBox()
{
 MemoryInputIcon* l = new MemoryInputIcon(_spinCols->getNumber().toInt(), this);
 IconAdder* memoryIconEditor = getIconEditor("Memory");
 l->setMemory(memoryIconEditor->getTableSelection()->getDisplayName());
 l->setSize(l->getPreferredSize().width(), l->getPreferredSize().height());
 l->setDisplayLevel(MEMORIES);
 //setNextLocation(l);
 l->setLocation(_lastX, _lastY);
 putItem(l);
 return l;
}

/**
 * Add a Light indicator to the target
 */
/*protected*/ LightIcon* Editor::addLight()
{
 LightIcon* l = new LightIcon(this);
 IconAdder* editor = getIconEditor("Light");
 l->setOffIcon(editor->getIcon("LightStateOff"));
 l->setOnIcon(editor->getIcon("LightStateOn"));
 l->setInconsistentIcon(editor->getIcon("BeanStateInconsistent"));
 l->setUnknownIcon(editor->getIcon("BeanStateUnknown"));
 l->setLight((Light*)editor->getTableSelection());
 l->setDisplayLevel(LIGHTS);
 //setNextLocation(l);
 l->setLocation(_lastX, _lastY);
 putItem(l);
 return l;
}

/*protected*/ ReporterIcon* Editor::addReporter()
{
 ReporterIcon* l = new ReporterIcon(this);
 IconAdder* reporterIconEditor = getIconEditor("Reporter");
 l->setReporter((Reporter*)reporterIconEditor->getTableSelection());
 l->setSize(l->getPreferredSize().width(), l->getPreferredSize().height());
 l->setDisplayLevel(REPORTERS);
//    setNextLocation(l);
 l->setLocation(_lastX, _lastY);
 putItem(l);
 return l;
}
/**
 * Button pushed, add a background image. Note that a background image
 * differs from a regular icon only in the level at which it's presented.
 */
void Editor::putBackground() {
    // most likely the image is scaled.  get full size from URL
    IconAdder* bkgrndEditor = getIconEditor("Background");
    QString url = bkgrndEditor->getIcon("background")->getURL();
    setUpBackground(url);
}


/**
 * Add an icon to the target
 */
/*protected*/ Positionable* Editor::putIcon()
{
 IconAdder* iconEditor = getIconEditor("Icon");
 QString url = iconEditor->getIcon("plainIcon")->getURL();
 NamedIcon* icon = NamedIcon::getIconByName(url);
 if (_debug) log->debug(tr("putIcon: ")+(icon==NULL?"NULL":"icon")+" url= "+url);
    PositionableLabel* l = new PositionableLabel(icon, this);
    l->setPopupUtility(NULL);        // no text
    l->setDisplayLevel(ICONS);
    //setNextLocation(l);
    l->setLocation(_lastX, _lastY);
    putItem(l);
    l->updateSize();
    return l;
}

/*public*/ MultiSensorIcon* Editor::addMultiSensor()
{
 MultiSensorIcon* m = new MultiSensorIcon(this);
 MultiSensorIconAdder* editor = (MultiSensorIconAdder*)getIconEditor("MultiSensor");
 m->setUnknownIcon(((IconAdder*)editor)->getIcon("BeanStateUnknown"));
 m->setInconsistentIcon(((IconAdder*)editor)->getIcon("BeanStateInconsistent"));
 m->setInactiveIcon(((IconAdder*)editor)->getIcon("SensorStateInactive"));
 int numPositions = editor->getNumIcons();
 for (int i=3; i<numPositions; i++)
 {
  NamedIcon* icon = editor->getIcon(i);
  QString sensor = editor->getSensor(i)->getName();
  m->addEntry(sensor, icon);
 }
 m->setUpDown(editor->getUpDown());
 m->setDisplayLevel(SENSORS);
 //setNextLocation(m);
 m->setLocation(_lastX, _lastY);
 putItem(m);
 return m;
}

/*protected*/ void Editor::addClock()
{
 AnalogClock2Display* l = new AnalogClock2Display(this);
 l->setOpaque(false);
 l->update();
 l->setDisplayLevel(CLOCK);
 setNextLocation(l);
 //l->setLocation(200,200);
 l->setVisible(true);
 l->setScale(1.0);
 putItem((Positionable*)l);
 l->paint(editScene);
}
#if 0
/*protected*/ void Editor::addRpsReporter() {
    RpsPositionIcon* l = new RpsPositionIcon(this);
    l->setSize(l->getPreferredSize().width, l->getPreferredSize().height);
    l->setDisplayLevel(SENSORS);
    setNextLocation(l);
    putItem(l);
}

/******************** end adding content *********************/

/*********************** Icon Editors utils ****************************/

#endif
/*public*/ void Editor::setTitle()
{
 #if 1
 QString name = "";
 if (((QWidget*)_targetPanel->parent())->window() !=NULL)
 {
  name=((JFrame*)((QWidget*)_targetPanel->parent())->window())->getTitle();
 }
 if (name==NULL || name==("")) JmriJFrame::setTitle(tr("Editor"));
 else JmriJFrame::setTitle(name+" "+tr("Editor"));
 //Iterator <JFrameItem> iter = _iconEditorFrame.values().iterator();
 QListIterator<JFrameItem*> iter(_iconEditorFrame->values());
 while (iter.hasNext()) {
     JFrameItem* frame = iter.next();
     frame->setTitle(frame->title()+" ("+name+")");
 }
 setName(name);
#else
 setWindowTitle(name);
 #endif
}
#if 1
/*protected*/ JFrameItem* Editor::makeAddIconFrame(QString name, bool add, bool table, IconAdder* editor)
{
 if (_debug) log->debug("makeAddIconFrame for "+name+", add= "+add+", table= "+table);
 QString txt;
 JFrameItem* frame = new JFrameItem(name, editor);
 frame->setMinimumSize(200, 500);
#if 1
 if (editor != NULL)
 {
//  QWidget* p = new QWidget();
  QVBoxLayout* pLayout= new QVBoxLayout;
//  p->setLayout(pLayout = new QVBoxLayout);
//  p->setContentsMargins(0,0,0,0);
//  pLayout->setContentsMargins(0,0,0,0);
//  pLayout->setSpacing(0);
  if (add)
  {
   txt = tr("To add a %1 to the Panel,").arg( /*Bundle.getMessage*/name);
  }
  else
  {
   txt = tr("To edit a %1 in the Panel,").arg(name);
  }
  pLayout->addWidget(new QLabel(txt));
  if (table)
  {
   txt = tr("Select a %1 from table, then press %2").arg(name).arg
                                 (add ? tr("Add to Panel") : tr("Update Panel"));
  }
  else
  {
   if ("MultiSensor"==(name))
   {
    txt = tr("Drag Sensors from table into the red boxes, then press %1").arg
                                     (add ? tr("Add to Panel") : tr("Update Panel"));
   }
   else
   {
    txt = tr("Select a %1 image, then press %2").arg(name).arg
                                     (add ? tr("Add to Panel") : tr("Update Panel"));
   }
  }
  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);

  pLayout->addWidget(new QLabel(txt));
//  QVBoxLayout* centralWidgetLayout;
//  if(frame->centralWidget() == NULL)
//  {
//   QWidget* centralWidget = new QWidget();
//   //centralWidget->setLayout(new BorderLayout);
//   centralWidget->setLayout(centralWidgetLayout = new QVBoxLayout);
//   frame->setCentralWidget(centralWidget);
//  }
//  else
//    centralWidgetLayout = (QVBoxLayout*)getContentPane()->layout();
  //frame->setSizePolicy(sizePolicy);

  //((BorderLayout*)frame->centralWidget()->layout())->addWidget(p,BorderLayout::North);
  QScrollArea* scrollArea = new QScrollArea;
  QWidget* scrollAreaWidget = new QWidget;
  QVBoxLayout* scrollAreaLayout;
  scrollAreaWidget->setLayout(scrollAreaLayout = new QVBoxLayout);
  scrollArea->setWidget(scrollAreaWidget);
  scrollArea->setWidgetResizable(true);
//  centralWidgetLayout->addWidget(scrollArea);
  frame->setCentralWidget(scrollArea);

//  centralWidgetLayout->addLayout(pLayout);
//  centralWidgetLayout->addWidget(editor);
  scrollAreaLayout->addLayout(pLayout);
  scrollAreaLayout->addWidget(editor);

  QMenuBar* menuBar = new QMenuBar();
  QMenu* findIcon = new QMenu(tr("Find Icons"));
  menuBar->addMenu(findIcon);

  QAction* editItem = new QAction(tr("Create/Edit Image Index"),this);
//        editItem.addActionListener(new ActionListener() {
//                Editor editor;
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    ImageIndexEditor ii = ImageIndexEditor.instance(editor);
//                    ii.pack();
//                    ii.setVisible(true);
//                }
//                ActionListener init(Editor ed) {
//                    editor = ed;
//                    return this;
//                }
//            }.init(this));
  EditItemActionListener* eial = new EditItemActionListener();
  eial->init(this);
  connect(editItem, SIGNAL(triggered()), eial, SLOT(actionPerformed()));
  findIcon->addAction(editItem);
  findIcon->addSeparator();

  QAction* searchItem = new QAction(tr("Search File System for Icons"),this);
//        searchItem.addActionListener(new ActionListener() {
//                IconAdder ea;
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    File dir = jmri.jmrit.catalog->DirectorySearcher.instance().searchFS();
//                    if (dir != NULL) {
//                        ea.addDirectoryToCatalog(dir);
//                    }
//                }
//                ActionListener init(IconAdder ed) {
//                    ea = ed;
//                    return this;
//                }
//        }.init(editor));
  SearchItemActionListener* sal = new SearchItemActionListener();
  sal->init(editor);
  connect(searchItem, SIGNAL(triggered()), sal,SLOT(actionPerformed()));
  findIcon->addAction(searchItem);
  frame->setMenuBar(menuBar);
  editor->setParent(frame);
  // when this window closes, check for saving
  if (add)
  {
//            frame.addWindowListener(new java.awt.event.WindowAdapter() {
//                    /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//                        jmri.jmrit.catalog->ImageIndexEditor.checkImageIndex();
//                        setDefaultCloseOperation(javax.swing.WindowConstants.HIDE_ON_CLOSE);
//                        if (_debug) log->debug("windowClosing: HIDE "+toQString());
//                    }
//            });
  }
 }
 else
 {
  log->error("No icon editor specified for "+name);
 }
 if (add)
 {
  txt = tr("Add %1").arg(name);
  _iconEditorFrame->insert(name, frame);
 }
 else
 {
  txt = tr("Edit %1 Icon").arg(name);
 }
#endif
 frame->setWindowTitle(txt+" ("+getTitle()+")");
 frame->adjustSize();
 //QTimer::singleShot(50, frame, SLOT(pack()));
 return frame;
}
void SearchItemActionListener::actionPerformed(ActionEvent */*e*/)
{
    QDir* dir = DirectorySearcher::instance()->searchFS();
if (dir != NULL) {
    ea->addDirectoryToCatalog(dir);
}

}
SearchItemActionListener* SearchItemActionListener::init(IconAdder *ed)
{
 ea = ed;
}
EditItemActionListener* EditItemActionListener::init(Editor *ed)
{
 editor = ed;
 return this;
}
void EditItemActionListener::actionPerformed(ActionEvent */*e*/)
{
 ImageIndexEditor* ii = ImageIndexEditor::instance(editor);
 ii->pack();
 ii->setVisible(true);
}

/********************* cleanup *************************/
#endif
/*protected*/ void Editor::removeFromTarget(Positionable* l)
{
#if 0 // TODO:
    _targetPanel.remove((Component)l);
    _highlightcomponent = NULL;
    Point p = l.getLocation();
    int w = l.getWidth();
    int h = l.getHeight();
    _targetPanel.validate();
    _targetPanel.repaint(p.x,p.y,w,h);
#endif
 if(l->_itemGroup != NULL && l->_itemGroup->scene()!= NULL)
 editScene->removeItem(l->_itemGroup);
}

/*public*/ bool Editor::removeFromContents(Positionable* l)
{
 removeFromTarget(l);
    //todo check that parent == _targetPanel
    //Container parent = this.getParent();
    // force redisplay
  _contents->remove(_contents->indexOf(l));
  return true;
}

/**
* On return of 'true', caller should call dispose()
*/
/*public*/ bool Editor::deletePanel()
{
 if (_debug) log->debug("deletePanel");
 // verify deletion
//int selectedValue = JOptionPane.showOptionDialog(_targetPanel,
//            Bundle.getMessage("QuestionA")+"\n"+Bundle.getMessage("QuestionB"),
//            Bundle.getMessage("DeleteVerifyTitle"),JOptionPane.YES_NO_OPTION,
//            JOptionPane.QUESTION_MESSAGE,NULL,
//            new Object[]{Bundle.getMessage("ButtonYesDelete"),Bundle.getMessage("ButtonNoCancel")},
//            Bundle.getMessage("ButtonNoCancel"));
//    // return without deleting if "No" response
//    return (selectedValue == JOptionPane.YES_OPTION);
 int selectedValue = QMessageBox::question(this, tr("Verify Delete Panel"),tr("Do you want to delete this panel?"), QMessageBox::Yes | QMessageBox::No);
 return (selectedValue == QMessageBox::Yes);
}

/*public*/ void Editor::dispose(bool clear)
{
#if 1 // TODO:
 if (_debug) log->debug(tr("Editor delete and dispose done. clear= ")+(clear?"true":"false"));
 QListIterator <JFrameItem*> iter( _iconEditorFrame->values());
 while (iter.hasNext())
 {
  JFrameItem* frame = iter.next();
  frame->setDefaultCloseOperation(JFrame::DO_NOTHING_ON_CLOSE);
     frame->dispose();
 }
 // delete panel - deregister the panel for saving
 ((ConfigXmlManager*)InstanceManager::configureManagerInstance())->deregister(this);
 PanelMenu::instance()->deletePanel(this);
 setVisible(false);
 if (clear)
 {
  _contents->clear();
 }
 //removeAll();
 while ( QWidget* w = findChild<QWidget*>() )
     delete w;
 //super.dispose();
 JmriJFrame::dispose();
#endif
}

#if 0
/****************** Mouse Methods ***********************/

/*public*/ void showToolTip(Positionable selection, MouseEvent event) {
    ToolTip tip = selection.getTooltip();
    QString txt = tip.getText();
    if (txt==NULL) {
        tip.setText(selection.getNameString());
    }
    tip.setLocation(selection.getX()+selection.getWidth()/2, selection.getY()+selection.getHeight());
    setToolTip(tip);
}
#endif
/*protected*/ int Editor::getItemX(Positionable* p, int deltaX)
{
 //if ((p instanceof MemoryIcon) && (p.getPopupUtility().getFixedWidth()==0)) {
 if(qobject_cast<MemoryIcon*>(p)!= NULL /*&& ((MemoryIcon*)p)->getFixedWidth == 0)*/)
 {
  MemoryIcon* pm = (MemoryIcon*) p;
  return pm->getOriginalX() + (int)qRound(deltaX/getPaintScale());
 }
// else
// if(qobject_cast<PositionableJComponent*>(p)!=NULL)
//     return ((PositionableJComponent*)p)->getX() + (int)qRound(deltaX/getPaintScale());
 else
 {
  return ((PositionableLabel*)p)->getX() + (int)qRound(deltaX/getPaintScale());
 }
}
/*protected*/ int Editor::getItemY(Positionable* p, int deltaY)
{
 //if ((p instanceof MemoryIcon) && (p.getPopupUtility().getFixedWidth()==0)) {
 if(qobject_cast<MemoryIcon*>(p)!= NULL /*&& ((MemoryIcon*)p)->getFixedWidth == 0)*/)
 {
  MemoryIcon* pm = (MemoryIcon*) p;
  return pm->getOriginalY() + (int)qRound(deltaY/getPaintScale());
 }
 else
// if(qobject_cast<PositionableJComponent*>(p)!=NULL)
//     return ((PositionableJComponent*)p)->getY() + (int)qRound(deltaY/getPaintScale());
// else
 {
  return ((PositionableLabel*)p)->getY() + (int)qRound(deltaY/getPaintScale());
 }
}

#if 0
/**
* Provide a method for external code to add items in popup menus
*/

/*public*/ void addToPopUpMenu(jmri.NamedBean nb, JMenuItem item, int menu){
    if(nb==NULL || item==NULL){
        return;
    }
    for(Positionable pos:_contents){
        if(pos.getNamedBean()==nb && pos.getPopupUtility()!=NULL){
            switch(menu){
                case VIEWPOPUPONLY : pos.getPopupUtility().addViewPopUpMenu(item); break;
                case EDITPOPUPONLY : pos.getPopupUtility().addEditPopUpMenu(item); break;
                default: pos.getPopupUtility().addEditPopUpMenu(item);
                         pos.getPopupUtility().addViewPopUpMenu(item);
            }
            return;
        } else if (pos instanceof SlipTurnoutIcon) {
            if(pos.getPopupUtility()!=NULL){
                SlipTurnoutIcon sti = (SlipTurnoutIcon)pos;
                if(sti.getTurnout(SlipTurnoutIcon.EAST)==nb || sti.getTurnout(SlipTurnoutIcon.WEST)==nb ||
                    sti.getTurnout(SlipTurnoutIcon.LOWEREAST)==nb || sti.getTurnout(SlipTurnoutIcon.LOWERWEST)==nb) {
                    switch(menu){
                        case VIEWPOPUPONLY : pos.getPopupUtility().addViewPopUpMenu(item); break;
                        case EDITPOPUPONLY : pos.getPopupUtility().addEditPopUpMenu(item); break;
                        default: pos.getPopupUtility().addEditPopUpMenu(item);
                                 pos.getPopupUtility().addViewPopUpMenu(item);
                    }
                    return;
                }
            }
        } else if (pos instanceof MultiSensorIcon) {
            if(pos.getPopupUtility()!=NULL){
                MultiSensorIcon msi = (MultiSensorIcon)pos;
                bool match = false;
                for(int i = 0; i<msi.getNumEntries(); i++){
                    if(msi.getSensorName(i)==(nb.getUserName())){
                        match = true;
                        break;
                    } else if (msi.getSensorName(i)==(nb.getSystemName())){
                        match = true;
                        break;
                    }
                }
                if(match){
                    switch(menu){
                        case VIEWPOPUPONLY : pos.getPopupUtility().addViewPopUpMenu(item); break;
                        case EDITPOPUPONLY : pos.getPopupUtility().addEditPopUpMenu(item); break;
                        default: pos.getPopupUtility().addEditPopUpMenu(item);
                                 pos.getPopupUtility().addViewPopUpMenu(item);
                    }
                    return;
                }
            }
        }
    }
}
#endif

/**
* Relocate item
*/
/*protected*/ void Editor::moveItem(Positionable* p, int deltaX, int deltaY)
{
    //if (_debug) log->debug("moveItem at ("+p.getX()+","+p.getY()+") delta ("+deltaX+", "+deltaY+")");
 if (getFlag(OPTION_POSITION, ((PositionableLabel*)p)->isPositionable()))
 {
  int xObj = getItemX( p, deltaX);
  int yObj = getItemY( p, deltaY);
  // don't allow negative placement, icon can become unreachable
  if (xObj < 0) xObj = 0;
  if (yObj < 0) yObj = 0;
  if(qobject_cast<PositionableLabel*>(p) != NULL)
   ((PositionableLabel*)p)->setLocation(xObj, yObj);
  else p->setLocation((double)xObj, (double)yObj);
  // and show!
 }
 //addToTarget(p);
 p->updateScene();
 if(qobject_cast<AnalogClock2Display*>(p)!=NULL)
  ((AnalogClock2Display*)p)->update();
}

/**
* Return a List of all items whose bounding rectangle contain the mouse position.
* ordered from top level to bottom
*/
/*protected*/ QList <Positionable*> Editor::getSelectedItems(QGraphicsSceneMouseEvent* event)
{
 double x = event->scenePos().x();
 double y = event->scenePos().y();
 return getSelectedItems(QPointF(x,y));
}
/*protected*/ QList <Positionable*> Editor::getSelectedItems(QPointF pt)
{
 double x = pt.x();
 double y = pt.y();
 QRectF rect;
 QVector <Positionable*>* selections = new QVector <Positionable*>();
 for (int i=0; i<_contents->size(); i++)
 {
  Positionable* p = _contents->at(i);
//  if(qobject_cast<PositionableLabel*>(p)==NULL)
//   continue;

  //            if (p instanceof PositionableShape && !(event->modifiers()&Qt::ShiftModifier)) {
  //            	continue;
  //            }
  int level;
#if 1
  if(qobject_cast<AnalogClock2Display*>(p) != NULL)
  {
   rect = ((AnalogClock2Display*)p)->getBounds();
   level = ((AnalogClock2Display*)p)->getDisplayLevel();
  }
  else
//  if(qobject_cast<LcdClockFrame*>(p) != NULL)
//  {
//   rect = ((LcdClockFrame*)p)->getBounds();
//   level = ((LcdClockFrame*)p)->getDisplayLevel();
//  }
//  else
  if(qobject_cast<LightIcon*>(p) != NULL)
  {
   LightIcon* li = (LightIcon*)p;
   if(li->isIcon())
   {
    NamedIcon* icon = li->getInconsistentIcon();
    rect = QRectF(li->getX(), li->getY(), icon->getIconWidth(), icon->getIconHeight());
   }
   else
    rect = li->getBounds();
   level = li->getDisplayLevel();
  }
  else if(qobject_cast<MemoryIcon*>(p)!= NULL)
  {
   rect = ((MemoryIcon*)p)->getBounds();
   level = ((MemoryIcon*)p)->getDisplayLevel();
  }
  else if(qobject_cast<MemoryInputIcon*>(p)!= NULL)
  {
   rect = ((MemoryInputIcon*)p)->getBounds();
   level = ((MemoryInputIcon*)p)->getDisplayLevel();
  }
  else if(qobject_cast<MemorySpinnerIcon*>(p)!= NULL)
  {
   rect = ((MemorySpinnerIcon*)p)->getBounds();
   level = ((MemorySpinnerIcon*)p)->getDisplayLevel();
  }
  else if(qobject_cast<MemoryComboIcon*>(p)!= NULL)
  {
   rect = ((MemoryComboIcon*)p)->getBounds();
   level = ((MemoryComboIcon*)p)->getDisplayLevel();
  }
  else if(qobject_cast<SignalHeadIcon*>(p)!= NULL)
  {
   rect = ((SignalHeadIcon*)p)->getBounds();
   level = ((SignalHeadIcon*)p)->getDisplayLevel();
   ((SignalHeadIcon*)p)->_icon = true;
  }
  else if(qobject_cast<SignalMastIcon*>(p)!= NULL)
  {
   rect = ((SignalMastIcon*)p)->getBounds();
   level = ((SignalMastIcon*)p)->getDisplayLevel();
  }
  else if(qobject_cast<MultiSensorIcon*>(p)!= NULL)
  {
   rect = ((MultiSensorIcon*)p)->getBounds();
   level = ((MultiSensorIcon*)p)->getDisplayLevel();
  }
  else if(qobject_cast<IndicatorTurnoutIcon*>(p)!= NULL)
  {
   rect = ((IndicatorTurnoutIcon*)p)->getBounds();
   level = ((IndicatorTurnoutIcon*)p)->getDisplayLevel();
  }
  else if(qobject_cast<IndicatorTrackIcon*>(p)!= NULL)
  {
   rect = ((IndicatorTrackIcon*)p)->getBounds();
   level = ((IndicatorTrackIcon*)p)->getDisplayLevel();
  }
  else if(qobject_cast<TurnoutIcon*>(p)!= NULL)
  {
   rect = ((TurnoutIcon*)p)->getBounds();
   level = ((TurnoutIcon*)p)->getDisplayLevel();
  }
  else if(qobject_cast<PositionableJComponent*>(p)!=NULL)
  {
   rect = ((PositionableJComponent*)p)->getBounds();
//   if(((PositionableJComponent*)p)->_itemGroup != NULL)
//   rect = ((PositionableJComponent*)p)->_itemGroup->boundingRect();
   level = ((PositionableJComponent*)p)->getDisplayLevel();
  }
  else
#endif
  {
   rect= ((PositionableLabel*)p)->getBounds();
//   if(!((PositionableLabel*)p)->isIcon())
//   {
//    PositionableLabel* l = (PositionableLabel*)p;
//    double h = l->getFont().pointSizeF();
//    double w = (h*2*(l->getText().length()))/3;
//    rect =  QRectF(x ,y ,w ,h);
//   }
   level = ((PositionableLabel*)p)->getDisplayLevel();
  }
     //if (_debug && !_dragging) log->debug("getSelectedItems: rect= ("+rect.x+","+rect.y+
     //                      ") width= "+rect.width+", height= "+rect.height+
     //                                    " isPositionable= "+p.isPositionable());
  QRectF rect2D(rect.x()*_paintScale, rect.y()*_paintScale, rect.width()*_paintScale,                                                        rect.height()*_paintScale);
//  if(rect2D.contains(x,y))
//   log->debug(((PositionableLabel*)p)->getNameString());
  if (rect2D.contains(x, y) && (level > BKG /*|| event->modifiers()&Qt::ControlModifier*/))
  {
      //qDebug() << tr("rect %1,%2,%3,%4 contains %5,%6").arg(rect2D.x()).arg(rect2D.y()).arg(rect2D.width()).arg(rect2D.height()).arg(x).arg(y);
   bool added =false;
      //int level = ((PositionableLabel*)p)->getDisplayLevel();
   for (int k=0; k<selections->size(); k++)
   {
    int selLevel;
//    if(qobject_cast<PositionableJComponent*>(selections->at(k))!= NULL)
//    {
//     selLevel = ((PositionableJComponent*)selections->at(k))->getDisplayLevel();
//    }
//    else
    {
     selLevel = ((PositionableLabel*)selections->at(k))->getDisplayLevel();
    }

    if (level >= selLevel)
    {
     selections->insert(k, p);
     added = true;       // OK to lie in the case of background icon
     break;
    }
   }
   if (!added)
   {
    selections->append(p);
   }
  }
 }
 //if (_debug)  log->debug("getSelectedItems at ("+x+","+y+") "+selections.size()+" found,");
 return selections->toList();
}

/*
* Gather all items inside _selectRect
* Keep old group if Control key is down
*/
/*protected*/ void Editor::makeSelectionGroup(QGraphicsSceneMouseEvent* event)
{
#if 1
 if (!(event->modifiers()&Qt::ControlModifier) || _selectionGroup==QList<Positionable*>())
 {
  _selectionGroup = QList <Positionable*>();
 }
 //QRect test =  QRect();
 QList <Positionable*> list = getContents();
 if (event->modifiers()&Qt::ShiftModifier)
 {
  for (int i=0; i < list.size(); i++)
  {
   Positionable* comp = list.at(i);
//   if (comp instanceof PositionableShape && !event.isShiftDown())
//   {
//    continue;
//   }
   if (_selectRect.intersects(((PositionableLabel*)comp)->getBounds(/*test*/)) &&
                            (event->modifiers()&Qt::ControlModifier || ((PositionableLabel*)comp)->getDisplayLevel()>BKG))
   {
     _selectionGroup.append(comp);
              /*  if (_debug) {
                    log->debug("makeSelectionGroup: selection: "+ comp.getNameQString()+
                                ", class= "+comp.getClass().getName());
                } */
   }
  }
 }
 else
 {
  for (int i=0; i < list.size(); i++)
  {
   Positionable* comp = list.at(i);
//            if (comp instanceof PositionableShape && !event.isShiftDown()) {
//                continue;
//            }
   if (_selectRect.contains(((PositionableLabel*)comp)->getBounds()) &&
                            (event->modifiers()&Qt::ControlModifier || ((PositionableLabel*)comp)->getDisplayLevel()>BKG))
   {
    _selectionGroup.append(comp);
              /*  if (_debug) {
                    log->debug("makeSelectionGroup: selection: "+ comp.getNameQString()+
                                ", class= "+comp.getClass().getName());
                } */
   }
  }
 }
 if (_debug) log->debug("makeSelectionGroup: "+QString("%1").arg(_selectionGroup.size())+" selected.");
 if (_selectionGroup.size() < 1)
 {
   _selectRect = QRectF();
  _selectionGroup = QList<Positionable*>();
 }
#endif
}

/*
* For the param, selection, Add to or delete from _selectionGroup.
* If not there, add.
* If there, delete.
* make new group if Cntl key is not held down
*/
/*protected*/ void Editor::modifySelectionGroup(Positionable* selection, QGraphicsSceneMouseEvent* event)
{
#if 1
 if (!event->modifiers()&Qt::ControlModifier || _selectionGroup==QList<Positionable*>())
 {
  _selectionGroup = QList <Positionable*>();
 }
 bool removed = false;
 if (((PositionableLabel*)selection)->getDisplayLevel()>BKG || event->modifiers()&Qt::ControlModifier)
 {
  if (_selectionGroup.contains(selection))
  {
   /*removed =*/ _selectionGroup.removeOne(selection);
   removed = true;
  }
  else
  {
   if (event->modifiers()&Qt::ShiftModifier /*|| !(selection instanceof PositionableShape)*/)
   {
    _selectionGroup.append(selection);
   }
  }
 }
 if (_debug)
 {
  log->debug("modifySelectionGroup: size= "+QString("%1").arg(_selectionGroup.size())+", selection "+
                  (removed ? "removed" : "added"));
 }
#endif
}

/*protected*/ bool Editor::setTextAttributes(Positionable* p, QMenu* popup)
{
 if (((PositionableLabel*)p)->getPopupUtility()==NULL)
 {
  return false;
 }
//    popup.add(new AbstractAction(Bundle.getMessage("TextAttributes")){
//        Positionable comp;
//        /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
//            new TextAttrDialog(comp);
//        }
//        AbstractAction init(Positionable pos) {
//            comp = pos;
//            return this;
//        }
//    }.init(p));
 QAction* textAttributesAction = new QAction(tr("Text Attributes"),this);
 saveP=p;
 popup->addAction(textAttributesAction);
 connect(textAttributesAction, SIGNAL(triggered()), this, SLOT(On_textAttributesAction_triggered()));
    return true;
}
void Editor::On_textAttributesAction_triggered() // SLOT[]
{
 TextAttrDialog* dlg = new TextAttrDialog(saveP, this);
 dlg->exec();
}

//class TextAttrDialog extends JDialog {
//    Positionable _pos;
//    jmri.jmrit.display.palette.DecoratorPanel _decorator;
TextAttrDialog::TextAttrDialog(Positionable* p, QWidget* _targetFrame) : JDialog(_targetFrame)
{
 //super(_targetFrame, Bundle.getMessage("TextAttributes"), true);
//    QFont f = font();
//    f.setPointSize(8);
//    setFont(f);
 setSizeGripEnabled(true);
 QVBoxLayout* thisLayout;
 editor = (Editor*)_targetFrame;
 setWindowTitle(tr("Text Attributes"));
 _pos = p;
 QWidget* panel = new QWidget();
 QVBoxLayout* panelLayout;
 panel->setLayout(panelLayout = new QVBoxLayout(panel/*, BoxLayout.Y_AXIS*/));
 _decorator = new DecoratorPanel(((PositionableLabel*)_pos)->getEditor());
 _decorator->initDecoratorPanel(_pos);
 panelLayout->addWidget(_decorator);
 panelLayout->addWidget(makeDoneButtonPanel());
    //setContentPane(panel);
 setLayout(thisLayout = new QVBoxLayout);
 setMinimumSize(600,600);
 //thisLayout->addLayout(panelLayout);
 QScrollArea* scrollArea;
 thisLayout->addWidget(scrollArea = new QScrollArea);
 scrollArea->setWidget(panel);
 pack();
//    setLocationRelativeTo((Component)_pos);
 setVisible(true);
}
/*protected*/ QWidget* TextAttrDialog::makeDoneButtonPanel() {
    QWidget* panel0 = new QWidget();
    panel0->setLayout(new /*FlowLayout()*/QHBoxLayout());
    QPushButton* doneButton = new QPushButton(tr("Done"));
//    doneButton.addActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent a) {
//                PositionablePopupUtil util = _decorator.getPositionablePopupUtil();
//                _decorator.getText(_pos);
//                if (_selectionGroup==NULL) {
//                    setAttributes(util, _pos, _decorator.isOpaque());
//                } else {
//                    setSelectionsAttributes(util, _pos, _decorator.isOpaque());
//                }
//                dispose();
//            }
//    });
    connect(doneButton, SIGNAL(clicked()), this, SLOT(doneButton_clicked()));
    panel0->layout()->addWidget(doneButton);

    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
//    cancelButton.addActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent a) {
//                dispose();
//            }
//    });
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    panel0->layout()->addWidget(cancelButton);
    return panel0;
}
//};
void TextAttrDialog::doneButton_clicked()
{
 PositionablePopupUtil* util = _decorator->getPositionablePopupUtil();
 _decorator->getText(_pos);
 if (editor->_selectionGroup==QList<Positionable*>())
 {
  editor->setAttributes(util, _pos, _decorator->isOpaque());
 }
 else
 {
  editor->setSelectionsAttributes(util, _pos, _decorator->isOpaque());
 }
 //dispose();
 editor->addToTarget(_pos);
 deleteLater();
}

/**
 * Called from setSelectionsAttributes - i.e. clone because maybe several Positionables use the data
 * @param newUtil
 * @param p
 * @param isOpaque
 */
/*protected*/ void Editor::setAttributes(PositionablePopupUtil* newUtil, Positionable* p, bool isOpaque) {
    ((PositionableLabel*)p)->setPopupUtility(newUtil->clone(p, ((PositionableLabel*)p)->getTextComponent()));
   ((PositionableLabel*)p)->setOpaque(isOpaque);
    //if (p instanceof PositionableLabel)
    if(qobject_cast<PositionableLabel*>(p)!= NULL)
    {
        PositionableLabel* pos = (PositionableLabel*)p;
        if (!pos->isText() || (pos->isText() && pos->isIcon())) {
            return;
        } else {
            int deg = pos->getDegrees();
            if (deg!=0) {
                pos->setOpaque(false);
                pos->saveOpaque(isOpaque);
                pos->rotate(0);
                int mar = newUtil->getMargin();
                int bor = newUtil->getBorderSize();
#if 0 // TODO:
                javax.swing.border.Border outlineBorder;
                if (bor==0) {
                    outlineBorder = BorderFactory.createEmptyBorder(0, 0, 0, 0);
                }else {
                    outlineBorder = new javax.swing.border.LineBorder(newUtil.getBorderColor(), bor);
                }
                javax.swing.border.Border borderMargin;
                if (isOpaque){
                    borderMargin = new javax.swing.border.LineBorder(pos.getBackground(), mar);
                } else{
                    borderMargin = BorderFactory.createEmptyBorder(mar, mar, mar, mar);
                }
                pos.setBorder(new javax.swing.border.CompoundBorder(outlineBorder, borderMargin));
#endif
                pos->setOpaque(isOpaque);
                pos->rotate(deg);
                if(pos->_itemGroup != NULL)
                {
//                 getTargetPanel()->invalidate(pos->_itemGroup->boundingRect());
                 addToTarget(pos);
                }
            }
        }
    }
    ((PositionableLabel*)p)->updateSize();
    ((PositionableLabel*)p)->repaint();
    //if (p instanceof PositionableIcon)
    if(qobject_cast<PositionableIcon*>(p)!=NULL)
    {
     NamedBean* bean = ((PositionableIcon*)p)->getNamedBean();
        if (bean!=NULL) {
            ((PositionableIcon*)p)->displayState(bean->getState());
        }
    }
}

/*protected*/ void Editor::setSelectionsAttributes(PositionablePopupUtil* util, Positionable* pos, bool isOpaque) {
    if (_selectionGroup!=QList<Positionable*>() && _selectionGroup.contains(pos)) {
        for (int i=0; i<_selectionGroup.size(); i++) {
            Positionable* p = _selectionGroup.at(i);
            //if ( p instanceof PositionableLabel )
            if(qobject_cast<PositionableLabel*>(p)!= NULL)
            {
                setAttributes(util, p, isOpaque);
            }
         }
    }
}

/*protected*/ void Editor::setSelectionsHidden(bool enabled, Positionable* p)

{
 if (_selectionGroup!=QList<Positionable*>() && _selectionGroup.contains(p))
 {
  for (int i=0; i<_selectionGroup.size(); i++) {
      ((PositionableLabel*)_selectionGroup.at(i))->setHidden(enabled);
  }
 }
}
#if 1
/*protected*/ bool Editor::setSelectionsPositionable(bool enabled, Positionable* p)
{
 if (_selectionGroup!=QList<Positionable*>() && _selectionGroup.contains(p))
 {
  for (int i=0; i<_selectionGroup.size(); i++)
  {
   _selectionGroup.at(i)->setPositionable(enabled);
  }
  return true;
 }
 else
 {
  return false;
 }
}
#endif
/*protected*/ void Editor::removeSelections(Positionable* p)
{
 if (_selectionGroup!=QList<Positionable*>() && _selectionGroup.contains(p))
 {
  for (int i=0; i<_selectionGroup.size(); i++)
  {
   Positionable* p = _selectionGroup.at(i);
   if(qobject_cast<PositionableLabel*>(p) != NULL)
    ((PositionableLabel*)p)->remove();
   if(qobject_cast<PositionableJComponent*>(p) != NULL)
   {
    removeFromTarget(p);
    ((PositionableJComponent*)p)->remove();
   }
  }
  _selectionGroup = QList<Positionable*>();
 }
}

/*protected*/ void Editor::setSelectionsScale(double s, Positionable* p)
{
 if (_selectionGroup!=QList<Positionable*>() && _selectionGroup.contains(p))
 {
  for (int i=0; i<_selectionGroup.size(); i++)
  {
   _selectionGroup.at(i)->setScale(s);
  }
 }
 else
 {
  p->setScale(s);
 }
}

/*protected*/ void Editor::setSelectionsRotation(int k, Positionable* p)
{
 if (_selectionGroup!=QList<Positionable*>() && _selectionGroup.contains(p))
 {
  for (int i=0; i<_selectionGroup.size(); i++)
  {
   ((PositionableLabel*) _selectionGroup.at(i))->rotate(k);
   //addToTarget(_selectionGroup.at(i));
   _selectionGroup.at(i)->updateScene();
  }
 }
 else
 {
  ((PositionableLabel*)p)->rotate(k);
  //addToTarget(p);
  p->updateScene();
 }
}

/*protected*/ void Editor::setSelectionsDockingLocation(Positionable* p)
{
 if (_selectionGroup!=QList<Positionable*>() && _selectionGroup.contains(p))
 {
  for (int i=0; i<_selectionGroup.size(); i++)
  {
   Positionable* pos = _selectionGroup.at(i);
   //if (pos instanceof LocoIcon)
   if(qobject_cast<LocoIcon*>(pos) != NULL)
   {
    ((LocoIcon*)pos)->setDockingLocation(((PositionableLabel*)pos)->getX(), ((PositionableLabel*)pos)->getY());
   }
  }
 }
 else
 //if (p instanceof LocoIcon)
 if(qobject_cast<LocoIcon*>(p) != NULL)
 {
  ((LocoIcon*)p)->setDockingLocation(((PositionableLabel*)p)->getX(), ((PositionableLabel*)p)->getY());
 }
}

/*protected*/ void Editor::dockSelections(Positionable* p)
{
 if (_selectionGroup!=QList<Positionable*>() && _selectionGroup.contains(p))
 {
  for (int i=0; i<_selectionGroup.size(); i++)
  {
   Positionable* pos = _selectionGroup.at(i);
   //if (pos instanceof LocoIcon)
   if(qobject_cast<LocoIcon*>(pos) != NULL)
   {
    ((LocoIcon*)pos)->dock();
   }
  }
 }
 //else if (p instanceof LocoIcon)
 else if(qobject_cast<LocoIcon*>(p) != NULL)
 {
  ((LocoIcon*)p)->dock();
 }
}

/*protected*/ bool Editor::showAlignPopup(Positionable* p) {
    if (_selectionGroup!=QList<Positionable*>() && _selectionGroup.contains(p)) {
        return true;
    } else {
        return false;
    }
}
#if 0
/*protected*/ void alignGroup(bool alignX, bool alignToFirstSelected) {
    if (_selectionGroup==NULL) {
        return;
    }
    int sum = 0;
    int cnt = 0;
    int ave = 0;

    for (int i=0; i<_selectionGroup.size(); i++) {
        Positionable comp = _selectionGroup.get(i);
        if (!getFlag(OPTION_POSITION, comp.isPositionable()))  { continue; }
        if (alignToFirstSelected) {
            if (alignX) {
                    ave = comp.getX();
                } else {
                    ave = comp.getY();
                }
                break;
            } else {
                if (alignX) {
                sum += comp.getX();
            } else {
                sum += comp.getY();
            }
        cnt++;
        }
    }

    if (!alignToFirstSelected) {
        ave = Math.round((float) sum / cnt);
    }

    for (int i=0; i<_selectionGroup.size(); i++) {
        Positionable comp = _selectionGroup.get(i);
        if (!getFlag(OPTION_POSITION, comp.isPositionable()))  { continue; }
        if (alignX) {
            comp.setLocation(ave, comp.getY());
        } else {
            comp.setLocation(comp.getX(), ave);
        }
    }
}

/*public*/ Rectangle getSelectRect() {
    return _selectRect;
}
#endif
/*public*/ void Editor::drawSelectRect(int x, int y)
{
    if(editScene && _selectRectItemGroup != NULL && _selectRectItemGroup->scene() != 0)
     editScene->removeItem(_selectRectItemGroup);

    int aX = getAnchorX();
    int aY = getAnchorY();
    int w = x - aX;
    int h = y - aY;
    if (x < aX) {
        aX = x;
        w = -w;
    }
    if (y < aY) {
        aY = y;
        h = -h;
    }
    _selectRect = QRect(aX, aY, w, h);
    QGraphicsRectItem* selectRectItem = new QGraphicsRectItem(_selectRect);
    selectRectItem->setPos(aX, aY);
    selectRectItem->setPen(QPen(QBrush(QColor(_selectRectColor)),1,Qt::DashLine));
    _selectRectItemGroup->addToGroup(selectRectItem);
    editScene->addItem(_selectRectItemGroup);
}

/*public*/ /*const*/ int Editor::getAnchorX() {
    return _anchorX;
}

/*public*/ /*const*/ int Editor::getAnchorY() {
    return _anchorY;
}
#if 0
/*public*/ void keyTyped(KeyEvent e) {
}

/*public*/ void keyPressed(KeyEvent e) {
    if (_selectionGroup==NULL) return;
    int x = 0;
    int y = 0;
    switch (e.getKeyCode()){
        case KeyEvent.VK_UP: y=-1;
                            break;
        case KeyEvent.VK_DOWN: y=1;
                            break;
        case KeyEvent.VK_LEFT: x=-1;
                            break;
        case KeyEvent.VK_RIGHT: x=1;
                                break;
    }
    //A cheat if the shift key isn't pressed then we move 5 pixels at a time.
    if(!e.isShiftDown()){
        y=y*5;
        x=x*5;
    }
    for (int i=0; i<_selectionGroup.size(); i++) {
        moveItem(_selectionGroup.get(i), x, y);
    }
    _targetPanel.repaint();
}

/*public*/ void keyReleased(KeyEvent e) {
}

/*********************** Abstract Methods ************************/

abstract /*public*/ void mousePressed(MouseEvent event);

abstract /*public*/ void mouseReleased(MouseEvent event);

abstract /*public*/ void mouseClicked(MouseEvent event);

abstract /*public*/ void mouseDragged(MouseEvent event);

abstract /*public*/ void mouseMoved(MouseEvent event);

abstract /*public*/ void mouseEntered(MouseEvent event);

abstract /*public*/ void mouseExited(MouseEvent event);

/*
 * set up target panel, frame etc.
 */
abstract /*protected*/ void init(QString name);
/*
 * Closing of Target frame window.
 */
abstract /*protected*/ void targetWindowClosingEvent(java.awt.event.WindowEvent e);
/**
 * Called from TargetPanel's paint method for additional drawing by editor view
 */
abstract /*protected*/ void paintTargetPanel(Graphics g);
/**
 * Set an object's location when it is created.
 */
abstract /*protected*/ void setNextLocation(Positionable obj);
/**
 * Editor Views should make calls to this class (Editor) to set popup menu items.
 * See 'Popup Item Methods' above for the calls.
 */
abstract /*protected*/ void showPopUp(Positionable p, MouseEvent event);
/**
 * After construction, initialize all the widgets to their saved config settings.
 */
abstract /*public*/ void initView();

/**
* set up item(s) to be copied by paste
*/
abstract /*protected*/ void copyItem(Positionable p);

//    // initialize logging
//    static org.apache.log4j.Logger log = org.apache.log4j.Logger.getLogger(Editor.class.getName());
//}
#endif
/*public*/ void Editor::setHighlightColor(QColor color) {
     _highlightColor = color;
}
/*public*/ void Editor::setSelectGroupColor(QColor color) {
    _selectGroupColor = color;
}
/*public*/ void Editor::setSelectRectColor(QColor color) {
    _selectRectColor = color;
}
/*public*/ void Editor::setSelectRectStroke(Qt::PenStyle stroke) {
    _selectRectStroke = stroke;
}
/*public*/ void Editor::setDefaultColors()
{
    _highlightColor = QColor(204, 207, 88);
    _selectGroupColor = QColor(204, 207, 88);
    _selectRectColor = QColor(Qt::red);
    _selectRectStroke = Qt::DashLine;
}

/*public*/ void Editor::paint(QGraphicsScene* g2d) {
    //Graphics2D g2d = (Graphics2D)g;
//    g2d.scale(_paintScale, _paintScale);
//    super.paint(g);
//    paintTargetPanel(g);
//    java.awt.Stroke stroke = g2d.getStroke();
//    Color color = g2d.getColor();
    if(_selectRectItemGroup != NULL && _selectRectItemGroup->scene() != 0)
    {
     g2d->removeItem(_selectRectItemGroup);
     //_selectRectItemGroup = NULL;
     _selectRectItemGroup = new QGraphicsItemGroup();
    }
    if (!_selectRect.isNull())
    {
     //Draw a rectangle on top of the image.
//     g2d.setStroke(_selectRectStroke);
//     g2d.setColor(_selectRectColor);
//     g.drawRect(_selectRect.x, _selectRect.y, _selectRect.width, _selectRect.height);
     QGraphicsRectItem* selectRectItem = new QGraphicsRectItem(_selectRect);
     selectRectItem->setPen(QPen(QBrush(_selectRectColor),1,_selectRectStroke));
     _selectRectItemGroup->addToGroup(selectRectItem);
    }
    if (_selectionGroup!=QList<Positionable*>())
    {
//        g2d.setColor(_selectGroupColor);
//        g2d.setStroke(new java.awt.BasicStroke(2.0f));
     for(int i=0; i<_selectionGroup.size();i++)
     {
//       g.drawRect(_selectionGroup.get(i).getX(), _selectionGroup.get(i).getY(),
//                           _selectionGroup.get(i).maxWidth(), _selectionGroup.get(i).maxHeight());
      QGraphicsRectItem* selectRectItem = new QGraphicsRectItem(_selectRect);
      selectRectItem->setPen(QPen(QBrush(_selectGroupColor),1,Qt::SolidLine));
      _selectRectItemGroup->addToGroup(selectRectItem);
     }
    }
    //Draws a border around the highlighted component
    if (!_highlightcomponent.isNull())
    {
//        g2d.setColor(_highlightColor);
//        g2d.setStroke(new java.awt.BasicStroke(2.0f));
//        g.drawRect(_highlightcomponent.x, _highlightcomponent.y,
//                   _highlightcomponent.width, _highlightcomponent.height);
     QGraphicsRectItem* selectRectItem = new QGraphicsRectItem(_highlightcomponent);
     selectRectItem->setPen(QPen(QBrush(_highlightColor),1,Qt::SolidLine));
     _selectRectItemGroup->addToGroup(selectRectItem);
    }
//    g2d.setColor(color);
//    g2d.setStroke(stroke);
//    if (_tooltip != null) {
//        _tooltip.paint(g2d, _paintScale);
//    }
 g2d->addItem(_selectRectItemGroup);
}
void Editor::setName(QString name)
{
 this->name = name;
 setWindowTitle(name);
}
QString Editor::getName() { return name;}
QString Editor::getTitle() { return name;}
void Editor::repaint()
{
 // TODO:
}
/*********************** Icon Editors utils ****************************/

///*public*/ static class JFrameItem : public JmriJFrame {
//    IconAdder* _editor;
JFrameItem::JFrameItem (QString name, IconAdder* editor, QWidget* parent): JmriJFrame(name, parent)
{
 //super(name);
 _editor = editor;
 //setName(name);
 setContentsMargins(0,0,0,0);
}
/*public*/ IconAdder* JFrameItem::getEditor() {
    return _editor;
}
/*public*/ QString JFrameItem::toString() {
    return this->title();
}
//};
/*public*/ QRectF Editor::getSelectRect() {
    return _selectRect;
}
/*public*/ void Editor::propertyChange(PropertyChangeEvent *)
{
 if (rosterBox->getSelectedRosterEntries()->length() != 0)
 {
  selectLoco(rosterBox->getSelectedRosterEntries()->at(0));
 }
}
/*
 * set up target panel, frame etc.
 */
/*abstract*/ /*protected*/ void Editor::init(QString name) {}
