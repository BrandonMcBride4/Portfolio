package edu.byu.cs.students.familymap.server;

/**
 * Created by bmcbrid4 on 8/14/2017.
 */

//Common functions in the Async tasks
public class TaskFunctions
{
    public static String getParamString(Object param)
    {
        if (param != null)
        {
            return (String) param;
        }
        return null;
    }
}
