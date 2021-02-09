package edu.byu.cs.students.familymap.filter;

import android.util.Log;
import android.widget.Toast;


import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import edu.byu.cs.students.familymap.model.Event;
import edu.byu.cs.students.familymap.model.FamilyTree;
import edu.byu.cs.students.familymap.model.Person;

/**
 * Created by Brandon on 8/9/2017.
 */

public class Filter
{
    private static final String TAG = "FilterClass";

    private static FamilyTree tree;
    private static List<Event> events;
    private static List<Person> persons;

    public static FamilyTree filterTree(FamilyTree originalTree)
    {
        tree = originalTree;
        //Needs to be a linked list in order to remove items from it
        events = new LinkedList<>(Arrays.asList(tree.getEvents()));
        persons = new LinkedList<>(Arrays.asList(tree.getPersons()));

        FilterRows filterRows = FilterRows.get(tree);
        List<FilterRow> rows = filterRows.getRows();
        for (FilterRow row : rows)
        {
            //Check the switches
            if (!row.isShowingEvents())
            {
                //If any of the event switches are off
                filterEvents(row.getTitle());

                //Mother side off
                if (row.getTitle().equals(filterRows.MOTHER))
                    filterParentSide(tree.getMotherSide());

                //Father side off
                if (row.getTitle().equals(filterRows.FATHER))
                    filterParentSide(tree.getFatherSide());

                //Male Events off
                if (row.getTitle().equals(filterRows.MALE))
                    filterGender("m");

                //Female Events off
                if (row.getTitle().equals(filterRows.FEMALE))
                    filterGender("f");
            }
        }

        //Convert result to a new tree
        Person[] personArray = new Person[persons.size()];
        personArray = persons.toArray(personArray);

        Event[] eventArray = new Event[events.size()];
        eventArray = events.toArray(eventArray);

        return new FamilyTree(personArray, eventArray);
    }

    private static void filterEvents(String eventType)
    {
        for (int i = 0; i < events.size(); i++)
        {
            if (eventType.toLowerCase().contains(events.get(i).getEventType().toLowerCase()))
            {
                events.remove(i);
                i--; //If not decremented, it will skip
            }
        }
    }

    private static void filterParentSide(List<Person> side)
    {
        for (Person person : side)
        {
            removeEvents(person);
            removePerson(person);
        }
    }

    private static void filterGender(String gender)
    {
        for (int i = 0; i < persons.size(); i++)
        {
            Person person = persons.get(i);
            if (person.getGender().toLowerCase().equals(gender.toLowerCase()))
            {

                removeEvents(person);
                removePerson(person);
                i--; //If not decremented, it will skip
            }
        }
    }

    private static void removeEvents(Person person)
    {
        for (int i = 0; i < events.size(); i++)
        {
            if (events.get(i).getPersonID().equals(person.getPersonID()))
            {
                events.remove(i);
                i--; //If not decremented, it will skip
            }
        }
    }

    private static void removePerson(Person person)
    {
        for (int i = 0; i < persons.size(); i++)
        {
            if (persons.get(i).getPersonID().equals(person.getPersonID()))
            {
                persons.remove(i);
                i--; //If not decremented, it will skip
            }
        }
    }

    public static List<Event> getEvents()
    {
        return events;
    }
}
