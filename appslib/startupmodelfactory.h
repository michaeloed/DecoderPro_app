#ifndef STARTUPMODELFACTORY_H
#define STARTUPMODELFACTORY_H
#include "jmriserviceproviderinterface.h"
#include "startupmodel.h"
#include "class.h"

/**
 * A factory for {@link apps.startup.StartupModel}s.
 *
 * StartupModelFactories are loaded via the Java {@link java.util.ServiceLoader}
 * mechanism to allow the actions JMRI can take on startup to be extended within
 * an external JAR.
 *
 * @author Randall Wood
 */
/*public*/ /*interface*/class StartupModelFactory : public  JmriServiceProviderInterface
{
 Q_OBJECT
public:
    StartupModelFactory() {}
    /**
     * Get the {@link java.lang.Class} that is generated by this factory. The
     * Class must be a subclass of {@link apps.startup.StartupModel}.
     *
     * @return The class this factory generates.
     */
    /*public*/ virtual /*Class<? extends StartupModel>*/ QString getModelClass() {return NULL;}

    /**
     * Get the description for models this factory generates. This is used in
     * the preferences UI to describe the class, so it should be short.
     *
     * @return A short description.
     */
    /*public*/ virtual QString getDescription() {return "";}

    /**
     * Get the action text for models this factory generates. This is used in
     * menus and UI elements that start the process of creating and editing a
     * new startup action.
     *
     * If the startup action is not configurable, this should be the same as
     * {@link #getDescription()}.
     *
     * @return Action text
     */
    /*public*/ virtual QString getActionText() {return "";}

    /**
     * Create a new instance of the model.
     *
     * @return the new instance
     */
    /*public*/ virtual StartupModel* newModel() {return NULL;}

    /**
     * Allow user to edit the model.
     *
     * @param model  the model to edit
     * @param parent the parent component for the editing UI
     */
    /*public*/ virtual void editModel(StartupModel* /*model*/, QWidget* /*parent*/) {}

    /**
     * Provides a mechanism for the {@link apps.StartupActionsManager} to run
     * any required post-construction initialization.
     */
    /*public*/ virtual void initialize() {}
};
#endif // STARTUPMODELFACTORY_H
