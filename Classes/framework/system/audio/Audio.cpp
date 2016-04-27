//
//  IAudio.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/12.
//
//

#include "IAudio.h"
#include "SimpleAudioEngine.h"
#include "../../tools/ITools.h"
#include "../file/IFile.h"

using namespace CocosDenshion;


static map<string, int> _audioBackground;
static map<string, int> _audioEffect;

static string   backgroundPlaying = "";//当前播放的背景音乐名字
static IAudio*  _IAudio = NULL;

IAudio* IAudio::getInstance()
{
    if(_IAudio == NULL)
    {
        _IAudio = new IAudio();
        _audioBackground.clear();
        _audioEffect.clear();
    }
    return _IAudio;
}


void IAudio::destroyInstance()
{
    if(_IAudio != NULL)
    {
        _audioBackground.clear();
        _audioEffect.clear();
        SAFE_DELETE(_IAudio);
    }
}

bool IAudio::preloadBackground(const string& filename)
{
    if(!IFile::isFileExist(filename))
        return NO;
    
    map<string, int>::iterator it = _audioBackground.find(filename);
    if(it == _audioBackground.end())
    {
        _audioBackground.insert(pair<string, int>(filename, 0));
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(filename.c_str());
        return YES;
    }
    else
    {
        return NO;
    }
}

bool IAudio::preloadEffect(const string& filename)
{
    if(!IFile::isFileExist(filename))
        return NO;
    
    map<string, int>::iterator it = _audioEffect.find(filename);
    if(it == _audioEffect.end())
    {
        _audioEffect.insert(pair<string, int>(filename, 0));
        SimpleAudioEngine::getInstance()->preloadEffect(filename.c_str());
        return YES;
    }
    else
    {
        return NO;
    }
}


void IAudio::playBackground(const string& filename)
{
    playBackground(filename, YES);
}


void IAudio::playBackground(const string& filename, bool bLoop)
{
    playBackground(filename, YES, 7);
}

void IAudio::stopBackgroundMusic()
{
    map<string, int>::iterator it = _audioBackground.find(backgroundPlaying);
    if(it != _audioBackground.end())
    {
        _audioBackground[backgroundPlaying] = 0;
    }
    backgroundPlaying = "";
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void IAudio::playBackground(const string& filename, bool bReplay, bool bLoop)
{
    if(backgroundPlaying.compare(filename) != 0)
    {
        stopBackgroundMusic();
    }
    backgroundPlaying = filename;
    map<string, int>::iterator it = _audioBackground.find(filename);
    if(it != _audioBackground.end())
    {
        if(bReplay)
        {
            _audioBackground[filename] = 1;
            SimpleAudioEngine::getInstance()->playBackgroundMusic(filename.c_str(), bLoop);
        }
        else
        {
            if(_audioBackground[filename] == 0)
            {
                _audioBackground[filename] = 1;
                SimpleAudioEngine::getInstance()->playBackgroundMusic(filename.c_str(), bLoop);
            }
        }
    }
}


void IAudio::playEffect(const string& filename)
{
    playEffect(filename, NO);
}


void IAudio::playEffect(const string& filename, bool bLoop)
{
    playEffect(filename, NO, bLoop);
}

void IAudio::playEffect(const string& filename, bool bReplay, bool bLoop)
{
    map<string, int>::iterator it = _audioEffect.find(filename);
    if(it != _audioEffect.end())
    {
        SimpleAudioEngine::getInstance()->playEffect(filename.c_str(), bLoop);
//        if(bReplay)
//        {
//            _audioEffect[filename] = 1;
//            SimpleAudioEngine::getInstance()->playEffect(filename.c_str(), bLoop);
//        }
//        else
//        {
//            if(_audioEffect[filename] == 0)
//            {
//                _audioEffect[filename] = 1;
//                SimpleAudioEngine::getInstance()->playEffect(filename.c_str(), bLoop);
//            }
//        }
    }
}

void IAudio::setVolume(float volume)
{
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
    SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
}


string IAudio::getBackgroundPlaying()
{
    return "";
}










