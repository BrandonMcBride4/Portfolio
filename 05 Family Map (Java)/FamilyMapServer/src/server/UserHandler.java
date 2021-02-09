package server;

import com.google.gson.JsonObject;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import dao.Database;
import model.InvalidModelException;
import model.User;
import request.LoginRequest;
import request.RegisterRequest;
import result.LoginResult;
import result.RegisterResult;
import service.LoginService;
import service.RegisterService;

import java.io.IOException;
import java.sql.SQLException;

public class UserHandler implements HttpHandler
{
    private HttpExchange e = null;

    @Override
    public void handle(HttpExchange e) throws IOException
    {
        this.e = e;
        String[] urlParam = Handler.getURLParam(e);
        String result;
        Database db = new Database();

        //Should either be /user/register or /user/login
        if (urlParam.length == 3)
        {
            if (urlParam[2].equals("login"))
            {
                handleLogin(db);
            }
            else if (urlParam[2].equals("register"))
            {
                handleRegister(db);
            }
            else
            {
                result = "ERROR: URL has invalid name.";
                Handler.printAndSendError(e, result, db);
            }
        }
        else
        {
            result = "ERROR: URL has wrong number of parameters.";
            Handler.printAndSendError(e, result, db);
        }

    }

    private void handleLogin(Database db)
    {
        try
        {
            JsonObject json = Handler.getJsonObject(e);
            if (json != null)
            {
                try
                {
                    String userName = Handler.getBodyPart(json, "userName");
                    String password = Handler.getBodyPart(json, "password");

                    //Check that username has been registered
                    if (!db.userDao.isUser(userName))
                    {
                        invalidLogin(db);
                        return;
                    }

                    User user = db.userDao.getUser(userName);

                    //Check for valid password
                    if (!password.equals(user.getPassword()))
                    {
                        invalidLogin(db);
                        return;
                    }

                    //Login
                    LoginRequest loginReq = new LoginRequest(userName, password);
                    LoginResult loginResult = new LoginService(db).login(loginReq);
                    Handler.sendResponse(loginResult, e, db);

                }
                catch (MissingPostException exc)
                {
                    ExceptionHandler.handleMissingPost("user", exc, db, e);
                }
                catch (SQLException exc)
                {
                    ExceptionHandler.handleSQLError("user", exc, db, e);
                }
            }
        }
        catch (Exception exc)
        {
            ExceptionHandler.generalError("user", exc, db, e);
        }
    }

    private void handleRegister(Database db)
    {
        try
        {
            JsonObject json = Handler.getJsonObject(e);
            if (json != null)
            {
                try
                {
                    //Check if username already exists
                    String userName = Handler.getBodyPart(json, "userName");
                    if (db.userDao.isUser(userName))
                    {
                        
                        Handler.sendJSONMessage("Username already exists.", e, db);
                        return;
                    }
                    String password = Handler.getBodyPart(json, "password");
                    String email = Handler.getBodyPart(json, "email");
                    String firstName = Handler.getBodyPart(json, "firstName");
                    String lastName = Handler.getBodyPart(json, "lastName");
                    String gender = Handler.getBodyPart(json, "gender");

                    //Make DTO Request object and register
                    RegisterRequest regRequest = new RegisterRequest(
                                                                     userName,
                                                                     password,
                                                                     email,
                                                                     firstName,
                                                                     lastName,
                                                                     gender);
                    RegisterResult regResult = new RegisterService(db).register(regRequest);
                    Handler.sendResponse(regResult, e , db);
                }
                catch (MissingPostException exc)
                {
                    ExceptionHandler.handleMissingPost("user", exc, db, e);
                }
                catch (InvalidModelException exc)
                {
                    ExceptionHandler.handleInvalidModel("user", exc, db, e);
                }
                catch (SQLException exc)
                {
                    ExceptionHandler.handleSQLError("user", exc, db, e);
                }
            }
        }
        catch (Exception exc)
        {
            ExceptionHandler.generalError("user", exc, db, e);
        }
    }

    private void invalidLogin(Database db)
    {
        Handler.sendJSONMessage("Wrong username or password.", e, db);
    }
}

