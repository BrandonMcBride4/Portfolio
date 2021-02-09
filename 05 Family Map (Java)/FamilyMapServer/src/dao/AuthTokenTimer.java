package dao;

import server.ExceptionHandler;

import java.sql.SQLException;
import java.util.TimerTask;

public class AuthTokenTimer extends TimerTask
{
    private String token;

    public AuthTokenTimer(String token)
    {
        this.token = token;
    }

    @Override
    public void run()
    {
        Database db = new Database();
        try
        {
            db.authTokenDao.deleteAuthToken(token);
            db.closeConnection(true);
        }
        catch (SQLException exc)
        {
            db.closeConnection(false);
            System.out.println("SQL Error when timer expires.");
        }
    }
}
