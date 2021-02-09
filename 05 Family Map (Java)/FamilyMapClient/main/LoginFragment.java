package edu.byu.cs.students.familymap.main;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.IdRes;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Toast;

import com.google.gson.JsonObject;

import edu.byu.cs.students.familymap.R;
import edu.byu.cs.students.familymap.model.*;
import edu.byu.cs.students.familymap.request.LoginRequest;
import edu.byu.cs.students.familymap.request.RegisterRequest;
import edu.byu.cs.students.familymap.server.*;

/**
 * Created by bmcbrid4 on 7/31/2017.
 */

public class LoginFragment extends Fragment
{
    private static final String TAG = "LoginFragment";

    //Used as a type for LoginTextWatcher
    static final String DEFAULT_SERVER_HOST = "10.11.48.146";
    static final String DEFAULT_SERVER_PORT = "8080";
    static final String DEFAULT_USERNAME = "brandon4";
    static final String DEFAULT_PASSWORD = "password";
    static final String DEFAULT_FIRST_NAME = "firstName";
    static final String DEFAULT_LAST_NAME = "lastName";
    static final String DEFAULT_EMAIL = "email";

    private static boolean loggedIn;

    //Assigned by reference
    private EditText editTextServerHost;
    private EditText editTextServerPort;
    private EditText editTextUsername;
    private EditText editTextPassword;
    private EditText editTextFirstName;
    private EditText editTextLastName;
    private EditText editTextEmail;

    private RadioButton maleButton;

    private Button signInButton;
    private Button registerButton;

    static String serverHost;
    static String serverPort;
    String username;
    String password;
    String firstName;
    String lastName;
    String email;
    String gender;

    private static AuthToken authTokenModel;

    private static FamilyTree tree;

    @Override
    public View onCreateView(LayoutInflater inflater,
                             ViewGroup container,
                             Bundle savedInstanceState)
    {
        //When you hit the back button on the map,
        //You don't have to re-login
        if (loggedIn)
            startFragment(new MapFragment(), getActivity());

        View v = inflater.inflate(R.layout.fragment_login, container, false);

        wireAllEditTexts(v);

        //Gender button assigns gender
        maleButton = (RadioButton) v.findViewById(R.id.male_radio_button);
        RadioGroup genderRadioGroup = (RadioGroup) v.findViewById(R.id.gender_radio_group);
        genderRadioGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener()
        {
            @Override
            public void onCheckedChanged(RadioGroup group, @IdRes int checkedId)
            {
                if (checkedId == maleButton.getId())
                    gender = "m";
                else
                    gender = "f";

                enableSignInButtons();
            }
        });

        //Login on button touch
        signInButton = (Button) v.findViewById(R.id.sign_in_button);
        signInButton.setEnabled(false);
        signInButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                LoginRequest request = new LoginRequest(username, password);
                new RegisterTask(getActivity()).execute("/user/login", null, request);
            }
        });

        //Register new user on button touch
        registerButton = (Button) v.findViewById(R.id.register_button);
        registerButton.setEnabled(false);
        registerButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                RegisterRequest request = new RegisterRequest(username,
                                                      password,
                                                      email,
                                                      firstName,
                                                      lastName,
                                                      gender);
                new RegisterTask(getActivity()).execute("/user/register", null, request);
            }
        });

        enableSignInButtons();

        return v;
    }

    private void wireAllEditTexts(View v)
    {
        serverHost = wireEditText(v, editTextServerHost, R.id.login_server_host_edit_text,
                                  DEFAULT_SERVER_HOST, true);
        serverPort = wireEditText(v, editTextServerPort, R.id.login_server_port_edit_text,
                                  DEFAULT_SERVER_PORT, true);
        username = wireEditText(v, editTextUsername, R.id.login_username_edit_text,
                                DEFAULT_USERNAME, true);
        password = wireEditText(v, editTextPassword, R.id.login_password_edit_text,
                                DEFAULT_PASSWORD, true);
        wireEditText(v, editTextFirstName, R.id.login_first_name_edit_text,
                     DEFAULT_FIRST_NAME, false);
        wireEditText(v, editTextLastName, R.id.login_last_name_edit_text,
                     DEFAULT_LAST_NAME, false);
        wireEditText(v, editTextEmail, R.id.login_email_edit_text,
                     DEFAULT_EMAIL, false);
    }

    private String wireEditText(View v, EditText editText, int id, String type, boolean setDefault)
    {
        editText = (EditText) v.findViewById(id);
        if (setDefault)
        {
            editText.setText(type);
        }
        editText.addTextChangedListener(new LoginTextWatcher(this, type));
        return type;
    }

    public static void startFragment(Fragment fragment, FragmentActivity activity)
    {
        FragmentManager fragmentManager = activity.getSupportFragmentManager();
        fragmentManager.beginTransaction()
                .replace(R.id.fragment_container, fragment)
                .addToBackStack(null)
                .commit();
    }

    public static boolean showError(JsonObject jsonObject, Context context)
    {
        //If a jsonObject has a message object, it is an error
        String errorMessage = JsonConverter.getOptionalBodyPart(jsonObject, "message");
        if (errorMessage != null)
        {
            Toast.makeText(context, errorMessage, Toast.LENGTH_SHORT).show();
            return true;
        }
        else
            return false;
    }

    void enableSignInButtons()
    {
        enableSignIn();
        enableRegister();
    }

    private void enableSignIn()
    {
        //Checks if all necessary texts for sign in are completed
        if (serverHost != null &&
                serverPort != null &&
                username != null &&
                password != null)
        {
            signInButton.setEnabled(true);
        }
        else
        {
            signInButton.setEnabled(false);
        }
    }

    private void enableRegister()
    {
        //Checks if all necessary texts for register are completed
        if (serverHost != null &&
                serverPort != null &&
                username != null &&
                password != null &&
                firstName != null &&
                lastName != null &&
                email != null &&
                gender != null)
        {
            registerButton.setEnabled(true);
        }
        else
        {
            registerButton.setEnabled(false);
        }
    }

    public static String getServerHost()
    {
        return serverHost;
    }

    public static String getServerPort()
    {
        return serverPort;
    }

    public static AuthToken getAuthTokenModel()
    {
        return authTokenModel;
    }

    public static void setAuthToken(AuthToken a)
    {
        authTokenModel = a;
    }

    public static boolean isLoggedIn()
    {
        return loggedIn;
    }

    public static void setLoggedIn(boolean status)
    {
        loggedIn = status;
    }

    public static FamilyTree getTree()
    {
        return tree;
    }

    public static void setTree(FamilyTree familyTree)
    {
        tree = familyTree;
    }
}
