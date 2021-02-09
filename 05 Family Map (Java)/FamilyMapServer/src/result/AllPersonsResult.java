package result;

import model.Person;

import java.util.List;

/**
 * Created by bmcbrid4 on 7/14/2017.
 */
public class AllPersonsResult
{
    private List<Person> data;

    /**
     * The success constructor for the result of the /person URL path.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None
     * </pre>
     *
     * @param data array of Person objects
     */
    public AllPersonsResult(List<Person> data)
    {
        this.data = data;
    }

    public List<Person> getData()
    {
        return data;
    }
}
