package service;

import dao.Database;
import model.Event;
import model.InvalidModelException;
import result.AllEventsResult;

import java.sql.SQLException;
import java.util.List;

/**
 * Created by bmcbrid4 on 7/17/2017.
 */
public class EventService
{
    private Database db;

    public EventService(Database db)
    {
        this.db = db;
    }

    /**
     * Returns the single Event object with the specified ID.
     *<pre>
     *     <b>Constraints on the input</b>:
     *     Valid eventID.
     * </pre>
     * @return
     * <pre>
     *     If no error, returns an object with all the data about the event, else it returns an error message.
     * </pre>
     * @param eventID the ID of the event we want info about
     * @param username the username of the user that the event should be linked to
     */
    public Event getEvent(String eventID, String username)
            throws SQLException, InvalidModelException
    {
        return db.eventDao.getEvent(eventID, username);
    }

    /**
     * Returns ALL events for ALL family members of the current user. The current
     * user is determined from the provided auth token.
     *
     * @return
     * <pre>
     *     If no error, returns an array of all Event objects in the database, else it returns an error message.
     * </pre>
     * @param username The username of the user that is retrieving all of the events
     * @throws SQLException thrown if a SQL error is thrown when accessing the database
     * @throws InvalidModelException thrown when a model object is invalid
     */
    public AllEventsResult getAllEvents(String username)throws SQLException, InvalidModelException
    {
        List<Event> events = db.eventDao.getAllEvents(username);
        return new AllEventsResult(events);
    }
}
