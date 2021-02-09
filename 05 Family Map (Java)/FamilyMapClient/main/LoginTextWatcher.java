package edu.byu.cs.students.familymap.main;

import android.text.Editable;
import android.text.TextWatcher;

/**
 * Created by Brandon on 8/13/2017.
 */

public class LoginTextWatcher implements TextWatcher
{
    //The switch case won't allow to call from LoginFragment
    final String DEFAULT_SERVER_HOST = LoginFragment.DEFAULT_SERVER_HOST;
    final String DEFAULT_SERVER_PORT = LoginFragment.DEFAULT_SERVER_PORT;
    final String DEFAULT_USERNAME = LoginFragment.DEFAULT_USERNAME;
    final String DEFAULT_PASSWORD = LoginFragment.DEFAULT_PASSWORD;
    final String DEFAULT_FIRST_NAME = LoginFragment.DEFAULT_FIRST_NAME;
    final String DEFAULT_LAST_NAME = LoginFragment.DEFAULT_LAST_NAME;
    final String DEFAULT_EMAIL = LoginFragment.DEFAULT_EMAIL;

    private LoginFragment loginFragment;
    private String type;

    public LoginTextWatcher(LoginFragment loginFragment, String type)
    {
        this.loginFragment = loginFragment;
        this.type = type;
    }

    @Override
    public void beforeTextChanged(CharSequence s, int start, int count, int after)
    {

    }

    @Override
    public void onTextChanged(CharSequence s, int start, int before, int count)
    {
        //The type tells which one should be edited
        switch (type)
        {
            case DEFAULT_SERVER_HOST:
                LoginFragment.serverPort = s.toString();
                break;
            case DEFAULT_SERVER_PORT:
                LoginFragment.serverPort = s.toString();
                break;
            case DEFAULT_USERNAME:
                loginFragment.username = s.toString();
                break;
            case DEFAULT_PASSWORD:
                loginFragment.password = s.toString();
                break;
            case DEFAULT_FIRST_NAME:
                loginFragment.firstName = s.toString();
                break;
            case DEFAULT_LAST_NAME:
                loginFragment.lastName = s.toString();
                break;
            case DEFAULT_EMAIL:
                loginFragment.email = s.toString();
                break;

        }
    }

    @Override
    public void afterTextChanged(Editable s)
    {
        loginFragment.enableSignInButtons();
    }
}
