package edu.byu.cs.students.familymap.search;

import android.content.Context;
import android.content.Intent;
import android.support.v4.app.Fragment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import edu.byu.cs.students.familymap.R;
import edu.byu.cs.students.familymap.SingleFragmentActivity;

public class SearchActivity extends SingleFragmentActivity
{

    public static Intent newIntent(Context packageContext)
    {
        Intent intent = new Intent(packageContext, SearchActivity.class);
        return intent;
    }

    @Override
    protected Fragment createFragment()
    {
        return SearchFragment.newInstance();
    }
}
