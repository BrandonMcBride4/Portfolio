package JUnitTests.DaoTest;

import dao.*;
import model.*;
import org.junit.*;

import java.sql.SQLException;
import java.util.List;

import static org.junit.Assert.*;

public class DaoTests
{
    static Database db = null;

    @BeforeClass
    public static void connect()
    {
        db = new Database();
    }

    @Before
    public void createModels() throws InvalidModelException
    {
        Models.createModels();
    }

    public static void clearDatabase(Database db) throws SQLException
    {
        db.authTokenDao.deleteAllAuthToken();
        db.userDao.deleteAllUsers();
        db.personDao.deleteAllPersons();
        db.eventDao.deleteAllEvents();
        db.createTablesIfNeeded();
    }

    public static void populateDatabase(Database db) throws SQLException
    {
        //Users
        db.userDao.createUser(Models.testUser1);
        db.userDao.createUser(Models.testUser2);
        db.userDao.createUser(Models.testUser3);
        db.userDao.createUser(Models.testUser4);

        //Persons
        db.personDao.createPerson(Models.testPerson1);
        db.personDao.createPerson(Models.testPerson2);
        db.personDao.createPerson(Models.testPerson3);
        db.personDao.createPerson(Models.testPerson4);

        //Events
        db.eventDao.createEvent(Models.testEvent1);
        db.eventDao.createEvent(Models.testEvent2);
        db.eventDao.createEvent(Models.testEvent3);
        db.eventDao.createEvent(Models.testEvent4);
        db.eventDao.createEvent(Models.testEvent5);
    }

    @After
    public void clear() throws SQLException
    {
        clearDatabase(db);
    }

    @AfterClass
    public static void disconnect() throws SQLException
    {
        db.closeConnection(true);
    }

    @Test
    public void testAuthTokenDao() throws SQLException
    {
        //Test create tokens
        String token = db.authTokenDao.createAuthToken(Models.testUser1);
        String token2 = db.authTokenDao.createAuthToken(Models.testUser1);
        String token3 = db.authTokenDao.createAuthToken(Models.testUser1);

        //Test getUsername
        assertFalse(db.authTokenDao.getUsername(token2).equals("asdf"));
        assertTrue(db.authTokenDao.getUsername(token).equals("testUser1"));
        assertTrue(db.authTokenDao.getUsername(token2).equals("testUser1"));
        assertTrue(db.authTokenDao.getUsername(token3).equals("testUser1"));

        //Test isToken
        assertTrue(db.authTokenDao.isToken(token));
        assertTrue(db.authTokenDao.isToken(token2));
        assertTrue(db.authTokenDao.isToken(token3));

        //Test delete single auth token
        db.authTokenDao.deleteAuthToken(token);
        assertFalse(db.authTokenDao.isToken(token));
        assertTrue(db.authTokenDao.isToken(token2));
        assertTrue(db.authTokenDao.isToken(token3));

        //Test delete all auth tokens
        db.authTokenDao.deleteAllAuthToken();
        assertFalse(db.authTokenDao.isToken(token2));
        assertFalse(db.authTokenDao.isToken(token3));

    }

    @Test
    public void testUserDao() throws SQLException, InvalidModelException
    {
        //Test createUser()
        populateDatabase(db);

        //Test getUser
        assertTrue(db.userDao.getUser("testUser1").getPersonID().equals("id1"));
        assertTrue(db.userDao.getUser("testUser2").getFirstName().equals("Test2"));
        assertTrue(db.userDao.getUser("testUser3").getLastName().equals("User3"));
        assertTrue(db.userDao.getUser("testUser4").getPassword().equals("password"));

        //Test isUser
        assertTrue(db.userDao.isUser("testUser1"));
        assertFalse(db.userDao.isUser("Testuser1"));

        //Test deleteAllUsers()
        db.userDao.deleteAllUsers();
        assertFalse(db.userDao.isUser("testUser1"));
        assertFalse(db.userDao.isUser("testUser2"));
        assertFalse(db.userDao.isUser("testUser3"));
        assertFalse(db.userDao.isUser("testUser4"));
    }

    @Test
    public void testPersonDao() throws SQLException, InvalidModelException
    {
        //Test createPerson()
        populateDatabase(db);

        //Test getPerson(personID)
        assertTrue(db.personDao.getPerson("id1").getSpouseID() == null);
        assertTrue(db.personDao.getPerson("id5").getFatherID() == null);
        assertTrue(db.personDao.getPerson("id6").getFirstName().equals("Mother"));

        //Test getPerson(personID, username)
        assertTrue(db.personDao.getPerson("id5", "testUser1").getFirstName().equals("Father"));
        assertFalse(db.personDao.getPerson("id5", "Testuser1") != null);

        //Test isPerson
        assertTrue(db.personDao.isPerson("id1"));
        assertTrue(db.personDao.isPerson("id5"));
        assertTrue(db.personDao.isPerson("id6"));
        assertFalse(db.personDao.isPerson("ID1"));
        assertFalse(db.personDao.isPerson("ID5"));
        assertFalse(db.personDao.isPerson("ID6"));

        //Test updatePerson(person)
        Models.testPerson1.setSpouseID("my-hot-wife");
        assertTrue(db.personDao.getPerson("id1").getSpouseID() == null);
        db.personDao.updatePerson(Models.testPerson1);
        assertFalse(db.personDao.getPerson("id1").getSpouseID() == null);
        assertTrue(db.personDao.getPerson("id1").getSpouseID().equals("my-hot-wife"));

        //Test getAllPersons(username)
        List<Person> persons = db.personDao.getAllPersons("testUser1");
        for (int i = 0; i < persons.size(); i++)
        {
            assertTrue(persons.get(i).getDescendant().equals("testUser1"));
        }

        //Test deleteAllPerson(username)
        db.personDao.deleteAllPersons("testUser1");
        //Didn't delete all persons
        List<Person> persons2 = db.personDao.getAllPersons("testUser2");
        for (int i = 0; i < persons2.size(); i++)
        {
            assertTrue(persons2.get(i).getDescendant().equals("testUser2"));
        }

        //If no persons for that username, getAllPersons returns null
        List<Person> persons3 = db.personDao.getAllPersons("testUser1");
        assertTrue(persons3.isEmpty());

        //Test deleteAllPersons()
        db.personDao.deleteAllPersons();
        assertFalse(db.personDao.isPerson("id1"));
        assertFalse(db.personDao.isPerson("id2"));
        assertFalse(db.personDao.isPerson("id3"));
    }

    @Test
    public void testEventDao() throws SQLException, InvalidModelException
    {
        //Test createEvent()
        populateDatabase(db);

        //Test getEvent(eventID)
        assertTrue(db.eventDao.getEvent("event1").getYear() == 1994);
        assertTrue(db.eventDao.getEvent("event2").getCity().equals("Sacramento"));
        assertTrue(db.eventDao.getEvent("event3").getLongitude() == 9.1833);
        assertTrue(db.eventDao.getEvent("event4").getLatitude() == 45.4333);

        //Test getEvent(eventID, username)
        assertTrue(db.eventDao.getEvent("event2", "testUser1").getEventType().equals("Baptism"));
        assertFalse(db.eventDao.getEvent("event2", "Testuser1") != null);

        //Test isEvent
        assertTrue(db.eventDao.isEvent("event1"));
        assertTrue(db.eventDao.isEvent("event2"));
        assertTrue(db.eventDao.isEvent("event3"));
        assertTrue(db.eventDao.isEvent("event4"));
        assertFalse(db.eventDao.isEvent("EVENT1"));
        assertFalse(db.eventDao.isEvent("EVENT2"));
        assertFalse(db.eventDao.isEvent("EVENT3"));
        assertFalse(db.eventDao.isEvent("EVENT4"));

        //Test getEventYear
        assertTrue(db.eventDao.getEventYear("id6", "Birth") == 1962);
        assertFalse(db.eventDao.getEventYear("id2", "Birth") == 0);

        //Test getAllEvents(username)
        List<Event> events = db.eventDao.getAllEvents("testUser1");
        for (int i = 0; i < events.size(); i++)
        {
            assertTrue(events.get(i).getDescendant().equals("testUser1"));
        }

        //Test deleteAllEvent(username)
        db.eventDao.deleteAllEvents("testUser1");

        //Didn't delete all events, just for testUser1
        List<Event> events2 = db.eventDao.getAllEvents("testUser2");
        for (int i = 0; i < events2.size(); i++)
        {
            assertTrue(events2.get(i).getDescendant().equals("testUser2"));
        }

        //If no events for that username, getAllEvents returns null
        List<Event> events3 = db.eventDao.getAllEvents("testUser1");
        assertTrue(events3.isEmpty());

        //Test deleteAllEvents()
        db.eventDao.deleteAllEvents();
        assertFalse(db.eventDao.isEvent("event1"));
        assertFalse(db.eventDao.isEvent("event2"));
        assertFalse(db.eventDao.isEvent("event3"));
    }

}
