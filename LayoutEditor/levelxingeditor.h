#ifndef LEVELXINGEDITOR_H
#define LEVELXINGEDITOR_H
#include "layoutturntableeditor.h"

class LevelXing;
class LevelXingEditor : public LayoutTurntableEditor
{
  Q_OBJECT
 public:
  /*public*/ LevelXingEditor(/*@Nonnull*/ LayoutEditor* layoutEditor);
  /*public*/ void editLayoutTrack(LevelXing* levelXing);


 private:
  // variables for Edit Track Segment pane
  /*private*/ LevelXing* levelXing;

  // variables for Edit Level Crossing pane
  /*private*/ JmriJFrame* editLevelXingFrame = nullptr;
  /*private*/ /*final*/ JCheckBox* editLevelXingHiddenCheckBox = new JCheckBox(tr("Hide Crossing"));  // NOI18N

  /*private*/ /*final*/ NamedBeanComboBox/*<Block>*/* editLevelXingBlock1NameComboBox = new NamedBeanComboBox(
          (BlockManager*)InstanceManager::getDefault("BlockManager"), nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
  /*private*/ /*final*/ NamedBeanComboBox/*<Block>*/* editLevelXingBlock2NameComboBox = new NamedBeanComboBox(
          (BlockManager*)InstanceManager::getDefault("BlockManager"), nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
  /*private*/ QPushButton *editLevelXingBlock1Button;
  /*private*/ QPushButton* editLevelXingBlock2Button;

  /*private*/ bool editLevelXingOpen = false;
  /*private*/ bool editLevelXingNeedsRedraw = false;
  /*private*/ bool editLevelXingNeedsBlockUpdate = false;

 private slots:
  /*private*/ void editLevelXingBlockACPressed(/*ActionEvent a*/);
  /*private*/ void editLevelXingBlockBDPressed(/*ActionEvent a*/);
  /*private*/ void editLevelXingDonePressed(/*ActionEvent a*/);
  /*private*/ void editLevelXingCancelPressed(/*ActionEvent a*/);


  friend class ELXWindowListener;
  friend class LXEeditLevelXingDonePressed;
  friend class LXEeditLevelXingCancelPressed;
};
class ELXWindowListener : public WindowListener
{
  Q_OBJECT
  LevelXingEditor* editor;
 public:
  ELXWindowListener(LevelXingEditor* editor) {this->editor = editor;}
  /*public*/ void windowClosing(QCloseEvent* e) {
      editor->editLevelXingCancelPressed();
  }

};
class LXEeditLevelXingDonePressed : public ActionListener
{
  Q_OBJECT
  LevelXingEditor* editor;
 public:
  LXEeditLevelXingDonePressed(LevelXingEditor* editor) {this->editor = editor;}
  void actionPerformed()
  {
  editor->editLevelXingDonePressed();
 }
};
class LXEeditLevelXingCancelPressed : public ActionListener
{
  Q_OBJECT
  LevelXingEditor* editor;
 public:
  LXEeditLevelXingCancelPressed(LevelXingEditor* editor) {this->editor = editor;}
  void actionPerformed()
  {
  editor->editLevelXingCancelPressed();
 }
};

#endif // LEVELXINGEDITOR_H
