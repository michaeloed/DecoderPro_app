#include "vsdpreferencesaction.h"
#include "jmrijframe.h"
#include "vsdecoderpreferencespane.h"
#include "vsdecodermanager.h"

//VSDPreferencesAction::VSDPreferencesAction(QObject *parent) :
//  AbstractAction(parent)
//{
//}
/*
 * <hr>
 * This file is part of JMRI.
 * <P>
 * JMRI is free software; you can redistribute it and/or modify it under
 * the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation. See the "COPYING" file for a copy
 * of this license.
 * <P>
 * JMRI is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * <P>
 *
 * @author			Mark Underwood Copyright (C) 2011
 * @version			$Revision: 28746 $
 */
//import java.awt.event.ActionEvent;
//import javax.swing.AbstractAction;
//import javax.swing.JFrame;
//import jmri.jmrit.vsdecoder.VSDecoderManager;
//import jmri.util.JmriJFrame;

//@SuppressWarnings("serial")
///*public*/ class VSDPreferencesAction extends AbstractAction {

/**
 * Constructor
 *
 * @param s Name for the action.
 */
/*public*/ VSDPreferencesAction::VSDPreferencesAction(QString s,QObject *parent) :
  AbstractAction(s, parent) {
    //super(s);
 connect(this, SIGNAL(triggered()), this, SLOT(actionPerformed()));
}

/*public*/ VSDPreferencesAction::VSDPreferencesAction(QObject *parent) :
  AbstractAction(tr("VSDecoder preferences"), parent)
{
    //this("VSDecoder preferences");
 connect(this, SIGNAL(triggered()), this, SLOT(actionPerformed()));
}

/*public*/ void VSDPreferencesAction::actionPerformed(ActionEvent* e)
{
    JmriJFrame* f = new JmriJFrame(tr("Virtual Sound Decoder preferences"), false, false);
    VSDecoderPreferencesPane* tpP = new VSDecoderPreferencesPane(VSDecoderManager::instance()->getVSDecoderPreferences());
//    f->add(tpP);
//    tpP.setContainer(f);
    f->setCentralWidget(tpP);
    f->setDefaultCloseOperation(JFrame::DISPOSE_ON_CLOSE);
    f->pack();
    f->setVisible(true);
    //f->requestFocus();
}
