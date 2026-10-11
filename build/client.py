import os
import subprocess
from dotenv import load_dotenv

load_dotenv()

def build_game(compiler: str):
    compiler_mode = compiler.lower()

    sfml_include = os.getenv("SFML_INCLUDE_PATH")
    sfml_lib = os.getenv("SFML_LIB_PATH")

    if compiler_mode in ["msvc", "cl"]:
        if not sfml_include or not sfml_lib:
            print("Error: Please configure SFML_INCLUDE_PATH and SFML_LIB_PATH at .env")
            return
        
        print("Compiling game with MSVC and SFML...")
        
        command = [
            "cl", "src/*.cpp",
            "/Zi", "/EHsc", "/nologo", "/std:c++20",
            f"/I{sfml_include}",
            "/Fe:Arrow",
            "/link", f"/LIBPATH:{sfml_lib}",
            "sfml-graphics.lib", "sfml-window.lib", "sfml-system.lib"
        ]
        
        compilation = subprocess.run(command)
        
        if compilation.returncode == 0:
            subprocess.run(["Arrow"])
            
    elif compiler_mode in ["g++", "gcc", "clang", "clang++"]:
        actual_compiler = "clang++" if "clang" in compiler_mode else "g++"
        print(f"Compiling game with {actual_compiler.upper()} and SFML...")
        
        command = [
            actual_compiler, "src/*.cpp",
            "-std=c++20", "-O2",
            "-o", "Arrow"
        ]
        
        if sfml_include:
            command.append(f"-I{sfml_include}")
        if sfml_lib:
            command.append(f"-L{sfml_lib}")
            
        command.extend(["-lsfml-graphics", "-lsfml-window", "-lsfml-system"])
        
        compilation = subprocess.run(command, shell=True if os.name == 'nt' else False)
        
        if compilation.returncode == 0:
            executable = "./Arrow" if os.name != 'nt' else "Arrow"
            subprocess.run([executable])
        
    else:
        print(f"C++ compiler '{compiler}' is unrecognized.")
