#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SFML/Audio.hpp>
#include <vector>
#include <optional>  

class SoundManager {
private:
    
    std::vector<sf::SoundBuffer> stepBuffers;
    std::optional<sf::Sound> stepSound;  
    
    
    sf::Clock stepClock;
    float stepDelay;
    bool wasMoving;
    float volume;
    
public:
    SoundManager();
    ~SoundManager();
    
    
    bool loadAllSounds();
    
    
    void updateFootsteps(bool isMoving);
    
    
    void setVolume(float vol);
};

#endif