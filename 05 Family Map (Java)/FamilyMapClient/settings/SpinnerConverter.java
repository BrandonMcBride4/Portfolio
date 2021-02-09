package edu.byu.cs.students.familymap.settings;

import android.graphics.Color;

import com.google.android.gms.maps.GoogleMap;

/**
 * Created by Brandon on 8/11/2017.
 */

//Line colors and map type enum
public enum SpinnerConverter
{
    Red(Color.RED),
    Green(Color.GREEN),
    Blue(Color.BLUE),
    Yellow(Color.YELLOW),
    White(Color.WHITE),
    Gray(Color.GRAY),
    Black(Color.BLACK),
    Normal(GoogleMap.MAP_TYPE_NORMAL),
    Hybrid(GoogleMap.MAP_TYPE_HYBRID),
    Satellite(GoogleMap.MAP_TYPE_SATELLITE),
    Terrain(GoogleMap.MAP_TYPE_TERRAIN);

    private int value;

    SpinnerConverter(int value)
    {
        this.value = value;
    }

    public int getValue()
    {
        return value;
    }
}
