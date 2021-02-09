package JUnitTests.ServiceTest;

import JUnitTests.DaoTest.DaoTests;
import JUnitTests.DaoTest.Models;
import dao.Database;
import model.*;
import org.junit.*;
import request.*;
import result.*;
import server.MissingPostException;
import service.*;

import java.sql.SQLException;
import java.util.List;

import static org.junit.Assert.*;

public class ServiceTests
{
    static Database db = null;

    @BeforeClass
    public static void connect()
    {
        db = new Database();
    }

    @AfterClass
    public static void disconnect() throws SQLException
    {
        db.closeConnection(true);
    }

    @Before
    public void createModels() throws InvalidModelException
    {
        Models.createModels();
    }

    @After
    public void clearDatabase() throws SQLException
    {
        DaoTests.clearDatabase(db);
    }

    @Test
    public void clearTest() throws SQLException
    {
        //There are things in the database to clear
        DaoTests.populateDatabase(db);
        ClearService clearService = new ClearService(db);
        ClearResult result = clearService.clear();
        assertTrue(result.getMessage().equals("Clear succeeded."));
    }

    @Test
    public void loadTest() throws SQLException, InvalidModelException
    {
        User[] users = new User[]
                {
                        Models.testUser1,
                        Models.testUser2,
                        Models.testUser3,
                        Models.testUser4
                };
        Person[] persons = new Person[]
                {
                        Models.testPerson1,
                        Models.testPerson2,
                        Models.testPerson3,
                        Models.testPerson4
                };
        Event[] events = new Event[]
                {
                        Models.testEvent1,
                        Models.testEvent2,
                        Models.testEvent3,
                        Models.testEvent4,
                        Models.testEvent5
                };
        LoadRequest loadRequest = new LoadRequest(users, persons, events);
        LoadResult loadResult = new LoadService(db).load(loadRequest);
        assertTrue(loadResult.getMessage().equals(
                "Successfully added 4 users, 4 persons, and 5 events to the database."));
    }

    //Test for invalid value
    @Test(expected = InvalidModelException.class)
    public void loadTest2() throws SQLException, InvalidModelException
    {
        User[] users = new User[]
                {
                        Models.testUser1,
                        Models.testUser2,
                        Models.testUser3,
                        Models.testUser4
                };
        Person[] persons = new Person[]
                {
                        Models.testPerson1,
                        Models.testPerson2,
                        Models.testPerson3,
                        Models.testPerson4
                };
        Event[] events = new Event[]
                {
                        Models.testEvent1,
                        Models.testEvent2,
                        Models.testEvent3,
                        Models.testEvent4,
                        Models.testEvent5,
                        new Event(
                                "event5",
                                "testUser2",
                                "id2",
                                45.5167,
                                -121.3167,
                                "United States",
                                "Portland",
                                "Death",
                                2084)
                };
        LoadRequest loadRequest = new LoadRequest(users, persons, events);
        new LoadService(db).load(loadRequest);
    }

    //Test for missing value
    @Test(expected = InvalidModelException.class)
    public void loadTest3() throws SQLException, InvalidModelException
    {
        User[] users = new User[]
                {
                        Models.testUser1,
                        Models.testUser2,
                        Models.testUser3,
                        Models.testUser4,
                        new User(
                                "testUser5",
                                "password",
                                "testuser5@email.com",
                                "Test5",
                                null,
                                "f",
                                "id4")
                };
        Person[] persons = new Person[]
                {
                        Models.testPerson1,
                        Models.testPerson2,
                        Models.testPerson3,
                        Models.testPerson4
                };
        Event[] events = new Event[]
                {
                        Models.testEvent1,
                        Models.testEvent2,
                        Models.testEvent3,
                        Models.testEvent4,
                        Models.testEvent5
                };
        LoadRequest loadRequest = new LoadRequest(users, persons, events);
        new LoadService(db).load(loadRequest);
    }

    //Duplicate username is tested before the service is called in the user handler
    @Test
    public void registerTest() throws SQLException, InvalidModelException, MissingPostException
    {
        RegisterRequest regReq = new RegisterRequest(
                "username",
                "password",
                "email",
                "firstName",
                "lastName",
                "m");
        RegisterResult regResult = new RegisterService(db).register(regReq);
        assertTrue(db.authTokenDao.isToken(regResult.getAuthToken()));
        assertTrue(db.userDao.isUser(regResult.getUserName()));
        assertTrue(db.personDao.isPerson(regResult.getPersonID()));
    }

    //Test for invalid value (Gender)
    @Test(expected = InvalidModelException.class)
    public void registerTest2() throws SQLException, InvalidModelException, MissingPostException
    {
        RegisterRequest regReq = new RegisterRequest(
                "username",
                "password",
                "email",
                "firstName",
                "lastName",
                "a");
        RegisterResult regResult = new RegisterService(db).register(regReq);
        assertTrue(db.authTokenDao.isToken(regResult.getAuthToken()));
        assertTrue(db.userDao.isUser(regResult.getUserName()));
        assertTrue(db.personDao.isPerson(regResult.getPersonID()));
    }

    //Test for missing value
    @Test(expected = InvalidModelException.class)
    public void registerTest3() throws SQLException, InvalidModelException, MissingPostException
    {
        RegisterRequest regReq = new RegisterRequest(
                "username",
                "password",
                null,
                "firstName",
                "lastName",
                "a");
        RegisterResult regResult = new RegisterService(db).register(regReq);
        assertTrue(db.authTokenDao.isToken(regResult.getAuthToken()));
        assertTrue(db.userDao.isUser(regResult.getUserName()));
        assertTrue(db.personDao.isPerson(regResult.getPersonID()));
    }

    //Username and password are already checked in the user handler before login
    @Test
    public void loginTest() throws SQLException, InvalidModelException
    {
        DaoTests.populateDatabase(db);
        LoginRequest loginReq = new LoginRequest("testUser1", "password");
        LoginResult loginResult = new LoginService(db).login(loginReq);
        assertTrue(db.authTokenDao.isToken(loginResult.getAuthToken()));
        assertTrue(db.userDao.isUser(loginResult.getUserName()));
        assertTrue(db.personDao.isPerson(loginResult.getPersonId()));
    }

    //Valid username and generations parameter are already checked in the fill handler
    @Test
    public void fillTest() throws SQLException, InvalidModelException, MissingPostException
    {
        DaoTests.populateDatabase(db);
        FillResult fillResult = new FillService(db, "testUser1", 4).fill();
        String[] messageWithOutNumbers = fillResult.getMessage().split("\\d+");
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < messageWithOutNumbers.length; i++)
        {
            sb.append(messageWithOutNumbers[i]);
        }
        assertTrue(sb.toString().equals("Successfully added  persons and  events to the database."));
        assertTrue(fillResult.getMessage().charAt(19) == '3');
        assertTrue(fillResult.getMessage().charAt(20) == '1');
        assertTrue(Character.isDigit(fillResult.getMessage().charAt(34)));
        assertTrue(Character.isDigit(fillResult.getMessage().charAt(35)));
        assertTrue(Character.isDigit(fillResult.getMessage().charAt(36)));
    }

    //Username is from auth token in the person handler, also checks validity of auth token
    @Test
    public void personTest() throws SQLException, InvalidModelException
    {
        DaoTests.populateDatabase(db);
        PersonService personService = new PersonService(db);

        //person/[personID]
        //Tests valid person
        Person p = personService.getPerson("id5", "testUser1");
        assertTrue(p.getPersonID().equals("id5"));
        assertTrue(p.getDescendant().equals("testUser1"));
        assertTrue(p.getSpouseID().equals("id6"));
        assertTrue(p.getFatherID() == null);
        assertTrue(p.getFirstName().equals("Father"));

        //Tests valid person, but not with that user
        p = personService.getPerson("id5", "testUser2");
        assertTrue(p == null);

        //Tests invalid personID
        p = personService.getPerson("ID5", "testUser2");
        assertTrue(p == null);

        //person/
        AllPersonsResult personsResult = personService.getAllPersons("testUser1");
        List<Person> persons = personsResult.getData();
        for (int i = 0; i < persons.size(); i++)
        {
            assertTrue(persons.get(i).getDescendant().equals("testUser1"));
        }

        for (int i = 0; i < persons.size(); i++)
        {
            assertFalse(persons.get(i).getDescendant().equals("testUser2"));
        }
    }

    //Username is from auth token in the person handler, also checks validity of auth token
    @Test
    public void eventTest() throws SQLException, InvalidModelException
    {
        DaoTests.populateDatabase(db);
        EventService eventService = new EventService(db);

        //event/[eventID]
        //Tests valid event
        Event e = eventService.getEvent("event1", "testUser1");
        assertTrue(e.getEventID().equals("event1"));
        assertTrue(e.getDescendant().equals("testUser1"));
        assertTrue(e.getCity().equals("Portland"));
        assertTrue(e.getYear() == 1994);
        assertTrue(e.getDescendant().equals("testUser1"));

        //Tests valid event, but not with that user
        e = eventService.getEvent("event1", "testUser2");
        assertTrue(e == null);

        //Tests invalid eventID
        e = eventService.getEvent("EVENT1", "testUser2");
        assertTrue(e == null);

        //event/
        AllEventsResult eventsResult = eventService.getAllEvents("testUser1");
        List<Event> events = eventsResult.getData();
        for (int i = 0; i < events.size(); i++)
        {
            assertTrue(events.get(i).getDescendant().equals("testUser1"));
        }

        for (int i = 0; i < events.size(); i++)
        {
            assertFalse(events.get(i).getDescendant().equals("testUser2"));
        }
    }
}
