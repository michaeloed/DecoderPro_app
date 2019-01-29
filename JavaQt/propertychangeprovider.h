#ifndef PROPERTYCHANGEPROVIDER_H
#define PROPERTYCHANGEPROVIDER_H
#include "propertychangelistener.h"

/**
 * A set of methods that would need to be implemented to ensure the implementing
 * class provides a complete external interface for property changes. This
 * interface is merely a convenience for developers to ensure support for
 * property change listening is thorough, if not complete. Developers of classes
 * implementing this interface still need to ensure that
 * {@link java.beans.PropertyChangeEvent}s are fired when properties are set.
 *
 * {@link jmri.beans.ArbitraryBean}, {@link jmri.beans.Bean}, and
 * {@link jmri.beans.ConstrainedBean} all provide complete implementations of
 * this interface.
 *
 * This interface defines all public methods of
 * {@link java.beans.PropertyChangeSupport} except the methods to fire
 * PropertyChangeEvents.
 *
 * @author Randall Wood
 */
/*public*/ /*interface*/class PropertyChangeProvider : public QObject{
Q_OBJECT
public:
    /**
     * Add a {@link java.beans.PropertyChangeListener} to the listener list.
     *
     * @param listener The PropertyChangeListener to be added
     */
    /*public*/ virtual void addPropertyChangeListener(/*QNullable*/ PropertyChangeListener* listener) {}

    /**
     * Add a {@link java.beans.PropertyChangeListener} for a specific property.
     *
     * @param propertyName The name of the property to listen on.
     * @param listener     The PropertyChangeListener to be added
     */
    /*public*/ virtual void addPropertyChangeListener(/*QNullable*/ QString propertyName, /*QNullable*/ PropertyChangeListener* /*listener*/) {}

    /**
     * Get all {@link java.beans.PropertyChangeListener}s currently attached to
     * this object.
     *
     * @return An array of PropertyChangeListeners.
     */
    //@Nonnull
    /*public*/ virtual QList<PropertyChangeListener*>* getPropertyChangeListeners() {return NULL;}

    /**
     * Get all {@link java.beans.PropertyChangeListener}s currently listening to
     * changes to the specified property.
     *
     * @param propertyName The name of the property of interest
     * @return An array of PropertyChangeListeners.
     */
    //@Nonnull
    /*public*/ virtual QList<PropertyChangeListener*>* getPropertyChangeListeners(/*QNullable*/ QString /*propertyName*/) {return NULL;}

    /**
     * Remove the specified listener from this object.
     *
     * @param listener The {@link java.beans.PropertyChangeListener} to remove.
     */
    /*public*/ virtual void removePropertyChangeListener(/*QNullable*/ PropertyChangeListener* /*listener*/) {}

    /**
     * Remove the specified listener of the specified property from this object.
     *
     * @param propertyName The name of the property to stop listening to.
     * @param listener     The {@link java.beans.PropertyChangeListener} to
     *                     remove.
     */
    /*public*/ virtual void removePropertyChangeListener(/*QNullable*/ QString propertyName, /*QNullable*/ PropertyChangeListener* /*listener*/) {}

};
#endif // PROPERTYCHANGEPROVIDER_H