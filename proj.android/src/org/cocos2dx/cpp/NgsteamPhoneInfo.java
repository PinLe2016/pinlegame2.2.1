package org.cocos2dx.cpp;


import java.io.BufferedReader;
import java.io.File;
import java.io.FileFilter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;

import java.util.Locale;
import java.util.regex.Pattern;

import org.json.JSONObject;

//import com.xinyinhe.ngsteam.NgsteamConst;
//import com.xinyinhe.ngsteam.NgsteamLog;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.location.Location;
import android.location.LocationManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Environment;
import android.os.StatFs;
import android.os.SystemClock;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.view.WindowManager;

public class NgsteamPhoneInfo
{
    private static final int NETWORK_WIFI = 0x0001;
    private static final int NETWORK_2G = 0x0002;
    private static final int NETWORK_3G = 0x0003;
    private static final int NETWORK_LTE = 0x0004;

    // private static final String phone_info = "phone_info";
    private static final String android_version = "androidversion";
    private static final String imsi = "imsi";
    private static final String imsi2 = "imsi2";
    private static final String imei = "imei";
    private static final String cpu = "cpu";
    private static final String cpu_core = "cpucore";
    private static final String cpu_freq = "cpufreq";
    private static final String model = "model";
    private static final String rooted = "rooted";
    private static final String net = "net";
    private static final String is_roam = "isroam";
    private static final String lbs_key = "lbskey";
    private static final String language = "language";
    private static final String mac = "mac";
    private static final String location = "location";
    private static final String smc_center = "smccenter";
    private static final String tele_number = "telenumber";
    private static final String tele2_number = "tele2number";
    private static final String ram = "ram";
    private static final String in_storage = "instorage";
    private static final String ex_storage = "exstorage";
    private static final String resolution = "resolution";
    private static final String opengl_es = "opengles";
    private static final String lcd_size = "lcdsize";

    public static String mAndroidVersion;
    public static String mImsi;
    public static String mImsi2;
    public static String mImei;
    public static String mCpu;
    public static int mCpuCore;
    public static String mCpuFreq;
    public static String mModel;
    public static int mRooted;
    public static int mNet;
    public static int mIsRoam;
    public static String mMac;
    public static String mLocation;
    public static String mLbsKey;
    // public static ArrayList<CellIdInfo> mCellIdInfos;
    // public static ArrayList<WifiPoint> mWifiPoints;
    // public static BroadcastReceiver mWifiReceiver;
    public static String mLanguage;
    public static String mSmcCenter;
    public static String mTeleNumber;
    public static String mTele2Number;
    public static long mRam;
    public static long mInStorage;
    public static long mExStorage;
    public static String mResolution;
    public static String mOpenglEs;
    public static String mLcdSize;

    private static Context mContext;
    private static NgsteamPhoneInfo self;

    public static NgsteamPhoneInfo getInstance(Context context)
    {
        if (self == null)
        {
            self = new NgsteamPhoneInfo(context);
        }
        else
        {
            double[] location_result;

            mNet = getNetType(mContext);
            location_result = getLocation(mContext);
            mLocation = location_result[0] + "x" + location_result[1];
        }
        return self;
    }

    private NgsteamPhoneInfo(Context context)
    {
        String[] result;
        double[] location_result;
        long[] size;

        mContext = context;
        self = this;
        // mWifiReceiver = null;

        result = getVersion();
        mAndroidVersion = result[1];
        mModel = result[2];

        result = getSimInfo(mContext);
        mImsi = result[0];
        mTeleNumber = result[1];
        mImsi2 = result[2];
        mTele2Number = result[3];

        mImei = getImeiInfo(mContext);

        result = getCpuInfo();
        mCpu = result[0];
        mCpuCore = getCpuCoreNum();
        mCpuFreq = getMaxCpuFreq();
        mNet = getNetType(mContext);
        mIsRoam = checkIsRoam(mContext);
        mRooted = checkIsRooted();
        mMac = getWifiMacAddr(mContext);
        mLanguage = getPhoneLanguage();
        mLbsKey = "";

        location_result = getLocation(mContext);
        mLocation = location_result[0] + "x" + location_result[1];
        mLbsKey = "";
        // mWifiPoints = getWifiPoint(mContext);
        // mCellIdInfos = getCellIdInfo(mContext);

        mLanguage = getLocaleLanguage();
        mSmcCenter = null;

        mRam = getTotalMemory();

        size = getRomMemory();
        mInStorage = size[0];

        size = getSDCardMemory();
        mExStorage = size[0];
        mResolution = getLcdWidthPixel(mContext) + "x"
                + getLcdHeightPixel(mContext);
        mOpenglEs = getOpenGLVersion(mContext);

        float width, height;
        double lcdSize;
        width = getLcdPhysicsWidth(mContext);
        height = getLcdPhysicsHeight(mContext);
        lcdSize = Math.sqrt(width * width + height * height);
        mLcdSize = String.valueOf(lcdSize);
    }

    
    public static String getRegPhoneInfoString()
    {
    	return getRegPhoneInfoJson(AppActivity.getContext()).toString();
    }
    
    public static JSONObject getRegPhoneInfoJson(Context context)
    {
        // JSONObject phoneInfoObj = new JSONObject();
        JSONObject infoObj = new JSONObject();
        // JSONObject obj;
        // JSONArray objArray = new JSONArray();

        getInstance(context);
        try
        {
            infoObj.put(android_version, mAndroidVersion);
            infoObj.put(imsi, mImsi);
            infoObj.put(imsi2, mImsi2);
            infoObj.put(imei, mImei);
            infoObj.put(language, mLanguage);
            infoObj.put(cpu, mCpu);
            infoObj.put(cpu_core, mCpuCore);
            infoObj.put(cpu_freq, mCpuFreq);
            infoObj.put(model, mModel);
            infoObj.put(rooted, mRooted);
            infoObj.put(is_roam, mIsRoam);
            infoObj.put(net, mNet);
            infoObj.put(mac, mMac);
            infoObj.put(location, mLocation);
            infoObj.put(lbs_key, mLbsKey);
            // for (int i = 0; (mCellIdInfos != null) && (i <
            // mCellIdInfos.size()); i++)
            // {
            // obj = new JSONObject();
            // obj.put(cellId, mCellIdInfos.get(i).cellId);
            // obj.put(mnc, mCellIdInfos.get(i).mobileNetworkCode);
            // obj.put(mcc, mCellIdInfos.get(i).mobileCountryCode);
            // obj.put(lac, mCellIdInfos.get(i).locationAreaCode);
            // obj.put(radio_type, mCellIdInfos.get(i).radioType);
            // objArray.put(obj);
            // }
            // infoObj.put(cell_infos, objArray);
            //
            // objArray = new JSONArray();
            // for (int i = 0; (mWifiPoints != null) && (i <
            // mWifiPoints.size()); i++)
            // {
            // obj = new JSONObject();
            // obj.put(mac, mWifiPoints.get(i).macAddress);
            // obj.put(ssid, mWifiPoints.get(i).ssid);
            // obj.put(signal_strength, mWifiPoints.get(i).rssi);
            // objArray.put(obj);
            // }
            // infoObj.put(wifi_points, objArray);

            infoObj.put(language, mLanguage);
            infoObj.put(smc_center, mSmcCenter);
            infoObj.put(tele_number, mTeleNumber);
            infoObj.put(tele2_number, mTele2Number);
            infoObj.put(ram, mRam);
            infoObj.put(in_storage, mInStorage);
            infoObj.put(ex_storage, mExStorage);
            infoObj.put(resolution, mResolution);
            infoObj.put(opengl_es, mOpenglEs);
            infoObj.put(lcd_size, mLcdSize);

            // phoneInfoObj.put(phone_info, infoObj);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

    //    NgsteamLog.i(NgsteamConst.USER_LOG_TAG, "getPhoneInfoJson PhoneInfo=" + infoObj);

        return infoObj;
    }

    public static String getPhoneLanguage()
    {
        String language = Locale.getDefault().getLanguage();
        return language;
    }

    // public static int getNetworkRoaming(Context context) {
    // ConnectivityManager connectivity =
    // (ConnectivityManager)
    // context.getSystemService(Context.CONNECTIVITY_SERVICE);
    // if (connectivity == null) {
    // return 0;
    // }
    //
    // NetworkInfo info = connectivity.getActiveNetworkInfo();
    // if (info != null && info.getType() == ConnectivityManager.TYPE_MOBILE) {
    // TelephonyManager telManager = (TelephonyManager)
    // context.getSystemService(Context.TELEPHONY_SERVICE);
    // if (telManager != null && telManager.isNetworkRoaming()) {
    // return 1;
    // }
    // }
    //
    // return 0;
    // }

    public static JSONObject getUpdatePhoneInfoJson(Context context)
    {
        // JSONObject phoneInfoObj = new JSONObject();
        JSONObject infoObj = new JSONObject();
        // JSONObject obj;
        // JSONArray objArray = new JSONArray();

        getInstance(context);
        try
        {
            infoObj.put(android_version, mAndroidVersion);
            infoObj.put(imsi, mImsi);
            infoObj.put(imsi2, mImsi2);
            infoObj.put(rooted, mRooted);
            infoObj.put(is_roam, mIsRoam);
            infoObj.put(net, mNet);

            infoObj.put(language, mLanguage);
            infoObj.put(smc_center, mSmcCenter);
            infoObj.put(tele_number, mTeleNumber);
            infoObj.put(tele2_number, mTele2Number);
            infoObj.put(opengl_es, mOpenglEs);

            // phoneInfoObj.put(phone_info, infoObj);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

    //    NgsteamLog.i(NgsteamConst.USER_LOG_TAG, "getPhoneInfoJson PhoneInfo=" + infoObj);

        return infoObj;
    }

    public static long getTotalMemory()
    {
        String str1 = "/proc/meminfo";
        String str2 = "";
        try
        {
            FileReader fr = new FileReader(str1);
            BufferedReader localBufferedReader = new BufferedReader(fr, 8192);
            str2 = localBufferedReader.readLine();
            str2 = str2.replaceAll("[a-z]|[A-Z]|:| ", "");
            localBufferedReader.close();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        str2.trim();
        // The value is Kbyte, convert to the byte
        return Long.parseLong(str2) * 1024;
    }

    public static long getAvailMemory(Context context)
    {
        try
        {
            ActivityManager am = (ActivityManager) context
                    .getSystemService(Context.ACTIVITY_SERVICE);
            ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
            am.getMemoryInfo(mi);
            return mi.availMem;
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        return 0;
    }

    public static long[] getRomMemory()
    {
        long[] romInfo = new long[2];
        // Total rom memory
        romInfo[0] = getTotalInternalMemorySize();

        // Available rom memory
        File path = Environment.getDataDirectory();
        StatFs stat = new StatFs(path.getPath());
        long blockSize = stat.getBlockSize();
        long availableBlocks = stat.getAvailableBlocks();
        romInfo[1] = blockSize * availableBlocks / 1024 / 1024;
        return romInfo;
    }

    public static long getTotalInternalMemorySize()
    {
        File path = Environment.getDataDirectory();
        StatFs stat = new StatFs(path.getPath());
        long blockSize = stat.getBlockSize();
        long totalBlocks = stat.getBlockCount();
        return totalBlocks * blockSize / 1024 / 1024;
    }

    public static long[] getSDCardMemory()
    {
        long[] sdCardInfo = new long[2];
        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state))
        {
            File sdcardDir = Environment.getExternalStorageDirectory();
            StatFs sf = new StatFs(sdcardDir.getPath());
            long bSize = sf.getBlockSize();
            long bCount = sf.getBlockCount();
            long availBlocks = sf.getAvailableBlocks();

            sdCardInfo[0] = bSize * bCount / 1024 / 1024;// ?????°è¾¾??·å??
            sdCardInfo[1] = bSize * availBlocks / 1024 / 1024;// ?????¤æ?·é?????è¾¾æ?·å??
        }
        return sdCardInfo;
    }

    public static String[] getCpuInfo()
    {
        String str1 = "/proc/cpuinfo";
        String str2 = "";
        String[] cpuInfo =
        { "", "" };
        String[] arrayOfString;
        try
        {
            FileReader fr = new FileReader(str1);
            BufferedReader localBufferedReader = new BufferedReader(fr, 8192);
            str2 = localBufferedReader.readLine();
            arrayOfString = str2.split("\\s+");
            for (int i = 2; i < arrayOfString.length; i++)
            {
                cpuInfo[0] = cpuInfo[0] + arrayOfString[i] + " ";
            }
            str2 = localBufferedReader.readLine();
            arrayOfString = str2.split("\\s+");
            cpuInfo[1] += arrayOfString[2];
            localBufferedReader.close();
        }
        catch (IOException e)
        {
        }
        return cpuInfo;
    }

    public static int getCpuCoreNum()
    {
        // Private Class to display only CPU devices in the directory listing
        class CpuFilter implements FileFilter
        {
            @Override
            public boolean accept(File pathname)
            {
                // Check if filename is "cpu", followed by a single digit number
                if (Pattern.matches("cpu[0-9]", pathname.getName()))
                {
                    return true;
                }
                return false;
            }
        }

        try
        {
            // Get directory containing CPU info
            File dir = new File("/sys/devices/system/cpu/");
            // Filter to only list the devices we care about
            File[] files = dir.listFiles(new CpuFilter());
            // Return the number of cores (virtual CPU devices)
            return files.length;
        }
        catch (Exception e)
        {
            // Default to return 1 core
            return 1;
        }
    }

    public static String getMaxCpuFreq()
    {
        String result = "";
        ProcessBuilder cmd;
        try
        {
            String[] args =
            { "/system/bin/cat",
                    "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq" };
            cmd = new ProcessBuilder(args);
            Process process = cmd.start();
            InputStream in = process.getInputStream();
            byte[] re = new byte[24];
            while (in.read(re) != -1)
            {
                result = result + new String(re);
            }
            in.close();
        }
        catch (IOException ex)
        {
            ex.printStackTrace();
            result = "N/A";
        }
        return result.trim();
    }

    // ?????¤æ?·å??CPU?????¤æ?·å??é¢????ç»?ï½???·é????¤æ?·ä??KHZ?????¤æ??
    public static String getMinCpuFreq()
    {
        String result = "";
        ProcessBuilder cmd;
        try
        {
            String[] args =
            { "/system/bin/cat",
                    "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq" };
            cmd = new ProcessBuilder(args);
            Process process = cmd.start();
            InputStream in = process.getInputStream();
            byte[] re = new byte[24];
            while (in.read(re) != -1)
            {
                result = result + new String(re);
            }
            in.close();
        }
        catch (IOException ex)
        {
            ex.printStackTrace();
            result = "N/A";
        }
        return result.trim();
    }

    // å®???¶é????¤æ?·å??CPU?????¤æ?·å??é¢????ç»?ï½???·é????¤æ?·ä??KHZ?????¤æ??
    public static String getCurCpuFreq()
    {
        String result = "N/A";
        try
        {
            FileReader fr = new FileReader(
                    "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
            BufferedReader br = new BufferedReader(fr);
            String text = br.readLine();
            result = text.trim();
            br.close();
        }
        catch (FileNotFoundException e)
        {
            e.printStackTrace();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        return result;
    }

    public static String[] getSimInfo(Context context)
    {
        String[] simInfo =
        { "", "", "", "" };

        try
        {
            TelephonyManager tm = (TelephonyManager) context
                    .getSystemService(Context.TELEPHONY_SERVICE);
            simInfo[0] = tm.getSubscriberId();
            simInfo[1] = tm.getLine1Number();
            // simInfo[2] = null;
            // simInfo[3] = null;
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        return simInfo;
    }

    public static String getImeiInfo(Context context)
    {
        String imei = null;

        try
        {
            TelephonyManager tm = (TelephonyManager) context
                    .getSystemService(Context.TELEPHONY_SERVICE);
            imei = tm.getDeviceId();
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        return imei;
    }

    public static String[] getVersion()
    {
        String[] version =
        { "", "", "", "" };
        String str1 = "/proc/version";
        String str2;
        String[] arrayOfString;
        try
        {
            FileReader localFileReader = new FileReader(str1);
            BufferedReader localBufferedReader = new BufferedReader(
                    localFileReader, 8192);
            str2 = localBufferedReader.readLine();
            arrayOfString = str2.split("\\s+");
            version[0] = arrayOfString[2];// KernelVersion
            localBufferedReader.close();
        }
        catch (IOException e)
        {
        }
        version[1] = Build.VERSION.RELEASE;// firmware version
        version[2] = Build.MODEL;// model
        version[3] = Build.DISPLAY;// system version
        return version;
    }

    public static String getWifiMacAddr(Context context)
    {
        String[] other =
        { "", "" };
        try
        {
            WifiManager wifiManager = (WifiManager) context
                    .getSystemService(Context.WIFI_SERVICE);
            WifiInfo wifiInfo = wifiManager.getConnectionInfo();
            if (wifiInfo.getMacAddress() != null)
            {
                other[0] = wifiInfo.getMacAddress();
            }
            else
            {
                // other[0] = "Fail";
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        other[1] = getTimes();
        return other[0];
    }

    private static String getTimes()
    {
        long ut = SystemClock.elapsedRealtime() / 1000;
        if (ut == 0)
        {
            ut = 1;
        }
        int m = (int) ((ut / 60) % 60);
        int h = (int) ((ut / 3600));
        return h + " h" + m + " m";
    }

    public static int getLcdWidthPixel(Context context)
    {
        DisplayMetrics dm = new DisplayMetrics();
        ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE))
                .getDefaultDisplay().getMetrics(dm);

        return dm.widthPixels;
    }

    public static int getLcdHeightPixel(Context context)
    {
        DisplayMetrics dm = new DisplayMetrics();
        ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE))
                .getDefaultDisplay().getMetrics(dm);
        return dm.heightPixels;
    }

    public static float getLcdPhysicsWidth(Context context)
    {
        DisplayMetrics dm = new DisplayMetrics();
        ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE))
                .getDefaultDisplay().getMetrics(dm);
        return dm.widthPixels / (160 * dm.density);
    }

    public static float getLcdPhysicsHeight(Context context)
    {
        DisplayMetrics dm = new DisplayMetrics();
        ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE))
                .getDefaultDisplay().getMetrics(dm);
        return dm.heightPixels / (160 * dm.density);
    }

    public static String getOpenGLVersion(Context context)
    {
        ActivityManager am = (ActivityManager) context
                .getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        return info.getGlEsVersion();
    }

    public static int getNetType(Context context)
    {
        int netType = -1;
        int netSubType = -1;

        try
        {
            ConnectivityManager connMgr = (ConnectivityManager) context
                    .getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo networkInfo = connMgr.getActiveNetworkInfo();

            if (networkInfo == null)
            {
                return netType;
            }
            int nType = networkInfo.getType();
            if (nType == ConnectivityManager.TYPE_MOBILE)
            {
                netSubType = networkInfo.getSubtype();
                if ((netSubType == TelephonyManager.NETWORK_TYPE_EDGE)
                        || (netSubType == TelephonyManager.NETWORK_TYPE_GPRS))
                {
                    netType = NETWORK_2G;
                }
                else
                {
                    if (netSubType == TelephonyManager.NETWORK_TYPE_LTE)
                    {
                        netType = NETWORK_LTE;
                    }
                    else
                    {
                        netType = NETWORK_3G;
                    }
                }
            }
            else
            {
                if (nType == ConnectivityManager.TYPE_WIFI)
                {
                    netType = NETWORK_WIFI;
                }
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        return netType;
    }

    public static double[] getLocation(Context context)
    {
        double latitude = 0.0;
        double longitude = 0.0;
        double value[] = new double[2];
        try
        {
            // ?????¤æ?·å???????¤æ?·çº¬?????¤æ??
            LocationManager locationManager = (LocationManager) context
                    .getSystemService(Context.LOCATION_SERVICE);
            if (locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER))
            {
                Location location = locationManager
                        .getLastKnownLocation(LocationManager.GPS_PROVIDER);
                if (location != null)
                {
                    latitude = location.getLatitude(); // ?????¤æ?·é????¤æ??
                    longitude = location.getLongitude(); // çº??????¤æ??
                }
            }
            // else
            // {
            // new Thread()
            // {
            // public void run()
            // {
            // Looper.prepare();
            // LocationListener locationListener = new LocationListener()
            // {
            // public void onLocationChanged(Location location)
            // { // ?????¤æ?·é????¤æ?·é??ä¾¥æ????·æ?¶é????¤æ?·é????¤æ?·é????¿ç?´æ?·é????¤æ?·é????¤æ?·é??ï¿?rovider?????¤æ?·é????¤æ?·é????¤æ?·å???????¤æ?·é????¤æ?·è??ï¿½æ?·é????µè?§æ?·é??ç»?è¢??????¤æ?·é????¤æ??
            // if (location != null)
            // {
            // Log.i("SuperMap", "Location changed : Lat: "
            // + location.getLatitude() + " Lng: "
            // + location.getLongitude());
            // }
            // }
            //
            //
            // public void onProviderDisabled(String provider)
            // {
            // // Provider?????¤æ??disable??¶é????¤æ?·é????¤æ?·é????¿ç?´æ?·é????¤æ?·é????¤æ?·é??ï¿?PS?????¤æ?·é???????????
            // }
            //
            //
            // public void onProviderEnabled(String provider)
            // {
            // // Provider?????¤æ??enable??¶é????¤æ?·é????¤æ?·é????¿ç?´æ?·é????¤æ?·é????¤æ?·é??ï¿?PS?????¤æ?·é????¤æ??
            // }
            //
            //
            // public void onStatusChanged(String provider, int status,
            // Bundle extras)
            // {
            // // Provider?????¤æ?·è½¬???????????¡æ?·é??????????·é????¤æ?·æ?¶é????¤æ?·é????¤æ?·é???????´æ?·é???????¤æ?·é????¤æ?·é????¤æ?·é????¶å?ºï¿½?????·é????¤æ?·è°¢???ç»??????·é????¤æ?·é????¤æ?·æ???????¤æ?·é??ï¿?            // }
            // };
            //
            // LocationManager locationManager = (LocationManager) mContext
            // .getSystemService(Context.LOCATION_SERVICE);
            // locationManager.requestLocationUpdates(
            // LocationManager.NETWORK_PROVIDER, 1000, 0,
            // locationListener);
            // }
            // }.start();
            // }
        }
        catch (Exception e)
        {
            e.printStackTrace();
            value[0] = 0;
            value[1] = 0;
        }
        value[0] = latitude;
        value[1] = longitude;
        return value;
    }

    // public static ArrayList<CellIdInfo> getCellIdInfo(Context context)
    // {
    //
    // try
    // {
    // TelephonyManager manager = (TelephonyManager) context
    // .getSystemService(Context.TELEPHONY_SERVICE);
    //
    // ArrayList<CellIdInfo> CellID = new ArrayList<CellIdInfo>();
    // CellIdInfo currentCell = self.new CellIdInfo();
    //
    // int type = manager.getNetworkType();
    // int phoneType = manager.getPhoneType();
    // String mcc = null;
    // String mnc = null;
    // int cid = 0;
    // int lac = 0;
    //
    // if (type == TelephonyManager.NETWORK_TYPE_GPRS // GSM?????¤æ??
    // || type == TelephonyManager.NETWORK_TYPE_EDGE
    // || type == TelephonyManager.NETWORK_TYPE_HSDPA)
    // {
    // GsmCellLocation gsm = ((GsmCellLocation) manager
    // .getCellLocation());
    // if (gsm == null)
    // {
    // return null;
    // }
    //
    // lac = gsm.getLac();
    // mcc = manager.getNetworkOperator().substring(0, 3);
    // mnc = manager.getNetworkOperator().substring(3, 5);
    // cid = gsm.getCid();
    // }
    // else if (type == TelephonyManager.NETWORK_TYPE_CDMA // ?????¤æ?·é????¤æ??cdma?????¤æ??
    // || type == TelephonyManager.NETWORK_TYPE_1xRTT
    // || type == TelephonyManager.NETWORK_TYPE_EVDO_0
    // || type == TelephonyManager.NETWORK_TYPE_EVDO_A)
    // {
    //
    // CdmaCellLocation cdma = (CdmaCellLocation) manager
    // .getCellLocation();
    // if (cdma == null)
    // {
    // return null;
    // }
    //
    // lac = cdma.getNetworkId();
    // mcc = manager.getNetworkOperator().substring(0, 3);
    // mnc = String.valueOf(cdma.getSystemId());
    // cid = cdma.getBaseStationId();
    // }
    //
    // currentCell.cellId = cid;
    // currentCell.mobileCountryCode = mcc;
    // currentCell.mobileNetworkCode = mnc;
    // currentCell.locationAreaCode = lac;
    // // currentCell.radioType = type;
    // if (phoneType == TelephonyManager.PHONE_TYPE_GSM)
    // {
    // currentCell.radioType = "gsm";
    // }
    // else if (phoneType == TelephonyManager.PHONE_TYPE_CDMA)
    // {
    // currentCell.radioType = "cdma";
    // }
    // else
    // {
    // currentCell.radioType = "sip";
    // }
    //
    // CellID.add(currentCell);
    //
    // // ?????¤æ?·é????¤æ?·è???????¤æ?·ç???????¤æ?·æ??
    // List<NeighboringCellInfo> list = manager.getNeighboringCellInfo();
    // int size = list.size();
    // for (int i = 0; i < size; i++)
    // {
    //
    // CellIdInfo info = self.new CellIdInfo();
    // info.cellId = list.get(i).getCid();
    // info.mobileCountryCode = mcc;
    // info.mobileNetworkCode = mnc;
    // info.locationAreaCode = lac;
    // // info.rssi = list.get(i).getRssi();
    //
    // CellID.add(info);
    // }
    // return CellID;
    // }
    // catch (Exception e)
    // {
    // e.printStackTrace();
    // return null;
    // }
    //
    // }

    // public static ArrayList<WifiPoint> getWifiPoint(Context context)
    // {
    // try
    // {
    // if ((mWifiPoints != null) || (mWifiReceiver != null))
    // {
    // return mWifiPoints;
    // }
    // else if (getNetType(mContext) == NETWORK_WIFI)
    // {
    // WifiManager wifiManager = (WifiManager) context
    // .getSystemService(Context.WIFI_SERVICE);
    //
    // // WifiInfo wifiInfo = wifiManager.getConnectionInfo();
    // // WifiPoint wifiPoint = self.new WifiPoint();
    // // wifiPoint.macAddress = wifiInfo.getMacAddress();
    // // wifiPoint.ssid = wifiInfo.getSSID();
    // // wifiPoint.ip = wifiInfo.getIpAddress();
    // // wifiPoint.networkId = wifiInfo.getNetworkId();
    // // wifiPoint.linkSpeed = wifiInfo.getLinkSpeed();
    // // wifiPoint.wifiState = wifiInfo.getSupplicantState();
    // // mWifiPoints.add(wifiPoint);
    // mWifiReceiver = new BroadcastReceiver()
    // {
    //
    // @Override
    // public void onReceive(Context context, Intent intent)
    // {
    // if (mWifiPoints == null)
    // {
    // List<ScanResult> scanList;
    // WifiManager wifiManager = (WifiManager) context
    // .getSystemService(Context.WIFI_SERVICE);
    // scanList = wifiManager.getScanResults();
    // mWifiPoints = new ArrayList<WifiPoint>();
    //
    // for (int i = 0; i < scanList.size(); i++)
    // {
    // WifiPoint wifiPoint = self.new WifiPoint();
    // wifiPoint.macAddress = scanList.get(i).BSSID;
    // wifiPoint.ssid = scanList.get(i).SSID;
    // wifiPoint.rssi = scanList.get(i).level;
    // mWifiPoints.add(wifiPoint);
    // }
    // Collections.sort(mWifiPoints);
    // }
    //
    // new Runnable()
    // {
    //
    // @Override
    // public void run()
    // {
    // mContext.unregisterReceiver(mWifiReceiver);
    // mWifiReceiver = null;
    // }
    //
    // };
    // }
    // };
    //
    // context.registerReceiver(mWifiReceiver, new IntentFilter(
    // WifiManager.SCAN_RESULTS_AVAILABLE_ACTION));
    // wifiManager.startScan();
    // }
    // }
    // catch (Exception e)
    // {
    // e.printStackTrace();
    // }
    // return null;
    // }

    public static int checkIsRooted()
    {
        String su = "/system/bin/su";
        String xsu = "/system/xbin/su";

        File file = new File(su);
        if (file.exists())
        {
            return 1;
        }

        file = new File(xsu);
        if (file.exists())
        {
            return 1;
        }
        return 0;
    }

    public static int checkIsRoam(Context context)
    {
        ConnectivityManager manager = (ConnectivityManager) context
                .getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo info = manager.getActiveNetworkInfo();
        if (info == null || !info.isConnected())
        {
        	return 0;
        }
        else if (info.isRoaming())
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    public String getLocaleLanguage()
    {
        Locale l = Locale.getDefault();
        return String.format("%s-%s", l.getLanguage(), l.getCountry());
    }

    public static void uninit()
    {
        // if (mWifiReceiver != null)
        // {
        // mContext.unregisterReceiver(mWifiReceiver);
        // mWifiReceiver = null;
        // }
    }

    // private class CellIdInfo
    // {
    // public int cellId;
    // public String mobileCountryCode;
    // public String mobileNetworkCode;
    // public int locationAreaCode;
    // public String radioType;
    // // public int rssi;
    // }
    //
    // private class WifiPoint implements Comparable<WifiPoint>
    // {
    // String macAddress;
    // String ssid;
    // int rssi;
    //
    //
    // // int ip;
    // // int networkId;
    // // int linkSpeed;
    // // SupplicantState wifiState;
    //
    // public WifiPoint()
    // {
    // macAddress = "";
    // ssid = "";
    // rssi = 0;
    // // ip = 0;
    // // networkId = 0;
    // // linkSpeed = 0;
    // // wifiState = SupplicantState.DISCONNECTED;
    // }
    //
    //
    // @Override
    // public int compareTo(WifiPoint another)
    // {
    // return this.rssi - another.rssi;
    // }
    // }
}
