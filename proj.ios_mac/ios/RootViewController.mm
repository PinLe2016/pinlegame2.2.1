/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "../../Classes/player/IPlayerData.h"
#import "../../Classes/player/config.h"
#import "../../Classes/chooseCity/LocPlay.h"
#import <CoreLocation/CoreLocation.h>
#import <Foundation/Foundation.h>
#import <MapKit/MKMapView.h>
#import "CCLocationManager.h"
#import "sys/xattr.h"


@interface RootViewController() <CLLocationManagerDelegate>
{
    CLLocationManager *locationmanager;
}

@end


@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
 - (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
 if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
 // Custom initialization
 }
 return self;
 }
 */

/*
 // Implement loadView to create a view hierarchy programmatically, without using a nib.
 - (void)loadView {
 }
 */

/*
 // Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
 - (void)viewDidLoad {
 [super viewDidLoad];
 }
 
 */
// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6




- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
    
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    
    if (glview)
    {
        CCEAGLView *eaglview = (CCEAGLView*) glview->getEAGLView();
        
        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

- (void)getPlayerPosition
{
    [[CCLocationManager shareLocation] getLocationCoordinate:^(CLLocationCoordinate2D locationCorrrdinate)
    {
        IPlayerData *iplayer = M_GET_PLAYER;
        iplayer->setCoordinate(locationCorrrdinate.longitude, locationCorrrdinate.latitude);
        CCLOG("GPS  %.2f  %.2f",locationCorrrdinate.longitude,locationCorrrdinate.latitude);
        IMessageManager::getInstance()->postNotification(MES_PLAYER_GETLOCATION);
    }];
}

-(void) getCity
{
    [[CCLocationManager shareLocation] getCity:^(NSString *addressString) {
        if (addressString) {
            string cityaddress=[addressString UTF8String];
            LocPlay::getInstance()->setIs_iphone(false);
            LocPlay::getInstance()->setCity(cityaddress);
            CCLOG("地址 %s ",cityaddress.c_str());
        }
        
    } province:^(NSString *addressString) {
        if (addressString) {
            string cityaddress=[addressString UTF8String];
            LocPlay::getInstance()->setProvince(cityaddress);
            CCLOG("地址 %s ",cityaddress.c_str());
        }
       
    } conty:^(NSString *addressString) {
        if (addressString) {
            string cityaddress=[addressString UTF8String];
            LocPlay::getInstance()->setConty(cityaddress);
            CCLOG("地址 %s ",cityaddress.c_str());
        }
       
    }];
}
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (1 == buttonIndex)
    {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"tel://10086"]];
    }
}

- (void)phonecall:(NSString*) number
{
    NSMutableString * str = [[NSMutableString alloc] initWithFormat:@"tel:%@", number];
    UIWebView * callWebview = [[UIWebView alloc] init];
    [callWebview loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:str]]];
    [self.view addSubview:callWebview];
    [callWebview release];
    [str release];
}


- (void) comment:(NSString*) appID
{
    NSMutableString * str = [[NSMutableString alloc] initWithFormat:@"http://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=%@&pageNumber=0&sortOrdering=2&type=Purple+Software&mt=8", appID];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
    [str release];
}


- (BOOL)addSkipBackupAttributeToItemAtURL:(NSURL *)URL
{
    const char* filePath = [[URL path] fileSystemRepresentation];
    
    const char* attrName = "com.apple.MobileBackup";
    u_int8_t attrValue = 1;
    
    int result = setxattr(filePath, attrName, &attrValue, sizeof(attrValue), 0, 0);
    return result == 0;
}

- (void)addSkipBackupAttributeToPath:(NSString*)path {
    u_int8_t b = 1;
    setxattr([path fileSystemRepresentation], "com.apple.MobileBackup", &b, 1, 0, 0);
}






@end
