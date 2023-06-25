/**
 * Interface for object to support a private and public id to identify the object
 */
public interface SupportId {

    /**
     * @return public identifier
     */
    String getPublicId();

    /**
     * @param value new public identifier
     */
    void setPublicId(String value);

    /**
     * @return private identifier
     */
    String getPrivateId();

    /**
     * @param value new private identifier
     */
    void setPrivateId(String value);

}
