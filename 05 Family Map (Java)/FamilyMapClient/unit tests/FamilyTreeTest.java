package edu.byu.cs.students.familymap;

import org.apache.commons.lang3.text.WordUtils;
import org.junit.*;

import java.util.ArrayList;
import java.util.List;

import edu.byu.cs.students.familymap.model.*;

import static org.junit.Assert.*;

/**
 * Created by bmcbrid4 on 8/1/2017.
 */

public class FamilyTreeTest
{
    private FamilyTree tree;

    @Before
    public void createModels() throws InvalidModelException
    {
        //Set up the tree before every test
        Models.createModels();
        tree = new FamilyTree(Models.persons, Models.events);
    }

    //Test if the events in alphabetical order
    @Test
    public void eventTypeTest()
    {
        List<String> eventTypes = tree.getEventTypes();
        assertFalse(eventTypes == null);
        assertTrue(eventTypes.get(0).equals("Baptism Events"));
        assertTrue(eventTypes.get(1).equals("Birth Events"));
        assertTrue(eventTypes.get(2).equals("Death Events"));
        assertTrue(eventTypes.get(3).equals("Graduation From College Events"));
        assertTrue(eventTypes.get(4).equals("Graduation From High School Events"));
        assertTrue(eventTypes.get(5).equals("Marriage Events"));
    }

    //Test if the events for a person is in the right order
    //Sorted by year
    //If same year then alphabetical
    //If Birth and baptism the same year - birth always first
    //Death always last if they have died

    //Events related to id1:
    // no death date
    // baptism and graduation from high school are the same year
    //Events related to id5:
    // birth and baptism are the same year
    // marriage and graduation from college are the same year
    //Events related to id6:
    // death and baptism are the same year
    @Test
    public void sortEventsForPersonTest()
    {
        List<Event> events1 = tree.sortEventsForPerson(Models.testPerson1.getPersonID());
        List<Event> events5 = tree.sortEventsForPerson(Models.testPerson5.getPersonID());
        List<Event> events6 = tree.sortEventsForPerson(Models.testPerson6.getPersonID());

        List<List<Event>> eventsList = new ArrayList<>();
        eventsList.add(events1);
        eventsList.add(events5);
        eventsList.add(events6);

        //Cycle through all event lists
        for (List<Event> events : eventsList)
        {
            //Birth event is first
            assertTrue(events.get(0).getEventType().toLowerCase().equals("birth"));

            //Cycle through all events in the list
            for (int i = 0; i < events.size(); i++)
            {
                Event event = events.get(i);

                //If there is a death event, it is last
                if (event.getEventType().toLowerCase().equals("death"))
                    assertTrue(i == events.size() - 1);

                if (i > 0)
                {
                    Event prevEvent = events.get(i - 1);
                    //Previous event's year should never be greater then then next
                    assertFalse(prevEvent.getYear() > event.getYear());

                    //Already tested birth and death
                    if (!(prevEvent.getEventType().toLowerCase().equals("birth") ||
                        event.getEventType().toLowerCase().equals("death")))
                    {
                        if (prevEvent.getYear() == event.getYear())
                        {
                            //If the year is the same and it's not a death or birth event,
                            //then it should be in ABC order.
                            assertTrue(prevEvent.getEventType().compareToIgnoreCase(event.getEventType()) < 0);
                        }
                        else
                        {
                            //If the year isn't equal previous event's year should be less
                            assertTrue(prevEvent.getYear() < event.getYear());
                        }
                    }

                }
            }
        }
    }

    //Tests if the you can get the person's child
    @Test
    public void getChildTest()
    {
        assertTrue(tree.getChild("id1") == null);

        assertTrue(tree.getChild("id5").getPersonID().equals("id1"));
        assertTrue(tree.getChild("id6").getPersonID().equals("id1"));

        assertTrue(tree.getChild("id7").getPersonID().equals("id5"));
        assertTrue(tree.getChild("id8").getPersonID().equals("id5"));

        assertTrue(tree.getChild("id9").getPersonID().equals("id6"));
        assertTrue(tree.getChild("id10").getPersonID().equals("id6"));

        assertTrue(tree.getChild("id11").getPersonID().equals("id7"));
        assertTrue(tree.getChild("id12").getPersonID().equals("id7"));

        assertTrue(tree.getChild("id13").getPersonID().equals("id8"));
        assertTrue(tree.getChild("id14").getPersonID().equals("id8"));

        assertTrue(tree.getChild("id15").getPersonID().equals("id9"));
        assertTrue(tree.getChild("id16").getPersonID().equals("id9"));

        assertTrue(tree.getChild("id17").getPersonID().equals("id10"));
        assertTrue(tree.getChild("id18").getPersonID().equals("id10"));
    }

    //Tests if it returns a list of the user's mother side
    @Test
    public void getMotherSideTest()
    {
        List<Person> motherSide = tree.getMotherSide();
        assertTrue(motherSide.size() == 7);
        for (int i = 0; i < motherSide.size(); i++)
        {
            Person person = motherSide.get(i);

            //Each person ID is unique
            if (i < 6)
            {
                assertFalse(person.getPersonID().equals(motherSide.get(i + 1).getPersonID()));
            }

            //Since each ID is unique and the size of the list is 7,
            //each id will get hit once and no more or less.
            assertTrue( person.getPersonID().equals("id6") ||
                        person.getPersonID().equals("id9") ||
                        person.getPersonID().equals("id10") ||
                        person.getPersonID().equals("id15") ||
                        person.getPersonID().equals("id16") ||
                        person.getPersonID().equals("id17") ||
                        person.getPersonID().equals("id18"));
        }
    }

    //Tests if it returns a list of the user's father side
    @Test
    public void getFatherSideTest()
    {
        List<Person> fatherSide = tree.getFatherSide();
        assertTrue(fatherSide.size() == 7);
        for (int i = 0; i < fatherSide.size(); i++)
        {
            Person person = fatherSide.get(i);

            //Each person ID is unique
            if (i < 6)
            {
                assertFalse(person.getPersonID().equals(fatherSide.get(i + 1).getPersonID()));
            }

            //Since each ID is unique and the size of the list is 7,
            //each id will get hit once and no more or less.
            assertTrue( person.getPersonID().equals("id5") ||
                        person.getPersonID().equals("id7") ||
                        person.getPersonID().equals("id8") ||
                        person.getPersonID().equals("id11") ||
                        person.getPersonID().equals("id12") ||
                        person.getPersonID().equals("id13") ||
                        person.getPersonID().equals("id14"));
        }
    }
}