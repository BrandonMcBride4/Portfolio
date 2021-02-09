package dao;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import server.Handler;

import java.io.*;

public class JsonConverter
{
    public JsonConverter()
    {

    }

    //arrayName will typically be "data" except in the case of load
    //it will be users, persons, and events.
    public JsonArray toJsonArray(String contents, String arrayName)
    {
        try
        {
            Gson gson = new Gson();
            JsonObject jsonObject = gson.fromJson(contents, JsonObject.class);
//            if(jsonObject.has(arrayName))
                return jsonObject.getAsJsonArray(arrayName);
//            else
//                throw new MissingPostException(arrayName);
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
        return null;
    }

    //arrayName will typically be "data" except in the case of load
    //it will be users, persons, and events.
    public JsonArray jsonFiletoJsonArray(String fileName, String arrayName)
    {
        try
        {
            InputStream is = new FileInputStream(fileName);
            String fileContents = Handler.inputStreamToString(is);
            return toJsonArray(fileContents, arrayName);
        }
        catch(IOException e)
        {
            System.out.println("Error in jsonFilestoJsonArray");
        }
        return null;
    }
}
