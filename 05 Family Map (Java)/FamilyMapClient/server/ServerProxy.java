package edu.byu.cs.students.familymap.server;

import android.util.Log;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * Created by Brandon on 7/31/2017.
 */

public class ServerProxy
{
    private static final String TAG = "ServerProxy";

    private final int TIMEOUT_MS = 3000;

    private String serverHost;
    private String serverPort;

    public ServerProxy(String serverHost, String serverPort)
    {
        this.serverHost = serverHost;
        this.serverPort = serverPort;
    }

    public JsonObject get(String command, String authToken)
    {
        // This method shows how to send a GET request to a server

        try
        {
            // Create a URL indicating where the server is running, and which
            // web API operation we want to call
            URL url = new URL("http://" + serverHost + ":" + serverPort + command);

//            Log.i(TAG, url.toString());

            // Start constructing our HTTP request
            HttpURLConnection http = (HttpURLConnection) url.openConnection();
            http.setConnectTimeout(TIMEOUT_MS);

            // Indicate that this request will not contain an HTTP request body
            http.setDoOutput(false);

            // Add an auth token to the request in the HTTP "Authorization" header
            http.addRequestProperty("Authorization", authToken);

            // Specify that we would like to receive the server's response in JSON
            http.addRequestProperty("Accept", "application/json");

            // Connect to the server and send the HTTP request
            http.connect();

            if (http.getResponseCode() == HttpURLConnection.HTTP_OK)
            {
                // Get the input stream containing the HTTP response body
                InputStream respBody = http.getInputStream();
                return JsonConverter.getJsonObject(respBody);
            }
            else
            {
                // The HTTP response status code indicates an error
                // occurred, so print out the message from the HTTP response
                Log.i(TAG, "ERROR: " + http.getResponseMessage());
            }
        }
        catch (IOException e)
        {
            // An exception was thrown, so display the exception's stack trace
//            e.printStackTrace();
        }
        return null;
    }

    // The claimRoute method calls the server's "/routes/claim" operation to
    // claim the route between Atlanta and Miami
    public JsonObject post(String command, String authToken, Object object)
    {
        // This method shows how to send a POST request to a server

        try
        {
            // Create a URL indicating where the server is running, and which
            // web API operation we want to call
            URL url = new URL("http://" + serverHost + ":" + serverPort + command);

            // Start constructing our HTTP request
            HttpURLConnection http = (HttpURLConnection) url.openConnection();
            http.setConnectTimeout(TIMEOUT_MS);


            // Specify that we are sending an HTTP POST request
            http.setRequestMethod("POST");
            // Indicate that this request will contain an HTTP request body
            http.setDoOutput(true);    // There is a request body

            // Add an auth token to the request in the HTTP "Authorization" header
            if (authToken != null)
                http.addRequestProperty("Authorization", authToken);
            // Specify that we would like to receive the server's response in JSON
            http.addRequestProperty("Accept", "application/json");

            // Connect to the server and send the HTTP request
            http.connect();

            // This is the JSON string we will send in the HTTP request body
            Gson gson = new Gson();
            String reqData = gson.toJson(object);

//            Log.i(TAG, "input: " + reqData);

            // Get the output stream containing the HTTP request body
            OutputStream reqBody = http.getOutputStream();
            // Write the JSON data to the request body
            writeString(reqData, reqBody);
            // Close the request body output stream, indicating that the
            // request is complete
            reqBody.close();

            if (http.getResponseCode() == HttpURLConnection.HTTP_OK)
            {
//                Log.i(TAG, "Server Connected");
                return JsonConverter.getJsonObject(http.getInputStream());
            }
            else
            {
                // The HTTP response status code indicates an error
                // occurred, so print out the message from the HTTP response
                Log.i(TAG, "ERROR: " + http.getResponseMessage());
            }
        }
        catch (IOException e)
        {
            // An exception was thrown, so display the exception's stack trace
            e.printStackTrace();
        }
        return null;
    }

    /*
        The readString method shows how to read a String from an InputStream.
    */
    private String readString(InputStream is) throws IOException
    {
        StringBuilder sb = new StringBuilder();
        InputStreamReader sr = new InputStreamReader(is);
        char[] buf = new char[1024];
        int len;
        while ((len = sr.read(buf)) > 0)
        {
            sb.append(buf, 0, len);
        }
        return sb.toString();
    }

    /*
        The writeString method shows how to write a String to an OutputStream.
    */
    private void writeString(String str, OutputStream os) throws IOException
    {
        OutputStreamWriter sw = new OutputStreamWriter(os);
        sw.write(str);
        sw.flush();
    }
}
