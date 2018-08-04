////////////
void links()
{
    
    client.println("<br>");
    client.println("<a href=http://" + publicIP + ":" + LISTEN_PORT + "/LOG.TXT download>Current Week Observations</a><br>");
    client.println("<br>");
    client.println("<a href=http://" + publicIP + ":" + LISTEN_PORT + "/SdBrowse >File Browser</a><br>");
    client.println("<br>");
    client.println("<a href=http://" + publicIP + ":" + LISTEN_PORT + "/Graphs >Graphed Weather Observations</a><br>");
    client.println("<br>");
    client.println("<a href=http://" + publicIP + ":" + LISTEN_PORT + "/README.TXT download>Server:  README</a><br>");
    client.println("<br>");
    //Show IP Adress on client screen
    client.print("Client IP: ");
    client.print(client.remoteIP().toString().c_str());
    client.println("</body>");
    client.println("</html>");
}

