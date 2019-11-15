#ifndef ALTERNATETRACKFRAME_H
#define ALTERNATETRACKFRAME_H

#include "operationsframe.h"
class ActionEvent;
namespace Operations
{
 class Track;
 class TrackEditFrame;
 class AlternateTrackFrame : public OperationsFrame
 {
  Q_OBJECT
 public:
  AlternateTrackFrame(TrackEditFrame* tef, QWidget* parent = 0);
 public slots:
  /*public*/ void buttonActionPerformed(ActionEvent* ae = 0);
 private:
  Logger* log;
  // combo boxes
  QComboBox* trackBox;// = new JComboBox<>();

  // radio buttons
  // major buttons
  QPushButton *saveButton;// = new JButton(Bundle.getMessage("Save"));

  Track* _track;

 };
}
#endif // ALTERNATETRACKFRAME_H