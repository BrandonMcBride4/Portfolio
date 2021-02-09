package model;

/**
 * Created by Brandon on 7/12/2017.
 */

import com.google.gson.JsonObject;
import server.Handler;
import server.MissingPostException;

public class Event
{
    private String eventID;
    private String descendant;
    private String personID;
    private double latitude;
    private double longitude;
    private String country;
    private String city;
    private String eventType;
    private int year;
    private String error;

    /**
     * The constructor for an Event.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     invariant(eventID, descendant, personID, latitude, longitude, country, city, eventType, year)
     * </pre>
     * @param eventID the unique identifier for this event
     * @param descendant the user (Username) to which this person belongs
     * @param personID the ID of the person the event belongs
     * @param latitude the latitude of the event location
     * @param longitude the longitude of the event location
     * @param country the country of the event location
     * @param city the city of the event location
     * @param eventType the type of event (birth, baptism, christening, marriage, death, etc.)
     * @param year the year in which the event occurred
     * @throws InvalidModelException thrown when invariant is false
     */
    public Event(String eventID, String descendant, String personID, double latitude,
                 double longitude, String country, String city, String eventType, int year)
                throws InvalidModelException
    {
        if (!invariant(eventID, descendant, personID, latitude,
                       longitude, country, city, eventType, year))
        {
            throw new InvalidModelException(error, "Event");
        }
        this.eventID = eventID;
        this.descendant = descendant;
        this.personID = personID;
        this.latitude = latitude;
        this.longitude = longitude;
        this.country = country;
        this.city = city;
        this.eventType = eventType;
        this.year = year;
    }

    public static Event jsonToEvent(JsonObject eventJSON) throws MissingPostException, InvalidModelException
    {
        String eventID = Handler.getBodyPart(eventJSON,"eventID");
        String descendant = Handler.getBodyPart(eventJSON,"descendant");
        String personID = Handler.getBodyPart(eventJSON,"personID");
        double latitude = Double.parseDouble(Handler.getBodyPart(eventJSON,"latitude"));
        double longitude = Double.parseDouble(Handler.getBodyPart(eventJSON,"longitude"));
        String country = Handler.getBodyPart(eventJSON,"country");
        String city = Handler.getBodyPart(eventJSON,"city");
        String eventType = Handler.getBodyPart(eventJSON,"eventType");
        int year = Integer.parseInt(Handler.getBodyPart(eventJSON,"year"));

        Event event = new Event(eventID, descendant, personID, latitude,
                                longitude, country, city, eventType, year);
        return event;
    }

    @Override
    public String toString()
    {
        return "Event{" +
                "eventID='" + eventID + '\'' +
                ", descendant='" + descendant + '\'' +
                ", personID='" + personID + '\'' +
                ", latitude=" + latitude +
                ", longitude=" + longitude +
                ", country='" + country + '\'' +
                ", city='" + city + '\'' +
                ", eventType='" + eventType + '\'' +
                ", year=" + year +
                '}';
    }

    public String getEventID()
    {
        return eventID;
    }

    public String getDescendant()
    {
        return descendant;
    }

    public String getPersonID()
    {
        return personID;
    }

    public double getLatitude()
    {
        return latitude;
    }

    public double getLongitude()
    {
        return longitude;
    }

    public String getCountry()
    {
        return country;
    }

    public String getCity()
    {
        return city;
    }

    public String getEventType()
    {
        return eventType;
    }

    public int getYear()
    {
        return year;
    }

    public void setEventID(String eventID)
    {
        this.eventID = eventID;
    }

    public void setDescendant(String descendant)
    {
        this.descendant = descendant;
    }

    public void setPersonID(String personID)
    {
        this.personID = personID;
    }

    public void setLatitude(double latitude)
    {
        this.latitude = latitude;
    }

    public void setLongitude(double longitude)
    {
        this.longitude = longitude;
    }

    public void setCountry(String country)
    {
        this.country = country;
    }

    public void setCity(String city)
    {
        this.city = city;
    }

    public void setEventType(String eventType)
    {
        this.eventType = eventType;
    }

    public void setYear(int year)
    {
        this.year = year;
    }

    /**
     * Checks if the parameters of the constructor are valid.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None
     * </pre>
     * @return
     * <pre>
     *     true if eventID, descendant, personID, latitude, longitude, country, city, eventType != ""
     *     AND year is &ge; 0 AND year &le; 9999
     *     AND descendant needs to be a valid username
     *     AND personID needs to link to a valid person
     *     false otherwise
     * </pre>
     * @param eventID the unique identifier for this event
     * @param descendant the user (Username) to which this person belongs
     * @param personID the ID of the person the event belongs
     * @param latitude the latitude of the event location
     * @param longitude the longitude of the event location
     * @param country the country of the event location
     * @param city the city of the event location
     * @param eventType the type of event (birth, baptism, christening, marriage, death, etc.)
     * @param year the year in which the event occurred
     */
    private boolean invariant(String eventID, String descendant, String personID, double latitude,
                              double longitude, String country, String city, String eventType, int year)
    {
        if (eventID == null)
        {
            error = "eventID is null";
            return false;
        }

        if (descendant == null)
        {
            error = "descendant is null for " + eventID;
            return false;
        }

        if (personID == null)
        {
            error = "personID is null for " + eventID + " for user: " + descendant;
            return false;
        }

        if (country == null)
        {
            error = "country is null for " + eventID + " for user: " + descendant;
            return false;
        }
        if (city == null)
        {
            error = "city is null for " + eventID + " for user: " + descendant;
            return false;
        }

        if (eventType == null)
        {
            error = " is null for " + eventID + " for user: " + descendant;
            return false;
        }

        if (latitude < -90 || latitude > 90)
        {
            error = "latitude is out of range, not a valid location for " +
                    eventID + " for user: " + descendant;
            return false;
        }

        if (longitude < -180 || longitude > 180)
        {
            error = "longitude is out of range, not a valid location for " +
                    eventID + " for user: " + descendant;
            return false;
        }

        if (year < 0 || year > 2017)
        {
            error = String.format("Invalid year when creating %s type of event", eventType);
            return false;
        }
            return true;
    }
}
