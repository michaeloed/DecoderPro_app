#include "throttleslistpanel.h"

//ThrottlesListPanel::ThrottlesListPanel(QWidget *parent) :
//    QWidget(parent)
//{
//}
// /*public*/ class ThrottlesListPanel extends JPanel {


/*public*/ ThrottlesListPanel::ThrottlesListPanel(QWidget *parent) {
        //super();
//        throttleFramesLM = new ThrottlesTableModel();
//        initGUI();
}
#if 0
    /*public*/ ThrottlesTableModel getTableModel() {
        return throttleFramesLM;
    }

/*private*/ void initGUI() {
        throttleFrames = new JTable(throttleFramesLM);
        throttleFrames.setSelectionMode(ListSelectionModel.SINGLE_SELECTION );
        throttleFrames.setRowHeight(ThrottlesTableCellRenderer.height);
        throttleFrames.setTableHeader(null);
        throttleFrames.setDefaultRenderer(Object.class, new ThrottlesTableCellRenderer());
        throttleFrames.addMouseListener( new MouseListener() {
        /*public*/ void mouseClicked(MouseEvent e) {
                    int row=throttleFrames.rowAtPoint(e.getPoint());
                    throttleFrames.getSelectionModel().setSelectionInterval(row, row);
            ((ThrottleFrame)throttleFramesLM.getValueAt(row, 0)).toFront();
        }
        /*public*/ void mouseEntered(MouseEvent arg0) {}
        /*public*/ void mouseExited(MouseEvent arg0) {}
        /*public*/ void mousePressed(MouseEvent arg0) {}
        /*public*/ void mouseReleased(MouseEvent arg0) {}
    });

    JScrollPane scrollPane1 = new JScrollPane(throttleFrames);
    setLayout(new BorderLayout());
    setPreferredSize(new Dimension(320,200));

    JToolBar throttleToolBar = new JToolBar("Throttles list toolbar");
    JButton jbNew = new JButton();
    jbNew.setIcon(new NamedIcon("resources/icons/throttles/new.png","resources/icons/throttles/new.png"));
    jbNew.setToolTipText(Bundle.getMessage("ThrottleToolBarNewWindowToolTip"));
    jbNew.setVerticalTextPosition(JButton.BOTTOM);
    jbNew.setHorizontalTextPosition(JButton.CENTER);
    jbNew.addActionListener(new ActionListener() {
        /*public*/ void actionPerformed(ActionEvent e) {
            ThrottleFrame tf = ThrottleFrameManager.instance().createThrottleFrame();
            tf.toFront();
        }
    });
    throttleToolBar.add(jbNew);

    throttleToolBar.addSeparator();
    throttleToolBar.add(new StopAllButton());
    throttleToolBar.add(new LargePowerManagerButton());

    add( throttleToolBar, BorderLayout.PAGE_START);
    add( scrollPane1, BorderLayout.CENTER);

    throttleToolBar.addSeparator();
    JButton jbPreferences = new JButton();
    jbPreferences.setIcon(new NamedIcon("resources/icons/throttles/preferences.png","resources/icons/throttles/Preferences24.png"));
    jbPreferences.setToolTipText(Bundle.getMessage("ThrottleToolBarPreferencesToolTip"));
    jbPreferences.setVerticalTextPosition(JButton.BOTTOM);
    jbPreferences.setHorizontalTextPosition(JButton.CENTER);
    jbPreferences.addActionListener( new ThrottlesPreferencesAction() );
    throttleToolBar.add(jbPreferences);
}

/*public*/ Element getXml() {
        Element me  = new Element("ThrottlesListPanel");
        java.util.ArrayList<Element> children = new java.util.ArrayList<Element>(1);
        children.add(WindowPreferences.getPreferences(this.getTopLevelAncestor()));
        me.setContent(children);
        return me;
}

/*public*/ void setXml(Element tlp) {
        Element window = tlp.getChild("window");
        if (window!=null)
                WindowPreferences.setPreferences(this.getTopLevelAncestor(), window);
}
#endif
