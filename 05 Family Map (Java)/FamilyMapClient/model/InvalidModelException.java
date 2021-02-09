package edu.byu.cs.students.familymap.model;

public class InvalidModelException extends Exception
{
    private String error;
    private String model;

    public InvalidModelException(String error, String model)
    {
        this.error = error;
        this.model = model;
    }

    public String getError()
    {
        return error;
    }

    public String getModel()
    {
        return model;
    }
}
