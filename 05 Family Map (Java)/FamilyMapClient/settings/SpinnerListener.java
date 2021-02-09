package edu.byu.cs.students.familymap.settings;

import android.view.View;
import android.widget.AdapterView;

/**
 * Created by Brandon on 8/11/2017.
 */

public class SpinnerListener implements AdapterView.OnItemSelectedListener
{
    private String type;
    private String spinnerValue;

    public SpinnerListener(String type)
    {
        this.type = type;
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id)
    {
        spinnerValue = parent.getItemAtPosition(position).toString();
        SettingsValues values = SettingsValues.getSettingsValues();

        //Assigns the setting's spinner based on the spinner type
        switch (type)
        {
            case SettingsFragment.SPINNER_LIFE_LINE:
                values.setLifeLinesColor(spinnerValue);
                break;
            case SettingsFragment.SPINNER_FAMILY_LINE:
                values.setFamilyLinesColor(spinnerValue);
                break;
            case SettingsFragment.SPINNER_SPOUSE_LINE:
                values.setSpouseLinesColor(spinnerValue);
                break;
            case SettingsFragment.SPINNER_MAP:
                values.setMapType(spinnerValue);
                break;
        }
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent)
    {

    }

    public String getType()
    {
        return type;
    }

    public String getSpinnerValue()
    {
        return spinnerValue;
    }
}
