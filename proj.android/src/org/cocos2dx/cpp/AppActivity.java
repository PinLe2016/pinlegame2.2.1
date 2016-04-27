/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
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
package org.cocos2dx.cpp;



import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONObject;

import cn.sharesdk.ShareSDKUtils;
//import com.cocos.CCPushHelper;
import com.tencent.mm.sdk.openapi.IWXAPI;


import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;


import android.location.Location;
import android.location.LocationManager;

import java.util.Locale;
import android.net.Uri;
import android.os.Bundle;

import com.umeng.mobclickcpp.MobClickCppHelper;

import android.util.Log;


import com.baidu.android.pushservice.PushConstants;
import com.baidu.android.pushservice.PushManager;
import com.baidu.location.BDLocation;
import com.baidu.location.BDLocationListener;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;
import com.baidu.location.BDNotifyListener;//假如用到位置提醒功能，需要import该类
import com.baidu.location.Poi;
import com.baidu.location.LocationClientOption.LocationMode;
import android.view.KeyEvent;


public class AppActivity extends Cocos2dxActivity  {
	
	private static 	Context mCtx;
	private static 	IWXAPI api;
	public static LocationClient mLocationClient = null;
	public static StringBuffer sb = new StringBuffer(256);
	public static StringBuffer province = new StringBuffer(256);
	public static StringBuffer city = new StringBuffer(256);
	public static StringBuffer conty = new StringBuffer(256);
	public BDLocationListener myListener = new BDLocationListener() {

		@Override
		public void onReceiveLocation(BDLocation location) {
			// TODO Auto-generated method stub
			if (null != location && location.getLocType() != BDLocation.TypeServerError) {
				
//				sb.append("time2222 : ");
				/**
				 * 时间也可以使用systemClock.elapsedRealtime()方法 获取的是自从开机以来，每次回调的时间；
				 * location.getTime() 是指服务端出本次结果的时间，如果位置不发生变化，则时间不变
				 */
//				sb.append(location.getTime());
//				sb.append("\nerror code : ");
//				sb.append(location.getLocType());
//				sb.append("\nlatitude : ");
//				sb.append(location.getLatitude());
//				sb.append("\nlontitude : ");
//				sb.append(location.getLongitude());
//				sb.append("\nradius : ");
//				sb.append(location.getRadius());
//				sb.append("\nCountryCode : ");
//				sb.append(location.getCountryCode());
//				sb.append("\nCountry : ");
//				sb.append(location.getCountry());
//				sb.append("\ncitycode : ");
//				sb.append(location.getCityCode());getProvince()
				province.append(location.getProvince());
				city.append(location.getCity());
				conty.append(location.getDistrict());
				
				sb.append("\nProvince : ");
				sb.append(location.getProvince());
				sb.append("\ncity : ");
				sb.append(location.getCity());
				sb.append("\nDistrict : ");
				sb.append(location.getDistrict());
//				sb.append("\nStreet : ");
//				sb.append(location.getStreet());
//				sb.append("\naddr : ");
//				sb.append(location.getAddrStr());
//				sb.append("\nDescribe: ");
//				sb.append(location.getLocationDescribe());
//				sb.append("\nDirection(not all devices have value): ");
//				sb.append(location.getDirection());
//				sb.append("\nPoi: ");
//				if (location.getPoiList() != null && !location.getPoiList().isEmpty()) {
//					for (int i = 0; i < location.getPoiList().size(); i++) {
//						Poi poi = (Poi) location.getPoiList().get(i);
//						sb.append(poi.getName() + ";");
//					}
//				}
				if (location.getLocType() == BDLocation.TypeGpsLocation) {// GPS定位结果
					sb.append("\nspeed : ");
					sb.append(location.getSpeed());// 单位：km/h
					sb.append("\nsatellite : ");
					sb.append(location.getSatelliteNumber());
					sb.append("\nheight : ");
					sb.append(location.getAltitude());// 单位：米
					sb.append("\ndescribe : ");
					sb.append("gps定位成功");
				} else if (location.getLocType() == BDLocation.TypeNetWorkLocation) {// 网络定位结果
					// 运营商信息
					sb.append("\noperationers : ");
					sb.append(location.getOperators());
					sb.append("\ndescribe : ");
					sb.append("网络定位成功");
				} else if (location.getLocType() == BDLocation.TypeOffLineLocation) {// 离线定位结果
					sb.append("\ndescribe : ");
					sb.append("离线定位成功，离线定位结果也是有效的");
				} else if (location.getLocType() == BDLocation.TypeServerError) {
					sb.append("\ndescribe : ");
					sb.append("服务端网络定位失败，可以反馈IMEI号和大体定位时间到loc-bugs@baidu.com，会有人追查原因");
				} else if (location.getLocType() == BDLocation.TypeNetWorkException) {
					sb.append("\ndescribe : ");
					sb.append("网络不同导致定位失败，请检查网络是否通畅");
				} else if (location.getLocType() == BDLocation.TypeCriteriaException) {
					sb.append("\ndescribe : ");
					sb.append("无法获取有效定位依据导致定位失败，一般是由于手机的原因，处于飞行模式下一般会造成这种结果，可以试着重启手机");
				}
//				Log.i("BaiduLocationApiDem",sb.toString());
			}
		}

	};
	private static 	AssetManager mngr = null;
	
	public 	static native boolean isTestMode();//从C++获取游戏是否为测试模式
	
	public	static native int getScreenWidth();	//从C++当前游戏分辨率
	public	static native int getScreenHeight();	
//	public	static native int getDeviceInfo(String info);

	public	static int	ScreenWidth;
	public	static int	ScreenHeight;
//	public	static JSONObject phoneInfo;
	 
	
	
	
    protected void onCreate(Bundle savedInstanceState)
    {	
		super.onCreate(savedInstanceState);	
		mCtx = this.getApplicationContext();
		mngr = getAssets();
		
		ScreenWidth  = getScreenWidth();
		ScreenHeight = getScreenHeight();
		
		
//		phoneInfo = NgsteamPhoneInfo.getRegPhoneInfoJson(mCtx);
//		getDeviceInfo(phoneInfo.toString());
		
		//------ShareSDK初始化---------
		ShareSDKUtils.prepare();
		//-----友盟SDK---
		MobClickCppHelper.init(this);
		 //---百度云推送
		 PushManager.startWork(getApplicationContext(),PushConstants.LOGIN_TYPE_API_KEY,"SHEWxPoo8HG42U8QR2OedF2bwS2Y2Uv2");
//		 PushManager.startWork(getApplicationContext(),PushConstants.LOGIN_TYPE_API_KEY,"qyvZ6C0nxFKHm8f2QtC9DP4N");
		 
		//-----百度定位
		 mLocationClient = new LocationClient(getApplicationContext());     //声明LocationClient类
		 mLocationClient.registerLocationListener( myListener );    //注册监听函数
		 mLocationClient.start();
		 
		 
//		 Log.d("我是王五", null);
		 
		 initLocation();
    }
    
    public boolean onKeyPreIme(int keyCode, KeyEvent event) {
//    	if (keyCode == KeyEvent.KEYCODE_BACK &&
//    	event.getAction() == KeyEvent.ACTION_UP) {
    	// Do your thing here
    	return false;
//    	}
//    	return super.onKeyUp(keyCode, event)
    	}
    
    static {
        MobClickCppHelper.loadLibrary();
    }

    public static Context getContext()
    {
    	return mCtx;
    }
    
//    public static String getSDPath()
//    { 
//        File sdDir = null; 
//        boolean sdCardExist = Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED);   //判断sd卡是否存在 
//        if(sdCardExist)   
//        {
//        	sdDir = Environment.getExternalStorageDirectory();//获取跟目录 
//        	return sdDir.toString(); 
//        } 
//        else
//        {
//        	return "/mnt/sdcard";
//        }     
//    }
    
//    public static String getAssetsPath(String filename)
//    {
////    	String path = "file:///android_asset/" + filename;
//    	
//    	String path = "/assets/" + filename;
//    	InputStream abpath = getClass().getResourceAsStream(path);
//    	
//    	String path = new String(InputStreamToByte(abpath ));
//    	
//		return path;
//    }
//    
    
    
    public static void phonecall(String str)
    {
		Context context = mCtx;
		Intent phoneIntent = new Intent("android.intent.action.CALL", Uri.parse("tel:" + str)); 
		//启动 
		context.getSystemService(Context.LOCATION_SERVICE);
		phoneIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK); 
		context.startActivity(phoneIntent);
    }
    
	public static double getLocation(int type)
    {
		Context context = mCtx;
		double longitude = 0.0;//经度
        double latitude = 0.0;//纬度
     
//        openGPS();
        try
        {
            LocationManager locationManager = (LocationManager) context
                    .getSystemService(Context.LOCATION_SERVICE);
            
            if (locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER))
            {
                Location location = locationManager
                        .getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
                if (location != null)
                {
                    latitude = location.getLatitude(); 
                    longitude = location.getLongitude(); 
                }
            }
            if(longitude == 0 || latitude == 0)
            {
                if (locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER))
                {
                    Location location = locationManager
                            .getLastKnownLocation(LocationManager.GPS_PROVIDER);
                    if (location != null)
                    {
                        latitude = location.getLatitude(); 
                        longitude = location.getLongitude(); 
                        
                    }
                }
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
            latitude = 0;
            longitude = 0;
        }
       
        if(type == 0)
        	return longitude;
        else
        	return latitude;
    }
	
	
	public static String getLocationString(int type)
    {
		mLocationClient.start();
		Log.i("BaiduLocationApiDem",sb.toString());
		if(type==0)
		{
			return province.toString();
		}
		if(type==1)
		{
			return city.toString();
		}
        if(type==2)
        {
        	return conty.toString();
        }
        return sb.toString();
    }
	
	private void initLocation()
	{
        LocationClientOption option = new LocationClientOption();
        option.setLocationMode(LocationMode.Hight_Accuracy);//可选，默认高精度，设置定位模式，高精度，低功耗，仅设备
        option.setCoorType("bd09ll");//可选，默认gcj02，设置返回的定位结果坐标系
        int span=1000;
//        option.setScanSpan(span);//可选，默认0，即仅定位一次，设置发起定位请求的间隔需要大于等于1000ms才是有效的
        option.setIsNeedAddress(true);//可选，设置是否需要地址信息，默认不需要
        option.setOpenGps(true);//可选，默认false,设置是否使用gps
        option.setLocationNotify(true);//可选，默认false，设置是否当gps有效时按照1S1次频率输出GPS结果
        option.setIsNeedLocationDescribe(true);//可选，默认false，设置是否需要位置语义化结果，可以在BDLocation.getLocationDescribe里得到，结果类似于“在北京天安门附近”
        option.setIsNeedLocationPoiList(true);//可选，默认false，设置是否需要POI结果，可以在BDLocation.getPoiList里得到
        option.setIgnoreKillProcess(false);//可选，默认true，定位SDK内部是一个SERVICE，并放到了独立进程，设置是否在stop的时候杀死这个进程，默认不杀死  
        option.SetIgnoreCacheException(false);//可选，默认false，设置是否收集CRASH信息，默认收集
        option.setEnableSimulateGps(false);//可选，默认false，设置是否需要过滤gps仿真结果，默认需要
        mLocationClient.setLocOption(option);
    }

}





