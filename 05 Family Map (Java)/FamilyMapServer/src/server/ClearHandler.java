package server;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import dao.Database;
import result.ClearResult;
import service.ClearService;

import java.io.IOException;
import java.sql.SQLException;

/**
 * Created by Brandon on 7/12/2017.
 */
class ClearHandler implements HttpHandler
{
    @Override
    public void handle(HttpExchange e) throws IOException
    {
        Database db = new Database();
        String result;
        System.out.println(e.getRequestURI().toString());
        try
        {
            ClearResult clearResult = new ClearService(db).clear();
            result = clearResult.getMessage();
            Handler.sendJSONMessage(result, e, db);
        }
        catch (SQLException exc)
        {
            ExceptionHandler.handleSQLError("clear", exc, db, e);
        }
        catch(Exception exc)
        {
            ExceptionHandler.generalError("clear", exc, db, e);
        }
    }
}
