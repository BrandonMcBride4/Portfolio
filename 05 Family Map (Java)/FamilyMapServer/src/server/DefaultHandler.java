package server;

import com.sun.net.httpserver.Headers;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.URI;
import java.util.Scanner;

class DefaultHandler implements HttpHandler
{
    @Override
    public void handle(HttpExchange e) throws IOException
    {
        Headers headers = e.getResponseHeaders();
        URI requestURI = e.getRequestURI();
        String request = requestURI.toString();
        //Limit makes it only split in once
        String[] urlParam = request.split("/",2);
        //localhost:8080/index.html
        //""
        //index.html

        //Get header type and send
        String urlPath;
        //When url is localhost:8080 (No slash)
        if("".equals(urlParam[1]) || urlParam.length <= 1)
        {
            urlPath = "index.html";
            headers.set("Content-Type", "text/html");
        }
        else
        {
            urlPath = urlParam[1];
            String contentType = request.split("/")[1];
            //localhost:8080/css/...
            if(contentType.equals("css"))
                headers.set("Content-Type", "text/css");

            //localhost:8080/img/...
            else if(contentType.equals("img"))
                headers.set("Content-Type", "image/png");

            //localhost:8080/HTML/...
            else
                headers.set("Content-Type", "text/html");
        }
        e.sendResponseHeaders(200, 0);

        OutputStreamWriter os = new OutputStreamWriter(e.getResponseBody());
        String filePath = "WebAPI/" + File.separator + urlPath;
        Scanner scan = null;
        try
        {

            scan = new Scanner(new FileReader(filePath));

        }
        catch(IOException exc)
        {
            String errorPage = "WebAPI/HTML/404.html";
            scan = new Scanner(new FileReader(errorPage));
        }

        StringBuilder sb = new StringBuilder();
        while(scan.hasNextLine())
            sb.append(scan.nextLine() + "\n");

        scan.close();
        os.write(sb.toString());
        os.close();
    }
}
