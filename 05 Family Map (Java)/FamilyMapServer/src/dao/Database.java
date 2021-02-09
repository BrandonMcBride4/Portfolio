package dao;


import java.io.File;
import java.sql.*;

public class Database
{
    public Connection connection = null;

    public UserDao userDao = new UserDao(connection);
    public AuthTokenDao authTokenDao = new AuthTokenDao(connection);
    public PersonDao personDao = new PersonDao(connection);
    public EventDao eventDao = new EventDao(connection);

    public Database()
    {
        loadDriver();
        openAndStartDb();
    }

    public void loadDriver()
    {
        try
        {
            final String driver = "org.sqlite.JDBC";
            Class.forName(driver);
        }
        catch(ClassNotFoundException e)
        {
            System.out.println("ERROR! Could not load database driver");
        }
    }

    public static void executeStatement(String stmt, Connection connection)
                                        throws SQLException
    {
        PreparedStatement prepStmt = connection.prepareStatement(stmt);
        prepStmt.executeUpdate();
    }

    public void createTablesIfNeeded() throws SQLException
    {
        String users =  "CREATE TABLE IF NOT EXISTS Users"+
                        "("+
                        "username varchar(255) NOT NULL PRIMARY KEY,"+
                        "password varchar(255) NOT NULL,"+
                        "email varchar(255) NOT NULL,"+
                        "firstName varchar(255) NOT NULL,"+
                        "lastName varchar(255) NOT NULL,"+
                        "gender char(1) NOT NULL,"+
                        "personID varchar(255) NOT NULL"+
                        ");";

        String persons= "CREATE TABLE IF NOT EXISTS Persons"+
                        "("+
                        "personID varchar(255) NOT NULL PRIMARY KEY,"+
                        "descendant varchar(255) NOT NULL,"+
                        "firstName varchar(255) NOT NULL,"+
                        "lastName varchar(255) NOT NULL,"+
                        "gender char(1) NOT NULL,"+
                        "fatherID varchar(255),"+
                        "motherID varchar(255),"+
                        "spouseID varchar(255)"+
                        ");";
        
        String events = "CREATE TABLE IF NOT EXISTS Events"+
                        "("+
                        "eventID varchar(255) NOT NULL PRIMARY KEY,"+
                        "descendant varchar(255) NOT NULL,"+
                        "personID varchar(255) NOT NULL,"+
                        "latitude varchar(255) NOT NULL,"+
                        "longitude varchar(255) NOT NULL,"+
                        "country varchar(255) NOT NULL,"+
                        "city varchar(255) NOT NULL,"+
                        "eventType varchar(255) NOT NULL,"+
                        "year integer NOT NULL"+
                        ");";

        String authTokens = "CREATE TABLE IF NOT EXISTS AuthTokens"+
                            "("+
                            "authToken varchar(255) NOT NULL PRIMARY KEY,"+
                            "username varchar(255) NOT NULL,"+
                            "personID varchar(255) NOT NULL"+
                            ");";
        Database.executeStatement(users,connection);
        Database.executeStatement(persons,connection);
        Database.executeStatement(events,connection);
        Database.executeStatement(authTokens,connection);
    }

    public void openAndStartDb()
    {
        String dbName = "db" + File.separator + "familyMap.sqlite";
        String connectionURL = "jdbc:sqlite:" + dbName;
        try
        {
            // Open a database connection
            connection = DriverManager.getConnection(connectionURL);
            // Start a transaction
            connection.setAutoCommit(false);
            createTablesIfNeeded();
            userDao.connection = connection;
            personDao.connection = connection;
            eventDao.connection = connection;
            authTokenDao.connection = connection;
        }
        catch (SQLException e)
        {
            e.printStackTrace();
            System.out.println("ERROR: Either could not open connection, start a transaction, or create tables");
        }
    }

    public void closeConnection(boolean success)
    {
        try
        {
            if (success)
            {
                connection.commit();
            }
            else
            {
                connection.rollback();
            }
        }
        catch (SQLException e)
        {
            System.out.println("ERROR: commit or rollback error");
        }
        finally
        {
            try
            {
                connection.close();
            }
            catch (SQLException e)
            {
                System.out.println("ERROR: Connection couldn't close");
            }
        }
        connection = null;
    }

    public static void finishSQL(PreparedStatement stmt, ResultSet rs) throws SQLException
    {
        if(stmt != null)
            stmt.close();
        if (rs != null)
            rs.close();
    }

    public static void finishSQL(PreparedStatement stmt) throws SQLException
    {
        if(stmt != null)
            stmt.close();
    }
}

