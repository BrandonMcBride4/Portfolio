package JUnitTests.ModelTest;

import model.AuthToken;
import org.junit.* ;
import static org.junit.Assert.* ;

public class AuthTokenModelTest
{
    @Test
    public void testValidAuthToken()
    {
        AuthToken a = new AuthToken("asdf", "asdf", "asdf");
        a.setAuthToken("token");
        a.setUserName("username");
        a.setPersonID("id1");

        assertFalse(a.getAuthToken() == null);
        assertFalse(a.getUserName() == null);
        assertFalse(a.getPersonID() == null);
        assertTrue(a.getAuthToken().equals("token"));
        assertTrue(a.getUserName().equals("username"));
        assertTrue(a.getPersonID().equals("id1"));
    }
}
