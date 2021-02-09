package edu.byu.cs.students.familymap.main;

import android.support.v4.app.Fragment;

import com.joanzapata.iconify.Iconify;
import com.joanzapata.iconify.fonts.FontAwesomeModule;

import edu.byu.cs.students.familymap.SingleFragmentActivity;

public class MainActivity extends SingleFragmentActivity
{
    @Override
    protected Fragment createFragment()
    {
        Iconify.with(new FontAwesomeModule());
        if (LoginFragment.isLoggedIn())
        {
            return new MapFragment();
        }
        else
        {
            return new LoginFragment();
        }
    }
}
