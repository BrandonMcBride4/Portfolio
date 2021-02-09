package JUnitTests.DaoTest;

import model.*;

public class Models
{
    public static User testUser1;
    public static User testUser2;
    public static User testUser3;
    public static User testUser4;

    public static Person testPerson1;
    public static Person testPerson2;
    public static Person testPerson3;
    public static Person testPerson4;

    public static Event testEvent1;
    public static Event testEvent2;
    public static Event testEvent3;
    public static Event testEvent4;
    public static Event testEvent5;

    public static void createModels() throws InvalidModelException
    {
        createTestUsers();
        createTestPersons();
        createTestEvents();
    }

    public static void createTestUsers() throws InvalidModelException
    {
        testUser1 = new User(
                "testUser1",
                "password",
                "testuser1@email.com",
                "Test1",
                "User1",
                "m",
                "id1");

        testUser2 = new User(
                "testUser2",
                "password",
                "testuser2@email.com",
                "Test2",
                "User2",
                "f",
                "id2");

        testUser3 = new User(
                "testUser3",
                "password",
                "testuser3@email.com",
                "Test3",
                "User3",
                "m",
                "id3");

        testUser4 = new User(
                "testUser4",
                "password",
                "testuser4@email.com",
                "Test4",
                "User4",
                "f",
                "id4");
    }

    public static void createTestPersons() throws InvalidModelException
    {
        testPerson1 = new Person(
                "testUser1",
                "id1",
                "Test1",
                "User1",
                "m",
                "id5",
                "id6",
                null);

        testPerson2 = new Person(
                "testUser1",
                "id5",
                "Father",
                "User1",
                "m",
                null,
                null,
                "id6");

        testPerson3 = new Person(
                "testUser1",
                "id6",
                "Mother",
                "User1",
                "f",
                null,
                null,
                "id5");

        testPerson4 = new Person(
                "testUser2",
                "id2",
                "Test2",
                "User2",
                "m",
                null,
                null,
                null);
    }

    public static void createTestEvents() throws InvalidModelException
    {
        testEvent1 = new Event(
                "event1",
                "testUser1",
                "id1",
                45.5167,
                -121.3167,
                "United States",
                "Portland",
                "Birth",
                1994);

        testEvent2 = new Event(
                "event2",
                "testUser1",
                "id1",
                38.5500,
                68.7833,
                "United States",
                "Sacramento",
                "Baptism",
                2002);

        testEvent3 = new Event(
                "event3",
                "testUser1",
                "id5",
                45.4667,
                9.1833,
                "Italy",
                "Milan",
                "Birth",
                1961);

        testEvent4 = new Event(
                "event4",
                "testUser1",
                "id6",
                45.4333,
                12.3333,
                "Italy",
                "Venice",
                "Birth",
                1962);

        testEvent5 = new Event(
                "event5",
                "testUser2",
                "id2",
                45.5167,
                -121.3167,
                "United States",
                "Portland",
                "Birth",
                1995);
    }
}
