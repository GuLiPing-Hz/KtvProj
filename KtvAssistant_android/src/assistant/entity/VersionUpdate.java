package assistant.entity;

import java.util.ArrayList;
import java.util.List;

public class VersionUpdate {

	public static final  String TAG_CODE = "VersionCode";
	public static final  String TAG_NAME = "VersionName";
	public static final  String TAG_URL = "ApkUrl";
	public static final  String TAG_DETAIL = "Detail";
	public static final  String TAG_FORCE ="ForceUpdate";
	
	public int versionCode = 0;
	public String versionName;
	public String apkUrl;
	public List<String> details = new ArrayList<String>();
	/*
	 * 1强制更新
	 * 0非强制更新
	 */
	public int forceUpdate = 0;
}
