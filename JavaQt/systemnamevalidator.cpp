#include "systemnamevalidator.h"
#include "manager.h"

SystemNameValidator::SystemNameValidator(QObject *parent) : InputVerifier()
{

}
/**
 * A {@link com.alexandriasoftware.swing.JInputValidator} that validates a
 * {@link jmri.NamedBean} system name.
 * <p>
 * Until the component gets focus, no validation icon is shown. Once the
 * component has focus the following icons are shown:
 * <ul>
 * <li>If the component is blank and required was false when the validator was
 * created, no validation is shown.</li>
 * <li>If the component is blank and required was true when the validator was
 * created, a warning icon is shown.</li>
 * <li>If the component has an invalid system name, an error icon is shown.</li>
 * <li>If the component has a potentially valid system name, a waring icon is
 * shown.</li>
 * <li>If the component has a valid system name, a success icon is shown.</li>
 * </ul>
 *
 * @author Randall Wood Copyright 2019
 */
// /*public*/ class SystemNameValidator extends JInputValidator {


    /**
     * Create a SystemNameValidator. Same as calling
     * {@link #SystemNameValidator(javax.swing.JComponent, jmri.Manager, boolean)}
     * with {@code required == false}.
     *
     * @param component the component to validate has a valid system name
     * @param manager   the manager that will be used for validation
     */
//    /*public*/ SystemNameValidator(/*@Nonnull*/ /*JComponent*/QWidget* component, /*@Nonnull*/ Manager* manager) {
//        this(component, manager, false);
//    }

    /**
     * Create a SystemNameValidator.
     *
     * @param component the component to validate has a valid system name
     * @param manager   the manager that will be used for validation
     * @param required  true if input must be valid and
     *                  {@link javax.swing.InputVerifier#verify(javax.swing.JComponent)}
     *                  must return true to allow focus change; false otherwise
     */
    /*public*/ SystemNameValidator::SystemNameValidator(/*@Nonnull*/ /*JComponent*/QWidget* component, /*@Nonnull*/ Manager* manager, bool required) {
        //super(component, true, required);
        setManager(manager);
        this->required = required;
    }
#if 0
    //@Override
    /*protected*/ Validation getValidation(/*JComponent*/QWidget* component, JInputValidatorPreferences preferences) {
        if (component instanceof JTextComponent) {
            JTextComponent jtc = (JTextComponent) component;
            String text = jtc.getText();
            if (text != null && !text.isEmpty()) {
                try {
                    if (manager instanceof ProxyManager) {
                        ProxyManager<?> proxyManager = (ProxyManager<?>) manager;
                        proxyManager.validateSystemNameFormat(text);
                    } else {
                        manager.makeSystemName(text, false);
                    }
                } catch (NamedBean.BadSystemNameException ex) {
                    if (manager.validSystemNameFormat(text) == NameValidity.VALID_AS_PREFIX_ONLY) {
                        return new Validation(Validation.Type.WARNING, Bundle.getMessage("SystemNameValidatorValidPrefix", text, manager.getBeanTypeHandled(),
                                trimHtmlTags(getToolTipText())), preferences);
                    }
                    return new Validation(Validation.Type.DANGER, Bundle.getMessage("SystemNameValidatorInvalid", ex.getMessage(), trimHtmlTags(getToolTipText())), preferences);
                }
                return new Validation(Validation.Type.SUCCESS, getToolTipText(), preferences);
            }
        }
        if (required) {
            return new Validation(Validation.Type.WARNING, Bundle.getMessage("SystemNameValidatorRequired", trimHtmlTags(getToolTipText())), preferences);
        }
        return getNoneValidation();
    }
#endif
    /*public*/ bool SystemNameValidator::isRequired() {
        return required;
    }

    /*public*/ void SystemNameValidator::setRequired(bool required) {
        this->required = required;
    }

    /**
     * Set the Manager used to validate system names.
     * <p>
     * If the manager changes, fires the a property change for the property
     * {@code manager} and calls {@link #verify(javax.swing.JComponent)} to
     * verify any text against the new manager.
     *
     * @param manager the new manager
     */
    /*public*/ void SystemNameValidator::setManager(/*@Nonnull*/ Manager* manager) {
//        Objects.requireNonNull(manager, "Cannot validate against a null manager");
        Manager* old = this->manager;
        if (old == nullptr || old != (manager)) {
            this->manager = manager;
#if 0
            getPropertyChangeSupport().firePropertyChange("manager", old, this.manager);
            verify(getComponent());
#endif
        }
    }


