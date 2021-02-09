package edu.byu.cs.students.familymap.server;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import java.io.*;
import java.util.Scanner;

public class JsonConverter
{
    //Gets a jsonObject from an InputStream, typically a response body
    public static JsonObject getJsonObject(InputStream is) throws IOException
    {
        String requestBody = inputStreamToString(is);
        Gson gson = new Gson();
        return gson.fromJson(requestBody, JsonObject.class);
    }

    public static String inputStreamToString(InputStream is) throws IOException
    {
        StringBuilder sb = new StringBuilder();
        Scanner scanner = new Scanner(new BufferedReader(new InputStreamReader(is)));
        while(scanner.hasNextLine())
        {
            sb.append(scanner.nextLine());
        }
        scanner.close();
        return sb.toString();
    }

    //Gets a body member of a json object
    //If that member isn't there, exception is thrown (hence mandatory)
    public static String getMandatoryBodyPart(JsonObject json, String bodyPart) throws MissingPostException
    {
        if (json.has(bodyPart))
        {
            return json.get(bodyPart).getAsString();
        }
        else
        {
            throw new MissingPostException(bodyPart);
        }
    }

    //Gets a body member of a json object
    //If it's not there return null
    public static String getOptionalBodyPart(JsonObject json, String bodyPart)
    {
        if (json.has(bodyPart))
        {
            return json.get(bodyPart).getAsString();
        }
        else
        {
            return null;
        }
    }
}
