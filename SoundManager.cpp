#include "SoundManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

SoundManager::SoundManager()
    : stepSound(std::nullopt)
    , stepDelay(0.3f)
    , wasMoving(false)
    , volume(80.0f) {

    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

SoundManager::~SoundManager() = default;

bool SoundManager::loadAllSounds() {
    
    sf::SoundBuffer buf1, buf2, buf3;

    if (!buf1.loadFromFile("sounds/step11.wav")) {
        std::cerr << "Warning: Could not load sounds/step1.wav" << std::endl;
       
    } else {
        stepBuffers.push_back(std::move(buf1));
    }

    if (!buf2.loadFromFile("sounds/step21.wav")) {
        std::cerr << "Warning: Could not load sounds/step2.wav" << std::endl;
  
    } else {
        stepBuffers.push_back(std::move(buf2));
    }

    if (!buf3.loadFromFile("sounds/step31.wav")) {
        std::cerr << "Warning: Could not load sounds/step3.wav" << std::endl;
  
    } else {
        stepBuffers.push_back(std::move(buf3));
    }

    if (stepBuffers.empty()) {
        std::cerr << "Error: No footstep sounds loaded! Game will run without footsteps." << std::endl;
        return false;
    }

    stepSound.emplace(stepBuffers[0]);
    stepSound->setVolume(volume);

    std::cout << "Loaded " << stepBuffers.size() << " footstep sounds" << std::endl;
    return true;
}

void SoundManager::updateFootsteps(bool isMoving) {
    if (!stepSound.has_value()) return;


    if (isMoving && !wasMoving) {
        stepClock.restart();
    }

    else if (isMoving && stepClock.getElapsedTime().asSeconds() >= stepDelay) {

        if (stepSound->getStatus() != sf::SoundSource::Status::Playing) {
            if (!stepBuffers.empty()) {
                int randomIndex = std::rand() % stepBuffers.size();
                stepSound->setBuffer(stepBuffers[randomIndex]);
                stepSound->play();
            }


            stepClock.restart();
        }
    }

    wasMoving = isMoving;
}

void SoundManager::setVolume(float vol) {
    volume = vol;
    if (stepSound.has_value()) {
        stepSound->setVolume(volume);
    }
}
