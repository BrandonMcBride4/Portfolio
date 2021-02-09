package edu.byu.cs.students.familymap.model;

import com.google.gson.JsonObject;

import java.io.Serializable;

import edu.byu.cs.students.familymap.server.JsonConverter;
import edu.byu.cs.students.familymap.server.MissingPostException;

/**
 * Created by Brandon on 7/12/2017.
 */
public class Person implements Serializable
{
    private String personID;
    private String descendant;
    private String firstName;
    private String lastName;
    private String gender;
    private String fatherID = null;
    private String motherID = null;
    private String spouseID = null;
    private String error;
    private int generations;

    /**
     * The constructor for a Person.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     invariant(personID, descendant, firstName, lastName, gender, fatherID, motherID, spouseID)
     * </pre>
     * @param personID the ID of a person
     * @param descendant the user (Username) to which this person belongs
     * @param firstName the first name of a person
     * @param lastName the last name of a person
     * @param gender the gender of a person
     * @param fatherID the ID of the father of a person
     * @param motherID the ID of the mother of a person
     * @param spouseID the ID of the spouse of a person
     * @throws InvalidModelException thrown when invariant is false
     */
    public Person(String descendant, String personID, String firstName, String lastName,
                  String gender, String fatherID, String motherID, String spouseID)
                  throws InvalidModelException
    {
        gender = gender.toLowerCase();
        if (!invariant(descendant, personID, firstName, lastName, gender, fatherID, motherID, spouseID))
            throw new InvalidModelException(error, "Person");
        this.descendant = descendant;
        this.personID = personID;
        this.firstName = firstName;
        this.lastName = lastName;
        this.gender = gender;
        this.fatherID = fatherID;
        this.motherID = motherID;
        this.spouseID = spouseID;
    }

    public static Person jsonToPerson(JsonObject userJsonObject) throws MissingPostException, InvalidModelException
    {
        String descendant = JsonConverter.getMandatoryBodyPart(userJsonObject, "descendant");
        String personID = JsonConverter.getMandatoryBodyPart(userJsonObject, "personID");
        String firstName = JsonConverter.getMandatoryBodyPart(userJsonObject, "firstName");
        String lastName = JsonConverter.getMandatoryBodyPart(userJsonObject, "lastName");
        String gender = JsonConverter.getMandatoryBodyPart(userJsonObject, "gender");
        String father = JsonConverter.getOptionalBodyPart(userJsonObject, "fatherID");
        String mother = JsonConverter.getOptionalBodyPart(userJsonObject, "motherID");
        String spouse = JsonConverter.getOptionalBodyPart(userJsonObject, "spouseID");

        Person person = new Person(descendant, personID, firstName,
                                   lastName, gender, father, mother, spouse);
        return person;
    }

    public String getPersonID()
    {
        return personID;
    }

    public String getDescendant()
    {
        return descendant;
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

    public String getFatherID()
    {
        return fatherID;
    }

    public String getMotherID()
    {
        return motherID;
    }

    public String getSpouseID()
    {
        return spouseID;
    }

    public void setPersonID(String personID)
    {
        this.personID = personID;
    }

    public void setDescendant(String descendant)
    {
        this.descendant = descendant;
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

    public void setFatherID(String fatherID)
    {
        this.fatherID = fatherID;
    }

    public void setMotherID(String motherID)
    {
        this.motherID = motherID;
    }

    public void setSpouseID(String spouseID)
    {
        this.spouseID = spouseID;
    }

    /**
     * Checks if the parameters of the constructor are valid.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None
     * </pre>
     * @return
     * <pre>
     *     true if personID, descendant, firstName, lastName != ""
     *     AND gender = "f" OR gender = "m"
     *     AND descnedant needs to be a valid username
     *     false otherwise
     * </pre>
     * @param personID the ID of a person
     * @param descendant the user (Username) to which this person belongs
     * @param firstName the first name of a person
     * @param lastName the last name of a person
     * @param gender the gender of a person
     * @param fatherID the ID of the father of a person
     * @param motherID the ID of the mother of a person
     * @param spouseID the ID of the spouse of a person
     */
    private boolean invariant(String descendant, String personID, String firstName, String lastName,
                              String gender, String fatherID, String motherID, String spouseID)
    {
        if (personID == null)
        {
            error = " is null";
            return false;
        }
        if (descendant == null)
        {
            error = "descendant is null for " + personID;
            return false;
        }
        if (firstName == null)
        {
            error = "firstName is null for " + personID + " for user: " + descendant;
            return false;
        }
        if (lastName == null)
        {
            error = "lastName is null for " + personID + " for user: " + descendant;
            return false;
        }
        if (gender == null)
        {
            error = "gender is null for " + personID + " for user: " + descendant;
            return false;
        }
        if(gender.length() != 1)
        {
            error = "gender is too many characters for " +
                    personID + " for user: " + descendant;
            return false;
        }
        if(gender.charAt(0) != 'm' && gender.charAt(0) != 'f')
        {
            error = "gender needs to be m or f for " +
                    personID + " for user: " + descendant;
            return false;
        }
        return true;
    }

    public String getFullName()
    {
       return getFirstName() + " " + getLastName();
    }

    public int getGenerations()
    {
        return generations;
    }

    public void setGenerations(int generations)
    {
        this.generations = generations;
    }
}
