package edu.byu.cs.students.familymap.server;

//Thrown when a Json object doesn't have a mandatory body part
public class MissingPostException extends Exception
{
    private String bodyPart;
    public MissingPostException(String bodyPart)
    {
        this.bodyPart = bodyPart;
    }

    public String getBodyPart()
    {
        return bodyPart;
    }

}
