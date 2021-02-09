package edu.byu.cs.students.familymap.server;

import android.os.AsyncTask;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.widget.Toast;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

import edu.byu.cs.students.familymap.main.LoginFragment;
import edu.byu.cs.students.familymap.model.InvalidModelException;
import edu.byu.cs.students.familymap.model.Person;

/**
 * Created by Brandon on 8/11/2017.
 */

public class PersonsTask extends AsyncTask<Object, Void, JsonObject>
{
    private static final String TAG = "PersonsTask";

    private FragmentActivity activity;
    private static Person[] personArray;
    private String authToken;

    public PersonsTask(FragmentActivity activity)
    {
        this.activity = activity;
    }

    @Override
    protected JsonObject doInBackground(Object... params)
    {
        try
        {
            String command = TaskFunctions.getParamString(params[0]);
            authToken = TaskFunctions.getParamString(params[1]);

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
        postGetPersons(jsonObject);
    }

    private void postGetPersons(JsonObject jsonObject)
    {
        try
        {
            //show error message, if it didn't work (Shouldn't ever have one)
            if (!LoginFragment.showError(jsonObject, activity))
            {
                //Create Person[] from Json Object
                jsonToPersonArray(jsonObject);

                //Get events
                String command = "/event/";
                new EventsTask(activity).execute(command, authToken);
            }
        }
        catch (Exception e)
        {
            //Typically thrown when cannot connect to servery
            Toast.makeText(activity, "Server Error", Toast.LENGTH_SHORT).show();
            Log.e(TAG, "Error in postGetPersons(JsonObject)");
            Log.e(TAG, e.getMessage());
        }
    }

    public static Person[] jsonToPersonArray(JsonObject jsonObject)
            throws MissingPostException, InvalidModelException
    {
        //Create Person[] from Json Object
        JsonArray personJsonArray = jsonObject.getAsJsonArray("data");
        personArray = new Person[personJsonArray.size()];
        for (int i = 0; i < personJsonArray.size(); i++)
        {
            JsonObject personJsonObject = personJsonArray.get(i).getAsJsonObject();
            Person tempPerson = Person.jsonToPerson(personJsonObject);
            personArray[i] = tempPerson;
        }

        return personArray;
    }

    public static Person[] getPersonArray()
    {
        return personArray;
    }
}
