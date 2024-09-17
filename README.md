# STT-TTS-AI
Step 1: OpenAI API Setup

Get an OpenAI API Key:

Sign up or log in to OpenAI.

Go to the API section and generate an API key. This will be needed for API requests.

Step 2: Install Required Libraries

You’ll still need to install and link certain libraries for HTTP requests (libcurl) and JSON parsing (nlohmann/json).

libcurl:

Follow the installation guide for Windows.

Link libcurl to your Visual Studio project (I’ll provide instructions below).

nlohmann/json:

Download the json.hpp file from the GitHub repo.
Add json.hpp to your project.

Step 3: Visual Studio Setup

3.1 Linking libcurl to Visual Studio

Install libcurl:

Download the curl for Windows (choose the binary that includes SSL).

Extract the files and note the directory location.

Configure Visual Studio:

Right-click on your project and go to Properties.

Under C/C++ -> General, add the path to the curl include directory in Additional Include Directories.

Under Linker -> General, add the path to the curl lib directory in Additional Library Directories.

Under Linker -> Input, add libcurl.lib to Additional Dependencies.

Change the wake word to what you want it to be!!!!
