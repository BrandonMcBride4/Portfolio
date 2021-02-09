package edu.byu.cs.students.familymap.person;

import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;


import com.bignerdranch.expandablerecyclerview.ViewHolder.ChildViewHolder;
import com.joanzapata.iconify.IconDrawable;
import com.joanzapata.iconify.fonts.FontAwesomeIcons;

import edu.byu.cs.students.familymap.R;
import edu.byu.cs.students.familymap.filter.Filter;
import edu.byu.cs.students.familymap.main.LoginFragment;
import edu.byu.cs.students.familymap.main.MapActivity;
import edu.byu.cs.students.familymap.model.Event;
import edu.byu.cs.students.familymap.model.Person;

/**
 * Created by Brandon on 8/10/2017.
 */

public class PersonChildViewHolder extends ChildViewHolder
{
    ImageView icon;
    private TextView titleText;
    private TextView descriptionText;

    String type;
    Person person;
    Event event;

    public PersonChildViewHolder(View itemView)
    {
        super(itemView);
        initWidgets();
    }

    public PersonChildViewHolder(LayoutInflater inflater, ViewGroup parent)
    {
        super(inflater.inflate(R.layout.holder_child_view, parent, false));
        initWidgets();
    }

    private void initWidgets()
    {
        final Context CONTEXT = itemView.getContext();
        //Connect widgets
        titleText = (TextView) itemView.findViewById(R.id.child_title_text);
        descriptionText = (TextView) itemView.findViewById(R.id.child_description_text);
        icon = (ImageView) itemView.findViewById(R.id.child_icon);

        //On click either starts a new person activity or map activity
        LinearLayout row = (LinearLayout) itemView.findViewById(R.id.child_linear_layout);
        row.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                //When event is null it is a person
                if (event == null)
                {
                    //Start new person activity with he person selected
                    Intent intent = PersonActivity.newIntent(CONTEXT, person);
                    CONTEXT.startActivity(intent);
                }
                else
                {
                    //The search activity pulls up events that arent' on the map
                    if (Filter.getEvents().contains(event))
                    {
                        //Start map activity with the event centered
                        Intent intent = MapActivity.newIntent(CONTEXT, event);
                        CONTEXT.startActivity(intent);
                    }
                    else
                    {
                        Toast.makeText(
                                CONTEXT,
                                "Event is currently being filtered.",
                                Toast.LENGTH_SHORT)
                                .show();
                    }
                }
            }
        });
    }

    //Called from the ChildHolder, binds the data of the selected row
    public void bind(EventFamilyModel data)
    {
        type = data.type;

        titleText.setText(data.titleString);
        descriptionText.setText(data.descriptionString);

        //The type determines the icon and if an event should be assigned or person
        switch (type)
        {
            case "event":
                setIcon(icon, FontAwesomeIcons.fa_map_marker, R.color.red);
                event = data.event;
                break;
            case "m":
                setIcon(icon, FontAwesomeIcons.fa_male, R.color.male_icon);
                person = data.kin;
                break;
            case "f":
                setIcon(icon, FontAwesomeIcons.fa_female, R.color.female_icon);
                person = data.kin;
                break;
        }
    }

    //Sets the icon of the item
    private void setIcon(ImageView item, FontAwesomeIcons icon, int color)
    {
        Drawable drawable = new IconDrawable(item.getContext(), icon)
                .colorRes(color);
        item.setImageDrawable(drawable);
    }
}
