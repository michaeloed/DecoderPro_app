#ifndef LINKINGOBJECT_H
#define LINKINGOBJECT_H
#include <QObject>
#include <QMenu>

/**
 * Adds capability for a object to load a browser page or bring a panel frame to
 * the top.
 *
 * Implementations must include code to do this:
 *
 * if (url.startsWith("frame:")) { // locate JmriJFrame and push to front String
 * frame = url.substring(6); final jmri.util.JmriJFrame jframe =
 * jmri.util.JmriJFrame.getFrame(frame); java.awt.EventQueue.invokeLater(new
 * Runnable() { &#64;Override public void run() { jframe.toFront();
 * jframe.repaint(); } }); } else {
 * jmri.util.ExternalLinkContentViewerUI.activateURL(new java.net.URL(url)); }
 * This code typically is done within the following Positionable call
 *
 * public void doMouseClicked(MouseEvent event);
 *
 *
 ** Positionable object may use the following call to edit the "url" string
 *
 * public boolean setLinkMenu(JPopupMenu popup) { // perhaps check if "url" is
 * OK or just this popup.add(CoordinateEdit.getLinkEditAction(this,
 * "EditLink")); return true; }
 *
 * See LinkingLabel.java or AnalogClock2Display.java for examples
 */
/*public*/ /*interface*/class LinkingObject //: public QObject//extends Cloneable
{
 //Q_OBJECT
public:
    /*public*/ virtual QString getURL() {return "";}

    /*public*/ virtual void setULRL(QString /*u*/) {}

    /*public*/ virtual void updateSize() {}

 /*public*/ virtual bool setLinkMenu(QMenu* /*popup*/) {return false;}
//    /*public*/ void doMouseClicked(MouseEvent event);
};
Q_DECLARE_INTERFACE(LinkingObject, "LinkingObject")
#endif // LINKINGOBJECT_H
