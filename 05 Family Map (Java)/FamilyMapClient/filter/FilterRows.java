package edu.byu.cs.students.familymap.filter;

import android.util.Log;

import java.util.ArrayList;
import java.util.List;

import edu.byu.cs.students.familymap.model.FamilyTree;

/**
 * Created by Brandon on 8/9/2017.
 */

//List of each filter
//Singleton so that it holds its value throughout the app
public class FilterRows
{
    final String FATHER = "Father's Side";
    final String MOTHER = "Mother's Side";
    final String MALE = "Male Events";
    final String FEMALE = "Female Events";

    private static FilterRows filterRows;

    private List<FilterRow> rows;

    public static FilterRows get(FamilyTree tree)
    {
        if (filterRows == null)
            filterRows = new FilterRows(tree);
        return filterRows;
    }

    private FilterRows(FamilyTree originalTree)
    {
        rows = new ArrayList<>();
        List<String> filterTitles = originalTree.getEventTypes();
        filterTitles.add(FATHER);
        filterTitles.add(MOTHER);
        filterTitles.add(MALE);
        filterTitles.add(FEMALE);

        for (String title : filterTitles)
        {
            FilterRow value = new FilterRow(title);
            rows.add(value);
        }
    }

    public List<FilterRow> getRows()
    {
        return rows;
    }
}
