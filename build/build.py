import sys
import os
import threading
from dotenv import load_dotenv
import server
import client

def main():
    load_dotenv()

    java_mode = os.getenv("JAVA_MODE", "javac")
    cpp_mode = os.getenv("CPP_MODE", "g++")

    if "game" in sys.argv:
        server_thread = threading.Thread(target=server.build_server, args=(java_mode,))
        server_thread.start()
        
        client.build_game(cpp_mode)

    elif "server" in sys.argv:
        server.build_server(java_mode)

    elif "client" in sys.argv:
        client.build_game(cpp_mode)

if __name__ == "__main__":
    main()
