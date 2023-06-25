import lombok.*;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

@EqualsAndHashCode(callSuper = true)
@NoArgsConstructor
public class ChildExample1 extends ParentExample {

    @Getter
    @Setter
    private ImprovedEnum improvedEnum;

    @Getter
    private String someText;

    private final Map<String, String> textLanguage = new HashMap<>();

    /**
     * Assign a new text value to the language identified
     * @param language language code
     * @param text that will be used
     */
    void addText(String language, String text) {
        textLanguage.putIfAbsent(language, text);
        if (Objects.equals(language, currentLocale.getLanguage())) {
            someText = text;
        }
    }
}
