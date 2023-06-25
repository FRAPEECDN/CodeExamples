import lombok.Getter;
import lombok.NoArgsConstructor;

import java.util.HashMap;
import java.util.Map;

@NoArgsConstructor
public class ChildExample2 extends ParentExample {

    @Getter
    private String someText;
    private final Map<String, String> textLanguage = new HashMap<>();

    /**
     * Demo to store languages, note someText will always override in this case
     * @param languageCode language code to use
     * @param someText text to place
     */
    public void addSomeText(String languageCode, String someText) {
        if (textLanguage.containsKey(languageCode)) {
            textLanguage.replace(languageCode, someText);

        } else {
            textLanguage.put(languageCode, someText);
        }
        this.someText = someText;
    }

    @Override
    public String toString() {
        StringBuilder strBuilder = new StringBuilder();
        for (String language : textLanguage.keySet()) {
            strBuilder.append(language);
            strBuilder.append(" : ");
            strBuilder.append(textLanguage.get(language));
            strBuilder.append(";");
        }

        return  strBuilder.toString();
    }

    @Override
    public String getCurrentLanguage() {
        return "Not important";
    }

    /**
     * Disables assignment of current language
     * @param language set current language
     */
    @Override
    public void setCurrentLanguage(String language) {
    }
}
