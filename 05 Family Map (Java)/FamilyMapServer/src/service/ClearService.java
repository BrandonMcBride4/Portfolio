package service;

import dao.*;
import result.ClearResult;

import java.sql.SQLException;

/**
 * Created by Brandon on 7/12/2017.
 */
public class ClearService
{
    private Database db;

    /**
     * The constructor for the service to clear the database.
     */
    public ClearService(Database db)
    {
        this.db = db;
    }

    /**
     * Deletes all data from the database.
     * @return
     * <pre>
     *     ClearResult object that contains either a success or error message string.
     * </pre>
     */
    public ClearResult clear() throws SQLException
    {
        db.userDao.deleteAllUsers();
        db.personDao.deleteAllPersons();
        db.eventDao.deleteAllEvents();
        db.authTokenDao.deleteAllAuthToken();
        db.createTablesIfNeeded();
        System.out.println("Database Cleared");
        return new ClearResult("Clear succeeded.");
    }
}
