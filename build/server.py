import os
import subprocess
from dotenv import load_dotenv

load_dotenv()

def build_server(compiler: str):
    compiler_mode = compiler.lower()
    
    if compiler_mode == "vscode":
        java_exe = os.getenv("JAVA_EXE_PATH")
        classpath = os.getenv("JAVA_CLASSPATH")
        
        if not java_exe or not classpath:
            print("Error: Please configure JAVA_EXE_PATH i JAVA_CLASSPATH at .env")
            return

        print("Compiling server with VSCode extension...")
        subprocess.run([java_exe, "-cp", classpath, "Main"])

    elif compiler_mode == "javac":
        print("Compiling server with javac and java...")
        compilation = subprocess.run(["javac", "-d", "../backend", "../backend/Main.java"])
        if compilation.returncode == 0:
            subprocess.run(["java", "-cp", "../backend", "Main"])
            
    else:
        print(f"Compiler '{compiler}' is unrecognized.")
