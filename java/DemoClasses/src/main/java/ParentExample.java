import lombok.RequiredArgsConstructor;

import java.util.Locale;
import java.util.UUID;

/**
 * Parent base class for the 2 children classes
 */
@RequiredArgsConstructor
public abstract class ParentExample implements SupportId, SupportLanguage {

    protected UUID publicId;
    protected int privateId;
    protected Locale currentLocale;

    @Override
    public String getPublicId() {
        return publicId.toString();
    }

    @Override
    public void setPublicId(String value) {
        try {
            publicId = UUID.fromString(value);
        } catch (IllegalArgumentException ignored) {
            publicId = null;
        }
    }

    @Override
    public String getPrivateId() {
        return Integer.toString(privateId);
    }

    @Override
    public void setPrivateId(String value) {
        try {
            Integer.parseInt(value);
        } catch (NumberFormatException ignore) {
            privateId = 0;
        }
    }

    @Override
    public String getCurrentLanguage() {
        return currentLocale.getLanguage();
    }

    @Override
    public void setCurrentLanguage(String language) {
        currentLocale = new Locale(language);
    }

}
