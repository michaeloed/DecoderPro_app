#ifndef INSTANCEINITIALIZER_H
#define INSTANCEINITIALIZER_H
#include <QObject>
#include "libPr3_global.h"

class LIBPR3SHARED_EXPORT InstanceInitializer : public QObject
{
 Q_OBJECT
public:
    //InstanceInitializer : QObject() {}
/**
 * Interface providing initialization of specific objects
 * by default.  This is used to move references to
 * specific subtypes out of the jmri package and into
 * e.g. jmri.managers.
 *<P>
 * Note that this is only needed when the object can't be
 * created with a no-arguments construtor.  In that case,
 * the {@link InstanceManagerAutoDefault} mechanism is a better choice.
 *
 * <P>
 * JMRI is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * <P>
 * @author			Bob Jacobsen Copyright (C) 2010
 * @version			$Revision: 22528 $
 * @since           2.9.4
 */
//public interface InstanceInitializer {

//public <T> Object getDefault(Class<T> type);
 virtual QObject* getDefault(QString type) const =0;

};
Q_DECLARE_INTERFACE(InstanceInitializer, "InstanceInitializer")
#endif // INSTANCEINITIALIZER_H
