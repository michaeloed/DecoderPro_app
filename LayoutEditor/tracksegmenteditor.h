#ifndef TRACKSEGMENTEDITOR_H
#define TRACKSEGMENTEDITOR_H
#include "layouttrackeditor.h"
#include "jcheckbox.h"
#include "tracksegment.h"
#include "jcombobox.h"
#include "namedbeancombobox.h"
#include "jtextfield.h"
#include "instancemanager.h"
#include "liblayouteditor_global.h"
#include "windowlistener.h"
#include "actionlistener.h"
#include "jmrijframe.h"
#include <QPushButton>
#include "layouteditor.h"

//class TrackSegment;
class LIBLAYOUTEDITORSHARED_EXPORT TrackSegmentEditor : public LayoutTrackEditor
{
  Q_OBJECT
 public:
  /*public*/ TrackSegmentEditor(/*@Nonnull*/ LayoutEditor* layoutEditor);
  /*public*/ void editLayoutTrack(/*@Nonnull*/ LayoutTrack* layoutTrack) override;

 private:
  static Logger* log;
  // variables for Edit Track Segment pane
  /*private*/ TrackSegment* trackSegment = nullptr;

  /*private*/ JmriJFrame* editTrackSegmentFrame = nullptr;
  /*private*/ /*final*/ JComboBox/*<String>*/* editTrackSegmentMainlineComboBox = new JComboBox();
  /*private*/ /*final*/ JComboBox/*<String>*/* editTrackSegmentDashedComboBox = new JComboBox();
  /*private*/ /*final*/ JCheckBox* editTrackSegmentHiddenCheckBox = new JCheckBox(tr("Hide Track"));  // NOI18N
  /*private*/ /*final*/ NamedBeanComboBox/*<Block>*/* editTrackSegmentBlockNameComboBox = new NamedBeanComboBox(
          (BlockManager*)InstanceManager::getDefault("BlockManager"), nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
  /*private*/ /*final*/ JTextField* editTrackSegmentArcTextField = new JTextField(5);
  /*private*/ QPushButton* editTrackSegmentSegmentEditBlockButton;

  /*private*/ int editTrackSegmentMainlineTrackIndex;
  /*private*/ int editTrackSegmentSideTrackIndex;
  /*private*/ int editTrackSegmentDashedIndex;
  /*private*/ int editTrackSegmentSolidIndex;
  /*private*/ bool editTrackSegmentOpen = false;
  /*private*/ bool editTrackSegmentNeedsRedraw = false;

 private slots:
  /*private*/ void editTrackSegmentEditBlockPressed(/*ActionEvent a*/);
  /*private*/ void editTracksegmentDonePressed(/*ActionEvent a*/);
  /*private*/ void editTrackSegmentCancelPressed(/*ActionEvent a*/);


  friend class TSE_editTracksegmentDonePressed;
  friend class TSE_editTracksegmentCancelPressed;
  friend class TSE_windowListener;
};

class TSE_editTracksegmentDonePressed : public ActionListener
{
  Q_OBJECT
  TrackSegmentEditor* editor;
 public:
  TSE_editTracksegmentDonePressed(TrackSegmentEditor* editor) {this->editor = editor;}
  void actionPerformed()
  {
   editor->editTracksegmentDonePressed();
  }
};

class TSE_editTracksegmentCancelPressed : public ActionListener
{
  Q_OBJECT
  TrackSegmentEditor* editor;
 public:
  TSE_editTracksegmentCancelPressed(TrackSegmentEditor* editor) {this->editor = editor;}
  void actionPerformed()
  {
   editor->editTrackSegmentCancelPressed();
  }
};

class TSE_windowListener : public WindowListener
{
  Q_OBJECT
  TrackSegmentEditor* editor;
 public:
  TSE_windowListener(TrackSegmentEditor* editor) { this->editor = editor;}
  /*public*/ void windowClosing(QCloseEvent* e) {
      editor->editTrackSegmentCancelPressed();
  }
};
#endif // TRACKSEGMENTEDITOR_H
