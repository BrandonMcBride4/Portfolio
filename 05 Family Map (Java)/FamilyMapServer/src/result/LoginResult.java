package result;

import model.AuthToken;

/**
 * Created by Brandon on 7/12/2017.
 */
public class LoginResult
{
    private String authToken;
    private String username;
    private String personId;

    /**
     * The success constructor for the result of the login service.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None
     * </pre>
     * @param a The object that contains the username, personID, and token for authorization
     */
    public LoginResult(AuthToken a)
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

    public String getPersonId()
    {
        return personId;
    }
}
