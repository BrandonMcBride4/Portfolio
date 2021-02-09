package service;

import dao.Database;
import model.InvalidModelException;
import model.Person;
import result.AllPersonsResult;

import java.sql.SQLException;
import java.util.List;

/**
 * Created by bmcbrid4 on 7/17/2017.
 */
public class PersonService
{
    private Database db;

    public PersonService(Database db)
    {
        this.db = db;
    }

    /**
     * Returns the single Person object with the specified ID.
     *<pre>
     *     <b>Constraints on the input</b>:
     *     Valid personID.
     * </pre>
     * @return
     * <pre>
     *     If no error, it returns an object with all the data about the person, else it returns null.
     * </pre>
     * @param personID the ID of the person we want the info about
     * @param username the username of the user
     */
    public Person getPerson(String personID, String username)
            throws SQLException, InvalidModelException
    {
        //person/[username]
        return db.personDao.getPerson(personID, username);
    }

    /**
     * Returns ALL family members of the current user. The current user is
     * determined from the provided auth token.
     *
     * @return
     * <pre>
     *     If no error, returns an array of all Person objects in the database, else it returns null.
     * </pre>
     * @param username the username of the user requesting all of the related Persons
     * @throws SQLException thrown if a SQL error is thrown when accessing the database
     * @throws InvalidModelException thrown when a model object is invalid
     */
    public AllPersonsResult getAllPersons(String username) throws SQLException, InvalidModelException
    {
        //person/
        List<Person> persons = db.personDao.getAllPersons(username);
        return new AllPersonsResult(persons);
    }
}
