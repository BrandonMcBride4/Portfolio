package edu.byu.cs.students.familymap.main;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;

import edu.byu.cs.students.familymap.SingleFragmentActivity;
import edu.byu.cs.students.familymap.model.Event;

public class MapActivity extends SingleFragmentActivity
{
    private static final String EXTRA_EVENT = "edu.byu.cs.students.familymap.main.event";

    public static Intent newIntent(Context packageContext, Event event)
    {
        Intent intent = new Intent(packageContext, MapActivity.class);
        intent.putExtra(EXTRA_EVENT, event);
        return intent;
    }

    @Override
    protected Fragment createFragment()
    {
        Event event = (Event) getIntent().getSerializableExtra(EXTRA_EVENT);
        return MapFragment.newInstance(event);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
    }
}
