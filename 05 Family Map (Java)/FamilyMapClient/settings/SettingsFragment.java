package edu.byu.cs.students.familymap.settings;

import android.os.Bundle;
import android.support.constraint.ConstraintLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.NavUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.Toast;

import edu.byu.cs.students.familymap.R;
import edu.byu.cs.students.familymap.main.LoginFragment;
import edu.byu.cs.students.familymap.model.FamilyTree;
import edu.byu.cs.students.familymap.server.PersonsTask;

/**
 * Created by Brandon on 8/11/2017.
 */

public class SettingsFragment extends Fragment
{
    final static String SPINNER_LIFE_LINE = "life";
    final static String SPINNER_FAMILY_LINE = "family";
    final static String SPINNER_SPOUSE_LINE = "spouse";
    final static String SPINNER_MAP = "map type";
    private final int NUMBER_OF_COLORS = 7;
    
    private Switch lifeLinesSwitch;
    private Switch familyLinesSwitch;
    private Switch spouseLinesSwitch;

    private Spinner lifeLinesSpinner;
    private Spinner familyLinesSpinner;
    private Spinner spouseLinesSpinner;
    private Spinner mapTypeSpinner;

    private SpinnerListener lifeLinesSpinnerListener;
    private SpinnerListener familyLinesSpinnerListener;
    private SpinnerListener spouseLinesSpinnerListener;
    private SpinnerListener mapTypeSpinnerListener;

    private ConstraintLayout resyncLayout;
    private ConstraintLayout logoutLayout;

    private SettingsValues settingsValues = SettingsValues.getSettingsValues();

    public static SettingsFragment newInstance()
    {
        Bundle args = new Bundle();

        SettingsFragment fragment = new SettingsFragment();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        //Inflate layout
        View v = inflater.inflate(R.layout.fragment_settings, container, false);

        connectWidgets(v);
        initWidgets();
        setResyncClick();
        setLogoutClick();

        return v;
    }

    private void connectWidgets(View v)
    {
        //Connect Switches
        lifeLinesSwitch = (Switch) v.findViewById(R.id.settings_life_story_lines_switch);
        familyLinesSwitch = (Switch) v.findViewById(R.id.settings_family_tree_lines_switch);
        spouseLinesSwitch = (Switch) v.findViewById(R.id.settings_spouse_lines_switch);

        //Connect Spinners
        lifeLinesSpinner = (Spinner) v.findViewById(R.id.settings_life_story_lines_color_list);
        familyLinesSpinner = (Spinner) v.findViewById(R.id.settings_family_tree_lines_color_list);
        spouseLinesSpinner = (Spinner) v.findViewById(R.id.settings_spouse_lines_color_list);
        mapTypeSpinner = (Spinner) v.findViewById(R.id.settings_map_type_list);

        //Connect ConstraintLayouts
        resyncLayout  = (ConstraintLayout) v.findViewById(R.id.settings_resync_constraintLayout);
        logoutLayout = (ConstraintLayout) v.findViewById(R.id.settings_logout_constraintLayout);
    }

    private void initWidgets()
    {
        initSpinners();
        initSwitches();
    }

    private void initSpinners()
    {
        //Set values for spinners
        setSpinnerValue(lifeLinesSpinner, settingsValues.getLifeLinesColor());
        setSpinnerValue(familyLinesSpinner, settingsValues.getFamilyLinesColor());
        setSpinnerValue(spouseLinesSpinner, settingsValues.getSpouseLinesColor());
        setSpinnerValue(mapTypeSpinner, settingsValues.getMapType());

        //Create Spinner Listeners
        lifeLinesSpinnerListener = new SpinnerListener(SPINNER_LIFE_LINE);
        familyLinesSpinnerListener = new SpinnerListener(SPINNER_FAMILY_LINE);
        spouseLinesSpinnerListener = new SpinnerListener(SPINNER_SPOUSE_LINE);
        mapTypeSpinnerListener = new SpinnerListener(SPINNER_MAP);

        //Connect Spinner Listeners
        lifeLinesSpinner.setOnItemSelectedListener(lifeLinesSpinnerListener);
        familyLinesSpinner.setOnItemSelectedListener(familyLinesSpinnerListener);
        spouseLinesSpinner.setOnItemSelectedListener(spouseLinesSpinnerListener);
        mapTypeSpinner.setOnItemSelectedListener(mapTypeSpinnerListener);
    }

    private void initSwitches()
    {
        //Set values for switches
        lifeLinesSwitch.setChecked(settingsValues.isLifeLinesShowing());
        familyLinesSwitch.setChecked(settingsValues.isFamilyLinesShowing());
        spouseLinesSwitch.setChecked(settingsValues.isSpouseLinesShowing());

        //Create Switch Listeners
        lifeLinesSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener()
        {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
                settingsValues.setLifeLinesShowing(isChecked);
            }
        });
        familyLinesSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener()
        {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
                settingsValues.setFamilyLinesShowing(isChecked);
            }
        });
        spouseLinesSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener()
        {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
                settingsValues.setSpouseLinesShowing(isChecked);
            }
        });
    }

    private void setSpinnerValue(Spinner spinner, String value)
    {
        int arrayPosition = SpinnerConverter.valueOf(value).ordinal();

        //This is to reset for the map type so Normal is set to 0 instead of 7
        if (arrayPosition >= NUMBER_OF_COLORS)
            arrayPosition -= NUMBER_OF_COLORS;

        spinner.setSelection(arrayPosition);
    }

    private void setResyncClick()
    {
        resyncLayout.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                //Resync
//                LoginFragment.setTree(null);
                String command = "/person/";
                String authToken = LoginFragment.getAuthTokenModel().getAuthToken();
                new PersonsTask(getActivity()).execute(command, authToken);
            }
        });
    }

    private void setLogoutClick()
    {
        logoutLayout.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                LoginFragment.setLoggedIn(false);
                NavUtils.navigateUpFromSameTask(getActivity());
            }
        });
    }
}
