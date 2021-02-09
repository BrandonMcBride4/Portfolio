package server;

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
