package dao;

import model.User;
import server.Server;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by Brandon on 7/12/2017.
 */
public class AuthTokenDao
{
    Connection connection = null;

    public AuthTokenDao(Connection connection)
    {
        this.connection = connection;
    }



    /**
     *Creates a record for a auth token in the authToken table in the database.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None, the AuthToken object should be validated when it is created.
     * </pre>
     * @param u the AuthToken object with the information about the auth token
     * @return the token created
     */
    public String createAuthToken(User u) throws SQLException
    {
        String token;
        //The do loop ensures the token is unique
        do
        {
            token = RandomData.idMaker();
        }
        while (isToken(token));

        //Deletes the token after the amount of seconds in the command line has expired.
        TimerTask timerTask = new AuthTokenTimer(token);
        Timer timer = new Timer(true);
        timer.schedule(timerTask, Server.getAuthTimeoutMilli());

        PreparedStatement stmt = null;
        String insertToken = "insert into AuthTokens (authToken, username, personID) values (?, ?, ?)";
        try
        {
            stmt = connection.prepareStatement(insertToken);
            stmt.setString(1, token);
            stmt.setString(2, u.getUsername());
            stmt.setString(3, u.getPersonID());


            if(stmt.executeUpdate() != 1)
            {
                System.out.println("Error in createAuthToken(User u)");
                throw new SQLException();
            }
        }
        catch(SQLException exc)
        {
            System.out.println("Error in AuthTokenDao, createAuthToken(User u)");
            throw new SQLException();
        }
        finally
        {
            if(stmt != null)
                stmt.close();
        }

        return token;
    }

    /**
     * Retrives a user record from the auth token when the /person and /event task is called.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     The AuthToken must be a valid AuthToken.
     * </pre>
     * @param token the auth token of the user we want to get records for
     * @return a User object with all the information about the user
     */
    public String getUsername(String token) throws SQLException
    {
        //person
        String username = null;
        PreparedStatement stmt = null;
        ResultSet rs = null;
        try
        {
            String queryUsername = "select username from AuthTokens where AuthTokens.authToken = ?";
            stmt = connection.prepareStatement(queryUsername);
            stmt.setString(1, token);
            rs = stmt.executeQuery();

            while(rs.next())
            {
                username = rs.getString(1);
            }
        }
        catch(SQLException exc)
        {
            if (username != null)
                System.out.println("Error in AuthTokenDao, getUsername(String username)");
        }
        finally
        {
            if(stmt != null)
                stmt.close();
            if (rs != null)
                rs.close();
        }
        return username;
    }

    public boolean isToken(String token) throws SQLException
    {
        if (getUsername(token) != null)
            return true;
        else
            return false;
    }

    /**
     * Deletes an auth token record from the authToken table in the database.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None, the AuthToken object should be validated when it is created.
     * </pre>
     * @param token the AuthToken to be deleted
     */
    public void deleteAuthToken(String token) throws SQLException
    {
        //When timer expires
        PreparedStatement stmt = null;
        try
        {
            String deleteToken = "delete from AuthTokens where AuthTokens.authToken = ?";
            stmt = connection.prepareStatement(deleteToken);
            stmt.setString(1, token);
            stmt.executeUpdate();
        }
        catch(SQLException exc)
        {
            System.out.println("Error in AuthTokenDao, deleteAuthToken(String username)");
            exc.printStackTrace();
        }
        finally
        {
            if(stmt != null)
                stmt.close();
        }

    }

    /**
     *Deletes all auth token records in the database.
     */
    public void deleteAllAuthToken() throws SQLException
    {
        //clear/
        //load/
        String drop = "DROP TABLE IF EXISTS AuthTokens;";
        Database.executeStatement(drop,connection);
    }
}