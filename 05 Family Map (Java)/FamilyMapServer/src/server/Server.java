package server;

import com.sun.net.httpserver.HttpServer;

import java.io.*;
import java.net.InetSocketAddress;

/**
 * Created by Brandon on 7/12/2017.
 */
public class Server
{
    private static long authTokenTimeoutMilli = 3600 * 1000;
    public Server(long authTimeout)
    {
        authTokenTimeoutMilli = authTimeout;
    }
    private static final int MAX_WAITING_CONNECTIONS = 12;
    private HttpServer server;

    private void run(String portNumber)
    {
        System.out.println("Initializing HTTP Server");
        try
        {
            server = HttpServer.create(
                    new InetSocketAddress(Integer.parseInt(portNumber)),
                    MAX_WAITING_CONNECTIONS);
        }
        catch (IOException e)
        {
            e.printStackTrace();
            return;
        }

        // Indicate that we are using the default "executor".
        // This line is necessary, but its function is unimportant for our purposes.
        server.setExecutor(null);

        System.out.println("Creating contexts");

        server.createContext("/clear", new ClearHandler());
        server.createContext("/load", new LoadHandler());
        server.createContext("/fill", new FillHandler());
        server.createContext("/user", new UserHandler());
        server.createContext("/event", new EventHandler());
        server.createContext("/person", new PersonHandler());
        server.createContext("/", new DefaultHandler());

        System.out.println("Starting server");

        server.start();
        System.out.println("Server started");
    }

    public static long getAuthTimeoutMilli()
    {
        return authTokenTimeoutMilli;
    }

    public static void main(String[] args)
    {
        if (args.length == 2)
        {
            String portNumber = args[0];
            long authTokenTimeoutMilli = Integer.parseInt(args[1]) * 1000;
            new Server(authTokenTimeoutMilli).run(portNumber);
        }
        else
        {
            System.err.println("Error: Not the right command line parameters.");
        }
    }
}
