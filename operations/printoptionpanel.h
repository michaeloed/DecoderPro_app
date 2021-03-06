#ifndef PRINTOPTIONPANEL_H
#define PRINTOPTIONPANEL_H

#include "operationspreferencespanel.h"

class QScrollArea;
class QFontComboBox;
class QGroupBox;
class File;
class JFileChooser;
class JTextField;
class QLabel;
namespace Operations
{
 class PrintOptionPanel : public OperationsPreferencesPanel
 {
  Q_OBJECT
 public:
  PrintOptionPanel(QWidget* parent = 0);
  /*public*/ QString getTabbedPreferencesTitle();
  /*public*/ QString getPreferencesTooltip();
  /*public*/ void savePreferences();
  /*public*/ bool isDirty();

 public slots:
  /*public*/ void buttonActionPerformed(QWidget* ae);
  /*public*/ void checkBoxActionPerformed(QWidget* ae);
  /*public*/ void comboBoxActionPerformed(QWidget* ae);

 private:
   Logger* log;
  // labels
  QLabel* logoURL;//= new JLabel("");

  // major buttons
  QPushButton* saveButton;//= new JButton(Bundle.getMessage("Save"));
  QPushButton* addLogoButton;//= new JButton(Bundle.getMessage("AddLogo"));
  QPushButton* removeLogoButton;//= new JButton(Bundle.getMessage("RemoveLogo"));

  QPushButton* addEngPickupComboboxButton;//= new JButton("+");
  QPushButton* deleteEngPickupComboboxButton;//= new JButton("-");
  QPushButton* addEngDropComboboxButton;//= new JButton("+");
  QPushButton* deleteEngDropComboboxButton;//= new JButton("-");
  QPushButton* addCarPickupComboboxButton;//= new JButton("+");
  QPushButton* deleteCarPickupComboboxButton;//= new JButton("-");
  QPushButton* addCarDropComboboxButton;//= new JButton("+");
  QPushButton* deleteCarDropComboboxButton;//= new JButton("-");
  QPushButton* addLocalComboboxButton;//= new JButton("+");
  QPushButton* deleteLocalComboboxButton;//= new JButton("-");
  QPushButton* addSwitchListPickupComboboxButton;//= new JButton("+");
  QPushButton* deleteSwitchListPickupComboboxButton;//= new JButton("-");
  QPushButton* addSwitchListDropComboboxButton;//= new JButton("+");
  QPushButton* deleteSwitchListDropComboboxButton;//= new JButton("-");
  QPushButton* addSwitchListLocalComboboxButton;//= new JButton("+");
  QPushButton* deleteSwitchListLocalComboboxButton;//= new JButton("-");

  // check boxes
  QCheckBox* tabFormatCheckBox;//= new JCheckBox(Bundle.getMessage("TabFormat"));
  QCheckBox* formatSwitchListCheckBox;//= new JCheckBox(Bundle.getMessage("SameAsManifest"));
  QCheckBox* editManifestCheckBox;//= new JCheckBox(Bundle.getMessage("UseTextEditor"));
  QCheckBox* printLocCommentsCheckBox;//= new JCheckBox(Bundle.getMessage("PrintLocationComments"));
  QCheckBox* printRouteCommentsCheckBox;//= new JCheckBox(Bundle.getMessage("PrintRouteComments"));
  QCheckBox* printLoadsEmptiesCheckBox;//= new JCheckBox(Bundle.getMessage("PrintLoadsEmpties"));
  QCheckBox* printTimetableNameCheckBox;//= new JCheckBox(Bundle.getMessage("PrintTimetableName"));
  QCheckBox* use12hrFormatCheckBox;//= new JCheckBox(Bundle.getMessage("12hrFormat"));
  QCheckBox* printValidCheckBox;//= new JCheckBox(Bundle.getMessage("PrintValid"));
  QCheckBox* sortByTrackCheckBox;//= new JCheckBox(Bundle.getMessage("SortByTrack"));
  QCheckBox* printHeadersCheckBox;//= new JCheckBox(Bundle.getMessage("PrintHeaders"));
  QCheckBox* truncateCheckBox;//= new JCheckBox(Bundle.getMessage("Truncate"));
  QCheckBox* departureTimeCheckBox;//= new JCheckBox(Bundle.getMessage("DepartureTime"));
  QCheckBox* trackSummaryCheckBox;//= new JCheckBox(Bundle.getMessage("TrackSummary"));
  QCheckBox* routeLocationCheckBox;//= new JCheckBox(Bundle.getMessage("RouteLocation"));

  // text field
  JTextField* pickupEngPrefix;//= new JTextField(10);
  JTextField* dropEngPrefix;//= new JTextField(10);
  JTextField* pickupCarPrefix;//= new JTextField(10);
  JTextField* dropCarPrefix;//= new JTextField(10);
  JTextField* localPrefix;//= new JTextField(10);
  JTextField* switchListPickupCarPrefix;//= new JTextField(10);
  JTextField* switchListDropCarPrefix;//= new JTextField(10);
  JTextField* switchListLocalPrefix;//= new JTextField(10);
  JTextField* hazardousTextField;//= new JTextField(20);

  // text area
  HtmlTextEdit* commentTextArea;//= new JTextArea(2, 90);

  QScrollArea* commentScroller;//= new JScrollPane(commentTextArea,
//          JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

  // combo boxes
  QFontComboBox* fontComboBox;//= new JComboBox<>();
  QComboBox* manifestFormatComboBox;//= Setup.getManifestFormatComboBox();
  QComboBox* manifestOrientationComboBox;//= Setup.getOrientationComboBox();
  QComboBox* fontSizeComboBox;//= new JComboBox<>();
  QComboBox* pickupComboBox;//= Setup.getPrintColorComboBox(); // colors
  QComboBox* dropComboBox;//= Setup.getPrintColorComboBox();
  QComboBox* localComboBox;//= Setup.getPrintColorComboBox();
  QComboBox* switchListOrientationComboBox;//= Setup.getOrientationComboBox();

  // message formats
  QList<QComboBox*> enginePickupMessageList;//= new ArrayList<>();
  QList<QComboBox*> engineDropMessageList;//= new ArrayList<>();
  QList<QComboBox*> carPickupMessageList;//= new ArrayList<>();
  QList<QComboBox*> carDropMessageList;//= new ArrayList<>();
  QList<QComboBox*> localMessageList;//= new ArrayList<>();
  QList<QComboBox*> switchListCarPickupMessageList;//= new ArrayList<>();
  QList<QComboBox*> switchListCarDropMessageList;//= new ArrayList<>();
  QList<QComboBox*> switchListLocalMessageList;//= new ArrayList<>();

  // manifest panels
  QWidget* pManifest;//= new JPanel();
  QGroupBox* pEngPickup;//= new JPanel();
  QGroupBox* pEngDrop;//= new JPanel();
  QGroupBox* pPickup;//= new JPanel();
  QGroupBox* pDrop;//= new JPanel();
  QGroupBox* pLocal;//= new JPanel();

  // switch list panels
  QGroupBox* pSwitchListOrientation;//= new JPanel();
  QGroupBox* pSwPickup;//= new JPanel();
  QGroupBox* pSwDrop;//= new JPanel();
  QGroupBox* pSwLocal;//= new JPanel();
  /*private*/ void loadFontComboBox();
  /*private*/ void setSwitchListVisible(bool b);
  /**
   * We always use the same file chooser in this class, so that the user's
   * last-accessed directory remains available.
   */
  JFileChooser* fc;// = XmlFile::userFileChooser(tr("Images"));
  /*private*/ File* selectFile();
  /*private*/ void updateLogoButtons();
  /*private*/ void loadFormatComboBox();
  /*private*/ void addComboBox(QWidget* panel, QList<QComboBox*> list, QComboBox* box);
  /*private*/ void removeComboBox(QWidget* panel, QList<QComboBox*>list);

 };
}
#endif // PRINTOPTIONPANEL_H
