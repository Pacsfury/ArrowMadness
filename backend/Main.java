import com.sun.net.httpserver.HttpServer;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;
import java.io.IOException;
import java.io.InputStream;
import java.io.ByteArrayOutputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {

    private static final Map<String, List<PlayerResult>> rooms = new HashMap<>();

    public static void main(String[] args) throws IOException {
        int port = 8080;
        HttpServer server = HttpServer.create(new InetSocketAddress(port), 0);
        
        server.createContext("/api/test", new TestHandler());
        server.createContext("/api/newroom", new NewRoomHandler());
        server.createContext("/api/ranking", new RankingHandler());
        
        server.setExecutor(null);
        server.start();
        System.out.println("Server working...");
    }

    static class TestHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            sendResponse(exchange, "{\"working\": \"true\"}", "application/json", 200);
        }
    }

    static class NewRoomHandler implements HttpHandler {
        @Override 
        public void handle(HttpExchange exchange) throws IOException {
        if ("POST".equalsIgnoreCase(exchange.getRequestMethod())) {
                InputStream is = exchange.getRequestBody();
                ByteArrayOutputStream bodyBytes = new ByteArrayOutputStream();
                byte[] buffer = new byte[1024];
                int bytesRead;
                while ((bytesRead = is.read(buffer)) != -1) {
                    bodyBytes.write(buffer, 0, bytesRead);
                }
                String body = new String(bodyBytes.toByteArray(), StandardCharsets.UTF_8);
                is.close();

                try {
                    String name = extractJsonValue(body, "name");
                    String roomId = extractJsonValue(body, "roomId");
                    int score = Integer.parseInt(extractJsonValue(body, "score"));

                    rooms.putIfAbsent(roomId, new ArrayList<>());
                    
                    rooms.get(roomId).add(new PlayerResult(name, score));

                    System.out.println("[" + roomId + "] " + name + " made " + score + " points.");

                    String response = "{\"status\":\"success\",\"message\":\"Added to room " + roomId + "\"}";
                    sendResponse(exchange, response, "application/json", 200);
                } catch (Exception e) {
                    sendResponse(exchange, "{\"error\":\"invalid json\"}", "application/json", 400);
                }
            } else {
                sendResponse(exchange, "Unvalid method", "text/plain", 405);
            }
        }
    }

    static class RankingHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            if ("GET".equalsIgnoreCase(exchange.getRequestMethod())) {
                String query = exchange.getRequestURI().getQuery();
                String roomId = "";
                if (query != null && query.contains("roomId=")) {
                    roomId = query.split("roomId=")[1].split("&")[0];
                }

                if (roomId.isEmpty() || !rooms.containsKey(roomId)) {
                    sendResponse(exchange, "{\"error\":\"room not found\"}", "application/json", 404);
                    return;
                }

                List<PlayerResult> players = new ArrayList<>(rooms.get(roomId));
                players.sort((p1, p2) -> Integer.compare(p2.score, p1.score));

                StringBuilder json = new StringBuilder("{\"roomId\":\"" + roomId + "\",\"ranking\":[");
                for (int i = 0; i < players.size(); i++) {
                    PlayerResult p = players.get(i);
                    json.append("{\"pos\":").append(i + 1)
                        .append(",\"name\":\"").append(p.name).append("\"")
                        .append(",\"score\":").append(p.score).append("}");
                    if (i < players.size() - 1) json.append(",");
                }
                json.append("]}");

                sendResponse(exchange, json.toString(), "application/json", 200);
            } else {
                sendResponse(exchange, "Unvalid method", "text/plain", 405);
            }
        }
    }

    private static String extractJsonValue(String json, String key) {
        String pattern = "\"" + key + "\":";
        int start = json.indexOf(pattern) + pattern.length();
        if (json.charAt(start) == '"') {
            start++;
            int end = json.indexOf("\"", start);
            return json.substring(start, end);
        } else {
            int end = json.indexOf(",", start);
            if (end == -1) end = json.indexOf("}", start);
            return json.substring(start, end).trim();
        }
    }

    private static void sendResponse(HttpExchange exchange, String response, String contentType, int statusCode) throws IOException {
        exchange.getResponseHeaders().set("Content-Type", contentType);
        byte[] responseBytes = response.getBytes(StandardCharsets.UTF_8);
        exchange.sendResponseHeaders(statusCode, responseBytes.length);
        OutputStream os = exchange.getResponseBody();
        os.write(responseBytes);
        os.close();
    } 
}
