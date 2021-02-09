package edu.byu.cs.students.familymap.person;

import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.NavUtils;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.bignerdranch.expandablerecyclerview.Model.ParentObject;
import com.joanzapata.iconify.IconDrawable;
import com.joanzapata.iconify.fonts.FontAwesomeIcons;

import java.util.ArrayList;
import java.util.List;

import edu.byu.cs.students.familymap.R;
import edu.byu.cs.students.familymap.main.LoginFragment;
import edu.byu.cs.students.familymap.model.FamilyTree;
import edu.byu.cs.students.familymap.model.Person;

/**
 * Created by Brandon on 8/8/2017.
 */

public class PersonFragment extends Fragment
{
    private static final String ARG_PERSON = "kin";

    private final int MENU_ICON_SIZE = 24;

    private RecyclerView recyclerView;
    private LinearLayoutManager linearLayoutManager;

    private Person person;

    public static PersonFragment newInstance(Person person)
    {
        Bundle args = new Bundle();
        args.putSerializable(ARG_PERSON, person);

        PersonFragment fragment = new PersonFragment();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        if (getArguments() != null)
        {
            person = (Person) getArguments().getSerializable(ARG_PERSON);
        }
        setHasOptionsMenu(true);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        View v = inflater.inflate(R.layout.fragment_person, container, false);

        //Connect Text views to view
        TextView firstNameTextView = (TextView) v.findViewById(R.id.person_first_name);
        TextView lastNameTextView = (TextView) v.findViewById(R.id.person_last_name);
        TextView genderTextView = (TextView) v.findViewById(R.id.person_gender);

        //Set text View
        firstNameTextView.setText(person.getFirstName());
        lastNameTextView.setText(person.getLastName());
        if (person.getGender().toLowerCase().equals("m"))
            genderTextView.setText("Male");
        else
            genderTextView.setText("Female");

        linearLayoutManager = new LinearLayoutManager(getContext());
        linearLayoutManager.setOrientation(LinearLayoutManager.VERTICAL);

        //Make Adapter
        PersonAdapter PersonAdapter = new PersonAdapter(getContext(), makeParentList());
        PersonAdapter.setParentAndIconExpandOnClick(true);
        PersonAdapter.setParentClickableViewAnimationDefaultDuration();
        PersonAdapter.setCustomParentAnimationViewId(R.id.parent_view_holder_arrow);

        //Connect Recycler View
        recyclerView = (RecyclerView) v.findViewById(R.id.person_collapsible_recycler_view);
        recyclerView.setLayoutManager(linearLayoutManager);
        recyclerView.setAdapter(PersonAdapter);

        return v;
    }

    private List<ParentObject> makeParentList()
    {
        List<ParentObject> parentList = new ArrayList<>();
        parentList.add(new PersonParentObject(person, PersonParentObject.EVENT_TITLE));
        parentList.add(new PersonParentObject(person, PersonParentObject.FAMILY_TITLE));
        return parentList;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater)
    {
        super.onCreateOptionsMenu(menu, inflater);
        inflater.inflate(R.menu.menu_person, menu);

        Drawable doubleUpIcon = new IconDrawable(getActivity(), FontAwesomeIcons.fa_angle_double_up)
                                    .colorRes(R.color.white).sizeDp(MENU_ICON_SIZE);
        MenuItem doubleUpArrow = (MenuItem) menu.findItem(R.id.person_double_up_arrow);
        doubleUpArrow.setIcon(doubleUpIcon);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        switch(item.getItemId())
        {
            //When the double up is pressed, go to parent activity (Map)
            case R.id.person_double_up_arrow:
                NavUtils.navigateUpFromSameTask(getActivity());
                return true;
            //When up button is pressed go back
            case android.R.id.home:
                getActivity().onBackPressed();
                return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
