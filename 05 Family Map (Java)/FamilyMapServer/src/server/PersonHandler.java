package server;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import dao.Database;
import model.InvalidModelException;
import model.Person;
import result.AllPersonsResult;
import service.PersonService;

import java.io.IOException;
import java.sql.SQLException;

public class PersonHandler implements HttpHandler
{
    private HttpExchange e = null;
    @Override
    public void handle(HttpExchange e) throws IOException
    {
        Database db = new Database();
        try
        {
            String[] urlParam = Handler.getURLParam(e);

            //Sends response if invalid authorization
            String username = Handler.authTokenToUserName(db, e);

            //If null Authorization is not valid
            if (username == null)
                return;

            PersonService personService = new PersonService(db);

            //person/
            if (urlParam.length == 2)
            {
                AllPersonsResult allPersonsResult = personService.getAllPersons(username);
                if (allPersonsResult == null)
                {
                    
                    Handler.sendJSONMessage("Internal server error.", e, db);
                    return;
                }
                Handler.sendResponse(allPersonsResult,e, db);
            }

            //person/[personID]
            else if (urlParam.length == 3)
            {
                String personIDReq = urlParam[2];

                //Checks for correct ID
                if (!db.personDao.isPerson(personIDReq))
                {
                    Handler.sendJSONMessage("Error: Invalid personID parameter.", e, db);
                    return;
                }

                //Checks if person belongs to the correct user
                Person person = personService.getPerson(personIDReq, username);
                if (person == null)
                {
                    Handler.sendJSONMessage("Error: Requested person does not belong to user.", e, db);
                    return;
                }
                Handler.sendResponse(person, e, db);
            }
            else
            {
                //error
                Handler.sendJSONMessage("Error: Invalid URL parameters.", e, db);
            }
            
        }
        catch (SQLException exc)
        {
            ExceptionHandler.handleSQLError("person", exc, db, e);
        }
        catch (InvalidModelException exc)
        {
            ExceptionHandler.handleInvalidModel("person", exc, db, e);
        }
        catch (Exception exc)
        {
            ExceptionHandler.generalError("person", exc, db, e);
        }
    }
}
