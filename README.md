# Arrow Madness

The game is still being actively developed.

## Building with Python

1. **Configure a `.env` file.**

   Create a `.env` file in the root directory with the following variables:
   * `JAVA_EXE_PATH`: Path to your `java` executable file.
   * `JAVA_CLASSPATH`: Path to the directory where your `.class` files will be generated.
   * `JAVA_MODE`: Method to compile Java. Use `"vscode"` (for the VSCode extension) or `"javac"` (for the standard compiler).
   * `SFML_INCLUDE_PATH`: Path to the `include/` folder of your SFML setup.
   * `SFML_LIB_PATH`: Path to the `lib/` folder of your SFML setup.
   * `CPP_MODE`: The C++ compiler you are using (e.g., `MSVC`, `G++`, `CLANG`).

2. **Run the Python build script:**
   ```bash
   python build/build.py game   # Build server and client
   python build/build.py server # Build server only
   python build/build.py client # Build client only
   ```

## Testing with Python

1. **COnfigure the `.env` file**

2. **Run Python**

   ```bash
   python test/main.py server # Test server (ping)
   ```

> Other test cases, as game and client will be coming son, as they are more complex