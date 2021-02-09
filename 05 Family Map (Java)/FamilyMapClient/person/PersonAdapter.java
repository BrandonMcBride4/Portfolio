package edu.byu.cs.students.familymap.person;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.bignerdranch.expandablerecyclerview.Adapter.ExpandableRecyclerAdapter;

import java.util.List;

import edu.byu.cs.students.familymap.R;

/**
 * Created by Brandon on 8/10/2017.
 */

public class PersonAdapter
        extends ExpandableRecyclerAdapter<PersonParentViewHolder, PersonChildViewHolder>
{
    private LayoutInflater inflater;

    public PersonAdapter(Context context, List parentItemList)
    {
        super(context, parentItemList);
        inflater = LayoutInflater.from(context);
    }

    //Inflate the list when it is collapsed
    @Override
    public PersonParentViewHolder onCreateParentViewHolder(ViewGroup viewGroup)
    {
        View view = inflater.inflate(R.layout.holder_parent_view, viewGroup, false);
        return new PersonParentViewHolder(view);
    }

    //Inflate the list when it is expanded
    @Override
    public PersonChildViewHolder onCreateChildViewHolder(ViewGroup viewGroup)
    {
        View view = inflater.inflate(R.layout.holder_child_view, viewGroup, false);
        return new PersonChildViewHolder(view);
    }

    //Bind the parent info
    @Override
    public void onBindParentViewHolder(PersonParentViewHolder parentViewHolder, int i, Object o)
    {
        PersonParentObject personParentObject = (PersonParentObject) o;
        parentViewHolder.parentText.setText(personParentObject.title);
    }

    //Bind the event or family info
    @Override
    public void onBindChildViewHolder(PersonChildViewHolder childViewHolder, int i, Object o)
    {
        EventFamilyModel eventFamilyModel = (EventFamilyModel) o;
        childViewHolder.bind(eventFamilyModel);
    }
}
