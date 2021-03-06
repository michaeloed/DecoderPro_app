#include "abstractpreferences.h"
#include "exceptions.h"
//#include "accesscontroller.h"
#include "stringtokenizer.h"
#include "eventobject.h"
#include <QLinkedList>
#include <QThread>
#include "preferencechangeevent.h"
#include "xmlsupport.h"

AbstractPreferences::AbstractPreferences()
{

}
/**
 * This class provides a skeletal implementation of the {@link Preferences}
 * class, greatly easing the task of implementing it.
 *
 * <p><strong>This class is for <tt>Preferences</tt> implementers only.
 * Normal users of the <tt>Preferences</tt> facility should have no need to
 * consult this documentation.  The {@link Preferences} documentation
 * should suffice.</strong>
 *
 * <p>Implementors must override the nine abstract service-provider interface
 * (SPI) methods: {@link #getSpi(String)}, {@link #putSpi(String,String)},
 * {@link #removeSpi(String)}, {@link #childSpi(String)}, {@link
 * #removeNodeSpi()}, {@link #keysSpi()}, {@link #childrenNamesSpi()}, {@link
 * #syncSpi()} and {@link #flushSpi()}.  All of the concrete methods specify
 * precisely how they are implemented atop these SPI methods.  The implementor
 * may, at his discretion, override one or more of the concrete methods if the
 * default implementation is unsatisfactory for any reason, such as
 * performance.
 *
 * <p>The SPI methods fall into three groups concerning exception
 * behavior. The <tt>getSpi</tt> method should never throw exceptions, but it
 * doesn't really matter, as any exception thrown by this method will be
 * intercepted by {@link #get(String,String)}, which will return the specified
 * default value to the caller.  The <tt>removeNodeSpi, keysSpi,
 * childrenNamesSpi, syncSpi</tt> and <tt>flushSpi</tt> methods are specified
 * to throw {@link BackingStoreException}, and the implementation is required
 * to throw this checked exception if it is unable to perform the operation.
 * The exception propagates outward, causing the corresponding API method
 * to fail.
 *
 * <p>The remaining SPI methods {@link #putSpi(String,String)}, {@link
 * #removeSpi(String)} and {@link #childSpi(String)} have more complicated
 * exception behavior.  They are not specified to throw
 * <tt>BackingStoreException</tt>, as they can generally obey their contracts
 * even if the backing store is unavailable.  This is true because they return
 * no information and their effects are not required to become permanent until
 * a subsequent call to {@link Preferences#flush()} or
 * {@link Preferences#sync()}. Generally speaking, these SPI methods should not
 * throw exceptions.  In some implementations, there may be circumstances
 * under which these calls cannot even enqueue the requested operation for
 * later processing.  Even under these circumstances it is generally better to
 * simply ignore the invocation and return, rather than throwing an
 * exception.  Under these circumstances, however, all subsequent invocations
 * of <tt>flush()</tt> and <tt>sync</tt> should return <tt>false</tt>, as
 * returning <tt>true</tt> would imply that all previous operations had
 * successfully been made permanent.
 *
 * <p>There is one circumstance under which <tt>putSpi, removeSpi and
 * childSpi</tt> <i>should</i> throw an exception: if the caller lacks
 * sufficient privileges on the underlying operating system to perform the
 * requested operation.  This will, for instance, occur on most systems
 * if a non-privileged user attempts to modify system preferences.
 * (The required privileges will vary from implementation to
 * implementation.  On some implementations, they are the right to modify the
 * contents of some directory in the file system; on others they are the right
 * to modify contents of some key in a registry.)  Under any of these
 * circumstances, it would generally be undesirable to let the program
 * continue executing as if these operations would become permanent at a later
 * time.  While implementations are not required to throw an exception under
 * these circumstances, they are encouraged to do so.  A {@link
 * SecurityException} would be appropriate.
 *
 * <p>Most of the SPI methods require the implementation to read or write
 * information at a preferences node.  The implementor should beware of the
 * fact that another VM may have concurrently deleted this node from the
 * backing store.  It is the implementation's responsibility to recreate the
 * node if it has been deleted.
 *
 * <p>Implementation note: In Sun's default <tt>Preferences</tt>
 * implementations, the user's identity is inherited from the underlying
 * operating system and does not change for the lifetime of the virtual
 * machine.  It is recognized that server-side <tt>Preferences</tt>
 * implementations may have the user identity change from request to request,
 * implicitly passed to <tt>Preferences</tt> methods via the use of a
 * static {@link ThreadLocal} instance.  Authors of such implementations are
 * <i>strongly</i> encouraged to determine the user at the time preferences
 * are accessed (for example by the {@link #get(String,String)} or {@link
 * #put(String,String)} method) rather than permanently associating a user
 * with each <tt>Preferences</tt> instance.  The latter behavior conflicts
 * with normal <tt>Preferences</tt> usage and would lead to great confusion.
 *
 * @author  Josh Bloch
 * @see     Preferences
 * @since   1.4
 */
// /*public*/ abstract class AbstractPreferences extends Preferences {

/**
 * Creates a preference node with the specified parent and the specified
 * name relative to its parent.
 *
 * @param parent the parent of this preference node, or NULL if this
 *               is the root.
 * @param name the name of this preference node, relative to its parent,
 *             or <tt>""</tt> if this is the root.
 * @throws IllegalArgumentException if <tt>name</tt> contains a slash
 *          (<tt>'/'</tt>),  or <tt>parent</tt> is <tt>NULL</tt> and
 *          name isn't <tt>""</tt>.
 */
/*protected*/ AbstractPreferences::AbstractPreferences(AbstractPreferences* parent, QString name)
{
 newNode = false;
 kidCache = new QMap<QString, AbstractPreferences*>();
 removed = false;

/**
 * Registered preference change listeners.
 */
//prefListeners =
//    new QList<PreferenceChangeListener*>;

/**
 * Registered node change listeners.
 */
//nodeListeners = new QList<NodeChangeListener*>;

/**
 * An object whose monitor is used to lock this node.  This object
 * is used in preference to the node itself to reduce the likelihood of
 * intentional or unintentional denial of service due to a locked node.
 * To avoid deadlock, a node is <i>never</i> locked by a thread that
 * holds a lock on a descendant of that node.
 */
 lock = new QObject();

    if (parent==NULL) {
        if (name != (""))
            throw  IllegalArgumentException("Root name '"+name+
                                               "' must be \"\"");
        this->_absolutePath = "/";
        root = this;
    } else {
        if (name.indexOf('/') != -1)
            throw  IllegalArgumentException("Name '" + name +
                                             "' contains '/'");
        if (name == (""))
          throw  IllegalArgumentException("Illegal name: empty string");

        root = parent->root;
        _absolutePath = (parent==root ? ("/" + name)
                                     : (parent->absolutePath() + "/" + name));
    }
    this->_name = name;
    this->_parent = parent;
}

/**
 * Implements the <tt>put</tt> method as per the specification in
 * {@link Preferences#put(String,String)}.
 *
 * <p>This implementation checks that the key and value are legal,
 * obtains this preference node's lock, checks that the node
 * has not been removed, invokes {@link #putSpi(String,String)}, and if
 * there are any preference change listeners, enqueues a notification
 * event for processing by the event dispatch thread.
 *
 * @param key key with which the specified value is to be associated.
 * @param value value to be associated with the specified key.
 * @throws NullPointerException if key or value is <tt>NULL</tt>.
 * @throws IllegalArgumentException if <tt>key.length()</tt> exceeds
 *       <tt>MAX_KEY_LENGTH</tt> or if <tt>value.length</tt> exceeds
 *       <tt>MAX_VALUE_LENGTH</tt>.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 */
/*public*/ void AbstractPreferences::put(QString key, QString value)
{
// if (key.isEmpty() || value.isEmpty())
//     throw NullPointerException();
 if (key.length() > MAX_KEY_LENGTH
    || value.length() > MAX_VALUE_LENGTH)
  throw  IllegalArgumentException("key ("
    + QString::number(key.length()) + ")"
    + " or value ("
    + QString::number(value.length()) + ")"
    + " to large");
//    /*synchronized*/(lock) {
 if (removed)
     throw IllegalStateException("Node has been removed.");

 putSpi(key, value);
 enqueuePreferenceChangeEvent(key, value);
// if (preferenceListeners != null)
//   fire(new PreferenceChangeEvent(this, key, value));
//    }
}

/**
 * Implements the <tt>get</tt> method as per the specification in
 * {@link Preferences#get(String,String)}.
 *
 * <p>This implementation first checks to see if <tt>key</tt> is
 * <tt>NULL</tt> throwing a <tt>NullPointerException</tt> if this is
 * the case.  Then it obtains this preference node's lock,
 * checks that the node has not been removed, invokes {@link
 * #getSpi(String)}, and returns the result, unless the <tt>getSpi</tt>
 * invocation returns <tt>NULL</tt> or throws an exception, in which case
 * this invocation returns <tt>def</tt>.
 *
 * @param key key whose associated value is to be returned.
 * @param def the value to be returned in the event that this
 *        preference node has no value associated with <tt>key</tt>.
 * @return the value associated with <tt>key</tt>, or <tt>def</tt>
 *         if no value is associated with <tt>key</tt>.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 * @throws NullPointerException if key is <tt>NULL</tt>.  (A
 *         <tt>NULL</tt> default <i>is</i> permitted.)
 */
/*public*/ QString AbstractPreferences::get(QString key, QString def) {
    if (key==NULL)
        throw NullPointerException("Null key");
//    /*synchronized*/(lock) {
        if (removed)
            throw IllegalStateException("Node has been removed.");

        QString result = NULL;
        try {
            result = getSpi(key);
        } catch (Exception e) {
            // Ignoring exception causes default to be returned
        }
        if(result == "preference:")
         result = "";
        return (result=="" ? def : result);
//    }
}

/**
 * Implements the <tt>remove(String)</tt> method as per the specification
 * in {@link Preferences#remove(String)}.
 *
 * <p>This implementation obtains this preference node's lock,
 * checks that the node has not been removed, invokes
 * {@link #removeSpi(String)} and if there are any preference
 * change listeners, enqueues a notification event for processing by the
 * event dispatch thread.
 *
 * @param key key whose mapping is to be removed from the preference node.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 * @throws NullPointerException {@inheritDoc}.
 */
/*public*/ void AbstractPreferences::remove(QString key) {
    //Objects.requireNonNull(key, "Specified key cannot be NULL");
//    /*synchronized*/(lock) {
        if (removed)
            throw new IllegalStateException("Node has been removed.");

        removeSpi(key);
        enqueuePreferenceChangeEvent(key, NULL);
//    }
}

/**
 * Implements the <tt>clear</tt> method as per the specification in
 * {@link Preferences#clear()}.
 *
 * <p>This implementation obtains this preference node's lock,
 * invokes {@link #keys()} to obtain an array of keys, and
 * iterates over the array invoking {@link #remove(String)} on each key.
 *
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 */
/*public*/ void AbstractPreferences::clear() //throws BackingStoreException
{
//    /*synchronized*/(lock) {
        QStringList _keys = keys();
        for (int i=0; i<_keys.length(); i++)
            remove(_keys[i]);
//    }
}

/**
 * Implements the <tt>putInt</tt> method as per the specification in
 * {@link Preferences#putInt(String,int)}.
 *
 * <p>This implementation translates <tt>value</tt> to a string with
 * {@link Integer#toString(int)} and invokes {@link #put(String,String)}
 * on the result.
 *
 * @param key key with which the string form of value is to be associated.
 * @param value value whose string form is to be associated with key.
 * @throws NullPointerException if key is <tt>NULL</tt>.
 * @throws IllegalArgumentException if <tt>key.length()</tt> exceeds
 *         <tt>MAX_KEY_LENGTH</tt>.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 */
/*public*/ void AbstractPreferences::putInt(QString key, int value) {
    put(key, QString::number(value));
}

/**
 * Implements the <tt>getInt</tt> method as per the specification in
 * {@link Preferences#getInt(String,int)}.
 *
 * <p>This implementation invokes {@link #get(String,String) <tt>get(key,
 * NULL)</tt>}.  If the return value is non-NULL, the implementation
 * attempts to translate it to an <tt>int</tt> with
 * {@link Integer#parseInt(String)}.  If the attempt succeeds, the return
 * value is returned by this method.  Otherwise, <tt>def</tt> is returned.
 *
 * @param key key whose associated value is to be returned as an int.
 * @param def the value to be returned in the event that this
 *        preference node has no value associated with <tt>key</tt>
 *        or the associated value cannot be interpreted as an int.
 * @return the int value represented by the string associated with
 *         <tt>key</tt> in this preference node, or <tt>def</tt> if the
 *         associated value does not exist or cannot be interpreted as
 *         an int.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 * @throws NullPointerException if <tt>key</tt> is <tt>NULL</tt>.
 */
/*public*/ int AbstractPreferences::getInt(QString key, int def) {
    int result = def;
    //try {
    bool ok = true;
        QString value = get(key, NULL);
        if (value != NULL)
            result = value.toInt(&ok);
    if(!ok) {
        // Ignoring exception causes specified default to be returned
    }

    return result;
}

/**
 * Implements the <tt>putLong</tt> method as per the specification in
 * {@link Preferences#putLong(String,long)}.
 *
 * <p>This implementation translates <tt>value</tt> to a string with
 * {@link Long#toString(long)} and invokes {@link #put(String,String)}
 * on the result.
 *
 * @param key key with which the string form of value is to be associated.
 * @param value value whose string form is to be associated with key.
 * @throws NullPointerException if key is <tt>NULL</tt>.
 * @throws IllegalArgumentException if <tt>key.length()</tt> exceeds
 *         <tt>MAX_KEY_LENGTH</tt>.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 */
/*public*/ void AbstractPreferences::putLong(QString key, long value) {
    put(key, QString::number(value));
}
#if 1/**
 * Implements the <tt>getLong</tt> method as per the specification in
 * {@link Preferences#getLong(String,long)}.
 *
 * <p>This implementation invokes {@link #get(String,String) <tt>get(key,
 * NULL)</tt>}.  If the return value is non-NULL, the implementation
 * attempts to translate it to a <tt>long</tt> with
 * {@link Long#parseLong(String)}.  If the attempt succeeds, the return
 * value is returned by this method.  Otherwise, <tt>def</tt> is returned.
 *
 * @param key key whose associated value is to be returned as a long.
 * @param def the value to be returned in the event that this
 *        preference node has no value associated with <tt>key</tt>
 *        or the associated value cannot be interpreted as a long.
 * @return the long value represented by the string associated with
 *         <tt>key</tt> in this preference node, or <tt>def</tt> if the
 *         associated value does not exist or cannot be interpreted as
 *         a long.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 * @throws NullPointerException if <tt>key</tt> is <tt>NULL</tt>.
 */
/*public*/ long AbstractPreferences::getLong(QString key, long def) {
    long result = def;
    try {
        QString value = get(key, NULL);
        bool bOk;
        if (value != NULL)
            result = value.toLong(&bOk);
        if(!bOk)
         throw NumberFormatException();
    } catch (NumberFormatException e) {
        // Ignoring exception causes specified default to be returned
    }

    return result;
}

/**
 * Implements the <tt>putBoolean</tt> method as per the specification in
 * {@link Preferences#putBoolean(String,bool)}.
 *
 * <p>This implementation translates <tt>value</tt> to a string with
 * {@link String#valueOf(bool)} and invokes {@link #put(String,String)}
 * on the result.
 *
 * @param key key with which the string form of value is to be associated.
 * @param value value whose string form is to be associated with key.
 * @throws NullPointerException if key is <tt>NULL</tt>.
 * @throws IllegalArgumentException if <tt>key.length()</tt> exceeds
 *         <tt>MAX_KEY_LENGTH</tt>.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 */
/*public*/ void AbstractPreferences::putBoolean(QString key, bool value) {
    put(key, (value? "true" : "false"));
}

/**
 * Implements the <tt>getBoolean</tt> method as per the specification in
 * {@link Preferences#getBoolean(String,bool)}.
 *
 * <p>This implementation invokes {@link #get(String,String) <tt>get(key,
 * NULL)</tt>}.  If the return value is non-NULL, it is compared with
 * <tt>"true"</tt> using {@link String#equalsIgnoreCase(String)}.  If the
 * comparison returns <tt>true</tt>, this invocation returns
 * <tt>true</tt>.  Otherwise, the original return value is compared with
 * <tt>"false"</tt>, again using {@link String#equalsIgnoreCase(String)}.
 * If the comparison returns <tt>true</tt>, this invocation returns
 * <tt>false</tt>.  Otherwise, this invocation returns <tt>def</tt>.
 *
 * @param key key whose associated value is to be returned as a bool.
 * @param def the value to be returned in the event that this
 *        preference node has no value associated with <tt>key</tt>
 *        or the associated value cannot be interpreted as a bool.
 * @return the bool value represented by the string associated with
 *         <tt>key</tt> in this preference node, or <tt>def</tt> if the
 *         associated value does not exist or cannot be interpreted as
 *         a bool.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 * @throws NullPointerException if <tt>key</tt> is <tt>NULL</tt>.
 */
/*public*/ bool AbstractPreferences::getBoolean(QString key, bool def) {
    bool result = def;
    QString value = get(key, NULL);
    if (value != NULL) {
        if (value == /*IgnoreCase*/("true"))
            result = true;
        else if (value == /*IgnoreCase*/("false"))
            result = false;
    }

    return result;
}

/**
 * Implements the <tt>putFloat</tt> method as per the specification in
 * {@link Preferences#putFloat(String,float)}.
 *
 * <p>This implementation translates <tt>value</tt> to a string with
 * {@link Float#toString(float)} and invokes {@link #put(String,String)}
 * on the result.
 *
 * @param key key with which the string form of value is to be associated.
 * @param value value whose string form is to be associated with key.
 * @throws NullPointerException if key is <tt>NULL</tt>.
 * @throws IllegalArgumentException if <tt>key.length()</tt> exceeds
 *         <tt>MAX_KEY_LENGTH</tt>.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 */
/*public*/ void AbstractPreferences::putFloat(QString key, float value) {
    put(key, QString::number(value));
}

/**
 * Implements the <tt>getFloat</tt> method as per the specification in
 * {@link Preferences#getFloat(String,float)}.
 *
 * <p>This implementation invokes {@link #get(String,String) <tt>get(key,
 * NULL)</tt>}.  If the return value is non-NULL, the implementation
 * attempts to translate it to an <tt>float</tt> with
 * {@link Float#parseFloat(String)}.  If the attempt succeeds, the return
 * value is returned by this method.  Otherwise, <tt>def</tt> is returned.
 *
 * @param key key whose associated value is to be returned as a float.
 * @param def the value to be returned in the event that this
 *        preference node has no value associated with <tt>key</tt>
 *        or the associated value cannot be interpreted as a float.
 * @return the float value represented by the string associated with
 *         <tt>key</tt> in this preference node, or <tt>def</tt> if the
 *         associated value does not exist or cannot be interpreted as
 *         a float.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 * @throws NullPointerException if <tt>key</tt> is <tt>NULL</tt>.
 */
/*public*/ float AbstractPreferences::getFloat(QString key, float def) {
    float result = def;
    try {
        QString value = get(key, NULL);
        bool bOk;
        if (value != NULL)
            result = (value.toFloat(&bOk));
        if(!bOk) throw NumberFormatException();
    } catch (NumberFormatException e) {
        // Ignoring exception causes specified default to be returned
    }

    return result;
}

/**
 * Implements the <tt>putDouble</tt> method as per the specification in
 * {@link Preferences#putDouble(String,double)}.
 *
 * <p>This implementation translates <tt>value</tt> to a string with
 * {@link Double#toString(double)} and invokes {@link #put(String,String)}
 * on the result.
 *
 * @param key key with which the string form of value is to be associated.
 * @param value value whose string form is to be associated with key.
 * @throws NullPointerException if key is <tt>NULL</tt>.
 * @throws IllegalArgumentException if <tt>key.length()</tt> exceeds
 *         <tt>MAX_KEY_LENGTH</tt>.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 */
/*public*/ void AbstractPreferences::putDouble(QString key, double value) {
    put(key, QString::number(value));
}

/**
 * Implements the <tt>getDouble</tt> method as per the specification in
 * {@link Preferences#getDouble(String,double)}.
 *
 * <p>This implementation invokes {@link #get(String,String) <tt>get(key,
 * NULL)</tt>}.  If the return value is non-NULL, the implementation
 * attempts to translate it to an <tt>double</tt> with
 * {@link Double#parseDouble(String)}.  If the attempt succeeds, the return
 * value is returned by this method.  Otherwise, <tt>def</tt> is returned.
 *
 * @param key key whose associated value is to be returned as a double.
 * @param def the value to be returned in the event that this
 *        preference node has no value associated with <tt>key</tt>
 *        or the associated value cannot be interpreted as a double.
 * @return the double value represented by the string associated with
 *         <tt>key</tt> in this preference node, or <tt>def</tt> if the
 *         associated value does not exist or cannot be interpreted as
 *         a double.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 * @throws NullPointerException if <tt>key</tt> is <tt>NULL</tt>.
 */
/*public*/ double AbstractPreferences::getDouble(QString key, double def) {
    double result = def;
    try {
        QString value = get(key, NULL);
        bool bOk;
        if (value != NULL)
            result =(value.toDouble(&bOk));
        if(!bOk) throw NumberFormatException();
    } catch (NumberFormatException e) {
        // Ignoring exception causes specified default to be returned
    }

    return result;
}
#if 0
/**
 * Implements the <tt>putByteArray</tt> method as per the specification in
 * {@link Preferences#putByteArray(String,byte[])}.
 *
 * @param key key with which the string form of value is to be associated.
 * @param value value whose string form is to be associated with key.
 * @throws NullPointerException if key or value is <tt>NULL</tt>.
 * @throws IllegalArgumentException if key.length() exceeds MAX_KEY_LENGTH
 *         or if value.length exceeds MAX_VALUE_LENGTH*3/4.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 */
/*public*/ void putByteArray(QString key,QByteArray value) {
    put(key, Base64.byteArrayToBase64(value));
}

/**
 * Implements the <tt>getByteArray</tt> method as per the specification in
 * {@link Preferences#getByteArray(String,byte[])}.
 *
 * @param key key whose associated value is to be returned as a byte array.
 * @param def the value to be returned in the event that this
 *        preference node has no value associated with <tt>key</tt>
 *        or the associated value cannot be interpreted as a byte array.
 * @return the byte array value represented by the string associated with
 *         <tt>key</tt> in this preference node, or <tt>def</tt> if the
 *         associated value does not exist or cannot be interpreted as
 *         a byte array.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 * @throws NullPointerException if <tt>key</tt> is <tt>NULL</tt>.  (A
 *         <tt>NULL</tt> value for <tt>def</tt> <i>is</i> permitted.)
 */
/*public*/ byte[] getByteArray(String key, byte[] def) {
    byte[] result = def;
    String value = get(key, NULL);
    try {
        if (value != NULL)
            result = Base64.base64ToByteArray(value);
    }
    catch (RuntimeException e) {
        // Ignoring exception causes specified default to be returned
    }

    return result;
}
#endif
/**
 * Implements the <tt>keys</tt> method as per the specification in
 * {@link Preferences#keys()}.
 *
 * <p>This implementation obtains this preference node's lock, checks that
 * the node has not been removed and invokes {@link #keysSpi()}.
 *
 * @return an array of the keys that have an associated value in this
 *         preference node.
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 */
/*public*/ QStringList AbstractPreferences::keys() throw (BackingStoreException) {
    /*synchronized(lock)*/ {
        if (removed)
            throw new IllegalStateException("Node has been removed.");

        return keysSpi();
    }
}
#endif
/**
 * Implements the <tt>children</tt> method as per the specification in
 * {@link Preferences#childrenNames()}.
 *
 * <p>This implementation obtains this preference node's lock, checks that
 * the node has not been removed, constructs a <tt>TreeSet</tt> initialized
 * to the names of children already cached (the children in this node's
 * "child-cache"), invokes {@link #childrenNamesSpi()}, and adds all of the
 * returned child-names into the set.  The elements of the tree set are
 * dumped into a <tt>String</tt> array using the <tt>toArray</tt> method,
 * and this array is returned.
 *
 * @return the names of the children of this preference node.
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 * @see #cachedChildren()
 */
/*public*/QStringList AbstractPreferences::childrenNames() throw (BackingStoreException) {
    /*synchronized(lock)*/ {
        if (removed)
            throw new IllegalStateException("Node has been removed.");

        QStringList s = QStringList(kidCache->keys());
        foreach (QString kid, childrenNamesSpi())
            s.append(kid);
        return s; //toArray(EMPTY_STRING_ARRAY);
    }
}

/**
 * Returns all known unremoved children of this node.
 *
 * @return all known unremoved children of this node.
 */
/*protected*/ /*final*/ QList<AbstractPreferences*> AbstractPreferences::cachedChildren()
{
 return kidCache->values(); //      .toArray(EMPTY_ABSTRACT_PREFS_ARRAY);
}

/*private*/ /*static*/ /*final*/ QList<AbstractPreferences*> AbstractPreferences::EMPTY_ABSTRACT_PREFS_ARRAY
    = /*new*/ QList<AbstractPreferences*>();

/**
 * Implements the <tt>parent</tt> method as per the specification in
 * {@link Preferences#parent()}.
 *
 * <p>This implementation obtains this preference node's lock, checks that
 * the node has not been removed and returns the parent value that was
 * passed to this node's constructor.
 *
 * @return the parent of this preference node.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 */
/*public*/ Preferences* AbstractPreferences::parent()
{
 /*synchronized(lock)*/
 {
  if (removed)
      throw new IllegalStateException("Node has been removed.");

  return _parent;
 }
}

/**
 * Implements the <tt>node</tt> method as per the specification in
 * {@link Preferences#node(String)}.
 *
 * <p>This implementation obtains this preference node's lock and checks
 * that the node has not been removed.  If <tt>path</tt> is <tt>""</tt>,
 * this node is returned; if <tt>path</tt> is <tt>"/"</tt>, this node's
 * root is returned.  If the first character in <tt>path</tt> is
 * not <tt>'/'</tt>, the implementation breaks <tt>path</tt> into
 * tokens and recursively traverses the path from this node to the
 * named node, "consuming" a name and a slash from <tt>path</tt> at
 * each step of the traversal.  At each step, the current node is locked
 * and the node's child-cache is checked for the named node.  If it is
 * not found, the name is checked to make sure its length does not
 * exceed <tt>MAX_NAME_LENGTH</tt>.  Then the {@link #childSpi(String)}
 * method is invoked, and the result stored in this node's child-cache.
 * If the newly created <tt>Preferences</tt> object's {@link #newNode}
 * field is <tt>true</tt> and there are any node change listeners,
 * a notification event is enqueued for processing by the event dispatch
 * thread.
 *
 * <p>When there are no more tokens, the last value found in the
 * child-cache or returned by <tt>childSpi</tt> is returned by this
 * method.  If during the traversal, two <tt>"/"</tt> tokens occur
 * consecutively, or the final token is <tt>"/"</tt> (rather than a name),
 * an appropriate <tt>IllegalArgumentException</tt> is thrown.
 *
 * <p> If the first character of <tt>path</tt> is <tt>'/'</tt>
 * (indicating an absolute path name) this preference node's
 * lock is dropped prior to breaking <tt>path</tt> into tokens, and
 * this method recursively traverses the path starting from the root
 * (rather than starting from this node).  The traversal is otherwise
 * identical to the one described for relative path names.  Dropping
 * the lock on this node prior to commencing the traversal at the root
 * node is essential to avoid the possibility of deadlock, as per the
 * {@link #lock locking invariant}.
 *
 * @param path the path name of the preference node to return.
 * @return the specified preference node.
 * @throws IllegalArgumentException if the path name is invalid (i.e.,
 *         it contains multiple consecutive slash characters, or ends
 *         with a slash character and is more than one character long).
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 */
/*public*/ Preferences* AbstractPreferences::node(QString path)
{
//    synchronized(lock) {
 if (removed)
     throw new IllegalStateException("Node has been removed.");
 if (path == (""))
     return this;
 if (path == ("/"))
     return root;
 if (path.at(0) != '/')
     return node(new StringTokenizer(path, "/", true));
//    }

 // Absolute path.  Note that we've dropped our lock to avoid deadlock
 return root->node(new StringTokenizer(path.mid(1), "/", true));
}

/**
 * tokenizer contains <name> {'/' <name>}*
 */
/*private*/ Preferences* AbstractPreferences::node(StringTokenizer* path)
{
 QString token = path->nextToken();
 if (token == ("/"))  // Check for consecutive slashes
     throw new IllegalArgumentException("Consecutive slashes in path");
//    synchronized(lock) {
  AbstractPreferences* child = kidCache->value(token);
  if (child == NULL) {
      if (token.length() > MAX_NAME_LENGTH)
          throw new IllegalArgumentException(
              "Node name " + token + " too long");
      child = childSpi(token);
      if (child->newNode)
          enqueueNodeAddedEvent(child);
      kidCache->insert(token, child);
  }
  if (!path->hasMoreTokens())
      return child;
  path->nextToken();  // Consume slash
  if (!path->hasMoreTokens())
      throw new IllegalArgumentException("Path ends with slash");
  return child->node(path);
//    }
}

/**
 * Implements the <tt>nodeExists</tt> method as per the specification in
 * {@link Preferences#nodeExists(String)}.
 *
 * <p>This implementation is very similar to {@link #node(String)},
 * except that {@link #getChild(String)} is used instead of {@link
 * #childSpi(String)}.
 *
 * @param path the path name of the node whose existence is to be checked.
 * @return true if the specified node exists.
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 * @throws IllegalArgumentException if the path name is invalid (i.e.,
 *         it contains multiple consecutive slash characters, or ends
 *         with a slash character and is more than one character long).
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method and
 *         <tt>pathname</tt> is not the empty string (<tt>""</tt>).
 */
/*public*/ bool AbstractPreferences::nodeExists(QString path)
    throw (BackingStoreException)
{
 /*synchronized(lock)*/
 {
  if (path == (""))
      return !removed;
  if (removed)
      throw new IllegalStateException("Node has been removed.");
  if (path == ("/"))
      return true;
  if (path.at(0) != '/')
      return nodeExists(new StringTokenizer(path, "/", true));
 }

 // Absolute path.  Note that we've dropped our lock to avoid deadlock
 return root->nodeExists(new StringTokenizer(path.mid(1), "/",
                                            true));
}

/**
 * tokenizer contains <name> {'/' <name>}*
 */
/*private*/ bool AbstractPreferences::nodeExists(StringTokenizer* path)
    throw (BackingStoreException)
{
 QString token = path->nextToken();
 if (token == ("/"))  // Check for consecutive slashes
     throw new IllegalArgumentException("Consecutive slashes in path");
 /*synchronized(lock)*/
 {
  AbstractPreferences* child = kidCache->value(token);
  if (child == NULL)
      child = getChild(token);
  if (child==NULL)
      return false;
  if (!path->hasMoreTokens())
      return true;
  path->nextToken();  // Consume slash
  if (!path->hasMoreTokens())
      throw new IllegalArgumentException("Path ends with slash");
  return child->nodeExists(path);
 }
}

/**

 * Implements the <tt>removeNode()</tt> method as per the specification in
 * {@link Preferences#removeNode()}.
 *
 * <p>This implementation checks to see that this node is the root; if so,
 * it throws an appropriate exception.  Then, it locks this node's parent,
 * and calls a recursive helper method that traverses the subtree rooted at
 * this node.  The recursive method locks the node on which it was called,
 * checks that it has not already been removed, and then ensures that all
 * of its children are cached: The {@link #childrenNamesSpi()} method is
 * invoked and each returned child name is checked for containment in the
 * child-cache.  If a child is not already cached, the {@link
 * #childSpi(String)} method is invoked to create a <tt>Preferences</tt>
 * instance for it, and this instance is put into the child-cache.  Then
 * the helper method calls itself recursively on each node contained in its
 * child-cache.  Next, it invokes {@link #removeNodeSpi()}, marks itself
 * as removed, and removes itself from its parent's child-cache.  Finally,
 * if there are any node change listeners, it enqueues a notification
 * event for processing by the event dispatch thread.
 *
 * <p>Note that the helper method is always invoked with all ancestors up
 * to the "closest non-removed ancestor" locked.
 *
 * @throws IllegalStateException if this node (or an ancestor) has already
 *         been removed with the {@link #removeNode()} method.
 * @throws UnsupportedOperationException if this method is invoked on
 *         the root node.
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 */
/*public*/ void AbstractPreferences::removeNode() throw (BackingStoreException) {
    if (this==root)
        throw new UnsupportedOperationException("Can't remove the root!");
    /*synchronized(parent.lock)*/ {
        removeNode2();
        _parent->kidCache->remove(_name);
    }
}

/*
 * Called with locks on all nodes on path from parent of "removal root"
 * to this (including the former but excluding the latter).
 */
/*private*/ void AbstractPreferences::removeNode2() throw (BackingStoreException) {
    /*synchronized(lock)*/ {
        if (removed)
            throw new IllegalStateException("Node already removed.");

        // Ensure that all children are cached
        QStringList kidNames = childrenNamesSpi();
        for (int i=0; i<kidNames.length(); i++)
            if (!kidCache->contains(kidNames.at(i)))
                kidCache->insert(kidNames.at(i), childSpi(kidNames.at(i)));

        // Recursively remove all cached children
        //for (Iterator<AbstractPreferences> i = kidCache.values().iterator();
        QListIterator<AbstractPreferences*> i(kidCache->values());
        while (i.hasNext())
        {
            try {
                i.next()->removeNode2();
//                i.remove();
            } catch (BackingStoreException x) { }
        }

        // Now we have no descendants - it's time to die!
        removeNodeSpi();
        removed = true;
        _parent->enqueueNodeRemovedEvent(this);
    }
}

/**
 * Implements the <tt>name</tt> method as per the specification in
 * {@link Preferences#name()}.
 *
 * <p>This implementation merely returns the name that was
 * passed to this node's constructor.
 *
 * @return this preference node's name, relative to its parent.
 */
/*public*/ QString AbstractPreferences::name() {
    return _name;
}

/**
 * Implements the <tt>absolutePath</tt> method as per the specification in
 * {@link Preferences#absolutePath()}.
 *
 * <p>This implementation merely returns the absolute path name that
 * was computed at the time that this node was constructed (based on
 * the name that was passed to this node's constructor, and the names
 * that were passed to this node's ancestors' constructors).
 *
 * @return this preference node's absolute path name.
 */
/*public*/ QString AbstractPreferences::absolutePath() {
    return _absolutePath;
}

/**
 * Implements the <tt>isUserNode</tt> method as per the specification in
 * {@link Preferences#isUserNode()}.
 *
 * <p>This implementation compares this node's root node (which is stored
 * in a private field) with the value returned by
 * {@link Preferences#userRoot()}.  If the two object references are
 * identical, this method returns true.
 *
 * @return <tt>true</tt> if this preference node is in the user
 *         preference tree, <tt>false</tt> if it's in the system
 *         preference tree.
 */
/*public*/ bool AbstractPreferences::isUserNode() {
#if 0
    return AccessController.doPrivileged(
        new PrivilegedAction<Boolean>() {
            /*public*/ Boolean run() {
                return root == Preferences.userRoot();
        }
        }).boolValue();
#else
 return true;
#endif
}
#if 0
/*public*/ void addPreferenceChangeListener(PreferenceChangeListener pcl) {
    if (pcl==NULL)
        throw new NullPointerException("Change listener is NULL.");
    synchronized(lock) {
        if (removed)
            throw new IllegalStateException("Node has been removed.");

        // Copy-on-write
        PreferenceChangeListener[] old = prefListeners;
        prefListeners = new PreferenceChangeListener[old.length + 1];
        System.arraycopy(old, 0, prefListeners, 0, old.length);
        prefListeners[old.length] = pcl;
    }
    startEventDispatchThreadIfNecessary();
}

/*public*/ void removePreferenceChangeListener(PreferenceChangeListener pcl) {
    synchronized(lock) {
        if (removed)
            throw new IllegalStateException("Node has been removed.");
        if ((prefListeners == NULL) || (prefListeners.length == 0))
            throw new IllegalArgumentException("Listener not registered.");

        // Copy-on-write
        PreferenceChangeListener[] newPl =
            new PreferenceChangeListener[prefListeners.length - 1];
        int i = 0;
        while (i < newPl.length && prefListeners[i] != pcl)
            newPl[i] = prefListeners[i++];

        if (i == newPl.length &&  prefListeners[i] != pcl)
            throw new IllegalArgumentException("Listener not registered.");
        while (i < newPl.length)
            newPl[i] = prefListeners[++i];
        prefListeners = newPl;
    }
}

/*public*/ void addNodeChangeListener(NodeChangeListener ncl) {
    if (ncl==NULL)
        throw new NullPointerException("Change listener is NULL.");
    synchronized(lock) {
        if (removed)
            throw new IllegalStateException("Node has been removed.");

        // Copy-on-write
        if (nodeListeners == NULL) {
            nodeListeners = new NodeChangeListener[1];
            nodeListeners[0] = ncl;
        } else {
            NodeChangeListener[] old = nodeListeners;
            nodeListeners = new NodeChangeListener[old.length + 1];
            System.arraycopy(old, 0, nodeListeners, 0, old.length);
            nodeListeners[old.length] = ncl;
        }
    }
    startEventDispatchThreadIfNecessary();
}

/*public*/ void removeNodeChangeListener(NodeChangeListener ncl) {
    synchronized(lock) {
        if (removed)
            throw new IllegalStateException("Node has been removed.");
        if ((nodeListeners == NULL) || (nodeListeners.length == 0))
            throw new IllegalArgumentException("Listener not registered.");

        // Copy-on-write
        int i = 0;
        while (i < nodeListeners.length && nodeListeners[i] != ncl)
            i++;
        if (i == nodeListeners.length)
            throw new IllegalArgumentException("Listener not registered.");
        NodeChangeListener[] newNl =
            new NodeChangeListener[nodeListeners.length - 1];
        if (i != 0)
            System.arraycopy(nodeListeners, 0, newNl, 0, i);
        if (i != newNl.length)
            System.arraycopy(nodeListeners, i + 1,
                             newNl, i, newNl.length - i);
        nodeListeners = newNl;
    }
}
#endif
// "SPI" METHODS

/**
 * Put the given key-value association into this preference node.  It is
 * guaranteed that <tt>key</tt> and <tt>value</tt> are non-NULL and of
 * legal length.  Also, it is guaranteed that this node has not been
 * removed.  (The implementor needn't check for any of these things.)
 *
 * <p>This method is invoked with the lock on this node held.
 * @param key the key
 * @param value the value
 */
/*protected*/ /*abstract*/ void AbstractPreferences::putSpi(QString /*key*/, QString /*value*/) {}

/**
 * Return the value associated with the specified key at this preference
 * node, or <tt>NULL</tt> if there is no association for this key, or the
 * association cannot be determined at this time.  It is guaranteed that
 * <tt>key</tt> is non-NULL.  Also, it is guaranteed that this node has
 * not been removed.  (The implementor needn't check for either of these
 * things.)
 *
 * <p> Generally speaking, this method should not throw an exception
 * under any circumstances.  If, however, if it does throw an exception,
 * the exception will be intercepted and treated as a <tt>NULL</tt>
 * return value.
 *
 * <p>This method is invoked with the lock on this node held.
 *
 * @param key the key
 * @return the value associated with the specified key at this preference
 *          node, or <tt>NULL</tt> if there is no association for this
 *          key, or the association cannot be determined at this time.
 */
/*protected*/ /*abstract*/ QString AbstractPreferences::getSpi(QString key) {return "";}

/**
 * Remove the association (if any) for the specified key at this
 * preference node.  It is guaranteed that <tt>key</tt> is non-NULL.
 * Also, it is guaranteed that this node has not been removed.
 * (The implementor needn't check for either of these things.)
 *
 * <p>This method is invoked with the lock on this node held.
 * @param key the key
 */
/*protected*/ /*abstract*/ void AbstractPreferences::removeSpi(QString key) {}

/**
 * Removes this preference node, invalidating it and any preferences that
 * it contains.  The named child will have no descendants at the time this
 * invocation is made (i.e., the {@link Preferences#removeNode()} method
 * invokes this method repeatedly in a bottom-up fashion, removing each of
 * a node's descendants before removing the node itself).
 *
 * <p>This method is invoked with the lock held on this node and its
 * parent (and all ancestors that are being removed as a
 * result of a single invocation to {@link Preferences#removeNode()}).
 *
 * <p>The removal of a node needn't become persistent until the
 * <tt>flush</tt> method is invoked on this node (or an ancestor).
 *
 * <p>If this node throws a <tt>BackingStoreException</tt>, the exception
 * will propagate out beyond the enclosing {@link #removeNode()}
 * invocation.
 *
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 */
/*protected*/ /*abstract*/ void AbstractPreferences::removeNodeSpi() throw (BackingStoreException) {}

/**
 * Returns all of the keys that have an associated value in this
 * preference node.  (The returned array will be of size zero if
 * this node has no preferences.)  It is guaranteed that this node has not
 * been removed.
 *
 * <p>This method is invoked with the lock on this node held.
 *
 * <p>If this node throws a <tt>BackingStoreException</tt>, the exception
 * will propagate out beyond the enclosing {@link #keys()} invocation.
 *
 * @return an array of the keys that have an associated value in this
 *         preference node.
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 */
/*protected*/ /*abstract*/ QStringList AbstractPreferences::keysSpi() throw (BackingStoreException) {return QStringList();}
/**
 * Returns the names of the children of this preference node.  (The
 * returned array will be of size zero if this node has no children.)
 * This method need not return the names of any nodes already cached,
 * but may do so without harm.
 *
 * <p>This method is invoked with the lock on this node held.
 *
 * <p>If this node throws a <tt>BackingStoreException</tt>, the exception
 * will propagate out beyond the enclosing {@link #childrenNames()}
 * invocation.
 *
 * @return an array containing the names of the children of this
 *         preference node.
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 */
/*protected*/ /*abstract*/ QStringList AbstractPreferences::childrenNamesSpi() throw (BackingStoreException) {return QStringList();}   // throws BackingStoreException;

/**
 * Returns the named child if it exists, or <tt>NULL</tt> if it does not.
 * It is guaranteed that <tt>nodeName</tt> is non-NULL, non-empty,
 * does not contain the slash character ('/'), and is no longer than
 * {@link #MAX_NAME_LENGTH} characters.  Also, it is guaranteed
 * that this node has not been removed.  (The implementor needn't check
 * for any of these things if he chooses to override this method.)
 *
 * <p>Finally, it is guaranteed that the named node has not been returned
 * by a previous invocation of this method or {@link #childSpi} after the
 * last time that it was removed.  In other words, a cached value will
 * always be used in preference to invoking this method.  (The implementor
 * needn't maintain his own cache of previously returned children if he
 * chooses to override this method.)
 *
 * <p>This implementation obtains this preference node's lock, invokes
 * {@link #childrenNames()} to get an array of the names of this node's
 * children, and iterates over the array comparing the name of each child
 * with the specified node name.  If a child node has the correct name,
 * the {@link #childSpi(String)} method is invoked and the resulting
 * node is returned.  If the iteration completes without finding the
 * specified name, <tt>NULL</tt> is returned.
 *
 * @param nodeName name of the child to be searched for.
 * @return the named child if it exists, or NULL if it does not.
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 */
/*protected*/ AbstractPreferences* AbstractPreferences::getChild(QString nodeName)
        throw (BackingStoreException) {
    /*synchronized(lock)*/ {
        // assert kidCache.get(nodeName)==NULL;
        QStringList kidNames = childrenNames();
        for (int i=0; i<kidNames.length(); i++)
            if (kidNames.at(i) == (nodeName))
                return childSpi(kidNames.at(i));
    }
    return NULL;
}

/**
 * Returns the named child of this preference node, creating it if it does
 * not already exist.  It is guaranteed that <tt>name</tt> is non-NULL,
 * non-empty, does not contain the slash character ('/'), and is no longer
 * than {@link #MAX_NAME_LENGTH} characters.  Also, it is guaranteed that
 * this node has not been removed.  (The implementor needn't check for any
 * of these things.)
 *
 * <p>Finally, it is guaranteed that the named node has not been returned
 * by a previous invocation of this method or {@link #getChild(String)}
 * after the last time that it was removed.  In other words, a cached
 * value will always be used in preference to invoking this method.
 * Subclasses need not maintain their own cache of previously returned
 * children.
 *
 * <p>The implementer must ensure that the returned node has not been
 * removed.  If a like-named child of this node was previously removed, the
 * implementer must return a newly constructed <tt>AbstractPreferences</tt>
 * node; once removed, an <tt>AbstractPreferences</tt> node
 * cannot be "resuscitated."
 *
 * <p>If this method causes a node to be created, this node is not
 * guaranteed to be persistent until the <tt>flush</tt> method is
 * invoked on this node or one of its ancestors (or descendants).
 *
 * <p>This method is invoked with the lock on this node held.
 *
 * @param name The name of the child node to return, relative to
 *        this preference node.
 * @return The named child node.
 */
/*protected*/ /*abstract*/ AbstractPreferences* AbstractPreferences::childSpi(QString name) {return NULL;}

/**
 * Returns the absolute path name of this preferences node.
 */
/*public*/ QString AbstractPreferences::toString() {
    return (this->isUserNode() ? QString("User") : QString("System")) +
           " Preference Node: " + this->absolutePath();
}

/**
 * Implements the <tt>sync</tt> method as per the specification in
 * {@link Preferences#sync()}.
 *
 * <p>This implementation calls a recursive helper method that locks this
 * node, invokes syncSpi() on it, unlocks this node, and recursively
 * invokes this method on each "cached child."  A cached child is a child
 * of this node that has been created in this VM and not subsequently
 * removed.  In effect, this method does a depth first traversal of the
 * "cached subtree" rooted at this node, calling syncSpi() on each node in
 * the subTree while only that node is locked. Note that syncSpi() is
 * invoked top-down.
 *
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 * @throws IllegalStateException if this node (or an ancestor) has been
 *         removed with the {@link #removeNode()} method.
 * @see #flush()
 */
/*public*/ void AbstractPreferences::sync() throw (BackingStoreException) {
    sync2();
}

/*private*/ void AbstractPreferences::sync2() throw (BackingStoreException)
{
 QList<AbstractPreferences*> cachedKids;

 /*synchronized(lock) */
 {
  if (removed)
      throw new IllegalStateException("Node has been removed");
  syncSpi();
  cachedKids = cachedChildren();
 }

 for (int i=0; i<cachedKids.length(); i++)
  cachedKids.at(i)->sync2();
}

/**
 * This method is invoked with this node locked.  The contract of this
 * method is to synchronize any cached preferences stored at this node
 * with any stored in the backing store.  (It is perfectly possible that
 * this node does not exist on the backing store, either because it has
 * been deleted by another VM, or because it has not yet been created.)
 * Note that this method should <i>not</i> synchronize the preferences in
 * any subnodes of this node.  If the backing store naturally syncs an
 * entire subtree at once, the implementer is encouraged to override
 * sync(), rather than merely overriding this method.
 *
 * <p>If this node throws a <tt>BackingStoreException</tt>, the exception
 * will propagate out beyond the enclosing {@link #sync()} invocation.
 *
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 */
/*protected*/ /*abstract*/ void AbstractPreferences::syncSpi() throw (BackingStoreException) {throw BackingStoreException();}

/**
 * Implements the <tt>flush</tt> method as per the specification in
 * {@link Preferences#flush()}.
 *
 * <p>This implementation calls a recursive helper method that locks this
 * node, invokes flushSpi() on it, unlocks this node, and recursively
 * invokes this method on each "cached child."  A cached child is a child
 * of this node that has been created in this VM and not subsequently
 * removed.  In effect, this method does a depth first traversal of the
 * "cached subtree" rooted at this node, calling flushSpi() on each node in
 * the subTree while only that node is locked. Note that flushSpi() is
 * invoked top-down.
 *
 * <p> If this method is invoked on a node that has been removed with
 * the {@link #removeNode()} method, flushSpi() is invoked on this node,
 * but not on others.
 *
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 * @see #flush()
 */
/*public*/ void AbstractPreferences::flush() throw (BackingStoreException) {
    flush2();
}

/*private*/ void AbstractPreferences::flush2() throw (BackingStoreException)
{
 QList<AbstractPreferences*> cachedKids;

 /*synchronized(lock)*/
 {
  flushSpi();
  if(removed)
   return;
  cachedKids = cachedChildren();
 }

 for (int i = 0; i < cachedKids.length(); i++)
     cachedKids.at(i)->flush2();
}

/**
 * This method is invoked with this node locked.  The contract of this
 * method is to force any cached changes in the contents of this
 * preference node to the backing store, guaranteeing their persistence.
 * (It is perfectly possible that this node does not exist on the backing
 * store, either because it has been deleted by another VM, or because it
 * has not yet been created.)  Note that this method should <i>not</i>
 * flush the preferences in any subnodes of this node.  If the backing
 * store naturally flushes an entire subtree at once, the implementer is
 * encouraged to override flush(), rather than merely overriding this
 * method.
 *
 * <p>If this node throws a <tt>BackingStoreException</tt>, the exception
 * will propagate out beyond the enclosing {@link #flush()} invocation.
 *
 * @throws BackingStoreException if this operation cannot be completed
 *         due to a failure in the backing store, or inability to
 *         communicate with it.
 */
/*protected*/ /*abstract*/ void AbstractPreferences::flushSpi() throw (BackingStoreException) { throw BackingStoreException();}

/**
 * Returns <tt>true</tt> iff this node (or an ancestor) has been
 * removed with the {@link #removeNode()} method.  This method
 * locks this node prior to returning the contents of the private
 * field used to track this state.
 *
 * @return <tt>true</tt> iff this node (or an ancestor) has been
 *       removed with the {@link #removeNode()} method.
 */
/*protected*/ bool AbstractPreferences::isRemoved() {
//    synchronized(lock) {
        return removed;
//    }
}

/**
 * Queue of pending notification events.  When a preference or node
 * change event for which there are one or more listeners occurs,
 * it is placed on this queue and the queue is notified.  A background
 * thread waits on this queue and delivers the events.  This decouples
 * event delivery from preference activity, greatly simplifying
 * locking and reducing opportunity for deadlock.
 */
/*private*/ /*static*/ /*final*/ QLinkedList<EventObject*>* AbstractPreferences::eventQueue = new QLinkedList<EventObject*>();


/**
 * A single background thread ("the event notification thread") monitors
 * the event queue and delivers events that are placed on the queue.
 */
/*private*/ /*static*/ class EventDispatchThread :  QObject /*Thread*/
{
 Q_OBJECT
signals:
 void notifyPreferenceChangeEvent(PreferenceChangeEvent*);
 void notifyNodechangedEvent(NodeChangeEvent*);

public slots:
 /*public*/ void start()
 {
  while(true)
  {
   // Wait on eventQueue till an event is present
   EventObject* event = NULL;
//            synchronized(eventQueue) {
   try
   {
//                    while (AbstractPreferences::eventQueue->isEmpty())
//                        AbstractPreferences::eventQueue->wait();
       event = AbstractPreferences::eventQueue->first();
       AbstractPreferences::eventQueue->removeFirst();
   }
   catch (InterruptedException e) {
       // XXX Log "Event dispatch thread interrupted. Exiting"
       return;
   }
//            }

   // Now we have event & hold no locks; deliver evt to listeners
   AbstractPreferences* src=(AbstractPreferences*)event->getSource();
   //if (event instanceof PreferenceChangeEvent)
   if(qobject_cast<PreferenceChangeEvent*>(event) != NULL)
   {
    PreferenceChangeEvent* pce = (PreferenceChangeEvent*)event;
//                PreferenceChangeListener[] listeners = src.prefListeners();
//                for (int i=0; i<listeners.length; i++)
//                    listeners[i].preferenceChange(pce);
    emit notifyPreferenceChangeEvent(pce);
} else {
    NodeChangeEvent* nce = (NodeChangeEvent*)event;
//                NodeChangeListener[] listeners = src.nodeListeners();
//                if (nce instanceof NodeAddedEvent) {
//                    for (int i=0; i<listeners.length; i++)
//                        listeners[i].childAdded(nce);
//                } else {
//                    // assert nce instanceof NodeRemovedEvent;
//                    for (int i=0; i<listeners.length; i++)
//                        listeners[i].childRemoved(nce);
//                }
    emit notifyNodechangedEvent(nce);
   }
  }
 }
};
#if 0
/*private*/ static Thread eventDispatchThread = NULL;

/**
 * This method starts the event dispatch thread the first time it
 * is called.  The event dispatch thread will be started only
 * if someone registers a listener.
 */
/*private*/ static synchronized void startEventDispatchThreadIfNecessary() {
    if (eventDispatchThread == NULL) {
        // XXX Log "Starting event dispatch thread"
        eventDispatchThread = new EventDispatchThread();
        eventDispatchThread.setDaemon(true);
        eventDispatchThread.start();
    }
}

/**
 * Return this node's preference/node change listeners.  Even though
 * we're using a copy-on-write lists, we use synchronized accessors to
 * ensure information transmission from the writing thread to the
 * reading thread.
 */
PreferenceChangeListener[] prefListeners() {
    synchronized(lock) {
        return prefListeners;
    }
}
NodeChangeListener[] nodeListeners() {
    synchronized(lock) {
        return nodeListeners;
    }
}
#endif
/**
 * Enqueue a preference change event for delivery to registered
 * preference change listeners unless there are no registered
 * listeners.  Invoked with this.lock held.
 */
/*private*/ void AbstractPreferences::enqueuePreferenceChangeEvent(QString key, QString newValue) {

//    if (prefListeners.length() != 0) {
//        /*synchronized(eventQueue)*/ {
//            eventQueue->append(new PreferenceChangeEvent(this, key, newValue));
//            eventQueue->notify();
//        }
//    }
 emit notifyPreferenceChanged(new PreferenceChangeEvent(this, key, newValue));
}

/**
 * Enqueue a "node added" event for delivery to registered node change
 * listeners unless there are no registered listeners.  Invoked with
 * this.lock held.
 */
/*private*/ void AbstractPreferences::enqueueNodeAddedEvent(Preferences* child) {
//    if (nodeListeners->length() != 0) {
//        synchronized(eventQueue) {
            eventQueue->append(new NodeAddedEvent(this, child));
//            eventQueue->notify();
//        }
//    }
  emit notifyNodeAdded(new NodeAddedEvent(this, child));
}

/**
 * Enqueue a "node removed" event for delivery to registered node change
 * listeners unless there are no registered listeners.  Invoked with
 * this.lock held.
 */
/*private*/ void AbstractPreferences::enqueueNodeRemovedEvent(Preferences* child) {
//    if (nodeListeners.length != 0) {
//        /*synchronized(eventQueue)*/ {
//            eventQueue->append(new NodeRemovedEvent(this, child));
//            eventQueue->notify();
//        }
//    }
    emit notifyNodeRemoved(new NodeRemovedEvent(this, child));
}

/**
 * Implements the <tt>exportNode</tt> method as per the specification in
 * {@link Preferences#exportNode(OutputStream)}.
 *
 * @param os the output stream on which to emit the XML document.
 * @throws IOException if writing to the specified output stream
 *         results in an <tt>IOException</tt>.
 * @throws BackingStoreException if preference data cannot be read from
 *         backing store.
 */
/*public*/ void AbstractPreferences::exportNode(QTextStream* os)
    throw (IOException, BackingStoreException)
{
    XmlSupport::_export(os, this, false);
}

/**
 * Implements the <tt>exportSubtree</tt> method as per the specification in
 * {@link Preferences#exportSubtree(OutputStream)}.
 *
 * @param os the output stream on which to emit the XML document.
 * @throws IOException if writing to the specified output stream
 *         results in an <tt>IOException</tt>.
 * @throws BackingStoreException if preference data cannot be read from
 *         backing store.
 */
/*public*/ void AbstractPreferences::exportSubtree(QTextStream* os)
    throw (IOException, BackingStoreException)
{
    XmlSupport::_export(os, this, true);
}

