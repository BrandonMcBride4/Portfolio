package edu.byu.cs.students.familymap.server;

import android.os.AsyncTask;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.widget.Toast;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

import edu.byu.cs.students.familymap.main.LoginFragment;
import edu.byu.cs.students.familymap.main.MapFragment;
import edu.byu.cs.students.familymap.model.Event;
import edu.byu.cs.students.familymap.model.FamilyTree;
import edu.byu.cs.students.familymap.model.InvalidModelException;

/**
 * Created by Brandon on 8/11/2017.
 */

public class EventsTask extends AsyncTask<Object, Void, JsonObject>
{
    private static final String TAG = "EventsTask";

    private FragmentActivity activity;
    private static Event[] eventArray;

    public EventsTask(FragmentActivity activity)
    {
        this.activity = activity;
    }

    @Override
    protected JsonObject doInBackground(Object... params)
    {
        try
        {
            String command = TaskFunctions.getParamString(params[0]);
            String authToken = TaskFunctions.getParamString(params[1]);

            return new ServerProxy(LoginFragment.getServerHost(), LoginFragment.getServerPort())
                    .get(command, authToken);
        }
        catch (Exception e)
        {
            //Typically thrown when cannot connect to servery
            Toast.makeText(activity, "Server Error", Toast.LENGTH_SHORT).show();
        }
        return null;
    }

    @Override
    protected void onPostExecute(JsonObject jsonObject)
    {
        postGetEvents(jsonObject);
    }

    //Either Logging in or Re-sy
    private void postGetEvents(JsonObject jsonObject)
    {
        try
        {
            //show error message, if it didn't work (Shouldn't ever have one)
            if (!LoginFragment.showError(jsonObject, activity))
            {
                //Create Event[] for FamilyTree
                jsonToEventArray(jsonObject);

                //Make tree
                FamilyTree tree = new FamilyTree(PersonsTask.getPersonArray(), eventArray);
                LoginFragment.setTree(tree);

                //Start Map Fragment
                MapFragment mapFragment = new MapFragment();
                LoginFragment.startFragment(mapFragment, activity);
            }
        }
        catch (Exception e)
        {
            //Typically thrown when cannot connect to servery
            Toast.makeText(activity, "Server Error", Toast.LENGTH_SHORT).show();
            Log.e(TAG, "Error in postGetEvents(JsonObject)");
            Log.e(TAG, e.getMessage());
        }
    }

    public static Event[] jsonToEventArray(JsonObject jsonObject)
            throws MissingPostException, InvalidModelException
    {
        //Create Event[] from a Json Object
        JsonArray eventJsonArray = jsonObject.getAsJsonArray("data");
        eventArray = new Event[eventJsonArray.size()];
        for (int i = 0; i < eventJsonArray.size(); i++)
        {
            JsonObject eventJsonObject = eventJsonArray.get(i).getAsJsonObject();
            Event tempEvent = Event.jsonToEvent(eventJsonObject);
            eventArray[i] = tempEvent;
        }

        return eventArray;
    }

    public static Event[] getEventArray()
    {
        return eventArray;
    }
}
