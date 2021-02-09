package dao;

import model.*;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by Brandon on 7/12/2017.
 */
public class PersonDao
{
    Connection connection = null;

    public PersonDao(Connection connection)
    {
        this.connection = connection;
    }

    /**
     *Creates a record for a person in the person table in the database.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     None, the Person object should be validated when it is created.
     * </pre>
     * @param p the Person object with the information about the person
     */
    public void createPerson(Person p) throws SQLException
    {
        PreparedStatement stmt = null;
        String insertPerson = "insert into Persons (personID, descendant, firstName, lastName, " +
                              "gender, fatherID, motherID, spouseID) values (?, ?, ?, ?, ?, ?, ?, ?)";
        try
        {
            stmt = connection.prepareStatement(insertPerson);
            stmt.setString(1, p.getPersonID());
            stmt.setString(2, p.getDescendant());
            stmt.setString(3, p.getFirstName());
            stmt.setString(4, p.getLastName());
            stmt.setString(5, p.getGender());
            stmt.setString(6, p.getFatherID());
            stmt.setString(7, p.getMotherID());
            stmt.setString(8, p.getSpouseID());


            if(stmt.executeUpdate() == 1)
            {
                // OK
            }
            else
            {
                System.out.println("Error in PersonDao, createPerson()");
                throw new SQLException();
            }
        }
        catch(SQLException exc)
        {
            System.out.println("Error in PersonDao, createPerson()");
            exc.printStackTrace();
            throw new SQLException();
        }
        finally
        {
            if(stmt != null)
                stmt.close();
        }
    }

    public void updatePerson(Person p) throws SQLException
    {
        PreparedStatement stmt = null;
        String updatePerson = "update Persons " +
                              "set personID = ?, descendant = ?, firstName = ?, lastName = ?," +
                              "gender = ?, fatherID = ?, motherID = ?, spouseID = ?" +
                              "where personID = ?";
        try
        {
            stmt = connection.prepareStatement(updatePerson);
            stmt.setString(1, p.getPersonID());
            stmt.setString(2, p.getDescendant());
            stmt.setString(3, p.getFirstName());
            stmt.setString(4, p.getLastName());
            stmt.setString(5, p.getGender());
            stmt.setString(6, p.getFatherID());
            stmt.setString(7, p.getMotherID());
            stmt.setString(8, p.getSpouseID());
            stmt.setString(9, p.getPersonID());



            if(stmt.executeUpdate() == 1)
            {
                // OK
            }
            else
            {
                System.out.println("Error in PersonDao, updatePerson()");
                throw new SQLException();
            }
        }
        catch(SQLException exc)
        {
            System.out.println("Error in PersonDao, updatePerson()");
            throw new SQLException();
        }
        finally
        {
            if(stmt != null)
                stmt.close();
        }
    }

    /**
     * Retrives a person record from the person ID.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     The personID must be a valid personID.
     * </pre>
     * @param personID the person ID of the person we want to get records for
     * @return a Person object with all the information about the person
     */
    public Person getPerson(String personID) throws SQLException, InvalidModelException
    {
        //person/[personID]
        //isPerson()
        Person person = null;
        PreparedStatement stmt = null;
        ResultSet rs = null;
        try
        {
            String queryPerson = "select * from Persons where Persons.personID = ?";
            stmt = connection.prepareStatement(queryPerson);
            stmt.setString(1, personID);
            rs = stmt.executeQuery();

            while(rs.next())
            {
                String qPersonID = rs.getString(1);
                String descendant = rs.getString(2);
                String firstName = rs.getString(3);
                String lastName = rs.getString(4);
                String gender = rs.getString(5);
                String fatherID = rs.getString(6);
                String motherID = rs.getString(7);
                String spouseID = rs.getString(8);
                person = new Person(descendant, qPersonID, firstName, lastName,
                                    gender, fatherID, motherID, spouseID);
            }
        }
        catch(SQLException exc)
        {
            if (person != null)
                System.out.println("Error in PersonDao, getPerson(String personID)");
        }
        finally
        {
            if(stmt != null)
                stmt.close();
            if (rs != null)
                rs.close();
        }
        return person;
    }

    /**
     * Retrives a person record from the person ID.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     The personID must be a valid personID.
     * </pre>
     * @param personID the person ID of the person we want to get records for
     * @return a Person object with all the information about the person
     */
    public Person getPerson(String personID, String username) throws SQLException, InvalidModelException
    {
        //person/[personID]
        //isPerson()
        Person person = null;
        PreparedStatement stmt = null;
        ResultSet rs = null;
        try
        {
            String queryPerson = "select * from Persons" +
                                 " where Persons.personID = ?" +
                                 " and Persons.descendant = ?";
            stmt = connection.prepareStatement(queryPerson);
            stmt.setString(1, personID);
            stmt.setString(2, username);
            rs = stmt.executeQuery();

            while(rs.next())
            {
                String qPersonID = rs.getString(1);
                String descendant = rs.getString(2);
                String firstName = rs.getString(3);
                String lastName = rs.getString(4);
                String gender = rs.getString(5);
                String fatherID = rs.getString(6);
                String motherID = rs.getString(7);
                String spouseID = rs.getString(8);
                person = new Person(descendant, qPersonID, firstName, lastName,
                        gender, fatherID, motherID, spouseID);
            }
        }
        catch(SQLException exc)
        {
            if (person != null)
                System.out.println("Error in PersonDao, getPerson(String personID, String username)");
        }
        finally
        {
            if(stmt != null)
                stmt.close();
            if (rs != null)
                rs.close();
        }
        return person;
    }

    public boolean isPerson(String personID) throws SQLException, InvalidModelException
    {
        if (getPerson(personID) != null)
            return true;
        else
            return false;
    }

    /**
     * Retrives all person records that are related to the given user
     * which is determined from the provided auth token.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     The auth token must be a valid auth token.
     * </pre>
     * @param username Username comes from the auth token
     * @return an array of Person objects
     */
    public List<Person> getAllPersons(String username) throws SQLException, InvalidModelException
    {
        //person
        List<Person> persons = null;
        PreparedStatement stmt = null;
        ResultSet rs = null;
        try
        {
            String queryPerson = "select * from Persons where Persons.descendant = ?";
            stmt = connection.prepareStatement(queryPerson);
            stmt.setString(1, username);
            rs = stmt.executeQuery();
            persons = new ArrayList<Person>();

            while(rs.next())
            {
                String qPersonID = rs.getString(1);
                String descendant = rs.getString(2);
                String firstName = rs.getString(3);
                String lastName = rs.getString(4);
                String gender = rs.getString(5);
                String fatherID = rs.getString(6);
                String motherID = rs.getString(7);
                String spouseID = rs.getString(8);
                persons.add(new Person(descendant, qPersonID, firstName, lastName,
                        gender, fatherID, motherID, spouseID));
            }
        }
        catch(SQLException exc)
        {
            System.out.println("Error in PersonDao, getAllPersons(String username)");
        }
        finally
        {
            if(stmt != null)
                stmt.close();
            if (rs != null)
                rs.close();
        }
        return persons;
    }

    /**
     * Deletes all person records in the database for the specified user.
     * <pre>
     *     <b>Constraints on the input</b>:
     *     Must be a valid user name.
     * </pre>
     * @param username the user name of the user to delete the info
     */
    public void deleteAllPersons(String username) throws SQLException
    {
        //fill/...
        PreparedStatement stmt = null;
        try
        {
            String deletePersons = "delete from Persons where Persons.descendant = ?";
            stmt = connection.prepareStatement(deletePersons);
            stmt.setString(1, username);
            stmt.executeUpdate();
        }
        catch(SQLException exc)
        {
            System.out.println("Error in PersonDao, deleteAllPersons(String username)");
            exc.printStackTrace();
        }
        finally
        {
            if(stmt != null)
                stmt.close();
        }
    }

    /**
     * Deletes all person records in the database for all users.
     */
    public void deleteAllPersons() throws SQLException
    {
        //clear
        //load
        String drop = "DROP TABLE IF EXISTS Persons;";
        Database.executeStatement(drop,connection);
    }
}
