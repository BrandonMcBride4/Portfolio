package edu.byu.cs.students.familymap.model;

import org.apache.commons.text.WordUtils;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;

/**
 * Created by bmcbrid4 on 8/1/2017.
 */

public class FamilyTree implements Serializable
{
    private Person[] persons;
    private Event[] events;

    private List<String> eventTypes;
    private List<Person> motherSide;
    private List<Person> fatherSide;
    private Person user;

    public FamilyTree (Person[] persons, Event[] events)
    {
        this.persons = persons;
        this.events = events;

        initEventTypes();

        if (persons.length > 0)
        {
            user = this.persons[0];
            initMotherSide(user);
            initFatherSide(user);
        }
    }

    //Used to sort Event objects by Event Year
    private class EventTypeComparator implements Comparator<Event>
    {
        @Override
        public int compare(Event o1, Event o2)
        {
            //Birth is always first
            if (o1.getEventType().toLowerCase().equals("birth"))
                return -1;
            else if (o2.getEventType().toLowerCase().equals("birth"))
                return 1;
            //Death is always last
            else if (o1.getEventType().toLowerCase().equals("death"))
                return 1;
            else if (o2.getEventType().toLowerCase().equals("death"))
                return -1;

            if (o1.getYear() == o2.getYear())
            {
                //Alphabetical if same year
                return o1.getEventType().compareToIgnoreCase(o2.getEventType());
            }
            else
                return o1.getYear() < o2.getYear()? -1 : 1;
        }
    }

    //Person activity
    public List<Event> sortEventsForPerson(String personID)
    {
        List<Event> eventsForPerson = new LinkedList<>();

        for (Event event : events)
        {
            //Add events related to the person
            if (event.getPersonID().equals(personID))
            {
                eventsForPerson.add(event);
            }
        }

        //Sort list with new comparator
        Collections.sort(eventsForPerson, new EventTypeComparator());

        return eventsForPerson;
    }

    //Person activity
   public Person getChild(String parentID)
    {
        //The user is not a parent
        if (parentID.equals(user.getPersonID()))
            return null;

        //If the personID is somebody's mother or father, that is their child
        for (Person person: persons)
        {
            if (person.getMotherID() != null &&
                person.getFatherID() != null)
            {
                if (person.getMotherID().equals(parentID) ||
                        person.getFatherID().equals(parentID))
                    return person;
            }
        }

        return null;
    }

    //Filter Activity
    private void initEventTypes()
    {
        eventTypes = new ArrayList<String>();
        for (Event event : events)
        {
            String eventType = WordUtils.capitalizeFully(event.getEventType()) + " Events";
            if (!eventTypes.contains(eventType))
            {
                eventTypes.add(eventType);
            }
        }

        //Sort events
        Collections.sort(eventTypes);
    }

    private void initMotherSide(Person root)
    {
        motherSide = new ArrayList<Person>();

        //Find the Mom
        Person mom = null;
        for (Person person : persons)
        {
            if (person.getPersonID().equals(root.getMotherID()))
            {
                mom = person;
                motherSide.add(mom);
            }
        }

        //Get the mom's ancestors
        if (mom != null)
        {
           getAncestors(mom, motherSide, 2);
        }
    }

    private void initFatherSide(Person root)
    {
        fatherSide = new ArrayList<Person>();

        //Find the Dad
        Person dad = null;
        for (Person person : persons)
        {
            if (person.getPersonID().equals(root.getFatherID()))
            {
                dad = person;
                fatherSide.add(dad);
            }
        }

        //Get the dad's ancestors
        if (dad != null)
        {
            getAncestors(dad, fatherSide, 2);
        }
    }

    //Root is going to be the mother or father
    //On initial call, generations should be 1 signifying
    //that the next person in the list is one generation ahead
    //The list in this function needs to be a data member to pass by reference
    public void getAncestors(Person root, List<Person> ancestors, int generations)
    {
        for (Person person : persons)
        {
            //Find the mother or father
            if (person.getPersonID().equals(root.getMotherID()))
            {
                //Setting the generations is used to draw the family lines
                person.setGenerations(generations);
                ancestors.add(person);

                //Gets the mother's ancestors
                getAncestors(person, ancestors, generations + 1);
            }
            else if (person.getPersonID().equals(root.getFatherID()))
            {
                //Setting the generations is used to draw the family lines
                person.setGenerations(generations);
                ancestors.add(person);

                //Gets the father's ancestors
                getAncestors(person, ancestors, generations + 1);
            }
        }
    }

    public List<String> getEventTypes()
    {
        return eventTypes;
    }

    public List<Person> getMotherSide()
    {
        return motherSide;
    }

    public List<Person> getFatherSide()
    {
        return fatherSide;
    }

    public Person getPerson(String personID)
    {
        if (personID == null)
            return null;

        for (Person person : persons)
        {
            if (personID.equals(person.getPersonID()))
                return person;
        }

        return null;
    }

    public Person[] getPersons()
    {
        return persons;
    }

    public Event[] getEvents()
    {
        return events;
    }
}
