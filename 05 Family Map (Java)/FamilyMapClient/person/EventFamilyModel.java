package edu.byu.cs.students.familymap.person;


import edu.byu.cs.students.familymap.model.Event;
import edu.byu.cs.students.familymap.model.Person;

/**
 * Created by Brandon on 8/10/2017.
 */

//Model for both events and family
//Used for Person activity and Search Activity
public class EventFamilyModel
{

    public String titleString;
    public String descriptionString;
    public String type;
    public Person kin;
    public Event event;

    //Type is either event, m, or f
    public EventFamilyModel(String titleString, String descriptionString, String type, Person kin, Event event)
    {
        this.titleString = titleString;
        this.descriptionString = descriptionString;
        this.type = type;
        this.event = event;
        this.kin = kin;
    }
}
