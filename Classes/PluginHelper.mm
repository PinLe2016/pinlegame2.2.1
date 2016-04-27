//
//  PluginHelper.m
//  PinLe
//
//  Created by 李辛 on 15/6/17.
//
//

#import <Foundation/Foundation.h>
#import "PluginHelper.h"
#import "../proj.ios_mac/ios/RootViewController.h"

static PluginHelper *_PluginHelper = nullptr;

PluginHelper::PluginHelper()
{
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
}

PluginHelper::~PluginHelper()
{
    if(viewController)
    {
        [(RootViewController*)viewController release];
        viewController = nullptr;
    }
}

PluginHelper* PluginHelper::getInstance()
{
    if(_PluginHelper == nullptr)
    {
        _PluginHelper = new PluginHelper();
    }
    return _PluginHelper;
}

void PluginHelper::getPlayerPosition()
{
    RootViewController* vc = (RootViewController*)viewController;
    if(vc)
    {
        vc.wantsFullScreenLayout = YES;
        [vc getPlayerPosition];
    }
}

void PluginHelper::getCity()
{
    RootViewController* vc = (RootViewController*)viewController;
    if(vc)
    {
        vc.wantsFullScreenLayout = YES;
        [vc getCity];
    }
}


void PluginHelper::phonecall(const std::string& number)
{
    RootViewController* vc = (RootViewController*)viewController;
    if(vc)
    {
        NSString *astring = [[NSString alloc] initWithUTF8String:number.c_str()];
        [vc phonecall:astring];
        [astring release];
    }
}

void PluginHelper::comment(const std::string& appID)
{
    RootViewController* vc = (RootViewController*)viewController;
    if(vc)
    {
        NSString *astring = [[NSString alloc] initWithUTF8String:appID.c_str()];
        [vc comment:astring];
        [astring release];
    }
}






