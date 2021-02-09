package edu.byu.cs.students.familymap.filter;

/**
 * Created by Brandon on 8/9/2017.
 */

//Model for each filter
//Title is either and event, Father or Mother Side, or Gender
//Description changes based on title type described above
public class FilterRow
{
    private String title;
    private String description;
    private boolean showingEvents = true;

    public FilterRow(String title)
    {
        this.title = title;

        if (title.contains("Side"))
            description = "FILTER BY " + title.toUpperCase() + " SIDE OF FAMILY";
        else if (title.toLowerCase().contains("male"))
            description = "FILTER EVENTS BASED ON GENDER";
        else
            description = "FILTER BY " + title.toUpperCase();
    }

    public boolean isShowingEvents()
    {
        return showingEvents;
    }

    public void setShowingEvents(boolean showingEvents)
    {
        this.showingEvents = showingEvents;
    }

    public String getTitle()
    {
        return title;
    }

    public String getDescription()
    {
        return description;
    }

}
