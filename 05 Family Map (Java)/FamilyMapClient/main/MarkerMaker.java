package edu.byu.cs.students.familymap.main;

import android.util.Log;

import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;

import java.util.List;

import edu.byu.cs.students.familymap.model.Event;
import edu.byu.cs.students.familymap.model.FamilyTree;

/**
 * Created by Brandon on 8/14/2017.
 */

public class MarkerMaker
{
    private static final String TAG = "MarkerMaker";

    private FamilyTree tree;
    private GoogleMap map;

    public MarkerMaker(FamilyTree tree, GoogleMap map)
    {
        this.tree = tree;
        this.map = map;
    }

    public void createAllMarkers()
    {
        //Use enum to assign mapColorHues by event type
        //Use the original tree in order to keep the mapColorHues consistent
        List<String> eventTypes = LoginFragment.getTree().getEventTypes();
        MapColorHue[] mapColorHues = MapColorHue.values();

        for (Event event : tree.getEvents())
        {
            //If statement so that the mapColorHues aren't ever out of bounds
            if (mapColorHues.length >= eventTypes.size())
            {
                //Consistent mapColorHues for different event types
                for (int i = 0; i < eventTypes.size(); i++)
                {
                    float markerColor = mapColorHues[i].colorValue;
                    //If statement needed to keep consistent colors
                    if (eventTypes.get(i).toLowerCase().contains(event.getEventType().toLowerCase()))
                    {
                        //Adds the marker to the map and the list
                        makeMarker(event, markerColor);
                    }
                }
            }
            else
                Log.e(TAG, "Need more ColorHues in color enum");
        }
    }

    private void makeMarker(Event event, float markerColor)
    {
        MarkerOptions markerOptions = new MarkerOptions()
                .position(event.getEventLatLng())
                .icon(BitmapDescriptorFactory.defaultMarker(markerColor));
        Marker marker = map.addMarker(markerOptions);
        marker.setTag(event);
    }

    //Enum of possible marker colors for events
    private enum MapColorHue
    {
        AZURE(BitmapDescriptorFactory.HUE_AZURE),
        YELLOW(BitmapDescriptorFactory.HUE_YELLOW),
        VIOLET(BitmapDescriptorFactory.HUE_VIOLET),
        RED(BitmapDescriptorFactory.HUE_RED),
        ORANGE(BitmapDescriptorFactory.HUE_ORANGE),
        GREEN(BitmapDescriptorFactory.HUE_GREEN),
        MAGENTA(BitmapDescriptorFactory.HUE_MAGENTA),
        BLUE(BitmapDescriptorFactory.HUE_BLUE),
        ROSE(BitmapDescriptorFactory.HUE_ROSE),
        CYAN(BitmapDescriptorFactory.HUE_CYAN);

        private float colorValue;

        MapColorHue(float colorValue)
        {
            this.colorValue = colorValue;
        }
    }
}
