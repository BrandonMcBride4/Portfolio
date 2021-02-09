package dao;

import model.InvalidModelException;
import model.User;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * Created by Brandon on 7/12/2017.
 */
public class UserDao
{
    Connection connection = null;

    public UserDao(Connection connection)
    {
        this.connection = connection;
    }

    /**
     *Creates a record for a user in the user table in the database.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None, the User object should be validated when it is created.
     * </pre>
     * @param u the User object with the information about the user
     */
    public void createUser(User u) throws SQLException
    {
        //user/register
        PreparedStatement stmt = null;
        String insertUser = "insert into Users (username, password, email, firstName, " +
                            "lastName, gender, personID) values (?, ?, ?, ?, ?, ?, ?)";
        try
        {
            stmt = connection.prepareStatement(insertUser);
            stmt.setString(1, u.getUsername());
            stmt.setString(2, u.getPassword());
            stmt.setString(3, u.getEmail());
            stmt.setString(4, u.getFirstName());
            stmt.setString(5, u.getLastName());
            stmt.setString(6, u.getGender());
            stmt.setString(7, u.getPersonID());


            if(stmt.executeUpdate() == 1)
            {
                // OK
            }
            else
            {
                System.out.println("Error in createUser()");
                throw new SQLException();
            }
        }
        catch(SQLException exc)
        {
            System.out.println("Error in UserDao, createUser(User u)");
            throw new SQLException();
        }
        finally
        {
            if(stmt != null)
                stmt.close();
        }
    }

    /**
     * Retrives a user record from the user name when logging in.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     The user name must be a valid user name.
     * </pre>
     * @param username the user name of the user we want to get records for
     * @return a User object with all the information about the user
     */
    public User getUser(String username) throws SQLException, InvalidModelException
    {
        //user/register
        //user/login
        User user = null;
        PreparedStatement stmt = null;
        ResultSet rs = null;
        try
        {
            String queryUser = "select * from Users where Users.username = ?";
            stmt = connection.prepareStatement(queryUser);
            stmt.setString(1, username);
            rs = stmt.executeQuery();

            while(rs.next())
            {
                String qUserName = rs.getString(1);
                String password = rs.getString(2);
                String email = rs.getString(3);
                String firstName = rs.getString(4);
                String lastName = rs.getString(5);
                String gender = rs.getString(6);
                String personId = rs.getString(7);
                user = new User(qUserName, password, email, firstName, lastName, gender, personId);
            }
        }
        catch(SQLException exc)
        {
            if (user != null)
                System.out.println("Error in UserDao, getUser(String username)");
        }
        finally
        {
            if(stmt != null)
                stmt.close();
            if (rs != null)
                rs.close();
        }
        return user;
    }

    public boolean isUser(String username) throws SQLException, InvalidModelException
    {
        if (getUser(username) != null)
            return true;
        else
            return false;
    }

    /**
     *Deletes all user records in the database.
     */
    public void deleteAllUsers() throws SQLException
    {
        //clear
        //load
        String drop = "DROP TABLE IF EXISTS Users;";
        Database.executeStatement(drop,connection);
    }
}
