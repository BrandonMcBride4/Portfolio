package edu.byu.cs.students.familymap.person;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.PersistableBundle;
import android.support.v4.app.Fragment;

import edu.byu.cs.students.familymap.SingleFragmentActivity;
import edu.byu.cs.students.familymap.model.Person;

/**
 * Created by Brandon on 8/8/2017.
 */

public class PersonActivity extends SingleFragmentActivity
{
    private static final String EXTRA_PERSON = "edu.byu.cs.students.familymap.kin";

    public static Intent newIntent(Context packageContext, Person person)
    {
        Intent intent = new Intent(packageContext, PersonActivity.class);
        intent.putExtra(EXTRA_PERSON, person);
        return intent;
    }

    @Override
    protected Fragment createFragment()
    {
        Person person = (Person) getIntent()
                        .getSerializableExtra(EXTRA_PERSON);
        return PersonFragment.newInstance(person);
    }
}
