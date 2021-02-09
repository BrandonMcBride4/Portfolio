package server;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.sun.net.httpserver.HttpExchange;
import dao.Database;

import java.io.*;
import java.net.URI;
import java.sql.SQLException;
import java.util.Scanner;

//A class of static functions that the Handlers use
public class Handler
{
    public static void sendResponse(Object object, HttpExchange e, Database db)
    {
        try
        {
            if (object != null)
            {
                e.sendResponseHeaders(200, 0);
                OutputStreamWriter os = new OutputStreamWriter(e.getResponseBody());
                Gson gson = new Gson();
                String jsonString = gson.toJson(object);
                os.write(jsonString);
                os.close();
            }
            else
            {
                e.sendResponseHeaders(200, -1);
            }
        }
        catch (IOException exception)
        {

        }
        db.closeConnection(true);
    }

    public static void sendJSONMessage(String message, HttpExchange e, Database db)
    {
        JsonObject object = new JsonObject();
        object.addProperty("message",message);
        sendResponse(object, e, db);
    }

    public static JsonObject getJsonObject(HttpExchange e) throws IOException
    {
        InputStream is = e.getRequestBody();
        String requestBody = Handler.inputStreamToString(is);
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

    public static String getBodyPart(JsonObject json, String bodyPart) throws MissingPostException
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

    public static void printAndSendError(HttpExchange e, String result, Database db)
    {
        System.out.println(result);
        sendJSONMessage(result, e, db);
    }

    public static String[] getURLParam(HttpExchange e)
    {
        URI requestURI = e.getRequestURI();
        String request = requestURI.toString();
        System.out.println(request);
        return request.split("/");
    }

    public static String authTokenToUserName(Database db, HttpExchange e) throws SQLException
    {
        String tokenReq = e.getRequestHeaders().getFirst("Authorization");
        if (!db.authTokenDao.isToken(tokenReq))
        {
            Handler.sendJSONMessage("Authorization invalid, need to login.", e, db);
            return null;
        }
        return db.authTokenDao.getUsername(tokenReq);
    }
}
