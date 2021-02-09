package edu.byu.cs.students.familymap.search;

import java.util.LinkedList;
import java.util.List;

import edu.byu.cs.students.familymap.model.Event;
import edu.byu.cs.students.familymap.model.FamilyTree;
import edu.byu.cs.students.familymap.model.Person;
import edu.byu.cs.students.familymap.person.EventFamilyModel;

/**
 * Created by Brandon on 8/13/2017.
 */

public class Searcher
{
    private FamilyTree tree;

    public Searcher(FamilyTree tree)
    {
        this.tree = tree;
    }

    public List<EventFamilyModel> search(String query)
    {
        List<EventFamilyModel> resultList = new LinkedList<>();
        query = query.toLowerCase();

        //Persons show up before events
        for (Person person : tree.getPersons())
        {
            String name = person.getFullName().toLowerCase();

            //Persons are only searched by first and last name
            if (name.contains(query))
            {
                resultList.add(new EventFamilyModel(person.getFullName(), "", person.getGender(), person, null));
            }
        }

        for (Event event : tree.getEvents())
        {
            String country = event.getCountry().toLowerCase();
            String city = event.getCity().toLowerCase();
            String eventType = event.getEventType().toLowerCase();
            String year = String.valueOf(event.getYear());

            //Events are searched by city, country, eventType, and year)
            if (country.contains(query) ||
                city.contains(query) ||
                eventType.contains(query) ||
                year.contains(query))
            {
                String title = event.eventStringFormat();
                String personName = tree.getPerson(event.getPersonID()).getFullName();
                resultList.add(new EventFamilyModel(title, personName, "event", null, event));
            }
        }
        return resultList;
    }
}
