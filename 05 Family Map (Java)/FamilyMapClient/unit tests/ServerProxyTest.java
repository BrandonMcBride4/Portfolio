package edu.byu.cs.students.familymap;

import com.google.gson.JsonObject;

import org.junit.*;

import java.net.SocketTimeoutException;

import static org.junit.Assert.*;

import edu.byu.cs.students.familymap.model.Event;
import edu.byu.cs.students.familymap.model.InvalidModelException;
import edu.byu.cs.students.familymap.model.Person;
import edu.byu.cs.students.familymap.request.LoginRequest;
import edu.byu.cs.students.familymap.request.RegisterRequest;
import edu.byu.cs.students.familymap.server.EventsTask;
import edu.byu.cs.students.familymap.server.JsonConverter;
import edu.byu.cs.students.familymap.server.MissingPostException;
import edu.byu.cs.students.familymap.server.PersonsTask;
import edu.byu.cs.students.familymap.server.ServerProxy;

/**
 * Created by Brandon on 8/13/2017.
 */

public class ServerProxyTest
{
    private final String SERVER_HOST = "10.11.48.146";
    private final String SERVER_PORT = "8080";
    private final String REGISTER_COMMAND = "/user/register/";
    private final String LOGIN_COMMAND = "/user/login/";
    private final String PERSON_COMMAND = "/person/";
    private final String EVENT_COMMAND = "/event/";

    private String authToken;

    private ServerProxy proxy;

    //Set up server connection and reset it before every test
    @Before
    public void setUpServerProxy()
    {
        proxy = new ServerProxy(SERVER_HOST, SERVER_PORT);
        proxy.post("/clear", null, null);
    }

    //Tests a valid register and an invalid one (username already exsts)
    @Test
    public void testRegister() throws MissingPostException
    {
        RegisterRequest request = testValidRegister();
        testInvalidRegister(request);
    }

    private RegisterRequest testValidRegister() throws MissingPostException
    {
        //Test valid username
        RegisterRequest request = new RegisterRequest("username",
                "password",
                "email",
                "firstName",
                "lastName",
                "m");
        JsonObject registerResult = proxy.post(REGISTER_COMMAND, null, request);
        testValidRegisterResult(registerResult);

        return request;
    }

    private void testValidRegisterResult(JsonObject result) throws MissingPostException
    {
        authToken = JsonConverter.getMandatoryBodyPart(result, "authToken");
        String username = JsonConverter.getMandatoryBodyPart(result, "username");
        String personId = JsonConverter.getMandatoryBodyPart(result, "personId");

        assertTrue(username.equals("username"));
        assertTrue(authToken != null);
        assertTrue(personId != null);
    }

    private void testInvalidRegister(RegisterRequest request) throws MissingPostException
    {
        //Test username already taken
        JsonObject registerError = proxy.post(REGISTER_COMMAND, null, request);
        String message = JsonConverter.getMandatoryBodyPart(registerError, "message");
        assertTrue(message.equals("Username already exists."));
    }

    //Tests a valid login and two invalid ones (wrong username and password)
    @Test
    public void testLogin() throws MissingPostException
    {
        //Since server clears every time, need an account
        testValidRegister();

        //Test valid login
        LoginRequest request = new LoginRequest("username", "password");
        JsonObject loginResult = proxy.post(LOGIN_COMMAND, null, request);
        testValidRegisterResult(loginResult);

        //Test wrong username
        request = new LoginRequest("usernam", "password");
        testInvalidLogin(request);

        //Test wrong password
        request = new LoginRequest("username", "passwor");
        testInvalidLogin(request);
    }

    private void testInvalidLogin(LoginRequest request) throws MissingPostException
    {
        JsonObject loginError = proxy.post(LOGIN_COMMAND, null, request);
        String message = JsonConverter.getMandatoryBodyPart(loginError, "message");
        assertTrue(message.equals("Wrong username or password."));
    }

    //Tests the conversion of the Json array to Person array
    @Test
    public void testPerson() throws MissingPostException, InvalidModelException
    {
        //Since server clears every time, need an account
        testValidRegister();

        JsonObject jsonPerson = proxy.get(PERSON_COMMAND, authToken);
        Person[] persons = PersonsTask.jsonToPersonArray(jsonPerson);
        Person user = persons[0];

        //When registered, 4 generations is created by default = 31 persons
        assertTrue(persons.length == 31);
        assertTrue(user.getFullName().equals("firstName lastName"));

        for (Person person : persons)
        {
            assertTrue(person.getDescendant().equals("username"));
        }
    }

    //Tests the conversion of the Json array to Event array
    @Test
    public void testEvent() throws MissingPostException, InvalidModelException
    {
        //Since server clears every time, need an account
        testValidRegister();

        JsonObject jsonEvent = proxy.get(EVENT_COMMAND, authToken);
        Event[] events = EventsTask.jsonToEventArray(jsonEvent);

        //When registered, 4 generations is created by default = 31 persons
        //Each person should have at least 4 events or more
        assertTrue(events.length > 31 * 4);

        for (Event event : events)
        {
            assertTrue(event.getDescendant().equals("username"));
        }
    }

    //An invalid command should return null
    @Test
    public void testInvalidCommand()
    {
        JsonObject jsonObject = proxy.get("/invalid/command", authToken);
        assertTrue(jsonObject == null);
    }

    //An error should return null
    @Test
    public void invalidServerHost()
    {
        ServerProxy invalidProxy = new ServerProxy("1.2.3.4", SERVER_PORT);
        JsonObject json = invalidProxy.get("/clear", null);
        assertTrue(json == null);
    }

    //An error should return null
    @Test
    public void invalidServerPort()
    {
        ServerProxy invalidProxy = new ServerProxy(SERVER_HOST, "8079");
        JsonObject json = invalidProxy.get("/clear", null);
        assertTrue(json == null);
    }
}
