package service;

import dao.Database;
import model.AuthToken;
import model.InvalidModelException;
import model.User;
import request.LoginRequest;
import result.LoginResult;

import java.sql.SQLException;

/**
 * Created by Brandon on 7/12/2017.
 */
public class LoginService
{
    private Database db = null;
    /**
     * The constructor for the LoginService.
     */
    public LoginService(Database db)
    {
        this.db = db;
    }

    /**
     * Logs in the user and returns an auth token.
     *<pre>
     *     <b>Constraints on the input</b>:
     *     LoginRequest should contain valid username and password.
     * </pre>
     * @return
     * <pre>
     *     LoadResult object that contains a new auth token, username, and personID.
     * </pre>
     * @param r contains the username and password
     * @throws SQLException thrown if a SQL error is thrown when accessing the database
     * @throws InvalidModelException thrown when a model object is invalid
     */
    public LoginResult login(LoginRequest r) throws SQLException, InvalidModelException
    {
        User user = db.userDao.getUser(r.getUserName());
        String token = db.authTokenDao.createAuthToken(user);
        AuthToken authToken = new AuthToken(token, user.getUsername(), user.getPersonID());
        return new LoginResult(authToken);
    }
}
