package result;

/**
 * Created by Brandon on 7/12/2017.
 */
public class FillResult
{
    private String message;

    /**
     * The constructor for the result of the fill service.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None
     * </pre>
     * @param message a success or error message
     */
    public FillResult(String message)
    {
        this.message = message;
    }

    public String getMessage()
    {
        return message;
    }
}
