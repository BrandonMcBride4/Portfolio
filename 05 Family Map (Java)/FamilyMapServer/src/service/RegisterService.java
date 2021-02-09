package service;

import dao.Database;
import dao.RandomData;
import model.AuthToken;
import model.InvalidModelException;
import model.User;
import request.RegisterRequest;
import result.*;
import server.MissingPostException;

import java.sql.SQLException;

/**
 * Created by Brandon on 7/12/2017.
 */
public class RegisterService
{
    Database db = null;

    /**
     * The constructor for the register service.
     */
    public RegisterService(Database db)
    {
        this.db = db;
    }

    /**
     * Creates a new user account, generates 4 generations of ancestor data for the new user,
     * logs the user in, and returns an auth token.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     Valid RegisterRequest object.
     * </pre>
     * @return
     * <pre>
     *     If no error, returns an auth token, else it returns an error message.
     * </pre>
     *
     * @param r the object that contains the info to create an account
     * @throws SQLException thrown if a SQL error is thrown when accessing the database
     * @throws InvalidModelException thrown when a model object is invalid
     */
    public RegisterResult register(RegisterRequest r) throws SQLException, InvalidModelException, MissingPostException
    {
        //Create user
        String personID = RandomData.makeUniquePersonID(db);
        User user = new User(r.getUserName(), r.getPassword(), r.getEmail(),
                             r.getFirstName(),r.getLastName(), r.getGender(), personID);
        db.userDao.createUser(user);

        //Create auth token for the new user
        String token = db.authTokenDao.createAuthToken(user);
        AuthToken authToken = new AuthToken(token, user.getUsername(), personID);

        //Fill 4 generations for new user
        new FillService(db, r.getUserName(), 4).fill();
        return new RegisterResult(authToken);
    }
}
