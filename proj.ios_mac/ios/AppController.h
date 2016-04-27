#import <UIKit/UIKit.h>


@class RootViewController;
//,BMKLocationServiceDelegate
@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    
    UINavigationController *navigationController;
}

@property(nonatomic, readonly) RootViewController* viewController;

@end

