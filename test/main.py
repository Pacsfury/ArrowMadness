import server
import sys

if "server" in sys.argv:
    server.server_run_test()
else:
    print("Invalid options")