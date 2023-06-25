/**
 * Improved enum, supporting some properties as well
 */
public enum ImprovedEnum {
    ALL("all"),
    SOME("some"),
    LIMITED("limited");

    private final String name;

    /**
     * Constructor for enum to fill the list
     * @param name name value to be assigned
     */
    ImprovedEnum(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return name;
    }
}
