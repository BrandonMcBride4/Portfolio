package JUnitTests.ModelTest;

import model.InvalidModelException;
import model.User;
import org.junit.* ;
import static org.junit.Assert.* ;

public class UserModelTest
{
    //Tests getters and setters
    @Test
    public void testValidUser() throws InvalidModelException
    {
            User user = new User(
                    "asdf",
                    "asdf",
                    "asdf",
                    "asdf",
                    "asdf",
                    "f",
                    "asdf");
            user.setUsername("username");
            user.setPassword("password");
            user.setEmail("email");
            user.setFirstName("firstName");
            user.setLastName("lastName");
            user.setGender("m");
            user.setPersonID("id1");

        assertFalse(user.getUsername() == null);
        assertFalse(user.getPassword() == null);
        assertFalse(user.getEmail() == null);
        assertFalse(user.getFirstName() == null);
        assertFalse(user.getLastName() == null);
        assertFalse(user.getGender() == null);
        assertFalse(user.getPersonID() == null);
        assertTrue(user.getUsername().equals("username"));
        assertTrue(user.getPassword().equals("password"));
        assertTrue(user.getEmail().equals("email"));
        assertTrue(user.getFirstName().equals("firstName"));
        assertTrue(user.getLastName().equals("lastName"));
        assertTrue(user.getGender().equals("m"));
        assertTrue(user.getPersonID().equals("id1"));
    }

    //Gender's length is not 1
    @Test(expected = InvalidModelException.class)
    public void createInvalidUser() throws InvalidModelException
    {
        User invalidUser = new User(
                    "asdf",
                    "asdf",
                    "asdf",
                    "asdf",
                    "asdf",
                    "asdf",
                    "asdf");

    }

    //Gender is not m or f
    @Test(expected = InvalidModelException.class)
    public void createInvalidUser2() throws InvalidModelException
    {
        User invalidUser = new User(
                "asdf",
                "asdf",
                "asdf",
                "asdf",
                "asdf",
                "a",
                "asdf");

    }

    //A non null member is null
    @Test(expected = InvalidModelException.class)
    public void createInvalidUser3() throws InvalidModelException
    {
        User invalidUser = new User(
                "asdf",
                "asdf",
                "asdf",
                null,
                "asdf",
                "a",
                "asdf");

    }

}
