package dao;

import model.Event;
import model.InvalidModelException;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by Brandon on 7/12/2017.
 */
public class EventDao
{
    Connection connection = null;

    public EventDao(Connection connection)
    {
        this.connection = connection;
    }

    /**
     * Creates a record for the event in the event table in the database.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None, the Event object should be validated when it is created
     * </pre>
     * @param e the Event object with the information about the event
     */
    public void createEvent(Event e) throws SQLException
    {
        //fill/...
        //load/
        PreparedStatement stmt = null;
        String insertEvent = "insert into Events (eventID, descendant, personID, latitude, longitude," +
                             "country, city, eventType, year) values (?, ?, ?, ?, ?, ?, ?, ?, ?)";
        try
        {
            stmt = connection.prepareStatement(insertEvent);
            stmt.setString(1, e.getEventID());
            stmt.setString(2, e.getDescendant());
            stmt.setString(3, e.getPersonID());
            stmt.setDouble(4, e.getLatitude());
            stmt.setDouble(5, e.getLongitude());
            stmt.setString(6, e.getCountry());
            stmt.setString(7, e.getCity());
            stmt.setString(8, e.getEventType());
            stmt.setInt(9, e.getYear());


            if(stmt.executeUpdate() == 1)
            {
                // OK
            }
            else
            {
                System.out.println("Error in EventDao, createEvent()");
                throw new SQLException();
            }
        }
        catch(SQLException exc)
        {
            System.out.println("Error in EventDao, createEvent()");
            exc.printStackTrace();
            throw new SQLException();
        }
        finally
        {
            if(stmt != null)
                stmt.close();
        }
    }

    /**
     * Retrives a event record from the event ID.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     The eventID must be a valid eventID.
     * </pre>
     * @param eventID the event ID of the event we want to get records for
     * @return an Event object with all the information about the event
     */
    public Event getEvent(String eventID) throws SQLException, InvalidModelException
    {
        //event/[eventID]
        //isEvent()
        Event event = null;
        PreparedStatement stmt = null;
        ResultSet rs = null;
        try
        {
            String queryPerson = "select * from Events where Events.eventID = ?";
            stmt = connection.prepareStatement(queryPerson);
            stmt.setString(1, eventID);
            rs = stmt.executeQuery();

            while(rs.next())
            {
                String qEventID = rs.getString(1);
                String descendant = rs.getString(2);
                String personID = rs.getString(3);
                double latitude = rs.getDouble(4);
                double longitude = rs.getDouble(5);
                String country = rs.getString(6);
                String city = rs.getString(7);
                String eventType = rs.getString(8);
                int year = rs.getInt(9);

                event = new Event(qEventID, descendant, personID, latitude,
                        longitude, country, city, eventType, year);
            }
        }
        catch(SQLException exc)
        {
            if (event != null)
                System.out.println("Error in EventDao, getEvent(String eventID)");
        }
        finally
        {
            if(stmt != null)
                stmt.close();
            if (rs != null)
                rs.close();
        }
        return event;
    }

    /**
     * Retrives a event record from the event ID.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     The eventID must be a valid eventID.
     * </pre>
     * @param eventID the event ID of the event we want to get records for
     * @return an Event object with all the information about the event
     */
    public Event getEvent(String eventID, String username) throws SQLException, InvalidModelException
    {
        //event/[eventID]
        //isEvent()
        Event event = null;
        PreparedStatement stmt = null;
        ResultSet rs = null;
        try
        {
            String queryPerson = "select * from Events where Events.eventID = ? and Events.descendant = ?";
            stmt = connection.prepareStatement(queryPerson);
            stmt.setString(1, eventID);
            stmt.setString(2, username);
            rs = stmt.executeQuery();

            while(rs.next())
            {
                String qEventID = rs.getString(1);
                String descendant = rs.getString(2);
                String personID = rs.getString(3);
                double latitude = rs.getDouble(4);
                double longitude = rs.getDouble(5);
                String country = rs.getString(6);
                String city = rs.getString(7);
                String eventType = rs.getString(8);
                int year = rs.getInt(9);

                event = new Event(qEventID, descendant, personID, latitude,
                        longitude, country, city, eventType, year);
            }
        }
        catch(SQLException exc)
        {
            if (event != null)
                System.out.println("Error in EventDao, getEvent(String eventID, String username)");
        }
        finally
        {
            if(stmt != null)
                stmt.close();
            if (rs != null)
                rs.close();
        }
        return event;
    }

    public int getEventYear(String personID, String eventType) throws SQLException
    {
        int year = 0;
        PreparedStatement stmt = null;
        ResultSet rs = null;
        try
        {
            String queryYear = "select year from Events " +
                               "where Events.personID = ? and Events.eventType = ?";
            stmt = connection.prepareStatement(queryYear);
            stmt.setString(1, personID);
            stmt.setString(2, eventType);
            rs = stmt.executeQuery();

            while(rs.next())
            {
                year = rs.getInt(1);
            }
        }
        catch(SQLException exc)
        {
            System.out.println("Error in EventDao, getEventYear(String personID)");
        }
        finally
        {
            if(stmt != null)
                stmt.close();
            if (rs != null)
                rs.close();
        }
        return year;
    }

    /**
     * Retrives all event records in the database.
     * @return an array of Event objects
     */
    public List<Event> getAllEvents(String username) throws SQLException, InvalidModelException
    {
        //event
        List<Event> events = null;
        PreparedStatement stmt = null;
        ResultSet rs = null;
        try
        {
            String queryPerson = "select * from Events where Events.descendant = ?";
            stmt = connection.prepareStatement(queryPerson);
            stmt.setString(1, username);
            rs = stmt.executeQuery();
            events = new ArrayList<Event>();

            while(rs.next())
            {
                String qEventID = rs.getString(1);
                String descendant = rs.getString(2);
                String personID = rs.getString(3);
                double latitude = rs.getDouble(4);
                double longitude = rs.getDouble(5);
                String country = rs.getString(6);
                String city = rs.getString(7);
                String eventType = rs.getString(8);
                int year = rs.getInt(9);

                events.add(new Event(qEventID, descendant, personID, latitude,
                        longitude, country, city, eventType, year));
            }
        }
        catch(SQLException exc)
        {
            System.out.println("Error in EventDao, getEvent(String username)");
        }
        finally
        {
            if(stmt != null)
                stmt.close();
            if (rs != null)
                rs.close();
        }
        return events;
    }

    public boolean isEvent(String eventID) throws SQLException, InvalidModelException
    {
        if (getEvent(eventID) != null)
            return true;
        else
            return false;
    }

    /**
     * Deletes all event records in the database for the specified user.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     Must be a valid user name.
     * </pre>
     * @param username the user name of the user to delete the event info
     */
    public void deleteAllEvents(String username) throws SQLException
    {
        //fill/...
        PreparedStatement stmt = null;
        try
        {
            String deleteEvents = "delete from Events where Events.descendant = ?";
            stmt = connection.prepareStatement(deleteEvents);
            stmt.setString(1, username);
            stmt.executeUpdate();
        }
        catch(SQLException exc)
        {
            System.out.println("Error in EventDao, deleteAllEvents(String username)");
            exc.printStackTrace();
        }
        finally
        {
            if(stmt != null)
                stmt.close();
        }

    }

    /**
     *Deletes all event records in the database.
     */
    public void deleteAllEvents() throws SQLException
    {
        //clear
        //load
        String drop = "DROP TABLE IF EXISTS Events;";
        Database.executeStatement(drop,connection);
    }
}
