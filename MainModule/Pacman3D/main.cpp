#include "includes/Game.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <dr_wav.h>
#include <vector>
#include <AudioEngine.h>

int main() 
{
	auto mainWindow = std::make_shared<GLWindow>(800, 600); // make the window.
	mainWindow->initialise();

	auto pacmangame = std::make_unique<Game>();
	pacmangame->generateGame(mainWindow); // create the game.
	
	/**************************************************************************************************************/
	auto audioengine = std::make_unique<AudioEngine>();
	audioengine->getHostDevice();
	audioengine->createContext();
	audioengine->createListener();
	audioengine->initializeStruct();
	audioengine->generateStereoBuffer();
	ALint sourceS = audioengine->getSourceState();
	ALint StereoS = audioengine->getStereoSource();
	alec(alSourcePlay(StereoS));
	alec(alGetSourcei(StereoS, AL_SOURCE_STATE, &sourceS));

		while (!mainWindow->shouldClose()) {
			glfwPollEvents();
			pacmangame->updateGame(mainWindow);
			mainWindow->swapBuffer();	
			alec(alGetSourcei(StereoS, AL_SOURCE_STATE, &sourceS));
		}
    return 0;
}
