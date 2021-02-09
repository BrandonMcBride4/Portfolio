package server;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import dao.Database;
import dao.JsonConverter;
import model.Event;
import model.InvalidModelException;
import model.Person;
import model.User;
import request.LoadRequest;
import result.LoadResult;
import service.ClearService;
import service.LoadService;

import java.io.IOException;
import java.sql.SQLException;

/**
 * Created by Brandon on 7/12/2017.
 */
class LoadHandler implements HttpHandler
{
    private HttpExchange e = null;
    @Override
    public void handle(HttpExchange e) throws IOException
    {
        Database db = new Database();
        try
        {
            ClearService clearService = new ClearService(db);
            clearService.clear();
            try
            {
                this.e = e;
                System.out.println(e.getRequestURI().toString());

                //Make the request DTO and call the service.
                LoadRequest loadRequest = makeLoadRequest();
                LoadResult loadResult = new LoadService(db).load(loadRequest);

                //Service is done, wrap things up
                Handler.sendJSONMessage(loadResult.getMessage(), e, db);
            }
            catch (SQLException exc)
            {
                ExceptionHandler.handleSQLError("load", exc, db, e);
            }
            catch (MissingPostException exc)
            {
                clearService.clear();
                ExceptionHandler.handleMissingPost("load", exc, db, e);
            }
            catch (InvalidModelException exc)
            {
                clearService.clear();
                ExceptionHandler.handleInvalidModel("load", exc, db, e);
            }
            catch (Exception exc)
            {
                clearService.clear();
                ExceptionHandler.generalError("load", exc, db, e);
            }
        }
        catch (SQLException exc)
        {
            ExceptionHandler.handleSQLError("load", exc, db, e);
        }

    }

    private LoadRequest makeLoadRequest()
            throws MissingPostException, IOException, InvalidModelException
    {
        JsonConverter converter = new JsonConverter();
        String requestString = Handler.inputStreamToString(e.getRequestBody());

        //Create User[] for LoadRequest
        JsonArray userJsonArray = converter.toJsonArray(requestString, "users");
        User[] userArray = new User[userJsonArray.size()];
        for (int i = 0; i < userJsonArray.size(); i++)
        {
            JsonObject userJsonObject = userJsonArray.get(i).getAsJsonObject();
            User tempUser = User.jsonToUser(userJsonObject);
            userArray[i] = tempUser;
        }

        //Create Person[] for LoadRequest
        JsonArray personJsonArray = converter.toJsonArray(requestString, "persons");
        Person[] personArray = new Person[personJsonArray.size()];
        for (int i = 0; i < personJsonArray.size(); i++)
        {
            JsonObject personJsonObject = personJsonArray.get(i).getAsJsonObject();
            Person tempPerson = Person.jsonToPerson(personJsonObject);
            personArray[i] = tempPerson;
        }

        //Create Event[] for LoadRequest
        JsonArray eventJsonArray = converter.toJsonArray(requestString, "events");
        Event[] eventArray = new Event[eventJsonArray.size()];
        for (int i = 0; i < eventJsonArray.size(); i++)
        {
            JsonObject eventJsonObject = eventJsonArray.get(i).getAsJsonObject();
            Event tempEvent = Event.jsonToEvent(eventJsonObject);
            eventArray[i] = tempEvent;
        }
        return new LoadRequest(userArray, personArray, eventArray);
    }
}
