package JUnitTests.ModelTest;

import model.Event;
import model.InvalidModelException;
import org.junit.* ;
import static org.junit.Assert.* ;

public class EventModelTest
{
    @Test
    public void testValidEvent() throws InvalidModelException
    {
        Event e = new Event(
                "asdf",
                "asdf",
                "asdf",
                1,
                2,
                "asdf",
                "asdf",
                "asdf",
                1980);
        e.setEventID("eventID");
        e.setDescendant("descendant");
        e.setPersonID("personID");
        e.setLatitude(45);
        e.setLongitude(45);
        e.setCountry("country");
        e.setCity("city");
        e.setEventType("eventType");
        e.setYear(2017);

        assertFalse(e.getEventID() == null);
        assertFalse(e.getDescendant() == null);
        assertFalse(e.getPersonID() == null);
        assertFalse(e.getLatitude() == 0);
        assertFalse(e.getLongitude() == 0);
        assertFalse(e.getCountry() == null);
        assertFalse(e.getCity() == null);
        assertFalse(e.getYear() == 0);
        assertTrue(e.getEventID().equals("eventID"));
        assertTrue(e.getDescendant().equals("descendant"));
        assertTrue(e.getPersonID().equals("personID"));
        assertTrue(e.getLatitude() == 45);
        assertTrue(e.getLongitude() == 45);
        assertTrue(e.getCountry().equals("country"));
        assertTrue(e.getCity().equals("city"));
        assertTrue(e.getEventType().equals("eventType"));
        assertTrue(e.getYear() == 2017);
    }

    //Non null member is null
    @Test(expected = InvalidModelException.class)
    public void createInvalidEvent() throws InvalidModelException
    {
        Event e = new Event(
                "asdf",
                "asdf",
                "asdf",
                45,
                45,
                null,
                null,
                null,
                2017);
    }

    //Latitude out of range
    @Test(expected = InvalidModelException.class)
    public void createInvalidEvent2() throws InvalidModelException
    {
        Event e = new Event(
                "asdf",
                "asdf",
                "asdf",
                95.123,
                45.123,
                "asdf",
                "asdf",
                "asdf",
                2017);
    }

    //Latitude is out of range
    @Test(expected = InvalidModelException.class)
    public void createInvalidEvent3() throws InvalidModelException
    {
        Event e = new Event(
                "asdf",
                "asdf",
                "asdf",
                -95.123,
                45.123,
                "asdf",
                "asdf",
                "asdf",
                2017);
    }

    //Longitude is out of range
    @Test(expected = InvalidModelException.class)
    public void createInvalidEvent4() throws InvalidModelException
    {
        Event e = new Event(
                "asdf",
                "asdf",
                "asdf",
                -85.123,
                185.423,
                "asdf",
                "asdf",
                "asdf",
                2017);
    }

    //Longitude is out of range
    @Test(expected = InvalidModelException.class)
    public void createInvalidEvent5() throws InvalidModelException
    {
        Event e = new Event(
                "asdf",
                "asdf",
                "asdf",
                -85.123,
                -185.423,
                "asdf",
                "asdf",
                "asdf",
                2017);
    }

    //Year is out of range
    @Test(expected = InvalidModelException.class)
    public void createInvalidEvent6() throws InvalidModelException
    {
        Event e = new Event(
                "asdf",
                "asdf",
                "asdf",
                -85.123,
                -185.423,
                "asdf",
                "asdf",
                "asdf",
                2019);
    }

    //Year is out of range
    @Test(expected = InvalidModelException.class)
    public void createInvalidEvent7() throws InvalidModelException
    {
        Event e = new Event(
                "asdf",
                "asdf",
                "asdf",
                -85.123,
                -185.423,
                "asdf",
                "asdf",
                "asdf",
                -5);
    }
}
