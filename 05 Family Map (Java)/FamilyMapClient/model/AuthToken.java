package edu.byu.cs.students.familymap.model;

/**
 * Created by Brandon on 7/12/2017.
 */
public class AuthToken
{
    private String authToken;
    private String username;
    private String personID;

    /**
     * The constructor for an AuthToken.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     invariant(authToken, username)
     * </pre>
     * @param authToken the unique token linked to a user
     * @param username the username of the user the auth token belongs
     * @param personID the ID of the user
     */
    public AuthToken(String authToken, String username, String personID)
    {
        this.authToken = authToken;
        this.username = username;
        this.personID = personID;
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
        return personID;
    }

    public void setAuthToken(String authToken)
    {
        this.authToken = authToken;
    }

    public void setUserName(String username)
    {
        this.username = username;
    }

    public void setPersonID(String personID)
    {
        this.personID = personID;
    }
}
