package edu.byu.cs.students.familymap.filter;

import android.content.Context;
import android.content.Intent;
import android.support.v4.app.Fragment;

import edu.byu.cs.students.familymap.SingleFragmentActivity;

public class FilterActivity extends SingleFragmentActivity
{
    public static Intent newIntent(Context packageContext)
    {
        Intent intent = new Intent(packageContext, FilterActivity.class);
        return intent;
    }

    @Override
    protected Fragment createFragment()
    {
        return FilterFragment.newInstance();
    }
}
