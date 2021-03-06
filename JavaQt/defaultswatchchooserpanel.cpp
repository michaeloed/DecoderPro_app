#include "defaultswatchchooserpanel.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include "gridbagconstraints.h"
#include "gridbaglayout.h"
#include <QBoxLayout>
#include <QGroupBox>
#include "jlabel.h"
#include "jcolorchooser.h"
#include <QTimer>

/*
 * Copyright (c) 1998, 2013, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

//package javax.swing.colorchooser;

//import javax.swing.*;
//import javax.swing.border.*;
//import javax.swing.event.*;
//import java.awt.*;
//import java.awt.image.*;
//import java.awt.event.*;
//import java.beans.PropertyChangeEvent;
//import java.beans.PropertyChangeListener;
//import java.io.Serializable;
//import javax.accessibility.*;


/**
 * The standard color swatch chooser.
 * <p>
 * <strong>Warning:
 * Serialized objects of this class will not be compatible with
 * future Swing releases. The current serialization support is
 * appropriate for short term storage or RMI between applications running
 * the same version of Swing.  As of 1.4, support for long term storage
 * of all JavaBeans™
 * has been added to the <code>java.beans package.
 * Please see {@link java.beans.XMLEncoder}.
 *
 * @author Steve Wilson
 */
//class DefaultSwatchChooserPanel extends AbstractColorChooserPanel {


/*public*/ DefaultSwatchChooserPanel::DefaultSwatchChooserPanel() {
    //super();
//    setInheritsPopupMenu(true);
}

/*public*/ QString DefaultSwatchChooserPanel::getDisplayName() {
    //return UIManager.getString("ColorChooser.swatchesNameText", getLocale());
 return "Swatches";
}

/*public*/ QString DefaultSwatchChooserPanel::getTitle() {return "Swatches";}

/**
 * Provides a hint to the look and feel as to the
 * <code>KeyEvent.VK constant that can be used as a mnemonic to
 * access the panel. A return value <= 0 indicates there is no mnemonic.
 * <p>
 * The return value here is a hint, it is ultimately up to the look
 * and feel to honor the return value in some meaningful way.
 * <p>
 * This implementation looks up the value from the default
 * <code>ColorChooser.swatchesMnemonic, or if it
 * isn't available (or not an <code>Integer) returns -1.
 * The lookup for the default is done through the <code>UIManager:
 * <code>UIManager.get("ColorChooser.swatchesMnemonic");.
 *
 * @return KeyEvent.VK constant identifying the mnemonic; <= 0 for no
 *         mnemonic
 * @see #getDisplayedMnemonicIndex
 * @since 1.4
 */
/*public*/ int DefaultSwatchChooserPanel::getMnemonic() {
    //return getInt("ColorChooser.swatchesMnemonic", -1);
 return -1;
}

/**
 * Provides a hint to the look and feel as to the index of the character in
 * <code>getDisplayName that should be visually identified as the
 * mnemonic. The look and feel should only use this if
 * <code>getMnemonic returns a value > 0.
 * <p>
 * The return value here is a hint, it is ultimately up to the look
 * and feel to honor the return value in some meaningful way. For example,
 * a look and feel may wish to render each
 * <code>AbstractColorChooserPanel in a JTabbedPane,
 * and further use this return value to underline a character in
 * the <code>getDisplayName.
 * <p>
 * This implementation looks up the value from the default
 * <code>ColorChooser.rgbDisplayedMnemonicIndex, or if it
 * isn't available (or not an <code>Integer) returns -1.
 * The lookup for the default is done through the <code>UIManager:
 * <code>UIManager.get("ColorChooser.swatchesDisplayedMnemonicIndex");.
 *
 * @return Character index to render mnemonic for; -1 to provide no
 *                   visual identifier for this panel.
 * @see #getMnemonic
 * @since 1.4
 */
/*public*/ int DefaultSwatchChooserPanel::getDisplayedMnemonicIndex() {
    //return getInt("ColorChooser.swatchesDisplayedMnemonicIndex", -1);
return -1;
}

/*public*/ QIcon DefaultSwatchChooserPanel::getSmallDisplayIcon() {
    return QIcon();
}

/*public*/ QIcon DefaultSwatchChooserPanel::getLargeDisplayIcon() {
    return QIcon();
}

/**
 * The background color, foreground color, and font are already set to the
 * defaults from the defaults table before this method is called.
 */
/*public*/ void DefaultSwatchChooserPanel::installChooserPanel(JColorChooser* enclosingChooser) {
    AbstractColorChooserPanel::installChooserPanel(enclosingChooser);
}

/*protected*/ void DefaultSwatchChooserPanel::buildChooser()
{
 QVBoxLayout* thisLayout= new QVBoxLayout(this);

 QString recentStr = "recent"; //UIManager.getString("ColorChooser.swatchesRecentText", getLocale());

 GridBagLayout* gb = new GridBagLayout();
 GridBagConstraints gbc = GridBagConstraints();
 JPanel* superHolder = new JPanel();
 superHolder->setLayout(gb);

 swatchPanel =  new MainSwatchPanel(this);
//    swatchPanel.putClientProperty(AccessibleContext.ACCESSIBLE_NAME_PROPERTY,
//                                  getDisplayName());
//    swatchPanel->setInheritsPopupMenu(true);

 recentSwatchPanel = new RecentSwatchPanel(this);
//    recentSwatchPanel->putClientProperty(AccessibleContext.ACCESSIBLE_NAME_PROPERTY,
//                                        recentStr);

//    mainSwatchKeyListener = new MainSwatchKeyListener();
//    mainSwatchListener = new MainSwatchListener();
//    swatchPanel.addMouseListener(mainSwatchListener);
//    swatchPanel.addKeyListener(mainSwatchKeyListener);
//    recentSwatchListener = new RecentSwatchListener();
//    recentSwatchKeyListener = new RecentSwatchKeyListener();
//    recentSwatchPanel.addMouseListener(recentSwatchListener);
//    recentSwatchPanel.addKeyListener(recentSwatchKeyListener);

 QGroupBox* mainHolder = new QGroupBox(/*new BorderLayout()*/);
 QHBoxLayout* mainHolderLayout = new QHBoxLayout(mainHolder);
//    Border border = new CompoundBorder( new LineBorder(Color.black),
//                                        new LineBorder(Color.white) );
//    mainHolder.setBorder(border);
 mainHolderLayout->addWidget(swatchPanel, 0, Qt::AlignCenter);

 gbc.anchor = GridBagConstraints::LAST_LINE_START;
 gbc.gridwidth = 1;
 gbc.gridheight = 2;
 Insets* oldInsets = gbc.insets;
 gbc.insets = new Insets(0, 0, 0, 10);
 gbc.gridy = 0;
 gbc.gridx = 0;
 gb->addWidget(mainHolder, gbc);
 gbc.insets = oldInsets;

//    recentSwatchPanel.setInheritsPopupMenu(true);
 QGroupBox* recentHolder = new QGroupBox( /*new BorderLayout() */);
 QVBoxLayout* recentHolderLayout = new QVBoxLayout(recentHolder);
//    recentHolder.setBorder(border);
//    recentHolder.setInheritsPopupMenu(true);
 recentHolderLayout->addWidget(recentSwatchPanel, 0, Qt::AlignCenter);

 JLabel* l = new JLabel(recentStr);
 l->setLabelFor(recentSwatchPanel);

 gbc.gridwidth = GridBagConstraints::REMAINDER;
 gbc.gridheight = 1;
 gbc.weighty = 1.0;
 gbc.gridx = 1;
 gb->addWidget(l, gbc);

 gbc.weighty = 0;
 gbc.gridheight = GridBagConstraints::REMAINDER;
 gbc.insets = new Insets(0, 0, 0, 2);
 gbc.gridy = 1;
 gb->addWidget(recentHolder, gbc);
//    superHolder.setInheritsPopupMenu(true);

 thisLayout->addWidget(superHolder);
}

/*public*/ void DefaultSwatchChooserPanel::uninstallChooserPanel(JColorChooser* enclosingChooser) {
    AbstractColorChooserPanel::uninstallChooserPanel(enclosingChooser);
//    swatchPanel.removeMouseListener(mainSwatchListener);
//    swatchPanel.removeKeyListener(mainSwatchKeyListener);
//    recentSwatchPanel.removeMouseListener(recentSwatchListener);
//    recentSwatchPanel.removeKeyListener(recentSwatchKeyListener);

    swatchPanel = nullptr;
    recentSwatchPanel = nullptr;
    mainSwatchListener = nullptr;
    mainSwatchKeyListener = nullptr;
    recentSwatchListener = nullptr;
    recentSwatchKeyListener = nullptr;

//    removeAll();  // strip out all the sub-components
}

/*public*/ void DefaultSwatchChooserPanel::updateChooser() {

}

#if 0
/*private*/ class RecentSwatchKeyListener extends KeyAdapter {
    /*public*/ void keyPressed(KeyEvent e) {
        if (KeyEvent.VK_SPACE == e.getKeyCode()) {
            Color color = recentSwatchPanel.getSelectedColor();
            setSelectedColor(color);
        }
    }
}

/*private*/ class MainSwatchKeyListener extends KeyAdapter {
    /*public*/ void keyPressed(KeyEvent e) {
        if (KeyEvent.VK_SPACE == e.getKeyCode()) {
            Color color = swatchPanel.getSelectedColor();
            setSelectedColor(color);
            recentSwatchPanel.setMostRecentColor(color);
        }
    }
}

class RecentSwatchListener extends MouseAdapter implements Serializable {
    /*public*/ void mousePressed(MouseEvent e) {
        if (isEnabled()) {
            Color color = recentSwatchPanel.getColorForLocation(e.getX(), e.getY());
            recentSwatchPanel.setSelectedColorFromLocation(e.getX(), e.getY());
            setSelectedColor(color);
            recentSwatchPanel.requestFocusInWindow();
        }
    }
}

class MainSwatchListener extends MouseAdapter implements Serializable {
    /*public*/ void mousePressed(MouseEvent e) {
        if (isEnabled()) {
            Color color = swatchPanel.getColorForLocation(e.getX(), e.getY());
            setSelectedColor(color);
            swatchPanel.setSelectedColorFromLocation(e.getX(), e.getY());
            recentSwatchPanel.setMostRecentColor(color);
            swatchPanel.requestFocusInWindow();
        }
    }
}

}

#endif

//class SwatchPanel extends JPanel {

//protected Color[] colors;
//protected Dimension swatchSize;
//protected Dimension numSwatches;
//protected Dimension gap;

///*private*/ int selRow;
///*private*/ int selCol;

/*public*/ SwatchPanel::SwatchPanel(DefaultSwatchChooserPanel *dscp) : JPanel() {
 this->dscp = dscp;
 setLayout(new QVBoxLayout());
 QTimer::singleShot(100, this, SLOT(init()));
}

void SwatchPanel::init()
{
    initValues();
    initColors();
    setMinimumWidth(numSwatches.width()*(swatchSize.width()+ gap.width()));
    setMinimumHeight(numSwatches.height()*(swatchSize.height()+ gap.height()));

    setToolTip(""); // register for events
//    setOpaque(true);
    setBackground(QColor(Qt::white));
//    setFocusable(true);
//    setInheritsPopupMenu(true);
#if 0
    addFocusListener(new FocusAdapter() {
        /*public*/ void focusGained(FocusEvent e) {
            repaint();
        }

        /*public*/ void focusLost(FocusEvent e) {
            repaint();
        }
    });

    addKeyListener(new KeyAdapter() {
        /*public*/ void keyPressed(KeyEvent e) {
            int typed = e.getKeyCode();
            switch (typed) {
                case KeyEvent.VK_UP:
                    if (selRow > 0) {
                        selRow--;
                        repaint();
                    }
                    break;
                case KeyEvent.VK_DOWN:
                    if (selRow < numSwatches.height - 1) {
                        selRow++;
                        repaint();
                    }
                    break;
                case KeyEvent.VK_LEFT:
                    if (selCol > 0 && SwatchPanel.this.getComponentOrientation().isLeftToRight()) {
                        selCol--;
                        repaint();
                    } else if (selCol < numSwatches.width - 1
                            && !SwatchPanel.this.getComponentOrientation().isLeftToRight()) {
                        selCol++;
                        repaint();
                    }
                    break;
                case KeyEvent.VK_RIGHT:
                    if (selCol < numSwatches.width - 1
                            && SwatchPanel.this.getComponentOrientation().isLeftToRight()) {
                        selCol++;
                        repaint();
                    } else if (selCol > 0 && !SwatchPanel.this.getComponentOrientation().isLeftToRight()) {
                        selCol--;
                        repaint();
                    }
                    break;
                case KeyEvent.VK_HOME:
                    selCol = 0;
                    selRow = 0;
                    repaint();
                    break;
                case KeyEvent.VK_END:
                    selCol = numSwatches.width - 1;
                    selRow = numSwatches.height - 1;
                    repaint();
                    break;
            }
        }
    });
#endif
}

/*public*/ QColor SwatchPanel::getSelectedColor() {
    return getColorForCell(selCol, selRow);
}

/*protected*/ void SwatchPanel::initValues() {

}

/*public*/ void SwatchPanel::paintEvent(QPaintEvent* evt)
{
 QPainter g(this);
 QRect rect = evt->rect();
 //g->setColor(getBackground());
 g.fillRect(0,0,width(), height(), QColor(Qt::lightGray));
 for (int row = 0; row < numSwatches.height(); row++)
 {
  int y = row * (swatchSize.height() + gap.height());
  for (int column = 0; column < numSwatches.width(); column++)
  {
   QColor c = getColorForCell(column, row);
   //g.setColor(c);
   int x;
//            if (!this.getComponentOrientation().isLeftToRight()) {
//                x = (numSwatches.width - column - 1) * (swatchSize.width + gap.width);
//            } else {
   x = column * (swatchSize.width() + gap.width());
//            }
   g.fillRect( x, y, swatchSize.width(), swatchSize.height(), c);
//            g.setColor(Color.black);
   g.setPen(QColor(Qt::black));
   g.drawLine( x+swatchSize.width()-1, y, x+swatchSize.width()-1, y+swatchSize.height()-1);
   g.drawLine( x, y+swatchSize.height()-1, x+swatchSize.width()-1, y+swatchSize.height()-1);

   if (selRow == row && selCol == column /*&& this->isFocusOwner()*/)
   {
       QColor c2 = QColor(c.red() < 125 ? 255 : 0,
               c.green() < 125 ? 255 : 0,
               c.blue() < 125 ? 255 : 0);
       g.setPen(c2);

       g.drawLine(x, y, x + swatchSize.width() - 1, y);
       g.drawLine(x, y, x, y + swatchSize.height() - 1);
       g.drawLine(x + swatchSize.width() - 1, y, x + swatchSize.width() - 1, y + swatchSize.height() - 1);
       g.drawLine(x, y + swatchSize.height() - 1, x + swatchSize.width() - 1, y + swatchSize.height() - 1);
       g.drawLine(x, y, x + swatchSize.width() - 1, y + swatchSize.height() - 1);
       g.drawLine(x, y + swatchSize.height() - 1, x + swatchSize.width() - 1, y);
   }
  }
 }
}

/*public*/ QSize SwatchPanel::getPreferredSize() {
    int x = numSwatches.width() * (swatchSize.width() + gap.width()) - 1;
    int y = numSwatches.height() * (swatchSize.height() + gap.height()) - 1;
    return QSize( x, y );
}

/*protected*/ void SwatchPanel::initColors() {}

/*public*/ QString SwatchPanel::getToolTipText(QMouseEvent* e) {
    QColor color = getColorForLocation(e->pos().x(), e->pos().y());
    return QString::number(color.red())+", "+ QString::number(color.green()) + ", " + QString::number(color.blue());
}

/*public*/ void SwatchPanel::setSelectedColorFromLocation(int x, int y) {
//    if (!this->getComponentOrientation().isLeftToRight()) {
//        selCol = numSwatches.width - x / (swatchSize.width + gap.width) - 1;
//    } else {
        selCol = x / (swatchSize.width() + gap.width());
//    }
    selRow = y / (swatchSize.height() + gap.height());
    repaint();
}

/*public*/ QColor SwatchPanel::getColorForLocation( int x, int y ) {
    int column;
//    if (!this->getComponentOrientation().isLeftToRight()) {
//        column = numSwatches.width - x / (swatchSize.width + gap.width) - 1;
//    } else {
        column = x / (swatchSize.width() + gap.width());
//    }
    int row = y / (swatchSize.height() + gap.height());
    return getColorForCell(column, row);
}

/*private*/ QColor SwatchPanel::getColorForCell( int column, int row) {
    return colors[ (row * numSwatches.width()) + column ]; // (STEVE) - change data orientation here
}

//}

//class RecentSwatchPanel extends SwatchPanel {
/*protected*/ void RecentSwatchPanel::initValues() {
    swatchSize = QSize(10,10); //UIManager.getDimension("ColorChooser.swatchesRecentSwatchSize", getLocale());
    numSwatches = QSize( 5, 7 );
    gap = QSize(2, 2);
}

/*protected*/ void RecentSwatchPanel::initColors() {
    QColor defaultRecentColor = QColor(Qt::white);//UIManager.getColor("ColorChooser.swatchesDefaultRecentColor", getLocale());
    int numColors = numSwatches.width() * numSwatches.height();

    colors = QVector<QColor>(numColors);
    for (int i = 0; i < numColors ; i++) {
        colors[i] = defaultRecentColor;
    }
    repaint();
}

/*public*/ void RecentSwatchPanel::setMostRecentColor(QColor c) {

    //System.arraycopy( colors, 0, colors, 1, colors.length()-1);
    colors[0] = c;
    repaint();
}

//}

//class MainSwatchPanel extends SwatchPanel {


/*protected*/ void MainSwatchPanel::initValues() {
    swatchSize = QSize(10,10);//UIManager.getDimension("ColorChooser.swatchesSwatchSize", getLocale());
    numSwatches = QSize( 31, 9 );
    gap = QSize(2, 2);
}

/*protected*/ void MainSwatchPanel::initColors() {
    QVector<int> rawValues = initRawValues();
    int numColors = rawValues.length() / 3;

    colors = QVector<QColor>(numColors);
    for (int i = 0; i < numColors ; i++) {
        colors[i] = QColor( rawValues[(i*3)], rawValues[(i*3)+1], rawValues[(i*3)+2] );
    }
    repaint();
}
/*protected*/ void SwatchPanel::mousePressEvent(QMouseEvent *e)
{
 mousePressed(e);
}

/*public*/ void SwatchPanel::mousePressed(QMouseEvent* e) {
    if (isEnabled()) {
        QColor color = SwatchPanel::getColorForLocation(e->pos().x(), e->pos().y());
        QObject* p = parent();
        dscp->setSelectedColor(color);
        dscp->swatchPanel->setSelectedColorFromLocation(e->pos().x(), e->pos().y());
        dscp->recentSwatchPanel->setMostRecentColor(color);
//        dscp->swatchPanel->requestFocusInWindow();

    }
}

/*private*/ QVector<int> MainSwatchPanel::initRawValues() {

    QVector<int> rawValues = {
 255, 255, 255, // first row.
 204, 255, 255,
 204, 204, 255,
 204, 204, 255,
 204, 204, 255,
 204, 204, 255,
 204, 204, 255,
 204, 204, 255,
 204, 204, 255,
 204, 204, 255,
 204, 204, 255,
 255, 204, 255,
 255, 204, 204,
 255, 204, 204,
 255, 204, 204,
 255, 204, 204,
 255, 204, 204,
 255, 204, 204,
 255, 204, 204,
 255, 204, 204,
 255, 204, 204,
 255, 255, 204,
 204, 255, 204,
 204, 255, 204,
 204, 255, 204,
 204, 255, 204,
 204, 255, 204,
 204, 255, 204,
 204, 255, 204,
 204, 255, 204,
 204, 255, 204,
 204, 204, 204,  // second row.
 153, 255, 255,
 153, 204, 255,
 153, 153, 255,
 153, 153, 255,
 153, 153, 255,
 153, 153, 255,
 153, 153, 255,
 153, 153, 255,
 153, 153, 255,
 204, 153, 255,
 255, 153, 255,
 255, 153, 204,
 255, 153, 153,
 255, 153, 153,
 255, 153, 153,
 255, 153, 153,
 255, 153, 153,
 255, 153, 153,
 255, 153, 153,
 255, 204, 153,
 255, 255, 153,
 204, 255, 153,
 153, 255, 153,
 153, 255, 153,
 153, 255, 153,
 153, 255, 153,
 153, 255, 153,
 153, 255, 153,
 153, 255, 153,
 153, 255, 204,
 204, 204, 204,  // third row
 102, 255, 255,
 102, 204, 255,
 102, 153, 255,
 102, 102, 255,
 102, 102, 255,
 102, 102, 255,
 102, 102, 255,
 102, 102, 255,
 153, 102, 255,
 204, 102, 255,
 255, 102, 255,
 255, 102, 204,
 255, 102, 153,
 255, 102, 102,
 255, 102, 102,
 255, 102, 102,
 255, 102, 102,
 255, 102, 102,
 255, 153, 102,
 255, 204, 102,
 255, 255, 102,
 204, 255, 102,
 153, 255, 102,
 102, 255, 102,
 102, 255, 102,
 102, 255, 102,
 102, 255, 102,
 102, 255, 102,
 102, 255, 153,
 102, 255, 204,
 153, 153, 153, // fourth row
 51, 255, 255,
 51, 204, 255,
 51, 153, 255,
 51, 102, 255,
 51, 51, 255,
 51, 51, 255,
 51, 51, 255,
 102, 51, 255,
 153, 51, 255,
 204, 51, 255,
 255, 51, 255,
 255, 51, 204,
 255, 51, 153,
 255, 51, 102,
 255, 51, 51,
 255, 51, 51,
 255, 51, 51,
 255, 102, 51,
 255, 153, 51,
 255, 204, 51,
 255, 255, 51,
 204, 255, 51,
 153, 255, 51,
 102, 255, 51,
 51, 255, 51,
 51, 255, 51,
 51, 255, 51,
 51, 255, 102,
 51, 255, 153,
 51, 255, 204,
 153, 153, 153, // Fifth row
 0, 255, 255,
 0, 204, 255,
 0, 153, 255,
 0, 102, 255,
 0, 51, 255,
 0, 0, 255,
 51, 0, 255,
 102, 0, 255,
 153, 0, 255,
 204, 0, 255,
 255, 0, 255,
 255, 0, 204,
 255, 0, 153,
 255, 0, 102,
 255, 0, 51,
 255, 0 , 0,
 255, 51, 0,
 255, 102, 0,
 255, 153, 0,
 255, 204, 0,
 255, 255, 0,
 204, 255, 0,
 153, 255, 0,
 102, 255, 0,
 51, 255, 0,
 0, 255, 0,
 0, 255, 51,
 0, 255, 102,
 0, 255, 153,
 0, 255, 204,
 102, 102, 102, // sixth row
 0, 204, 204,
 0, 204, 204,
 0, 153, 204,
 0, 102, 204,
 0, 51, 204,
 0, 0, 204,
 51, 0, 204,
 102, 0, 204,
 153, 0, 204,
 204, 0, 204,
 204, 0, 204,
 204, 0, 204,
 204, 0, 153,
 204, 0, 102,
 204, 0, 51,
 204, 0, 0,
 204, 51, 0,
 204, 102, 0,
 204, 153, 0,
 204, 204, 0,
 204, 204, 0,
 204, 204, 0,
 153, 204, 0,
 102, 204, 0,
 51, 204, 0,
 0, 204, 0,
 0, 204, 51,
 0, 204, 102,
 0, 204, 153,
 0, 204, 204,
 102, 102, 102, // seventh row
 0, 153, 153,
 0, 153, 153,
 0, 153, 153,
 0, 102, 153,
 0, 51, 153,
 0, 0, 153,
 51, 0, 153,
 102, 0, 153,
 153, 0, 153,
 153, 0, 153,
 153, 0, 153,
 153, 0, 153,
 153, 0, 153,
 153, 0, 102,
 153, 0, 51,
 153, 0, 0,
 153, 51, 0,
 153, 102, 0,
 153, 153, 0,
 153, 153, 0,
 153, 153, 0,
 153, 153, 0,
 153, 153, 0,
 102, 153, 0,
 51, 153, 0,
 0, 153, 0,
 0, 153, 51,
 0, 153, 102,
 0, 153, 153,
 0, 153, 153,
 51, 51, 51, // eigth row
 0, 102, 102,
 0, 102, 102,
 0, 102, 102,
 0, 102, 102,
 0, 51, 102,
 0, 0, 102,
 51, 0, 102,
 102, 0, 102,
 102, 0, 102,
 102, 0, 102,
 102, 0, 102,
 102, 0, 102,
 102, 0, 102,
 102, 0, 102,
 102, 0, 51,
 102, 0, 0,
 102, 51, 0,
 102, 102, 0,
 102, 102, 0,
 102, 102, 0,
 102, 102, 0,
 102, 102, 0,
 102, 102, 0,
 102, 102, 0,
 51, 102, 0,
 0, 102, 0,
 0, 102, 51,
 0, 102, 102,
 0, 102, 102,
 0, 102, 102,
 0, 0, 0, // ninth row
 0, 51, 51,
 0, 51, 51,
 0, 51, 51,
 0, 51, 51,
 0, 51, 51,
 0, 0, 51,
 51, 0, 51,
 51, 0, 51,
 51, 0, 51,
 51, 0, 51,
 51, 0, 51,
 51, 0, 51,
 51, 0, 51,
 51, 0, 51,
 51, 0, 51,
 51, 0, 0,
 51, 51, 0,
 51, 51, 0,
 51, 51, 0,
 51, 51, 0,
 51, 51, 0,
 51, 51, 0,
 51, 51, 0,
 51, 51, 0,
 0, 51, 0,
 0, 51, 51,
 0, 51, 51,
 0, 51, 51,
 0, 51, 51,
 51, 51, 51 };
     return rawValues;
 }
//}
