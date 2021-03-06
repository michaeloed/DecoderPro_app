#include "compoundborder.h"

/**
 * A composite Border class used to compose two Border objects
 * into a single border by nesting an inside Border object within
 * the insets of an outside Border object.
 *
 * For example, this class may be used to add blank margin space
 * to a component with an existing decorative border:
 *
 * <pre>
 *    Border border = comp.getBorder();
 *    Border margin = new EmptyBorder(10,10,10,10);
 *    comp.setBorder(new CompoundBorder(border, margin));
 * </pre>
 * <p>
 * <strong>Warning:</strong>
 * Serialized objects of this class will not be compatible with
 * future Swing releases. The current serialization support is
 * appropriate for short term storage or RMI between applications running
 * the same version of Swing.  As of 1.4, support for long term storage
 * of all JavaBeans&trade;
 * has been added to the <code>java.beans</code> package.
 * Please see {@link java.beans.XMLEncoder}.
 *
 * @author David Kloba
 */
//@SuppressWarnings("serial")
///*public*/ class CompoundBorder extends AbstractBorder {

    /**
     * Creates a compound border with NULL outside and inside borders.
     */
    /*public*/ CompoundBorder::CompoundBorder(QWidget* parent) : AbstractBorder(parent) {
        this->outsideBorder = NULL;
        this->insideBorder = NULL;
    }

    /**
     * Creates a compound border with the specified outside and
     * inside borders.  Either border may be NULL.
     * @param outsideBorder the outside border
     * @param insideBorder the inside border to be nested
     */
    //@ConstructorProperties({"outsideBorder", "insideBorder"})
    /*public*/ CompoundBorder::CompoundBorder(Border* outsideBorder, Border* insideBorder, QWidget *parent) : AbstractBorder(parent)
{
        this->outsideBorder = outsideBorder;
        this->insideBorder = insideBorder;
    }

    /**
     * Returns whether or not the compound border is opaque.
     *
     * @return {@code true} if the inside and outside borders
     *         are each either {@code NULL} or opaque;
     *         or {@code false} otherwise
     */
    //@Override
    /*public*/ bool CompoundBorder::isBorderOpaque() {
        return (outsideBorder == NULL || outsideBorder->isBorderOpaque()) &&
               (insideBorder == NULL || insideBorder->isBorderOpaque());
    }
#if 1
    /**
     * Paints the compound border by painting the outside border
     * with the specified position and size and then painting the
     * inside border at the specified position and size offset by
     * the insets of the outside border.
     * @param c the component for which this border is being painted
     * @param g the paint graphics
     * @param x the x position of the painted border
     * @param y the y position of the painted border
     * @param width the width of the painted border
     * @param height the height of the painted border
     */
    /*public*/ void CompoundBorder::paintBorder(QWidget* c, /*QPainter g,*/ int x, int y, int width, int height) {
        Insets*  nextInsets;
        int px, py, pw, ph;

        px = x;
        py = y;
        pw = width;
        ph = height;

        if(outsideBorder != NULL) {
            outsideBorder->paintBorder(c, /*g,*/ px, py, pw, ph);

            nextInsets = outsideBorder->getBorderInsets(c);
            px += nextInsets->left;
            py += nextInsets->top;
            pw = pw - nextInsets->right - nextInsets->left;
            ph = ph - nextInsets->bottom - nextInsets->top;
        }
        if(insideBorder != NULL)
            insideBorder->paintBorder(c, /*g,*/ px, py, pw, ph);

    }
#endif
    /**
     * Reinitialize the insets parameter with this Border's current Insets.
     * @param c the component for which this border insets value applies
     * @param insets the object to be reinitialized
     */
    /*public*/ Insets* CompoundBorder::getBorderInsets(QWidget* c, Insets* insets) {
        Insets*  nextInsets;

        insets->top = insets->left = insets->right = insets->bottom = 0;
        if(outsideBorder != NULL) {
            nextInsets = outsideBorder->getBorderInsets(c);
            insets->top += nextInsets->top;
            insets->left += nextInsets->left;
            insets->right += nextInsets->right;
            insets->bottom += nextInsets->bottom;
        }
        if(insideBorder != NULL) {
            nextInsets = insideBorder->getBorderInsets(c);
            insets->top += nextInsets->top;
            insets->left += nextInsets->left;
            insets->right += nextInsets->right;
            insets->bottom += nextInsets->bottom;
        }
        return insets;
    }

    /**
     * Returns the outside border object.
     */
    /*public*/ Border* CompoundBorder::getOutsideBorder() {
        return outsideBorder;
    }

    /**
     * Returns the inside border object.
     */
    /*public*/ Border* CompoundBorder::getInsideBorder() {
        return insideBorder;
    }


