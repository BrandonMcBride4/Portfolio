package edu.byu.cs.students.familymap;

import edu.byu.cs.students.familymap.model.*;

//Creates models to test on
public class Models
{
    public static User testUser1;
    public static User testUser2;
    public static User testUser3;
    public static User testUser4;
    public static User[] users;

    public static Person testPerson1;
    public static Person testPerson2;
    public static Person testPerson3;
    public static Person testPerson4;
    public static Person testPerson5;
    public static Person testPerson6;
    public static Person testPerson7;
    public static Person testPerson8;
    public static Person testPerson9;
    public static Person testPerson10;
    public static Person testPerson11;
    public static Person testPerson12;
    public static Person testPerson13;
    public static Person testPerson14;
    public static Person testPerson15;
    public static Person testPerson16;
    public static Person testPerson17;
    public static Person testPerson18;

    public static Person[] persons;


    public static Event testEvent1;
    public static Event testEvent2;
    public static Event testEvent3;
    public static Event testEvent4;
    public static Event testEvent5;
    public static Event testEvent6;
    public static Event testEvent7;
    public static Event testEvent8;
    public static Event testEvent9;
    public static Event testEvent10;
    public static Event testEvent11;
    public static Event testEvent12;
    public static Event testEvent13;
    public static Event testEvent14;
    public static Event testEvent15;

    public static Event[] events;

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
        users = new User[]
                {
                    testUser1,
                    testUser2,
                    testUser3,
                    testUser4
                };
    }

    public static void createTestPersons() throws InvalidModelException
    {
        testPerson1 = new Person(
                "testUser1",
                "id1",
                "Brandon",
                "McBride",
                "m",
                "id5",
                "id6",
                null);

        testPerson5 = new Person(
                "testUser1",
                "id5",
                "Mark",
                "McBride",
                "m",
                "id7",
                "id8",
                "id6");

        testPerson6 = new Person(
                "testUser1",
                "id6",
                "DeAnn",
                "Gardner",
                "f",
                "id9",
                "id10",
                "id5");

        testPerson7 = new Person(
                "testUser1",
                "id7",
                "Robert",
                "McBride",
                "m",
                "id11",
                "id12",
                "id8");

        testPerson8 = new Person(
                "testUser1",
                "id8",
                "Ione",
                "Mackay",
                "f",
                "id13",
                "id14",
                "id7");

        testPerson9 = new Person(
                "testUser1",
                "id9",
                "Ballard",
                "Gardner",
                "m",
                "id15",
                "id16",
                "id10");

        testPerson10 = new Person(
                "testUser1",
                "id10",
                "Lola",
                "Greene",
                "f",
                "id17",
                "id18",
                "id9");

        testPerson11 = new Person(
                "testUser1",
                "id11",
                "Gamel",
                "McBride",
                "m",
                null,
                null,
                "id12");

        testPerson12 = new Person(
                "testUser1",
                "id12",
                "Emma",
                "Roylance",
                "f",
                null,
                null,
                "id11");

        testPerson13 = new Person(
                "testUser1",
                "id13",
                "Ernest",
                "Mackay",
                "m",
                null,
                null,
                "id14");

        testPerson14 = new Person(
                "testUser1",
                "id14",
                "Fannie",
                "Stoddard",
                "f",
                null,
                null,
                "id13");

        testPerson15 = new Person(
                "testUser1",
                "id15",
                "Milford",
                "Gardner",
                "m",
                null,
                null,
                "id16");

        testPerson16 = new Person(
                "testUser1",
                "id16",
                "Alda",
                "Fugal",
                "f",
                null,
                null,
                "id15");

        testPerson17 = new Person(
                "testUser1",
                "id17",
                "Joseph",
                "Greene",
                "m",
                null,
                null,
                "id18");

        testPerson18 = new Person(
                "testUser1",
                "id18",
                "Martha",
                "Anderson",
                "f",
                null,
                null,
                "id17");

        testPerson4 = new Person(
                "testUser2",
                "id2",
                "Test2",
                "User2",
                "m",
                "id2.2",
                "id2.3",
                null);

        testPerson2 = new Person(
                "testUser2",
                "id2.2",
                "Test2",
                "User2",
                "m",
                null,
                null,
                "id2.3");

        testPerson3 = new Person(
                "testUser2",
                "id2.3",
                "Test2",
                "User2",
                "f",
                null,
                null,
                "id2.3");

        persons = new Person[]
                {
                    testPerson1,
                    //testPerson2,3,4 don't belong to testUser1
                    testPerson5,
                    testPerson6,
                    testPerson7,
                    testPerson8,
                    testPerson9,
                    testPerson10,
                    testPerson11,
                    testPerson12,
                    testPerson13,
                    testPerson14,
                    testPerson15,
                    testPerson16,
                    testPerson17,
                    testPerson18
                };
    }

    //Events related to id1:
        // no death date
        // baptism and graduation from high school are the same year
    //Events related to id5:
        // birth and baptism are the same year
        // marriage and graduation from college are the same year
    //Events related to id6:
        // death and baptism are the same year
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
                2012);

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

        testEvent6 = new Event(
                "event6",
                "testUser1",
                "id1",
                45.4333,
                12.3333,
                "Italy",
                "Venice",
                "Graduation from High School",
                2012);


        testEvent7 = new Event(
                "event7",
                "testUser1",
                "id1",
                45.4333,
                12.3333,
                "Italy",
                "Venice",
                "Graduation from College",
                2016);


        testEvent8 = new Event(
                "event8",
                "testUser1",
                "id5",
                45.4333,
                12.3333,
                "Italy",
                "Venice",
                "Graduation from high school",
                1980);


        testEvent9 = new Event(
                "event9",
                "testUser1",
                "id5",
                45.4333,
                12.3333,
                "Italy",
                "Venice",
                "Baptism",
                1961);


        testEvent10 = new Event(
                "event10",
                "testUser1",
                "id5",
                45.4333,
                12.3333,
                "Italy",
                "Venice",
                "Graduation from college",
                1983);


        testEvent11 = new Event(
                "event11",
                "testUser1",
                "id5",
                45.4333,
                12.3333,
                "Italy",
                "Venice",
                "marriage",
                1983);


        testEvent12 = new Event(
                "event12",
                "testUser1",
                "id6",
                45.4333,
                12.3333,
                "Italy",
                "Venice",
                "Marriage",
                1983);


        testEvent13 = new Event(
                "event13",
                "testUser1",
                "id6",
                45.4333,
                12.3333,
                "Italy",
                "Venice",
                "Graduation from high school",
                1981);


        testEvent14 =new Event(
                "event14",
                "testUser1",
                "id5",
                45.4333,
                12.3333,
                "Italy",
                "Venice",
                "Death",
                2015);


        testEvent15 = new Event(
                "event15",
                "testUser1",
                "id6",
                45.4333,
                12.3333,
                "Italy",
                "Venice",
                "dEaTh",
                2010);




        events = new Event[]
        {
                testEvent1,
                testEvent2,
                testEvent3,
                testEvent4,
                //testEvent5 belongs to a different user
                testEvent6,
                testEvent7,
                testEvent8,
                testEvent9,
                testEvent10,
                testEvent11,
                testEvent12,
                testEvent13,
                testEvent14,
                testEvent15,

        };
    }
}
