package edu.byu.cs.students.familymap.server;

import android.os.AsyncTask;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.widget.Toast;

import com.google.gson.JsonObject;

import edu.byu.cs.students.familymap.main.LoginFragment;
import edu.byu.cs.students.familymap.model.AuthToken;
import edu.byu.cs.students.familymap.request.LoginRequest;
import edu.byu.cs.students.familymap.request.RegisterRequest;

/**
 * Created by Brandon on 8/11/2017.
 */

public class RegisterTask extends AsyncTask<Object, Void, JsonObject>
{
    private static final String TAG = "RegisterTask";
    private FragmentActivity activity;
    private String username;


    public RegisterTask(FragmentActivity activity)
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

            //Command is either a login request or a register request
            if (command.contains("login"))
            {
                username = ((LoginRequest) params[2]).getUserName();
            }
            else
            {
                username = ((RegisterRequest) params[2]).getUserName();
            }

            return new ServerProxy(LoginFragment.getServerHost(), LoginFragment.getServerPort())
                    .post(command, authToken, params[2]);
        }
        catch(Exception e)
        {
            //Typically thrown when cannot connect to server
            Toast.makeText(activity, "Server Error", Toast.LENGTH_SHORT).show();
            Log.e(TAG, "Error in postGetPersons(JsonObject)");
            Log.e(TAG, e.getMessage());
        }
        return null;
    }

    @Override
    protected void onPostExecute(JsonObject jsonObject)
    {
        postLogin(jsonObject);
    }

    private void postLogin(JsonObject jsonObject)
    {
        try
        {
            //Show why login/register didn't work, if it didn't
            if (!LoginFragment.showError(jsonObject, activity))
            {
                LoginFragment.setLoggedIn(true);
                String authToken = JsonConverter.getOptionalBodyPart(jsonObject, "authToken");
                String personID = JsonConverter.getOptionalBodyPart(jsonObject, "personId");
                LoginFragment.setAuthToken(new AuthToken(authToken, username, personID));

                //Get Persons
                String command = "/person/";
                new PersonsTask(activity).execute(command, authToken);
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
}
