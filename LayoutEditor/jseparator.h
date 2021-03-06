#ifndef JSEPARATOR_H
#define JSEPARATOR_H

#include <QFrame>

class JSeparator : public QFrame
{
    Q_OBJECT
public:
    enum ORIENTATION
    {
     HORIZONTAL = 0,
     VERTICAL = 1
    };

    explicit JSeparator(enum ORIENTATION e = HORIZONTAL, QWidget *parent = 0);

signals:

public slots:

};

#endif // JSEPARATOR_H
