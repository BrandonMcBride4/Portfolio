package edu.byu.cs.students.familymap.main;

import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.model.Polyline;
import com.google.android.gms.maps.model.PolylineOptions;

import java.util.LinkedList;
import java.util.List;

import edu.byu.cs.students.familymap.model.Event;
import edu.byu.cs.students.familymap.model.FamilyTree;
import edu.byu.cs.students.familymap.model.Person;
import edu.byu.cs.students.familymap.settings.SettingsValues;

/**
 * Created by Brandon on 8/14/2017.
 */

public class MapLineDrawer
{
    final int DEFAULT_LINE_WIDTH = 10;
    final int BASE_GENERATIONS = 0;
    final float THICKEST_FAMILY_LINE_WIDTH = 24;

    FamilyTree tree;
    GoogleMap map;
    Person selectedPerson;
    Event selectedEvent;
    SettingsValues settings;

    private List<Polyline> lines = MapFragment.lines;
    private List<Person> ancestors;
    private int lifeLinesColor;
    private int familyLinesColor;
    private int spouseLinesColor;

    public MapLineDrawer(FamilyTree tree, GoogleMap map, Person selectedPerson, Event selectedEvent)
    {
        this.tree = tree;
        this.map = map;
        this.selectedPerson = selectedPerson;
        this.selectedEvent = selectedEvent;

        settings = SettingsValues.getSettingsValues();
        lifeLinesColor = SettingsValues.getSpinnerValue(settings.getLifeLinesColor());
        familyLinesColor = SettingsValues.getSpinnerValue(settings.getFamilyLinesColor());
        spouseLinesColor = SettingsValues.getSpinnerValue(settings.getSpouseLinesColor());
    }

    public void drawLines()
    {
        //If not called, lines won't erase when you select a different event
        removePreviousLines();

        //Checks if the switch is on or off from the settings
        if (settings.isLifeLinesShowing())
            drawLifeLines();
        if (settings.isFamilyLinesShowing())
            drawFamilyLines();
        if (settings.isSpouseLinesShowing())
            drawSpouseLines();
    }

    private void removePreviousLines()
    {
        for (int i = 0; i < lines.size(); i++)
        {
            lines.get(i).remove();
        }
        lines.clear();
    }

    private void drawLifeLines()
    {
        List<Event> events = tree.sortEventsForPerson(selectedPerson.getPersonID());

        //size - 1 because we are reaching one ahead
        // so if not it would reach one ahead out of bounds
        for (int i = 0; i < events.size() - 1; i++)
        {
            drawLine(events.get(i), events.get(i+1), lifeLinesColor, DEFAULT_LINE_WIDTH);
        }
    }

    private void drawFamilyLines()
    {
        //Since getAncestors is recursive, it adds to the ancestors list by reference.
        ancestors = new LinkedList<>();
        tree.getAncestors(selectedPerson, ancestors, BASE_GENERATIONS);
        drawLinesToParents(selectedPerson);

        for (int i = 0; i < ancestors.size(); i++)
        {
            Person ancestor = ancestors.get(i);
            drawLinesToParents(ancestor);
        }
    }

    private void drawLinesToParents(Person child)
    {
        if (child.getFatherID() != null)
        {
            Event childBirth = getChildBirth(child);

            //If person has a father, they must have a mother because...biology
            Person father = tree.getPerson(child.getFatherID());
            Person mother = tree.getPerson(child.getMotherID());

            drawLineToParent(childBirth, father);
            drawLineToParent(childBirth, mother);
        }
    }

    private Event getChildBirth(Person child)
    {
        //The root should should be the selected event, not the earliest event
        Event childBirth;
        if (child.getPersonID().equals(selectedPerson.getPersonID()))
        {
            childBirth = selectedEvent;
        }
        else
        {
            childBirth = tree.sortEventsForPerson(child.getPersonID()).get(0);
        }

        return childBirth;
    }

    private void drawLineToParent(Event childBirth, Person parent)
    {
        float width = calculateWidthBasedOnGeneration(parent.getGenerations());

        //Mother or father could be filtered out, even though their id is not null
        if (parent != null)
        {
            Event fatherBirth = tree.sortEventsForPerson(parent.getPersonID()).get(0);
            drawLine(childBirth, fatherBirth, familyLinesColor, width);
        }
    }

    private float calculateWidthBasedOnGeneration(int generation)
    {
        //Line's width half every generation
        float thickestWidth = THICKEST_FAMILY_LINE_WIDTH;
        float width = ((float) (thickestWidth / (Math.pow(2, generation))));

        //Width can't be negative
        if (width < 0)
        {
            width = 0;
        }

        return width;
    }

    private void drawSpouseLines()
    {
        //Get spouse
        String spouseID = selectedPerson.getSpouseID();
        Person spouse = tree.getPerson(spouseID);

        //spouseID could exist and spouse be null if the spouse is being filtered
        if (spouse != null)
        {
            Event spouseBirth = tree.sortEventsForPerson(spouseID).get(0);
            drawLine(selectedEvent, spouseBirth, spouseLinesColor, DEFAULT_LINE_WIDTH);
        }
    }

    private void drawLine(Event e1, Event e2, int color, float width)
    {
        PolylineOptions polylineOptions = new PolylineOptions();
        polylineOptions.add(e1.getEventLatLng());
        polylineOptions.add(e2.getEventLatLng());
        polylineOptions.color(color).width(width);
        //Add line to the list and map
        lines.add(map.addPolyline(polylineOptions));
    }
}
