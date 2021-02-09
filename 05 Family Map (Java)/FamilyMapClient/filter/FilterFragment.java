package edu.byu.cs.students.familymap.filter;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;

import edu.byu.cs.students.familymap.R;
import edu.byu.cs.students.familymap.main.LoginFragment;

/**
 * Created by Brandon on 8/9/2017.
 */

public class FilterFragment extends Fragment
{
    private final String TAG = "FilterFragment";

    private RecyclerView filterRecyclerView;
    private FilterAdapter filterAdapter;

    public static FilterFragment newInstance()
    {
        Bundle args = new Bundle();

        FilterFragment fragment = new FilterFragment();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        View v = inflater.inflate(R.layout.fragment_filter, container, false);

        //Connect RecyclerView to layout
        filterRecyclerView = (RecyclerView) v.findViewById(R.id.filter_recycler_view);
        filterRecyclerView.setLayoutManager(new LinearLayoutManager(getActivity()));

        updateUI();

        return v;
    }

    @Override
    public void onResume()
    {
        super.onResume();
        updateUI();
    }

    private void updateUI()
    {
        //Init values
        FilterRows filterRows = FilterRows.get(LoginFragment.getTree());
        List<FilterRow> rows = filterRows.getRows();

        //Connect adapter
        if (filterAdapter == null)
        {
            filterAdapter = new FilterAdapter(rows);
            filterRecyclerView.setAdapter(filterAdapter);
        }
        else
        {
            filterAdapter.notifyDataSetChanged();
        }
    }

    private class FilterAdapter extends RecyclerView.Adapter<FilterHolder>
    {
        private List<FilterRow> rows;

        public FilterAdapter(List<FilterRow> rows)
        {
            this.rows = rows;
        }

        @Override
        public FilterHolder onCreateViewHolder(ViewGroup parent, int viewType)
        {
            LayoutInflater layoutInflater = LayoutInflater.from(getActivity());
            return new FilterHolder(layoutInflater, parent);
        }

        @Override
        public void onBindViewHolder(FilterHolder holder, int position)
        {
            FilterRow row = rows.get(position);
            //Connect each holder to adapter
            holder.bind(row);
        }

        @Override
        public int getItemCount()
        {
            return rows.size();
        }
    }

    private class FilterHolder extends RecyclerView.ViewHolder
    {
        private FilterRow row;

        private TextView titleTextView;
        private TextView descriptionTextView;
        private Switch filterSwitch;

        public FilterHolder (LayoutInflater inflater, ViewGroup parent)
        {
            super(inflater.inflate(R.layout.list_filter_item, parent, false));

            //Connect widgets
            titleTextView = (TextView) itemView.findViewById(R.id.filter_title);
            descriptionTextView = (TextView) itemView.findViewById(R.id.filter_description);
            filterSwitch = (Switch) itemView.findViewById(R.id.filter_switch);

            filterSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener()
            {
                @Override
                public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
                {
                    row.setShowingEvents(isChecked);
                }
            });
        }

        //Bind model to view
        public void bind(FilterRow row)
        {
            this.row = row;
            titleTextView.setText(row.getTitle());
            descriptionTextView.setText(row.getDescription());
            filterSwitch.setChecked(row.isShowingEvents());
        }
    }
}
