package service;

import dao.Database;
import model.Event;
import model.InvalidModelException;
import model.Person;
import model.User;
import request.LoadRequest;
import result.LoadResult;

import java.sql.SQLException;

/**
 * Created by Brandon on 7/12/2017.
 */
public class LoadService
{
    Database db = null;
    /**
     * The constructor for the load service
     */
    public LoadService(Database db)
    {
        this.db = db;
    }

    /**
     * Clears all data from the database, and then loads the
     * posted user, person, and event data into the database.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     The arrays must contain valid objects
     * </pre>
     * @return
     * <pre>
     *     LoadResult object that contains either a success or error message string.
     * </pre>
     * @param r contains an arrays of User, Person, and Event objects
     * @throws SQLException thrown if a SQL error is thrown when accessing the database
     * @throws InvalidModelException thrown when a model object is invalid
     */
    public LoadResult load(LoadRequest r) throws SQLException, InvalidModelException
    {
        //Add each User in the User[] into the database.
        for (int i = 0; i < r.getUsers().length; i++)
        {
            User user = r.getUsers()[i];
            if (db.userDao.isUser(user.getUsername()))
                throw new InvalidModelException(String.format("duplicate username - %s - found",
                                                              user.getUsername()), "User");
            db.userDao.createUser(user);
        }

        //Add each Person in the Person[] into the database.
        for (int i = 0; i < r.getPersons().length; i++)
        {
            Person person = r.getPersons()[i];
            if (db.personDao.isPerson(person.getPersonID()))
                throw new InvalidModelException(String.format("duplicate personID - %s - found",
                                                              person.getPersonID()), "Person");
            db.personDao.createPerson(person);
        }

        //Add each Event in the Event[] into the database.
        for (int i = 0; i < r.getEvents().length; i++)
        {
            Event event = r.getEvents()[i];
            //Check for duplicate eventID
            if (db.eventDao.isEvent(event.getEventID()))
                throw new InvalidModelException(String.format("duplicate eventID - %s - found",
                                                              event.getEventID()), "Event");
            //Check if personID is valid
            if (!db.personDao.isPerson(event.getPersonID()))
            {
                String error = String.format("event - %s - does not correlate with a valid personID",
                                             event.getEventID());
                throw new InvalidModelException(error, "Event");
            }
            
            db.eventDao.createEvent(event);
        }

        String result = String.format("Successfully added %d users, %d persons, and %d events to the database.",
                                      r.getUsers().length, r.getPersons().length, r.getEvents().length);
        return new LoadResult(result);
    }
}
