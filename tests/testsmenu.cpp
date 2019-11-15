#include "testsmenu.h"
#include "RosterTest/rostertestaction.h"
#include "RosterTest/rosterentrytestaction.h"
#include "RosterTest/rosterentrypanetestaction.h"
#include "ProfileTest/profiletestaction.h"
#include "ProfileTest/profilemanagertestaction.h"
#include "loconet/slotmanagertestaction.h"
#include "loconet/loconetslottestaction.h"
#include "loconet/lnopsmodeprogrammertestaction.h"
#include "loconet/loconetmessagetestaction.h"
#include "loconet/lntrafficcontrollertestaction.h"
#include "sprog/sprogmessagetestaction.h"
#include "loconet/lnportcontrollertestaction.h"
#include "loconet/lnpacketizertestaction.h"
#include "ProfileTest/profileutilstestaction.h"
#include "loconet/lntrafficroutertestaction.h"
#include "loconet/lnpowermanagertestaction.h"
#include "loconet/lnturnouttestaction.h"
#include "loconet/lnsensortestaction.h"
#include "loconet/lnreportertestaction.h"
#include "loconet/lnsensoraddresstestaction.h"
#include "loconet/lnsensormanagertestaction.h"
#include "loconet/lndeferprogrammertestaction.h"
#include "loconet/lnturnoutmanagertestaction.h"
#include "misc/proxyturnoutmanagertestaction.h"
#include "loconet/lnlighttestaction.h"
#include "misc/proxysensormanagertestaction.h"
#include "misc/proxyreportmanagertestaction.h"
#include "misc/proxylightmanagertestaction.h"
#include "misc/multiindexprogrammerfacadetestaction.h"
#include "misc/addressedhighcvprogranmmerfacadetestaction.h"
#include "misc/opsmodedelayedprogrammerfacadetestaction.h"
#include "misc/verifywriteprogrammerfacadetestaction.h"
#include "misc/twoindextcsprogrammerfacadetestaction.h"
#include "misc/paneprogpanetestaction.h"
#include "misc/paneprogframetestaction.h"
#include "misc/decoderfiletestaction.h"
#include "loconet/loconetmessageinterprettestaction.h"
#include "loconet/lnreportermanagertestaction.h"
#include "loconet/loconetsystemconnectionmemotestaction.h"
#include "misc/defaultmemorymanagertestaction.h"
#include "misc/lightcontroltestaction.h"
#include "NamedBeans/namedbeanusernamecomparatortestaction.h"
#include "loconet/loconetmessageexceptiontestaction.h"
#include "loconet/loconetthrottletestaction.h"
#include "misc/nmrapackettestaction.h"
#include "misc/accessoryopsmodeprogrammerfacadetestaction.h"
#include "signals/dccsignalheadtestaction.h"
#include "signals/singleturnoutsignalheadtestaction.h"
#include "signals/doubleturnoutsignalheadtestaction.h"
#include "signals/virtualsignalheadtestaction.h"
#include "signals/signalheadsignalmasttestaction.h"
#include "signals/signalheadsignalmastaddpanetestaction.h"
#include "signals/matrixsignalmasttestaction.h"
#include "signals/matrixsignalmastaddpanetestaction.h"
#include "signals/dccsignalmastaddpanetestaction.h"
#include "signals/tripleoutputsignalheadtestaction.h"
#include "signals/tripleturnoutsignalheadtestaction.h"
#include "signals/virtualsignalmastaddpanetestaction.h"
#include "signals/turnoutsignalmastaddpanetestaction.h"
#include "signals/turnoutsignalmasttestaction.h"


TestsMenu::TestsMenu(QWidget *parent) : QMenu(parent)
{
    setTitle(tr("Tests"));
    QMenu* rosterTestMenu = new QMenu(tr("Roster Tests"));
    addMenu(rosterTestMenu);
    rosterTestMenu->addAction(new RosterTestAction("Roster Test", this));
    rosterTestMenu->addAction(new RosterEntryTestAction("Roster Entry Test", this));
    rosterTestMenu->addAction(new RosterEntryPaneTestAction("Roster Entry Pane Test", this));

    QMenu* profileTestMenu = new QMenu(tr("Profile Tests"));
    addMenu(profileTestMenu);
    profileTestMenu->addAction(new ProfileTestAction(this));
    profileTestMenu->addAction(new ProfileManagerTestAction(this));
    profileTestMenu->addAction(new ProfileUtilsTestAction(this));

    QMenu* loconetTestMenu = new QMenu(tr("LocoNet Tests ..."));
    addMenu(loconetTestMenu);
    loconetTestMenu->addAction(new SlotManagerTestAction(this));
    loconetTestMenu->addAction(new LocoNetSlotTestAction(this));
    loconetTestMenu->addAction(new LoconetSystemConnectionMemoTestAction(this));
    loconetTestMenu->addAction(new DefaultMemoryManagerTestAction(this));
    loconetTestMenu->addAction(new LocoNetMessageExceptionTestAction(this));
    QMenu* loconetProgrammersMenu = new QMenu(tr("Programmers ..."));
    loconetTestMenu->addMenu(loconetProgrammersMenu);
    loconetProgrammersMenu->addAction(new LnOpsModeProgrammerTestAction(this));
    loconetProgrammersMenu->addAction(new LnDeferProgrammerTestAction(this));
    loconetTestMenu->addAction(new LocoNetMessageTestAction(this));
    loconetTestMenu->addAction(new LnTrafficControllerTestAction(this));
    loconetTestMenu->addAction(new LnPortControllerTestAction(this));
    loconetTestMenu->addAction(new LnPacketizerTestAction(this));
    loconetTestMenu->addAction(new LnPowerManagerTestAction(this));
    QMenu* loconetLightMenu = new QMenu(tr("Lights ..."));
    loconetTestMenu->addMenu(loconetLightMenu);
    loconetLightMenu->addAction(new LnLightTestAction(this));
    loconetLightMenu->addAction(new LightControlTestAction(this));
    QMenu* loconetSensorsMenu = new QMenu(tr("Sensors ..."));
    loconetTestMenu->addMenu(loconetSensorsMenu);
    loconetSensorsMenu->addAction(new LnSensorTestAction(this));
    QMenu* loconetReporterMenu = new QMenu(tr("Reporters"));
    loconetTestMenu->addMenu(loconetReporterMenu);
    loconetReporterMenu->addAction(new LnReporterTestAction(this));
    loconetReporterMenu->addAction(new LnReporterManagerTestAction(this));
    loconetSensorsMenu->addAction(new LnSensorAddressTestAction(this));
    loconetSensorsMenu->addAction(new LnSensorManagerTestAction(this));
    QMenu* loconetTurnoutsMenu = new QMenu(tr("Turnouts ..."));
    loconetTestMenu->addMenu(loconetTurnoutsMenu);
    loconetTurnoutsMenu->addAction(new LnTurnoutTestAction(this));
    loconetTurnoutsMenu->addAction(new LnTurnoutManagerTestAction(this));
    loconetTestMenu->addAction(new LocoNetMessageInterpretTestAction(this));

    QMenu* sprogTestMenu = new QMenu(tr("Sprog"));
    addMenu(sprogTestMenu);
    sprogTestMenu->addAction(new SprogMessageTestAction(this));
    QMenu* proxyMenu = new QMenu(tr("Proxy Managers ..."));
    addMenu(proxyMenu);
    proxyMenu->addAction(new ProxyTurnoutManagerTestAction(this));
    proxyMenu->addAction(new ProxySensorManagerTestAction(this));
    proxyMenu->addAction(new ProxyReporterManagerTestAction(this));
    proxyMenu->addAction(new ProxyLightManagerTestAction(this));
    QMenu* facadeMenu = new QMenu(tr("Programmer facades ..."));
    addMenu(facadeMenu);
    facadeMenu->addAction(new AccessoryOpsModeProgrammerFacadeTestAction(this));
    facadeMenu->addAction(new AddressedHighCvProgranmmerFacadeTestAction(this));
    facadeMenu->addAction(new MultiIndexProgrammerFacadeTestAction(this));
    facadeMenu->addAction(new OpsModeDelayedProgrammerFacadeTestAction(this));
    facadeMenu->addAction(new TwoIndexTcsProgrammerFacadeTestAction(this));
    facadeMenu->addAction(new VerifyWriteProgrammerFacadeTestAction(this));
    QMenu* panelsMenu = new QMenu(tr("Panels ..."));
    addMenu(panelsMenu);
    panelsMenu->addAction(new PaneProgFrameTestAction(this));
    panelsMenu->addAction(new PaneProgPaneTestAction(this));
    panelsMenu->addAction(new DecoderFileTestAction(this));
    QMenu* namedBeansMenu = new QMenu(tr("NamedBeans ..."));
    addMenu(namedBeansMenu);
    namedBeansMenu->addAction(new NamedBeanUserNameComparatorTestAction(this));
    QMenu* loconetThrottleMenu = new QMenu(tr("Throttle test"));
    loconetTestMenu->addMenu(loconetThrottleMenu);
    loconetThrottleMenu->addAction(new LocoNetThrottleTestAction(this));
    loconetThrottleMenu->addAction(new NmraPacketTestAction(this));
    QMenu* signalMenu = new QMenu(tr("Signals ..."));
    addMenu(signalMenu);
    signalMenu->addAction(new DccSignalHeadTestAction(this));
    signalMenu->addAction(new DccSignalMastAddPaneTestAction(this));
    signalMenu->addAction(new DoubleTurnoutSignalHeadTestAction(this));
    signalMenu->addAction(new MatrixSignalMastTestAction(this));
    signalMenu->addAction(new MatrixSignalMastAddPaneTestAction(this));
    signalMenu->addAction(new SingleTurnoutSignalHeadTestAction(this));
    signalMenu->addAction(new SignalHeadSignalMastTestAction(this));
    signalMenu->addAction(new SignalHeadSignalMastAddPaneTestAction(this));
    signalMenu->addAction(new TripleOutputSignalHeadTestAction(this));
    signalMenu->addAction(new TripleTurnoutSignalHeadTestAction(this));
    signalMenu->addAction(new TurnoutSignalMastTestAction(this));
    signalMenu->addAction(new TurnoutSignalMastAddPaneTestAction(this));
    signalMenu->addAction(new VirtualSignalHeadTestAction(this));

}