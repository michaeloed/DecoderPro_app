#ifndef PANEOPSPROGFRAME_H
#define PANEOPSPROGFRAME_H
#include "paneprogframe.h"
#include "libPr3_global.h"

class LIBPR3SHARED_EXPORT PaneOpsProgFrame : public PaneProgFrame
{
    Q_OBJECT
public:
    //explicit PaneOpsProgFrame(QWidget *parent = 0);
    /*public*/ PaneOpsProgFrame(DecoderFile* decoderFile, RosterEntry* r,
                            QString name, QString file, Programmer* p, QWidget *parent = 0);
    void addHelp();
    /*public*/ void dispose();

signals:

public slots:
private:
    QWidget* modePane;
    Logger* log;
protected:
    /*protected*/ QWidget *getModePane() override;
    /*protected*/ void pickProgrammerMode(/*@NonNull*/ QDomElement programming);

};

#endif // PANEOPSPROGFRAME_H
