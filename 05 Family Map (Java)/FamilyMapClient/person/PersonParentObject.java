package edu.byu.cs.students.familymap.person;

import com.bignerdranch.expandablerecyclerview.Model.ParentObject;

import java.util.ArrayList;
import java.util.List;

import edu.byu.cs.students.familymap.main.LoginFragment;
import edu.byu.cs.students.familymap.model.Event;
import edu.byu.cs.students.familymap.model.FamilyTree;
import edu.byu.cs.students.familymap.model.Person;

/**
 * Created by Brandon on 8/10/2017.
 */

public class PersonParentObject implements ParentObject
{
    static final String EVENT_TITLE = "LIFE EVENTS";
    static final String FAMILY_TITLE = "FAMILY";

    private List<Object> eventList;
    private List<Object> familyList;

    String title;
    private Person person;

    //Title is either LIFE EVENTS, or FAMILY
    //The person is the person of the person activity
    public PersonParentObject(Person p, String title)
    {
        person = p;
        eventList = new ArrayList<>();
        this.title = title;
        FamilyTree tree = LoginFragment.getTree();

        //Add sorted events
        if (title.equals(EVENT_TITLE))
        {
            List<Event> personEventList = tree.sortEventsForPerson(person.getPersonID());
            for (Event event : personEventList)
            {
                String eventString = event.eventStringFormat();
                String name = person.getFullName();
                eventList.add(new EventFamilyModel(eventString, name, "event", null, event));
            }
        }
        //Add Family Members
        else
        {
            familyList = new ArrayList<>();
            addFamily(tree.getPerson(person.getFatherID()), "Father");
            addFamily(tree.getPerson(person.getMotherID()), "Mother");
            addFamily(tree.getPerson(person.getSpouseID()), "Spouse");
            addFamily(tree.getChild(person.getPersonID()), "Child");
        }
    }

    //Add family members to the familyList
    private void addFamily(Person kin, String relation)
    {
        if (kin != null)
        {
            familyList.add(new EventFamilyModel(kin.getFullName(), relation, kin.getGender(), kin, null));
        }
    }

    @Override
    public List<Object> getChildObjectList()
    {
        if (title.equals(EVENT_TITLE))
        {
            return eventList;
        }
        else
        {
            return familyList;
        }
    }

    @Override
    public void setChildObjectList(List<Object> list)
    {
        eventList = list;
        familyList = list;
    }
}
