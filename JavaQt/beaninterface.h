#ifndef BEANINTERFACE_H
#define BEANINTERFACE_H
#include <QObject>
#include <QVariant>
#include <QSet>

/**
 * Simple interface for basic methods that implement JMRI Bean handling methods.
 *
 * Various methods in {@link jmri.beans.Beans} test that objects implement this
 * interface before attempting to get or set properties of those objects.
 * Classes implementing this interface can bypass the need to introspect the
 * class to manipulate a property, and can also implement properties that the
 * Beans class will be able to manipulate that do not conform to JavaBeans
 * coding standards.
 *
 * {@link ArbitraryBean} provides generic implementations of these methods that
 * allow the manipulation of properties defined at runtime if your class can
 * extend or extends a subclass of Bean.
 *
 * @author rhwood
 * @see Beans
 * @see Bean
 */
/*public*/ /*interface*/ class BeanInterface {
public:
    /**
     * Set the value of an element in an indexed property.
     * <p>
     * <b>NOTE</b> Implementing methods <i>must not call</i>
     * <code>Bean.setIndexedProperty()</code>, as doing so will cause a stack
     * overflow. Implementing methods may call
     * <code>Beans.setIntrospectedIndexedProperty()</code> instead.
     *
     * @param key   name of the property
     * @param index index of the property element to change
     * @param value the value to set the property to
     */
 /*public*/ virtual void setIndexedProperty(/*@Nonnull*/ QString /*key*/, int /*index*/, /*@CheckForNull*/ QVariant /*value*/){}

    /**
     * Get the value of an element in an indexed property.
     * <p>
     * <b>NOTE</b> Implementing methods <i>must not call</i>
     * <code>Bean.getIndexedProperty()</code>, as doing so will cause a stack
     * overflow. Implementing methods may call
     * <code>Beans.getIntrospectedIndexedProperty()</code> instead.
     *
     * @param key   name of the property
     * @param index index of the property element to change
     * @return value of the property or null
     */
    //@CheckForNull
 /*public*/ virtual QVariant getIndexedProperty(/*@Nonnull*/ QString /*key*/, int /*index*/) {return QVariant();}

    /**
     * Set the value of a property.
     * <p>
     * <b>NOTE</b> Implementing methods <i>must not call</i>
     * <code>Bean.setProperty()</code>, as doing so will cause a stack overflow.
     * Implementing methods may call
     * <code>Beans.setIntrospectedProperty()</code> instead.
     *
     * @param key   name of the property
     * @param value the value to set the property to
     */
 /*public*/ virtual void setProperty(/*@Nonnull*/ QString /*key*/, /*@CheckForNull*/ QVariant /*value*/) {}

    /**
     * Get the value of a property.
     * <p>
     * <b>NOTE</b> Implementing methods <i>must not call</i>
     * <code>Bean.getProperty()</code>, as doing so will cause a stack overflow.
     * Implementing methods may call
     * <code>Beans.getIntrospectedProperty()</code> instead.
     *
     * @param key name of the property
     * @return The value of the property or null
     */
    //@CheckForNull
    /*public*/ virtual QVariant getProperty(/*@Nonnull*/ QString /*key*/) {return QVariant();}

    /**
     * Test that a property exists.
     * <p>
     * <b>NOTE</b> Implementing method <i>must not call</i>
     * <code>Bean.hasProperty()</code>, as doing so will cause a stack overflow.
     * Implementing methods may call
     * <code>Beans.hasIntrospectedProperty()</code> instead.
     *
     * @param key name of the property
     * @return true is property <i>key</i> exists
     */
 /*public*/ virtual bool hasProperty(/*@Nonnull*/ QString /*key*/) {return false;}

    /**
     * Test that a property exists and is indexed.
     * <p>
     * <b>NOTE</b> Implementing method <i>must not call</i>
     * <code>Bean.hasIndexedProperty()</code>, as doing so will cause a stack
     * overflow. Implementing methods may call
     * <code>Beans.hasIntrospectedIndexedProperty()</code> instead.
     *
     * @param key name of the property
     * @return true is property <i>key</i> exists and is indexed
     */
    /*public*/ virtual bool hasIndexedProperty(/*@Nonnull*/ QString /*key*/) {return false;}

    /**
     * List all property names or keys.
     * <p>
     * <b>NOTE</b> Implementing method <i>must not call</i>
     * <code>Bean.getPropertyNames()</code>, as doing so will cause a stack
     * overflow. Implementing methods may call
     * <code>Beans.getIntrospectedPropertyNames()</code> instead.
     * <p>
     * <b>NOTE</b> Implementations of this method should not return null.
     *
     * @return property names or an empty Set.
     */
    //@Nonnull
 /*public*/ virtual QSet<QString> getPropertyNames() {return QSet<QString>();}
};
Q_DECLARE_INTERFACE(BeanInterface, "BeanInterface")
#endif // BEANINTERFACE_H
