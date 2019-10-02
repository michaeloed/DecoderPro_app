#ifndef JUNITUTIL_H
#define JUNITUTIL_H

#include <QObject>
#include "exceptions.h"

class ReleaseUntil;
class Logger;
class Profile;
class JUnitUtil : public QObject
{
    Q_OBJECT
public:
    explicit JUnitUtil(QObject *parent = nullptr);
    static /*final*/ /*public*/ int DEFAULT_RELEASETHREAD_DELAY;// = 50;
    static /*final*/ /*public*/ int WAITFOR_DELAY_STEP;// = 5;
    static /*final*/ /*public*/ int WAITFOR_MAX_DELAY;// = 10000;
    /*public*/ static void setUp();
    /*public*/ static void tearDown();
    /*public*/ static void resetProfileManager();
    /*public*/ static void initRosterConfigManager();
    /*public*/ static void resetProfileManager(Profile* profile);
    static /*public*/ void waitFor(ReleaseUntil *condition, QString name);
    static /*public*/ bool waitFor(ReleaseUntil* condition);
    static /*public*/ void waitFor(int time);
    /*public*/ static void runTests(QObject *test, QStringList testList);
    static QString getTestClassName();

signals:

public slots:

private:
    /*private*/ /*final*/ static Logger* log;// = LoggerFactory.getLogger(JUnitUtil.class);
    static bool printSetUpTearDownNames;// = bool.getbool("jmri.util.JUnitUtil.printSetUpTearDownNames"); // false unless set true

    /**
     * When true, checks that calls to setUp and tearDown properly alterante, printing an
     * error message with context information on System.err if inconsistent calls are observed.
     * <p>
     * Set from the jmri.util.JUnitUtil.checkSetUpTearDownSequence environment variable.
     */
    static bool checkSetUpTearDownSequence;// = bool.getbool("jmri.util.JUnitUtil.checkSetUpTearDownSequence"); // false unless set true

    /**
     * Adds extensive error information to the output of checkSetUpTearDownSequence.
     * Note: The context checking and storage required for this takes a lot of time.
     * <p>
     * Set from the jmri.util.JUnitUtil.checkSequenceDumpsStack environment variable.
     */
    static bool checkSequenceDumpsStack;// =    bool.getbool("jmri.util.JUnitUtil.checkSequenceDumpsStack"); // false unless set true

    /**
     * Check for any threads left behind after a test calls {@link #tearDown}
     * <p>
     * Set from the jmri.util.JUnitUtil.checkRemnantThreads environment variable.
     */
    static bool checkRemnantThreads;// =    bool.getbool("jmri.util.JUnitUtil.checkRemnantThreads"); // false unless set true

    /**
     * Check for tests that take an excessive time
     * <p>
     * Set from the jmri.util.JUnitUtil.checkTestDuration environment variable.
     */
    static bool checkTestDuration;// =      bool.getbool("jmri.util.JUnitUtil.checkTestDuration"); // false unless set true
    static long    checkTestDurationMax;// =   Long.getLong("jmri.util.JUnitUtil.checkTestDurationMax", 5000); // milliseconds

    static qint64   checkTestDurationStartTime;// = 0;  // working value

    static /*private*/ int threadCount;// = 0;

    static /*private*/ bool didSetUp;// = false;
    static /*private*/ bool didTearDown;// = true;
    static /*private*/ QString lastSetUpClassName;// = "<unknown>";
    static /*private*/ QString lastSetUpThreadName;// = "<unknown>";
//    static /*private*/ StackTraceElement[] lastSetUpStackTrace = new StackTraceElement[0];
    static /*private*/ QString lastTearDownClassName;// = "<unknown>";
    static /*private*/ QString lastTearDownThreadName;// = "<unknown>";
//    static /*private*/ StackTraceElement[] lastTearDownStackTrace = new StackTraceElement[0];

    static /*private*/ bool isLoggingInitialized;// = false;
    static /*private*/ QString initPrefsDir;// = null;
    static QString testClassName;
};

class /*interface*/ ReleaseUntil : public QObject
{
    Q_OBJECT
public:
    /*public*/ bool ready() throw (Exception) {}

};
#endif // JUNITUTIL_H
