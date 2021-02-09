package result;

import model.AuthToken;

/**
 * Created by Brandon on 7/12/2017.
 */
public class RegisterResult
{
    private String authToken;
    private String username;
    private String personId;

    /**
     * The success constructor for the result of the register service.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None
     * </pre>
     * @param a the unique token linked to a user, contains username, personID, and the token
     */
    public RegisterResult(AuthToken a)
    {
        this.authToken = a.getAuthToken();
        this.username = a.getUserName();
        this.personId = a.getPersonID();
    }

    public String getAuthToken()
    {
        return authToken;
    }

    public String getUserName()
    {
        return username;
    }

    public String getPersonID()
    {
        return personId;
    }
}
