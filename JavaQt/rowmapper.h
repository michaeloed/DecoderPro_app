#ifndef ROWMAPPER_H
#define ROWMAPPER_H
#include <QObject>
#include "treepath.h"
#include <QVector>
/**
 * Defines the requirements for an object that translates paths in
 * the tree into display rows.
 *
 * @author Scott Violet
 */
/*public*/ class  RowMapper : QObject
{
 Q_OBJECT
public:
    /**
     * Returns the rows that the TreePath instances in <code>path</code>
     * are being displayed at. The receiver should return an array of
     * the same length as that passed in, and if one of the TreePaths
     * in <code>path</code> is not valid its entry in the array should
     * be set to -1.
     */
    virtual QVector<int> getRowsForPaths(QVector<TreePath*> /*path*/) const {return QVector<int>();}
};

#endif // ROWMAPPER_H