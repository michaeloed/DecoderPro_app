#ifndef WATCHINGLABEL_H
#define WATCHINGLABEL_H

#include <QLabel>
#include "libPr3_global.h"

class LIBPR3SHARED_EXPORT WatchingLabel : public QLabel
{
    Q_OBJECT
public:
    //explicit WatchingLabel(QWidget *parent = 0);
    /*public*/ WatchingLabel(QString name, QWidget* c, QWidget *parent = 0);

signals:

public slots:
private:
    QWidget* comp;
    QWidget* self;
};

#endif // WATCHINGLABEL_H
