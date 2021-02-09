package request;

/**
 * Created by Brandon on 7/12/2017.
 */
public class LoginRequest
{
    private String userName;
    private String password;

    /**
     * The constructor for a LoginRequest.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None, Already checked before
     * </pre>
     * @param userName the user name of a user
     * @param password the password of a user
     */
    public LoginRequest(String userName, String password)
    {
        this.userName = userName;
        this.password = password;
    }

    public String getUserName()
    {
        return userName;
    }

    public String getPassword()
    {
        return password;
    }

}
