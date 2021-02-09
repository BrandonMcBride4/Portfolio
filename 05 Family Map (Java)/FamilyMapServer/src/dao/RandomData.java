package dao;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import model.InvalidModelException;

import java.io.File;
import java.sql.SQLException;
import java.util.Calendar;
import java.util.Random;

public class RandomData
{
    static Random staticRand = new Random(Calendar.getInstance().getTime().getTime());
    public Random rand = new Random(Calendar.getInstance().getTime().getTime());

    String fnames = "jsonFiles" + File.separator + "fnames.json";
    String mnames = "jsonFiles" + File.separator + "mnames.json";
    String snames = "jsonFiles" + File.separator + "snames.json";
    String locations = "jsonFiles" + File.separator + "locations.json";

    JsonConverter converter = new JsonConverter();
    JsonArray fNamesArray = converter.jsonFiletoJsonArray(fnames,"data");
    JsonArray mNamesArray = converter.jsonFiletoJsonArray(mnames,"data");
    JsonArray sNamesArray = converter.jsonFiletoJsonArray(snames,"data");
    JsonArray locationsArray = converter.jsonFiletoJsonArray(locations,"data");

    public static String idMaker()
    {
        return Integer.toHexString((staticRand.nextInt(0xffff) << 16) + staticRand.nextInt(0xffff));
    }

    public static String makeUniquePersonID(Database db) throws SQLException, InvalidModelException
    {
        String personID;
        do
        {
            personID = RandomData.idMaker();
        }
        while(db.personDao.isPerson(personID));
        return personID;
    }

    public static String makeUniqueEventID(Database db) throws SQLException, InvalidModelException
    {
        String eventID;
        do
        {
            eventID = RandomData.idMaker();
        }
        while(db.eventDao.isEvent(eventID));
        return eventID;
    }

    public String getRandomMaleName()
    {
        return mNamesArray.get(rand.nextInt(mNamesArray.size() - 1)).getAsString();
    }

    public String getRandomFemaleName()
    {
        return fNamesArray.get(rand.nextInt(fNamesArray.size() - 1)).getAsString();
    }

    public String getRandomSurname()
    {
        return sNamesArray.get(rand.nextInt(sNamesArray.size() - 1)).getAsString();
    }

    public JsonObject getRandomLocation()
    {
        return locationsArray.get(rand.nextInt(locationsArray.size() - 1)).getAsJsonObject();
    }

    public int getRandomInterval(int a, int b)
    {
        return rand.nextInt(b - a + 1) + a;
    }
}
