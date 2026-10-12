import time
import os
import threading
import requests
import dotenv
import sys

project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
if project_root not in sys.path:
    sys.path.insert(0, project_root)

from build import server

dotenv.load_dotenv()

def server_test(url="http://localhost:8080/api/test"):
    try:
        response = requests.get(url, timeout=5)
        return response.json().get("working") == "true" or response.json().get("working") is True
    except requests.exceptions.RequestException:
        return False

def server_run_test(url="http://localhost:8080/api/test"):
    java_mode = os.getenv("JAVA_MODE", "javac")
    
    server_thread = threading.Thread(
        target=server.build_server, 
        args=(java_mode,), 
        daemon=True
    )
    server_thread.start()
    
    max_retries = 10
    for _ in range(max_retries):
        if server_test(url):
            print("Server is working")
            return True
        time.sleep(1) 

    print("Error: Server didn't respond.")
    return False
