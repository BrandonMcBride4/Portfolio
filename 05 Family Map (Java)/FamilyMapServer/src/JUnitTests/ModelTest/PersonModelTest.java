package JUnitTests.ModelTest;

import model.InvalidModelException;
import model.Person;
import org.junit.* ;
import static org.junit.Assert.* ;

public class PersonModelTest
{
    //Tests getters and setters
    @Test
    public void testValidPerson() throws InvalidModelException
    {
        Person p = new Person(
                "",
                "",
                "",
                "",
                "m",
                null,
                null,
                null);
        p.setDescendant("descendant");
        p.setPersonID("personID");
        p.setFirstName("firstName");
        p.setLastName("lastName");
        p.setGender("f");
        p.setFatherID("fatherID");
        p.setMotherID("motherID");
        p.setSpouseID("spouseID");

        assertFalse(p.getDescendant() == null);
        assertFalse(p.getPersonID() == null);
        assertFalse(p.getFirstName() == null);
        assertFalse(p.getLastName() == null);
        assertFalse(p.getGender() == null);
        assertFalse(p.getFatherID() == null);
        assertFalse(p.getMotherID() == null);
        assertFalse(p.getSpouseID() == null);
        assertTrue(p.getDescendant().equals("descendant"));
        assertTrue(p.getPersonID().equals("personID"));
        assertTrue(p.getFirstName().equals("firstName"));
        assertTrue(p.getLastName().equals("lastName"));
        assertTrue(p.getGender().equals("f"));
        assertTrue(p.getFatherID().equals("fatherID"));
        assertTrue(p.getMotherID().equals("motherID"));
        assertTrue(p.getSpouseID().equals("spouseID"));
    }

    //Gender's length is more than 1
    @Test(expected = InvalidModelException.class)
    public void createInvalidPerson() throws InvalidModelException
    {
        Person person = new Person(
                "asdf",
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
    public void createInvalidPerson2() throws InvalidModelException
    {
        Person person = new Person(
                "asdf",
                "asdf",
                "asdf",
                "asdf",
                "a",
                "asdf",
                "asdf",
                "asdf");
    }

    //A non null member is null
    @Test(expected = InvalidModelException.class)
    public void createInvalidPerson3() throws InvalidModelException
    {
        Person person = new Person(
                "asdf",
                null,
                "asdf",
                "asdf",
                "f",
                "asdf",
                "asdf",
                "asdf");
    }
}
