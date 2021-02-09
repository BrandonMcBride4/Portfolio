package service;

import com.google.gson.JsonObject;
import dao.Database;
import dao.RandomData;
import model.Event;
import model.InvalidModelException;
import model.Person;
import model.User;
import result.FillResult;
import server.Handler;
import server.MissingPostException;

import java.sql.SQLException;

/**
 * Created by Brandon on 7/12/2017.
 */
public class FillService
{
    RandomData randGen = new RandomData();

    private String username;
    private int generations;
    private Database db;
    int personsAdded = 0;
    int eventsAdded = 0;

    /**
     * The constructor for the fill service.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     The user name must be a valid user name.
     *     generations must be &ge; 1 AND &le; 4
     * </pre>
     * @param username the user name of the user to fill records for
     * @param generations how many generations to fill out
     */
    public FillService(Database db, String username, int generations)
    {
        this.db = db;
        this.username = username;
        this.generations = generations;
    }

    /**
     * Populates the server's database with generated data
     * for the specified user name and number of generations.
     * @return
     * <pre>
     *  FillResult object that contains either a success or error message string.
     * </pre>
     * @throws SQLException thrown if a SQL error is thrown when accessing the database
     * @throws InvalidModelException thrown when a model object is invalid
     * @throws MissingPostException thrown when a JSON object is invalid
     */
    public FillResult fill() throws SQLException, InvalidModelException, MissingPostException
    {
        //Delete contents
        db.personDao.deleteAllPersons(username);
        db.eventDao.deleteAllEvents(username);

        //Document User
        User tempUser = db.userDao.getUser(username);
        enterUserAsPerson(tempUser);

        //Fill out all of the generations
        if (generations > 0)
            makeParents(tempUser.getPersonID(), generations);

        return new FillResult(String.format("Successfully added %d persons and %d events to the database.",
                                            personsAdded, eventsAdded));
    }

    //The user is not yet documented in the Persons and Event table, user is alive
    private void enterUserAsPerson(User u)
            throws SQLException, InvalidModelException, MissingPostException
    {
        Person p = new Person(u.getUsername(), u.getPersonID(), u.getFirstName(), u.getLastName(),
                              u.getGender(), null, null, null);
        db.personDao.createPerson(p);
        personsAdded++;

        int birthYear = computeBirthYear(0);
        int age = 2017 - birthYear;

        makeEvent(u.getPersonID(), "Birth", birthYear);
        makeEvent(u.getPersonID(), "Baptism", computeBaptismYear(birthYear));

        //High school graduation
        if (age >= 18)
            makeEvent(u.getPersonID(), "Graduation from high school", birthYear + 18);

        //College graduation
        if (age > 21)
        {
            if (isGraduatedFromCollege())
                makeEvent(u.getPersonID(), "Graduation from college",
                        randGen.getRandomInterval(birthYear + 21, 2017));
        }
    }

    //Recursively makes all parents
    private void makeParents(String childPersonID, int genLeft)
            throws SQLException, InvalidModelException, MissingPostException
    {
        //Get child
        Person child = db.personDao.getPerson(childPersonID);

        //Make parents
        Person father = makeFather(child);
        Person mother = makeMother();
        child.setFatherID(father.getPersonID());
        child.setMotherID(mother.getPersonID());
        father.setSpouseID(mother.getPersonID());
        mother.setSpouseID(father.getPersonID());

        //Enter parents in the database
        db.personDao.createPerson(father);
        db.personDao.createPerson(mother);
        personsAdded += 2;
        db.personDao.updatePerson(child);

        //Make events for parents
        int childBirthYear = db.eventDao.getEventYear(child.getPersonID(), "Birth");
        makeEvents(father.getPersonID(), childBirthYear);
        makeEvents(mother.getPersonID(), childBirthYear);

        //Make the parents' parents if called for
        if (genLeft > 1)
        {
            makeParents(child.getFatherID(), genLeft - 1);
            makeParents(child.getMotherID(), genLeft - 1);
        }
    }

    private Person makeFather(Person child) throws SQLException, InvalidModelException
    {
        String personID = RandomData.makeUniquePersonID(db);
        String firstName = randGen.getRandomMaleName();
        String lastName = child.getLastName();
        String gender = "m";
        return new Person(child.getDescendant(), personID, firstName, lastName, gender, null, null, null);
    }

    private Person makeMother() throws SQLException, InvalidModelException
    {
        String personID = RandomData.makeUniquePersonID(db);
        String firstName = randGen.getRandomFemaleName();
        String lastName = randGen.getRandomSurname();
        String gender = "f";
        return new Person(username, personID, firstName, lastName, gender, null, null, null);
    }

    private void makeEvents(String personID, int childBirthYear)
            throws SQLException, InvalidModelException, MissingPostException
    {
        //Birth
        int birthYear = computeBirthYear(childBirthYear);
        makeEvent(personID, "Birth", birthYear);

        //Death
        int age = 2018 - birthYear;
        int oldestBoundary = 2018;
        if (isDead(age))
        {
            oldestBoundary = computeDeathYear(birthYear);
            makeEvent(personID, "Death", oldestBoundary);
        }

        //Marriage
        int marriageYear = computeMarriageYear(personID, birthYear, childBirthYear);
        makeEvent(personID, "Marriage", marriageYear);

        //Baptism
        int baptismYear = computeBaptismYear(birthYear);
        makeEvent(personID, "Baptism", baptismYear);

        //High school graduation
        makeEvent(personID, "Graduation from high school", birthYear + 18);

        //College graduation
        if (isGraduatedFromCollege())
            makeEvent(personID, "Graduation from college",
                      randGen.getRandomInterval(birthYear + 21, birthYear + 30));

        //Second Marriage after spouse death
        int secondMarriageYear = computeSecondMarriageYear(personID, oldestBoundary);
        if (isMarriedTwice(secondMarriageYear))
            makeEvent(personID, "Marriage", secondMarriageYear);
    }

    private void makeEvent(String personID, String eventType, int year)
            throws SQLException, InvalidModelException, MissingPostException
    {
        String eventID = RandomData.makeUniqueEventID(db);
        JsonObject location = randGen.getRandomLocation();
        String country = Handler.getBodyPart(location, "country");
        String city = Handler.getBodyPart(location, "city");
        double latitude = Double.parseDouble(Handler.getBodyPart(location, "latitude"));
        double longitude = Double.parseDouble(Handler.getBodyPart(location, "longitude"));
        Event event = new Event(eventID, username, personID, latitude, longitude, country, city, eventType, year);
        db.eventDao.createEvent(event);
        eventsAdded++;
    }

    //If no child then childBirthYear = 0,
    //Else the child was born when parent was between the ages of 20 and 40
    private int computeBirthYear(int childBirthYear)
    {
        if (childBirthYear == 0)
            return randGen.getRandomInterval(2017-80,2017-12);
        else
            return childBirthYear - randGen.getRandomInterval(20, 40);
    }

    //Chances of dying at certain ages
    private boolean isDead(int age)
    {
        if (age >= 105)
            return true;
        else if (age >= 100)
            return chances(100,60);
        else if (age >= 90)
            return chances(100, 15);
        else if (age >= 80)
            return (chances(100, 3) || chances(100, 4));
        else if (age >= 70)
            return chances(100, 2);
        else if (age >= 60)
            return !chances(100, 12);
        else if (age >= 50)
            return !chances(100, 15);
        else if (age >= 40)
            return !chances(100, 30);
        else
            return false;
    }

    //Random year between age 40 and 100
    private int computeDeathYear(int birthday)
    {
        int year = 2019;
        while (year > 2018)
            year = randGen.getRandomInterval(birthday + 40, birthday + 100);
        return year;
    }

    //Marriage date lines up with spouse
    private int computeMarriageYear(String personID, int birthYear, int childBirthYear)
            throws SQLException, InvalidModelException
    {
        String spouseID = db.personDao.getPerson(personID).getSpouseID();
        int marriageYear = db.eventDao.getEventYear(spouseID, "Marriage");
        if (marriageYear == 0)
            marriageYear = randGen.getRandomInterval(birthYear + 18, childBirthYear);
        return marriageYear;
    }

    //Random year between age 0 and 40
    private int computeBaptismYear(int birthYear)
    {
        int age = 2017 - birthYear;
        if (age < 40)
            return randGen.getRandomInterval(birthYear, 2017);
        else
            return randGen.getRandomInterval(birthYear, birthYear + 40);
    }

    //Random year between the person's death and their spouse's death, else zero if not the case
    private int computeSecondMarriageYear(String personID, int deathYear)
            throws SQLException, InvalidModelException
    {
        String spouseID = db.personDao.getPerson(personID).getSpouseID();
        int spouseDeathYear = db.eventDao.getEventYear(spouseID, "Death");
        if (spouseDeathYear == 0)
            return spouseDeathYear;
        if (spouseDeathYear >= deathYear)
            return 0;
        return randGen.getRandomInterval(spouseDeathYear, deathYear);

    }

    //50% chance this person graduated from college
    private boolean isGraduatedFromCollege()
    {
        return chances(100, 2);
    }

    //If spouse died before person, there might be a chance this person married again
    private boolean isMarriedTwice(int secondMarriageYear)
    {
        if (secondMarriageYear == 0)
            return false;
        return (chances(100, 2) || chances(100, 3));
    }

    //Generates random probability
    private boolean chances(int bound, int mod)
    {
        if (randGen.getRandomInterval(1, bound) % mod == 0)
            return true;
        else
            return false;
    }
}
