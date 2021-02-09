package server;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import dao.Database;
import model.InvalidModelException;
import result.FillResult;
import service.FillService;

import java.io.IOException;
import java.sql.SQLException;

/**
 * Created by Brandon on 7/12/2017.
 */
public class FillHandler implements HttpHandler
{
    @Override
    public void handle(HttpExchange e) throws IOException
    {
        Database db = new Database();
        try
        {
            String[] urlParam = Handler.getURLParam(e);
            int generations = 4;

            //fill/username/{generations}
            if (urlParam.length == 4)
            {
                try
                {
                    generations = Integer.parseInt(urlParam[3]);
                    if (generations < 0)
                        throw new Exception();
                }
                catch (Exception exc)
                {
                    Handler.sendJSONMessage("Invalid generations parameter.", e, db);
                    return;
                }
            }
            else if (urlParam.length != 3)
            {
                //error
                Handler.sendJSONMessage("Error: Invalid URL parameters.", e, db);
            }

            //fill/username
            String username = urlParam[2];
            if (!db.userDao.isUser(username))
            {
                Handler.sendJSONMessage("Invalid username.", e, db);
                return;
            }

            FillService service = new FillService(db, username, generations);
            FillResult fillResult = service.fill();
            Handler.sendResponse(fillResult,e, db);
        }
        catch (SQLException exc)
        {
            ExceptionHandler.handleSQLError("fill", exc, db, e);
        }
        catch (InvalidModelException exc)
        {
            ExceptionHandler.handleInvalidModel("fill", exc, db, e);
        }
        catch (MissingPostException exc)
        {
            ExceptionHandler.handleMissingPost("fill", exc, db, e);
        }
        catch (Exception exc)
        {
            ExceptionHandler.generalError("fill", exc, db, e);
        }
    }
}
