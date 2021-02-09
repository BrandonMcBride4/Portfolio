package server;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import dao.Database;
import model.InvalidModelException;
import model.Event;
import result.AllEventsResult;
import service.EventService;

import java.io.IOException;
import java.sql.SQLException;

public class EventHandler implements HttpHandler
{
    @Override
    public void handle(HttpExchange e) throws IOException
    {
        Database db = new Database();
        try
        {
            String[] urlParam = Handler.getURLParam(e);

            //sends response if invalid authorization
            String username = Handler.authTokenToUserName(db, e);

            //If null Authorization is not valid
            if (username == null)
                return;

            EventService eventService = new EventService(db);

            //event/
            if (urlParam.length == 2)
            {
                AllEventsResult allEventsResult = eventService.getAllEvents(username);
                if (allEventsResult == null)
                {
                    Handler.sendJSONMessage("Data is null", e, db);
                    return;
                }
                Handler.sendResponse(allEventsResult, e, db);
            }

            //event/[eventID]
            else if (urlParam.length == 3)
            {
                String eventIDReq = urlParam[2];

                //Checks for correct ID
                if (!db.eventDao.isEvent(eventIDReq))
                {
                    Handler.sendJSONMessage("Error: Invalid eventID parameter.", e, db);
                    return;
                }

                //Checks if event belongs to the correct user
                Event event = eventService.getEvent(eventIDReq, username);
                if (event == null)
                {
                    Handler.sendJSONMessage("Error: Requested event does not belong to user.", e, db);
                    return;
                }
                Handler.sendResponse(event,e, db);
            }
            else
            {
                //error
                Handler.sendJSONMessage("Error: Invalid URL parameters.", e, db);
            }
            
        }
        catch (SQLException exc)
        {
            ExceptionHandler.handleSQLError("event", exc, db, e);
        }
        catch (InvalidModelException exc)
        {
            ExceptionHandler.handleInvalidModel("event", exc, db, e);
        }
        catch (Exception exc)
        {
            ExceptionHandler.generalError("event", exc, db, e);
        }
    }
}
