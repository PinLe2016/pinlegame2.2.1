package org.cocos2dx.cpp;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.content.Intent;
import android.text.TextUtils;
import android.util.Log;

import com.baidu.android.pushservice.PushMessageReceiver;

/*
 * Push消息处理receiver。请编写您需要的回调函数， 一般来说： onBind是必须的，用来处理startWork返回值；
 *onMessage用来接收透传消息； onSetTags、onDelTags、onListTags是tag相关操作的回调；
 *onNotificationClicked在通知被点击时回调； onUnbind是stopWork接口的返回值回调
 * 返回值中的errorCode，解释如下：
 *0 - Success
 *10001 - Network Problem
 *10101  Integrate Check Error
 *30600 - Internal Server Error
 *30601 - Method Not Allowed
 *30602 - Request Params Not Valid
 *30603 - Authentication Failed
 *30604 - Quota Use Up Payment Required
 *30605 -Data Required Not Found
 *30606 - Request Time Expires Timeout
 *30607 - Channel Token Timeout
 *30608 - Bind Relation Not Found
 *30609 - Bind Number Too Many
 * 当您遇到以上返回错误时，如果解释不了您的问题，请用同一请求的返回值requestId和errorCode联系我们追查问题。
 *
 */

public class MyPushMessageReceiver extends PushMessageReceiver {

	@Override
	public void onBind(Context arg0, int arg1, String arg2, String arg3,
			String arg4, String arg5) {
		// TODO Auto-generated method stub
		 String responseString = "onBind errorCode=" + arg1 + " appid="
	                + arg2 + " userId=" + arg3 + " channelId=" + arg4
	                + " requestId=" + arg5;
		 System.out.printf("f1=%s",responseString);
	        Log.d(TAG, responseString);
//	        __android_log_print( "cocos2d-x debug info",  responseString);
	}

	@Override
	public void onDelTags(Context arg0, int arg1, List<String> arg2,
			List<String> arg3, String arg4) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onListTags(Context arg0, int arg1, List<String> arg2,
			String arg3) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onMessage(Context arg0, String arg1, String arg2) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onNotificationArrived(Context arg0, String arg1, String arg2,
			String arg3) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onNotificationClicked(Context arg0, String arg1, String arg2,
			String arg3) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onSetTags(Context arg0, int arg1, List<String> arg2,
			List<String> arg3, String arg4) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onUnbind(Context arg0, int arg1, String arg2) {
		// TODO Auto-generated method stub
		
	}

	
}
