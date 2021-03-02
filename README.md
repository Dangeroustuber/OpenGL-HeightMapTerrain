# Autumn 2020 exam Elvis Arifagic

**AUDIO**  

This exam deliverable has audio. It is advised to decrease the default speaker volume of your system to atleast 15% in order to avoid any loud music!  
Consider yourself warned! (although i reduced the bitrate and master out level so it is very quiet).

This program compiles with almost if not all c++ compilers.

You can run this project on linux by running following commands.  

**Linux**:  

mkdir build-dir    
cd    build-dir    
cmake ..    
make    

From here you can select to run the "HeightMapScene.exe" file that will have been built.  

**Windows:** 
Download the newest Cmake GUI **"https://cmake.org/download/"**

Find the folder of the source code and tell CMake where to build the binaries.
Press configure button and then click generate to make the binaries.
From there you can run the .exe which would be in the "build" folder.

**Windows/visual studio:**

With visual studio you open the folder through the GUI/Clone the project and press "CTRL+SHIFT+B" to build the project once it's loaded.  
From there you are ready to run the program by running HeightSceneTest.cpp

**Windows/CLion/g++:**

CLion unlike visual studio has no solution. CLion deals in CMake projects. Therefore either clone using CLion GUI or at startup press
"new project from source" and select the cloned folder. When you open this folder CMake will automatically get generated and it's ready to run.  
Click and run the HeightMapScene.exe executable.

**Windows/visual studio with vcpkg (windows package manager)**

Go to the github repository for vcpkg and clone that/download zip. You can put in **anywhere.**  
**open powershell** in the same folder. Run command: `.\bootstrap-vcpkg.bat`, if you are using cmake, use the last line for the next part     
Run command: `.\vcpkg integrate install`    
Now you are able to run these following commands: `.\vcpkg install package_name:x64-windows`      
For this project you will need to install the following packages **glew,glfw3,assimp,glm,openAL, dr_WAV**


# Features
    - Directional,Point and spotlight are in the scene. Spotlight acts as Flashlight.
    - Moose moving freely in the scene
    - Airplane that moves in square formation
    - Instance drawn 10000 fish that are placed underwater.
    - About 3000 instance drawn trees that are placed on hilltops.
    - Free 3D camera movement in the scene.
    - Eagle model overlooking the fish in the water.
    - Heightmap is loaded and geometry is created for it.
    - Cmake modular system where you can run the previous assignement.
    - Pseudo game engine with a render module and a core libary from where you draw objects to use.
    - Audio that plays for the duration of the sample that is loaded, currently it is a song i made that is 5 min long.
    - Skybox that displays a forest.
    - Follow underneath the plane by HOLDING O.

# extra information  

**Flashlight**  
You can toggle the flashlight with 'F', the scene is a little dark on purpose for this effect to stand out.  



