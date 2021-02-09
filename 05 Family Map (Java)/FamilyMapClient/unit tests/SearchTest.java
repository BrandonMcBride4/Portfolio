package edu.byu.cs.students.familymap;

import java.util.List;

import org.junit.*;
import static org.junit.Assert.*;

import edu.byu.cs.students.familymap.model.FamilyTree;
import edu.byu.cs.students.familymap.model.InvalidModelException;
import edu.byu.cs.students.familymap.person.EventFamilyModel;
import edu.byu.cs.students.familymap.search.Searcher;

/**
 * Created by Brandon on 8/13/2017.
 */

public class SearchTest
{
    private FamilyTree tree;
    private Searcher searcher;

    //Reset tree and searcher before every test
    @Before
    public void createModels() throws InvalidModelException
    {
        Models.createModels();
        tree = new FamilyTree(Models.persons, Models.events);
        searcher = new Searcher(tree);
    }

    //Test if it searches on one char
    //Also tests if persons come before events when they both are present in the search result
    @Test
    public void oneCharSearchTest()
    {
        String query = "l";
        List<EventFamilyModel> queryList = searcher.search(query);

        assertTrue(queryList.size() == 19);
        for (int i = 0; i < queryList.size(); i++)
        {
            EventFamilyModel model = queryList.get(i);

            //Tests that persons should be before events
            if (i <= 5)
                assertTrue(model.kin != null);
            else
                assertTrue(model.event != null);

            assertTrue(model.titleString.toLowerCase().contains(query) ||
                       model.descriptionString.toLowerCase().contains(query));
        }
    }

    //Tests first name
    @Test
    public void searchFirstNameTest()
    {
        String query = "lola";
        List<EventFamilyModel> queryList = searcher.search(query);

        assertTrue(queryList.size() == 1);
        for (EventFamilyModel model : queryList)
        {
            assertTrue(model.titleString.toLowerCase().contains(query));
            assertTrue(model.type.equals(model.kin.getGender()));
            assertTrue(model.kin.getFirstName().toLowerCase().contains(query));
            assertTrue(model.event == null);
        }
    }

    //Tests last name search
    @Test
    public void searchLastNameTest()
    {
        String query = "mcbride";
        List<EventFamilyModel> queryList = searcher.search(query);

        assertTrue(queryList.size() == 4);
        for (EventFamilyModel model : queryList)
        {
            assertTrue(model.titleString.toLowerCase().contains(query));
            assertTrue(model.type.equals(model.kin.getGender()));
            assertTrue(model.kin.getLastName().toLowerCase().contains(query));
            assertTrue(model.event == null);
        }
    }

    //Test event search, two different types come up in one search
    @Test
    public void searchEventTypeTest()
    {
        String query = "graduation";
        List<EventFamilyModel> queryList = searcher.search(query);

        assertTrue(queryList.size() == 5);
        for (EventFamilyModel model : queryList)
        {
            assertTrue(model.titleString.toLowerCase().contains(query));
            assertTrue(model.type.equals("event"));
            assertTrue(model.kin == null);
            assertTrue(model.event.getEventType().toLowerCase().contains(query));
        }
    }

    //Tests city search
    @Test
    public void searchCityTest()
    {
        String query = "venice";
        List<EventFamilyModel> queryList = searcher.search(query);

        assertTrue(queryList.size() == 11);
        for (EventFamilyModel model : queryList)
        {
            //No persons has a first or last name with venice
            assertTrue(model.titleString.toLowerCase().contains(query));
            assertTrue(model.type.equals("event"));
            assertTrue(model.kin == null);
            assertTrue(model.event.getCity().toLowerCase().equals(query));
        }
    }

    //Tests country search
    @Test
    public void searchCountryTest()
    {
        String query = "italy";
        List<EventFamilyModel> queryList = searcher.search(query);

        assertTrue(queryList.size() == 12);
        for (EventFamilyModel model : queryList)
        {
            assertTrue(model.titleString.toLowerCase().contains(query));
            assertTrue(model.type.equals("event"));
            assertTrue(model.kin == null);
            assertTrue(model.event.getCountry().toLowerCase().contains(query));
        }
    }

    //Tests year search
    @Test
    public void searchYearTest()
    {
        String query = "2012";
        List<EventFamilyModel> queryList = searcher.search(query);

        assertTrue(queryList.size() == 2);
        for (EventFamilyModel model : queryList)
        {
            assertTrue(model.titleString.contains(query));
            assertTrue(model.type.equals("event"));
            assertTrue(model.kin == null);
            assertTrue(String.valueOf(model.event.getYear()).equals(query));
        }
    }

    //This search shouldn't find anything
    @Test
    public void noResultSearch()
    {
        String query = "mcbridee";
        List<EventFamilyModel> queryList = searcher.search(query);

        assertTrue(queryList.isEmpty());
    }
}
