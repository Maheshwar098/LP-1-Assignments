import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Client {
    public static void main(String[] args) {
        final String SERVER_IP = "localhost";
        final int SERVER_PORT = 12345;

        try (Socket socket = new Socket(SERVER_IP, SERVER_PORT);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true)) {

            System.out.println("Connected to server on " + SERVER_IP + ":" + SERVER_PORT);

            // Hardcoded message to be sent to the server
            String messageToSend = "Hello, Server!";

            // Send the hardcoded message to the server
            out.println(messageToSend);
            System.out.println("Sent to server: " + messageToSend);

            // Receive and print the server's response
            String response = in.readLine();
            System.out.println("Server response: " + response);

        } catch (IOException e) {
            System.err.println("Error: " + e.getMessage());
        }
    }
}
