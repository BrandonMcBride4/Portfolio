package edu.byu.cs.students.familymap.person;

import android.graphics.drawable.Drawable;
import android.view.View;
import android.view.animation.RotateAnimation;
import android.widget.ImageButton;
import android.widget.TextView;

import com.bignerdranch.expandablerecyclerview.ViewHolder.ParentViewHolder;
import com.joanzapata.iconify.IconDrawable;
import com.joanzapata.iconify.fonts.FontAwesomeIcons;

import edu.byu.cs.students.familymap.R;

/**
 * Created by Brandon on 8/10/2017.
 */

public class PersonParentViewHolder extends ParentViewHolder
{
    TextView parentText;
    ImageButton parentArrow;

    public PersonParentViewHolder(View itemView)
    {
        super(itemView);

        //Connect widgets
        parentText = (TextView) itemView.findViewById(R.id.parent_view_holder_text);
        parentArrow = (ImageButton) itemView.findViewById(R.id.parent_view_holder_arrow);

        //Draw Icon
        Drawable arrowIcon = new IconDrawable(itemView.getContext(), FontAwesomeIcons.fa_arrow_circle_up)
                .colorRes(R.color.black).sizeDp(24);
        parentArrow.setImageDrawable(arrowIcon);
    }
}
