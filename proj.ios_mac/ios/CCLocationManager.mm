//
//  CCLocationManager.m
//  MMLocationManager
//
//  Created by WangZeKeJi on 14-12-10.
//  Copyright (c) 2014年 Chen Yaoqiang. All rights reserved.
//

#import "CCLocationManager.h"

@interface CCLocationManager (){
    CLLocationManager *_manager;

}
@property (nonatomic, strong) LocationBlock locationBlock;
@property (nonatomic, strong) NSStringBlock cityBlock;
@property (nonatomic, strong) NSStringBlock provinceBlock;
@property (nonatomic, strong) NSStringBlock contyBlock;

@property (nonatomic, strong) NSStringBlock addressBlock;
@property (nonatomic, strong) LocationErrorBlock errorBlock;

@end

@implementation CCLocationManager


+ (CCLocationManager *)shareLocation{
    static dispatch_once_t pred = 0;
    __strong static id _sharedObject = nil;
    dispatch_once(&pred, ^{
        _sharedObject = [[self alloc] init];
    });
    return _sharedObject;
}

- (id)init {
    self = [super init];
    if (self)
    {
        NSUserDefaults *standard = [NSUserDefaults standardUserDefaults];
        
        float longitude = [standard floatForKey:CCLastLongitude];
        float latitude = [standard floatForKey:CCLastLatitude];
        self.longitude = longitude;
        self.latitude = latitude;
        self.lastCoordinate = CLLocationCoordinate2DMake(longitude,latitude);
        self.lastCity = [standard objectForKey:CCLastCity];
        self.lastAddress=[standard objectForKey:CCLastAddress];
    }
    return self;
}
//获取经纬度
- (void) getLocationCoordinate:(LocationBlock) locaiontBlock
{
    self.locationBlock = [locaiontBlock copy];
    [self startLocation];
}

- (void) getLocationCoordinate:(LocationBlock) locaiontBlock  withAddress:(NSStringBlock) addressBlock
{
    self.locationBlock = [locaiontBlock copy];
    self.addressBlock = [addressBlock copy];
    [self startLocation];
}

- (void) getAddress:(NSStringBlock)addressBlock
{
    self.addressBlock = [addressBlock copy];
    [self startLocation];
}
//获取省市
- (void) getCity:(NSStringBlock)cityBlock province:(NSStringBlock)provinceBlock conty:(NSStringBlock)contyBlock
{
    self.cityBlock = [cityBlock copy];
    self.provinceBlock = [provinceBlock copy];
    self.contyBlock = [contyBlock copy];
    [self startLocation];
}

//- (void) getCity:(NSStringBlock)cityBlock error:(LocationErrorBlock) errorBlock
//{
//    self.cityBlock = [cityBlock copy];
//    self.errorBlock = [errorBlock copy];
//    [self startLocation];
//}
#pragma mark CLLocationManagerDelegate
- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation
{
    
    NSUserDefaults *standard = [NSUserDefaults standardUserDefaults];
    

    CLGeocoder *geocoder=[[CLGeocoder alloc]init];
    [geocoder reverseGeocodeLocation:newLocation completionHandler:^(NSArray *placemarks,NSError *error)
     {
         if (placemarks.count > 0)
         {
             CLPlacemark *placemark = [placemarks objectAtIndex:0];
            _lastProvince   = [NSString stringWithFormat:@"%@",placemark.administrativeArea];
             _lastCity   = [NSString stringWithFormat:@"%@",placemark.locality];
             _lastConty   = [NSString stringWithFormat:@"%@",placemark.subLocality];
             
             [standard setObject:_lastCity forKey:CCLastCity];//省市地址
            _lastAddress = [NSString stringWithFormat:@"%@%@%@%@%@%@",placemark.country,placemark.administrativeArea,placemark.locality,placemark.subLocality,placemark.thoroughfare,placemark.subThoroughfare];//详细地址
             
//            NSLog(@"______%@",_lastCity);
//            NSLog(@"______%@",_lastAddress);
             
//             NSLog(@"______%@",placemark.administrativeArea);
//             NSLog(@"______%@",placemark.locality);
//             NSLog(@"______%@",placemark.subLocality);
         }
         
         if (_cityBlock)
         {
             _cityBlock(_lastCity);
             _cityBlock = nil;
         }
         if (_provinceBlock)
         {
             _provinceBlock(_lastProvince);
             _provinceBlock = nil;
         }
         if (_contyBlock)
         {
             _contyBlock(_lastConty);
             _contyBlock = nil;
         }
         
         if (_addressBlock)
         {
             _addressBlock(_lastCity);
             _addressBlock = nil;
         }
     }];
    
    _lastCoordinate = CLLocationCoordinate2DMake(newLocation.coordinate.latitude ,newLocation.coordinate.longitude );
   
    NSLog(@"!!! _lastCoordinate latitude = %f, longitude = %f", _lastCoordinate.latitude, _lastCoordinate.longitude);

    
    if (_locationBlock) {
        _locationBlock(_lastCoordinate);
        _locationBlock = nil;
    }
    [standard setObject:@(newLocation.coordinate.latitude) forKey:CCLastLatitude];
    [standard setObject:@(newLocation.coordinate.longitude) forKey:CCLastLongitude];

    [manager stopUpdatingLocation];
    
}


-(void)startLocation
{
    if([CLLocationManager locationServicesEnabled] && [CLLocationManager authorizationStatus] != kCLAuthorizationStatusDenied)
    {
        _manager=[[CLLocationManager alloc] init];
        _manager.delegate = self;
        _manager.desiredAccuracy = kCLLocationAccuracyBest;
        if([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0f)
        {
            [_manager requestAlwaysAuthorization];
            [_manager requestWhenInUseAuthorization];     //NSLocationWhenInUseDescription
        }
        else
        {
            
        }
        _manager.distanceFilter = 100;
        [_manager startUpdatingLocation];
    }
    else
    {
//        UIAlertView *alvertView=[[UIAlertView alloc]initWithTitle:@"提示" message:@"需要开启定位服务,请到设置->隐私,打开定位服务" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles: nil];
//        [alvertView show];
        
    }
    
}
- (void)locationManager:(CLLocationManager *)manager
       didFailWithError:(NSError *)error{
    [self stopLocation];

}
-(void)stopLocation
{
    _manager = nil;
}


@end
