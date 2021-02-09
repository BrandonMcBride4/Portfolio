package edu.byu.cs.students.familymap.settings;

/**
 * Created by Brandon on 8/11/2017.
 */

//Singleton so that it holds its value throughout the app
public class SettingsValues
{
    private static SettingsValues settingsValues;

    private boolean lifeLinesShowing;
    private boolean familyLinesShowing;
    private boolean spouseLinesShowing;

    private String lifeLinesColor;
    private String familyLinesColor;
    private String spouseLinesColor;
    private String mapType;

    public static SettingsValues getSettingsValues()
    {
        if (settingsValues == null)
            settingsValues = new SettingsValues();

        return settingsValues;
    }

    //Default values for the settings;
    private SettingsValues()
    {
        lifeLinesShowing = true;
        familyLinesShowing = true;
        spouseLinesShowing = true;
        lifeLinesColor = "Red";
        familyLinesColor = "Green";
        spouseLinesColor = "Blue";
        mapType = "Normal";
    }

    //Returns a color int or MapType int
    public static int getSpinnerValue(String settingsValue)
    {
        SpinnerConverter[] values = SpinnerConverter.values();
        for (SpinnerConverter value : values)
        {
            if (settingsValue.equals(value.name()))
                return value.getValue();
        }
        return -1;
    }

    public static void setSettingsValues(SettingsValues settingsValues)
    {
        SettingsValues.settingsValues = settingsValues;
    }

    public boolean isLifeLinesShowing()
    {
        return lifeLinesShowing;
    }

    public void setLifeLinesShowing(boolean lifeLinesShowing)
    {
        this.lifeLinesShowing = lifeLinesShowing;
    }

    public boolean isFamilyLinesShowing()
    {
        return familyLinesShowing;
    }

    public void setFamilyLinesShowing(boolean familyLinesShowing)
    {
        this.familyLinesShowing = familyLinesShowing;
    }

    public boolean isSpouseLinesShowing()
    {
        return spouseLinesShowing;
    }

    public void setSpouseLinesShowing(boolean spouseLinesShowing)
    {
        this.spouseLinesShowing = spouseLinesShowing;
    }

    public String getLifeLinesColor()
    {
        return lifeLinesColor;
    }

    public void setLifeLinesColor(String lifeLinesColor)
    {
        this.lifeLinesColor = lifeLinesColor;
    }

    public String getFamilyLinesColor()
    {
        return familyLinesColor;
    }

    public void setFamilyLinesColor(String familyLinesColor)
    {
        this.familyLinesColor = familyLinesColor;
    }

    public String getSpouseLinesColor()
    {
        return spouseLinesColor;
    }

    public void setSpouseLinesColor(String spouseLinesColor)
    {
        this.spouseLinesColor = spouseLinesColor;
    }

    public String getMapType()
    {
        return mapType;
    }

    public void setMapType(String mapType)
    {
        this.mapType = mapType;
    }
}
