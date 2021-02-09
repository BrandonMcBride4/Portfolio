package server;

import com.sun.net.httpserver.HttpExchange;
import dao.Database;
import model.InvalidModelException;

import java.sql.SQLException;

public class ExceptionHandler
{
    public static void handleSQLError(String handlerName, SQLException exc,
                                      Database db, HttpExchange e)
    {
        handlerName = handlerName.toLowerCase();
        db.closeConnection(false);
        Handler.printAndSendError(e, String.format("SQL error in %s handler.", handlerName), db);
        exc.printStackTrace();
    }

    public static void handleMissingPost(String handlerName, MissingPostException exc,
                                         Database db, HttpExchange e)
    {
        handlerName = handlerName.toLowerCase();
        if (handlerName.equals("fill"))
        {
            String error = String.format("Location JSON file has an invalid typo for %s in fill handler.",
                                         exc.getBodyPart());
            Handler.printAndSendError(e, error, db);
        }
        else
        {
            String error = String.format("Missing %s.",
                                         exc.getBodyPart());
            Handler.printAndSendError(e, error, db);
        }
    }

    public static void handleInvalidModel(String handlerName,
                                          InvalidModelException exc,
                                          Database db,
                                          HttpExchange e)
    {
        String error = String.format("%s model is invalid, %s.", exc.getModel(), exc.getError());
        Handler.printAndSendError(e, error, db);
    }

    public static void generalError(String handlerName, Exception exc,
                                    Database db, HttpExchange e)
    {
        handlerName = handlerName.toLowerCase();
        Handler.printAndSendError(e,"Error in " + handlerName + " handler.", db);
        System.out.println(exc.getMessage());
        exc.printStackTrace();
    }
}
