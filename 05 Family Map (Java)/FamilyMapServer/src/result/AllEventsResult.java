package result;

import model.Event;

import java.util.List;

/**
 * Created by bmcbrid4 on 7/14/2017.
 */
public class AllEventsResult
{
    private List<Event> data;

    /**
     * The success constructor for the result of the /event URL path.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None
     * </pre>
     * @param data array of Event objects
     */
    public AllEventsResult(List<Event> data)
    {
        this.data = data;
    }

    public List<Event> getData()
    {
        return data;
    }

    public void setData(List<Event> data)
    {
        this.data = data;
    }
}
