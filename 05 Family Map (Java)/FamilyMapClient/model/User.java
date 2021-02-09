package edu.byu.cs.students.familymap.model;

import com.google.gson.JsonObject;

import edu.byu.cs.students.familymap.server.JsonConverter;
import edu.byu.cs.students.familymap.server.MissingPostException;


/**
 * Created by Brandon on 7/12/2017.
 */
public class User
{
    private String username;
    private String password;
    private String email;
    private String firstName;
    private String lastName;
    private String gender;
    private String personID;
    private String error;

    /**
     * The constructor for a User.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     invariant(username, password, email, firstName, lastName, gender, personID)
     * </pre>
     * @param username the user name of a user
     * @param password the password of a user
     * @param email the email of a user
     * @param firstName the first name of a user
     * @param lastName the last name of a user
     * @param gender the gender of the user
     * @param personID the personID of the user
     * @throws InvalidModelException thrown when invariant is false
     */
    public User(String username, String password, String email, String firstName,
                String lastName, String gender, String personID) throws InvalidModelException
    {
        gender = gender.toLowerCase();
        if (!invariant(username, password, email, firstName, lastName, gender, personID))
            throw new InvalidModelException(error, "User");
        this.username = username;
        this.password = password;
        this.email = email;
        this.firstName = firstName;
        this.lastName = lastName;
        this.gender = gender;
        this.personID = personID;
    }

    public static User jsonToUser(JsonObject userJsonObject) throws MissingPostException, InvalidModelException
    {
        String userName = JsonConverter.getMandatoryBodyPart(userJsonObject, "userName");
        String password = JsonConverter.getMandatoryBodyPart(userJsonObject, "password");
        String email = JsonConverter.getMandatoryBodyPart(userJsonObject, "email");
        String firstName = JsonConverter.getMandatoryBodyPart(userJsonObject, "firstName");
        String lastName = JsonConverter.getMandatoryBodyPart(userJsonObject, "lastName");
        String gender = JsonConverter.getMandatoryBodyPart(userJsonObject, "gender");
        String personID = JsonConverter.getMandatoryBodyPart(userJsonObject, "personID");

        User user = new User(userName, password, email, firstName, lastName, gender, personID);
        return user;
    }

    @Override
    public String toString()
    {
        return "User{" +
                "username='" + username + '\'' +
                ", password='" + password + '\'' +
                ", email='" + email + '\'' +
                ", firstName='" + firstName + '\'' +
                ", lastName='" + lastName + '\'' +
                ", gender='" + gender + '\'' +
                ", personID='" + personID + '\'' +
                '}';
    }

    public String getUsername()
    {
        return username;
    }

    public String getPassword()
    {
        return password;
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

    public String getPersonID()
    {
        return personID;
    }

    public void setUsername(String username)
    {
        this.username = username;
    }

    public void setPassword(String password)
    {
        this.password = password;
    }

    public void setEmail(String email)
    {
        this.email = email;
    }

    public void setFirstName(String firstName)
    {
        this.firstName = firstName;
    }

    public void setLastName(String lastName)
    {
        this.lastName = lastName;
    }

    public void setGender(String gender)
    {
        this.gender = gender;
    }

    public void setPersonID(String personID)
    {
        this.personID = personID;
    }

    /**
     * Checks if the parameters of the constructor are valid.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None
     * </pre>
     * @return
     * <pre>
     *     true if username, password, email, firstName, lastName, gender, and personID != ""
     *     AND gender = "f" OR gender = "m"
     *     false otherwise
     * </pre>
     * @param username the user name of a user
     * @param password the password of a user
     * @param email the email of a user
     * @param firstName the first name of a user
     * @param lastName the last name of a user
     * @param gender the gender of the user
     * @param personID the personID of the user
     */
    private boolean invariant(String username, String password, String email, String firstName,
                              String lastName, String gender, String personID)
    {
        if(username == null)
        {
            error = "username is null";
            return false;
        }
        if(password == null)
        {
            error = "password is null for " + username;
            return false;
        }
        if(email == null)
        {
            error = "email is null for " + username;
            return false;
        }
        if(firstName == null)
        {
            error = "firstName is null for " + username;
            return false;
        }
        if(lastName == null)
        {
            error = "lastName is null for " + username;
            return false;
        }
        if(gender == null)
        {
            error = "gender is null for " + username;
            return false;
        }
        if(gender.length() != 1)
        {
            error = "gender is too many characters for " + username;
            return false;
        }
        if(gender.charAt(0) != 'm' && gender.charAt(0) != 'f')
        {
            error = "gender needs to be m or f for " + username;
            return false;
        }
        if(personID == null)
        {
            error = "personID is null for " + username;
            return false;
        }
        return true;
    }
}
