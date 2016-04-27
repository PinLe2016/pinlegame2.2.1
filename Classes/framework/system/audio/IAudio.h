//
//  MyAudio.h
//  PinLe
//
//  Created by Kennedy on 15/6/12.
//
//

#ifndef __PinLe__MyAudio__
#define __PinLe__MyAudio__

#include "cocos2d.h"
using namespace std;

class IAudio
{
public:
    static  IAudio* getInstance();
    static  void destroyInstance();
    
    
    bool    preloadBackground(const string& filename);
    bool    preloadEffect(const string& filename);
    
    void    playBackground(const string& filename);
    void    playBackground(const string& filename, bool bLoop);
    void    playBackground(const string& filename, bool bReplay, bool bLoop);
    void    stopBackgroundMusic();
    
    void    playEffect(const string& filename);
    void    playEffect(const string& filename, bool bLoop);
    void    playEffect(const string& filename, bool bReplay, bool bLoop);
    
    void    setVolume(float volume);
    
    string  getBackgroundPlaying();
};



#endif /* defined(__PinLe__MyAudio__) */
