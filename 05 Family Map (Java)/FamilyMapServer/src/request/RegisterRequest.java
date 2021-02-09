package request;

/**
 * Created by Brandon on 7/12/2017.
 */
public class RegisterRequest
{
    private String userName;
    private String password;
    private String email;
    private String firstName;
    private String lastName;
    private String gender;

    /**
     * The constructor for a RegisterRequest.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     invariant(username, password, email, firstName, lastName, gender)
     * </pre>
     * @param userName the user name of a user
     * @param password the password of a user
     * @param email the email of a user
     * @param firstName the first name of a user
     * @param lastName the last name of a user
     * @param gender the gender of the user
     */
    public RegisterRequest(String userName, String password, String email, String firstName,
                           String lastName, String gender) throws AssertionError
    {
        this.userName = userName;
        this.password = password;
        this.email = email;
        this.firstName = firstName;
        this.lastName = lastName;
        this.gender = gender;
    }

    @Override
    public String toString()
    {
        return "RegisterRequest{" +
                "userName='" + userName + '\'' +
                ", password='" + password + '\'' +
                ", email='" + email + '\'' +
                ", firstName='" + firstName + '\'' +
                ", lastName='" + lastName + '\'' +
                ", gender='" + gender + '\'' +
                '}';
    }

    public String getUserName()
    {
        return userName;
    }


    public String getPassword()
    {
        return password;
    }

    public void setPassword(String password)
    {
        this.password = password;
    }

    public String getEmail()
    {
        return email;
    }

    public String getFirstName()
    {
        return firstName;
    }

    public String getLastName()
    {
        return lastName;
    }

    public String getGender()
    {
        return gender;
    }
}
