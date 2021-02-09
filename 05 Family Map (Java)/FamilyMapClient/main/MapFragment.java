package edu.byu.cs.students.familymap.main;

import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.Icon;
import android.os.Bundle;
import android.support.v4.app.NavUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.google.android.gms.maps.CameraUpdate;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.Polyline;

import com.joanzapata.iconify.IconDrawable;
import com.joanzapata.iconify.fonts.FontAwesomeIcons;

import java.util.LinkedList;
import java.util.List;

import edu.byu.cs.students.familymap.filter.*;
import edu.byu.cs.students.familymap.person.PersonActivity;
import edu.byu.cs.students.familymap.R;
import edu.byu.cs.students.familymap.model.*;
import edu.byu.cs.students.familymap.search.SearchActivity;
import edu.byu.cs.students.familymap.settings.*;

/**
 * Created by Brandon on 8/7/2017.
 */

public class MapFragment extends SupportMapFragment
{
    private final String TAG = "MapFragment";
    private final String INITIAL_PERSON_TEXT = "Click on a marker";
    private final String INITIAL_EVENT_TEXT = "to see event details";
    private final String MALE = "m";

    private final int MENU_ICON_SIZE = 24;
    private final int GENDER_ICON_SIZE = 40;

    private static final String ARG_EVENT = "event";

    private FamilyTree tree = Filter.filterTree(LoginFragment.getTree());
    private Event selectedEvent;
    private Person selectedPerson;
    private GoogleMap map;

    static List<Polyline> lines;
    private int mapType;

    private ImageView genderIcon;
    private TextView personText;
    private TextView eventText;

    public static MapFragment newInstance(Event event)
    {

        Bundle args = new Bundle();
        args.putSerializable(ARG_EVENT, event);

        MapFragment fragment = new MapFragment();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle bundle)
    {
        super.onCreate(bundle);
        setHasOptionsMenu(true);

        if (getArguments() != null)
        {
            selectedEvent = (Event) getArguments().getSerializable(ARG_EVENT);
        }

        initialize();
    }

    private void initialize()
    {
        tree = Filter.filterTree(LoginFragment.getTree());
        SettingsValues settings = SettingsValues.getSettingsValues();
        mapType = SettingsValues.getSpinnerValue(settings.getMapType());
        lines = new LinkedList<>();
    }

    @Override
    public View onCreateView(LayoutInflater layoutInflater, ViewGroup viewGroup, Bundle bundle)
    {
        super.onCreateView(layoutInflater, viewGroup, bundle);
        View v = layoutInflater.inflate(R.layout.fragment_map, viewGroup, false);

        //Link views
        LinearLayout personLayout = (LinearLayout) v.findViewById(R.id.person_map_layout);
        genderIcon = (ImageView) v.findViewById(R.id.gender_icon);
        personText = (TextView) v.findViewById(R.id.person_text);
        eventText = (TextView) v.findViewById(R.id.event_text);

        //Initialize the person box
        personText.setText(INITIAL_PERSON_TEXT);
        eventText.setText(INITIAL_EVENT_TEXT);
        genderIcon.setImageIcon(Icon.createWithResource(getActivity(), R.mipmap.ic_launcher));

        SupportMapFragment mapFragment =
                (SupportMapFragment) getChildFragmentManager()
                                     .findFragmentById(R.id.map_fragment);

        mapFragment.getMapAsync(new OnMapReadyCallback()
        {
            @Override
            public void onMapReady(GoogleMap googleMap)
            {
                map = googleMap;
                updateUI();

                map.setOnMarkerClickListener(new GoogleMap.OnMarkerClickListener()
                {
                    @Override
                    public boolean onMarkerClick(Marker marker)
                    {
                        selectedEvent = (Event) marker.getTag();
                        processEvent();

                        return false;
                    }
                });

                //Centers map on Event for MapActivity at the beginning
                beginningEvent();
            }
        });

        personLayout.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                //selectedPerson is null in the beginning before any person is clicked
                if (selectedPerson != null)
                {
                    Intent intent = PersonActivity.newIntent(getActivity(), selectedPerson);
                    startActivity(intent);
                }
            }
        });
        return v;
    }

    private void updateUI()
    {
        if (map == null)
        {
            return;
        }

        map.clear();
        map.setMapType(mapType);
        new MarkerMaker(tree, map).createAllMarkers();
    }

    private void beginningEvent()
    {
        //Centers map on Event for MapActivity at the beginning
        if (selectedEvent != null)
        {
            moveCameraToEvent(selectedEvent);
            processEvent();
        }
    }

    private void moveCameraToEvent(Event event)
    {
        LatLng position = event.getEventLatLng();
        CameraUpdate update = CameraUpdateFactory.newLatLng(position);
        map.animateCamera(update);
    }

    private void processEvent()
    {
        if (selectedEvent != null)
        {
            showSelectedPerson();
            new MapLineDrawer(tree, map, selectedPerson, selectedEvent).drawLines();
        }
    }

    private void showSelectedPerson()
    {
        selectedPerson = tree.getPerson(selectedEvent.getPersonID());

        String gender = selectedPerson.getGender();
        drawGenderIcon(gender);

        personText.setText(selectedPerson.getFirstName() + " " + selectedPerson.getLastName());
        eventText.setText(selectedEvent.eventStringFormat());
    }

    private void drawGenderIcon(String gender)
    {
        if (gender.equals(MALE))
        {
            Drawable maleIcon = new IconDrawable(getActivity(), FontAwesomeIcons.fa_male)
                    .colorRes(R.color.male_icon)
                    .sizeDp(GENDER_ICON_SIZE);
            genderIcon.setImageDrawable(maleIcon);
        }
        else
        {

            Drawable femaleIcon = new IconDrawable(getActivity(), FontAwesomeIcons.fa_female)
                    .colorRes(R.color.female_icon)
                    .sizeDp(GENDER_ICON_SIZE);
            genderIcon.setImageDrawable(femaleIcon);
        }
    }

    //Inflate the menu
    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater)
    {
        //Determines if the fragment came from the MainActivity or MapActivity
        if (getActivity().getClass().equals(MapActivity.class))
        {
            inflater.inflate(R.menu.menu_person, menu);

            Drawable doubleUpIcon = new IconDrawable(
                    getActivity(),
                    FontAwesomeIcons.fa_angle_double_up)
                    .colorRes(R.color.white).sizeDp(MENU_ICON_SIZE);
            MenuItem doubleUpArrow = (MenuItem) menu.findItem(R.id.person_double_up_arrow);
            doubleUpArrow.setIcon(doubleUpIcon);
        }
        else
        {
            super.onCreateOptionsMenu(menu, inflater);
            inflater.inflate(R.menu.menu_map, menu);


//        //Connect widgets
            MenuItem optionsSearch = (MenuItem) menu.findItem(R.id.options_search);
            MenuItem optionsFilter = (MenuItem) menu.findItem(R.id.options_filter);
            MenuItem optionsSettings = (MenuItem) menu.findItem(R.id.options_settings);

            //Set Icons
            setIcon(optionsSearch, FontAwesomeIcons.fa_search);
            setIcon(optionsFilter, FontAwesomeIcons.fa_filter);
            setIcon(optionsSettings, FontAwesomeIcons.fa_gear);

            //Set on click listeners
            //When the search button is touched
            optionsSearch.setOnMenuItemClickListener(new MenuItem.OnMenuItemClickListener()
            {
                @Override
                public boolean onMenuItemClick(MenuItem item)
                {
                    Intent intent = SearchActivity.newIntent(getActivity());
                    startActivity(intent);
                    return false;
                }
            });

            //When the filter button is touched
            optionsFilter.setOnMenuItemClickListener(new MenuItem.OnMenuItemClickListener()
            {
                @Override
                public boolean onMenuItemClick(MenuItem item)
                {
                    Intent intent = FilterActivity.newIntent(getActivity());
                    startActivity(intent);
                    return false;
                }
            });

            //When the settings button is touched
            optionsSettings.setOnMenuItemClickListener(new MenuItem.OnMenuItemClickListener()
            {
                @Override
                public boolean onMenuItemClick(MenuItem item)
                {
                    Intent intent = SettingsActivity.newIntent(getActivity());
                    startActivity(intent);
                    return false;
                }
            });
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        //Determines if the activity came from the MapActivity or MainActivity
        if (getActivity().getClass().equals(MapActivity.class))
        {
            switch (item.getItemId())
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
        }
        return super.onOptionsItemSelected(item);
    }

    private void setIcon(MenuItem item, FontAwesomeIcons icon)
    {
        Drawable drawable = new IconDrawable(getActivity(), icon)
                .colorRes(R.color.white)
                .sizeDp(MENU_ICON_SIZE);
        item.setIcon(drawable);
    }

    @Override
    public void onResume()
    {
        super.onResume();
        initialize();
        updateUI();
    }
}