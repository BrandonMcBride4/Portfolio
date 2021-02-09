package edu.byu.cs.students.familymap;

import org.junit.*;

import java.util.List;


import static org.junit.Assert.*;


import edu.byu.cs.students.familymap.filter.*;
import edu.byu.cs.students.familymap.model.*;

/**
 * Created by Brandon on 8/9/2017.
 */

public class FilterTest
{
    private FamilyTree tree;
    private List<FilterRow> rows;

    @Before
    public void createModels() throws InvalidModelException
    {
        //Reset tree before every test
        Models.createModels();
        tree = new FamilyTree(Models.persons, Models.events);
        rows = FilterRows.get(tree).getRows();

        //Turn on all filter switches before every test
        for (FilterRow row : rows)
        {
            row.setShowingEvents(true);
        }
    }

    //Tests if all switches are on
    @Test
    public void testAllFiltersOn()
    {
        //Turn all switches on
        for (FilterRow row : rows)
        {
            row.setShowingEvents(true);
        }

        //Filter removes people and events based on the switches
        FamilyTree filteredTree = Filter.filterTree(tree);

        Person[] filteredPersons = filteredTree.getPersons();
        Event[] filteredEvents = filteredTree.getEvents();

        //These should be full (15 persons, 14 events)
        assertTrue(filteredPersons.length == 15);
        assertTrue(filteredEvents.length == 14);
    }

    //Tests if all switches are off (No events)
    @Test
    public void testAllFiltersOff()
    {
        //Turn all switches off
        for (FilterRow row : rows)
        {
            row.setShowingEvents(false);
        }

        //Filter removes people and events based on the switches
        FamilyTree filteredTree = Filter.filterTree(tree);

        Person[] filteredPersons = filteredTree.getPersons();
        Event[] filteredEvents = filteredTree.getEvents();

        //These should be empty
        assertTrue(filteredPersons.length == 0);
        assertTrue(filteredEvents.length == 0);
    }

    //Tests if both mother and father side switch is off (only the user should appear)
    @Test
    public void testMotherSideAndFatherSideOff()
    {
        //Turn off only Mother Side and Father Side
        for (FilterRow row : rows)
        {
            if (row.getTitle().contains("Side"))
                row.setShowingEvents(false);
        }

        //Only user should remain
        FamilyTree filteredTree = Filter.filterTree(tree);

        Person[] filteredPersons = filteredTree.getPersons();
        Event[] filteredEvents = filteredTree.getEvents();
        Person user = filteredPersons[0];

        assertTrue(filteredPersons.length == 1);
        assertTrue(user.getFullName().equals("Brandon McBride"));

        assertTrue(filteredEvents.length == 4);
        List<Event> sortedEvents = filteredTree.sortEventsForPerson(user.getPersonID());

        assertTrue(sortedEvents.get(0).getEventType().equals("Birth"));
        assertTrue(sortedEvents.get(1).getEventType().equals("Baptism"));
        assertTrue(sortedEvents.get(2).getEventType().equals("Graduation from High School"));
        assertTrue(sortedEvents.get(3).getEventType().equals("Graduation from College"));
    }

    //Tests if one gender switch is turned off and if they are both off
    @Test
    public void testGenderFilters()
    {
        //When Male Events are off all should be female
        assertTrue(testGender("Male Events", "Female Events", "f"));
        //When Female Events are off all should be male
        assertTrue(testGender("Female Events", "Male Events", "m"));

        //When both are off no events should exist
        for (FilterRow row : rows)
        {
            if (row.getTitle().toLowerCase().contains("male"))
                row.setShowingEvents(false);
        }

        FamilyTree filteredTree = Filter.filterTree(tree);

        Person[] filteredPersons = filteredTree.getPersons();
        Event[] filteredEvents = filteredTree.getEvents();

        assertTrue(filteredPersons.length == 0);
        assertTrue(filteredEvents.length == 0);
    }

    //Tests Male or Female Filter
    //The parameters should be testGender("Male Events", "Female Events", "f") for female
    //The parameters should be testGender("Female Events", "Male Events", "m") for male
    private boolean testGender(String filterOff, String filterOn, String gender)
    {
        //Turn off gender
        for (FilterRow row : rows)
        {
            if (row.getTitle().equals(filterOff))
                row.setShowingEvents(false);
            if (row.getTitle().equals(filterOn))
                row.setShowingEvents(true);
        }

        FamilyTree filteredTree = Filter.filterTree(tree);

        Person[] filteredPersons = filteredTree.getPersons();
        Event[] filteredEvents = filteredTree.getEvents();

        //Check all persons are the right gender
        for (Person person : filteredPersons)
        {
            assertTrue(person.getGender().equals(gender));
        }

        //Check persons related to all events are the right gender
        for (Event event : filteredEvents)
        {
            Person personRelatedToEvent = filteredTree.getPerson(event.getPersonID());
            assertTrue(personRelatedToEvent.getGender().equals(gender));
        }

        return true;
    }

    //All but the graduation switch is turned off
    @Test
    public void testEventsFilter()
    {
        //Turn off these events
        for (FilterRow row : rows)
        {
            if (row.getTitle().contains("Baptism") ||
                row.getTitle().contains("Birth") ||
                row.getTitle().contains("Death") ||
                row.getTitle().contains("Marriage"))
            {
                row.setShowingEvents(false);
            }
        }

        FamilyTree filteredTree = Filter.filterTree(tree);

        Event[] filteredEvents = filteredTree.getEvents();

        //Events should be a graduation event
        for (Event event : filteredEvents)
        {
            assertTrue(event.getEventType().toLowerCase().contains("graduation"));
        }
    }
}
