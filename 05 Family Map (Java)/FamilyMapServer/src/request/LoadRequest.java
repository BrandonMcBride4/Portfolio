package request;

import model.*;

/**
 * Created by bmcbrid4 on 7/13/2017.
 */
public class LoadRequest
{
    private User[] users;
    private Person[] persons;
    private Event[] events;

    /**
     * The constructor for a LoadRequest.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     invariant(users, persons, events)
     * </pre>
     * @param users array of User objects
     * @param persons array of Person objects
     * @param events array of Event objects
     */
    public LoadRequest(User[] users, Person[] persons, Event[] events)
    {
        this.users = users;
        this.persons = persons;
        this.events = events;
    }

    public User[] getUsers()
    {
        return users;
    }

    public Person[] getPersons()
    {
        return persons;
    }

    public Event[] getEvents()
    {
        return events;
    }

}
