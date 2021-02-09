package edu.byu.cs.students.familymap.search;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.SearchView;

import java.util.LinkedList;
import java.util.List;

import edu.byu.cs.students.familymap.R;
import edu.byu.cs.students.familymap.main.LoginFragment;
import edu.byu.cs.students.familymap.person.EventFamilyModel;
import edu.byu.cs.students.familymap.person.PersonChildViewHolder;

/**
 * Created by Brandon on 8/12/2017.
 */

public class SearchFragment extends Fragment
{
    private static final String TAG = "SearchFragment";

    private SearchView searchView;
    private RecyclerView recyclerView;
    private SearchAdapter searchAdapter;

    private List<EventFamilyModel> queryList = new LinkedList<>();


    public static SearchFragment newInstance()
    {
        Bundle args = new Bundle();

        SearchFragment fragment = new SearchFragment();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        View v = inflater.inflate(R.layout.fragment_search, container, false);
        searchView = (SearchView) v.findViewById(R.id.search_view);
        recyclerView = (RecyclerView) v.findViewById(R.id.search_recycler_view);

        //Gets the query string
        searchView.setOnQueryTextListener(new SearchView.OnQueryTextListener()
        {
            @Override
            public boolean onQueryTextSubmit(String query)
            {
                queryList = new Searcher(LoginFragment.getTree()).search(query);
                updateUI();
                Log.i(TAG, "Query: " + query);
                return true;
            }

            @Override
            public boolean onQueryTextChange(String newText)
            {
                return false;
            }
        });

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
            searchAdapter = new SearchAdapter(queryList);
            recyclerView.setAdapter(searchAdapter);
            recyclerView.setLayoutManager(new LinearLayoutManager(getActivity()));
    }

    private class SearchAdapter extends RecyclerView.Adapter<PersonChildViewHolder>
    {
        private List<EventFamilyModel> queryList;

        public SearchAdapter(List<EventFamilyModel> queryList)
        {
            this.queryList = queryList;
        }

        @Override
        public PersonChildViewHolder onCreateViewHolder(ViewGroup parent, int viewType)
        {
            LayoutInflater layoutInflater = LayoutInflater.from(getActivity());
            return new PersonChildViewHolder(layoutInflater, parent);
        }

        @Override
        public void onBindViewHolder(PersonChildViewHolder holder, int position)
        {
            EventFamilyModel row = queryList.get(position);
            //Binds the model object to the view holder
            holder.bind(row);
        }

        @Override
        public int getItemCount()
        {
            return queryList.size();
        }
    }
}
