package assistant.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

import tiange.ktv.assistant.R;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import assistant.global.MyXml;
import assistant.task.AsyncUpdateAPK;

public class ApplicationUtil {
	private  Context cont;
	private int current_version_code;
	//private String current_version_name;
	
	public int m_nVersion = 2;
	public double m_nPackageSize = 1.2;
	public static String apkName = "KtvAssistant.apk";
	public static String apkMimeType = "application/vnd.android.package-archive";
	/**
	 * 
	 * @param con activity上下文
	 * @param VersionCode 应用版本号
	 * @param VersionName 应用版本名称
	 */
	public ApplicationUtil(Context con, int VersionCode, String VersionName){
		this.cont = con;
		this.current_version_code = VersionCode;
		//this.current_version_name = VersionName;
	}
	
	public ApplicationUtil(Context con, int VersionCode){
		this.cont = con;
		this.current_version_code = VersionCode;
	}
	
	/**
	 * 检查下载的文件是否完整有效，并检查更新
	 * 该函数在AboutPage和WblogMain中都有调用
	 * @param bManual 是否手动更新标志位 true是   false不是
	 * @return
	 */
    public boolean CheckConfigFile(boolean bManual){
    	MyXml ItemXml = new MyXml();
		try{
			boolean bLoad = false;
			
			try{
				bLoad = ItemXml.loadIS(cont.openFileInput("baseconfig.xml"),"UTF-8");
			}catch(Exception e){
				e.printStackTrace();
			}
			
			if(!bLoad)//先读取下载的文件，如读取失败则读取原始备份文件
			{
				ItemXml.loadIS(cont.getResources().openRawResource(R.raw.baseconfig),"UTF-8");				
			}
			
			if(!ItemXml.SelectNodeToList("//Config")){
				//System.out.println("SelectNodeToList 失败");
				return false;					
			}
			
			if(ItemXml.QueryNode(false) != null){
				//System.out.println("nVer = " + nVer);
				//System.out.println(ItemXml.GetValueByName("LowestVer"));
				
				String sLatestVer = ItemXml.GetValueByName("LatestVer");				
				String sLowestVer = ItemXml.GetValueByName("LowestVer");
				
				if(sLatestVer==null || sLowestVer == null){//读取失败
					//System.out.println("vgirl_baseconfig.xml读取失败");
					return false;
				}
				else{//读取成功
					//System.out.println("vgirl_baseconfig.xml读取成功");
					if(!bManual){
						CopyFile("baseconfig.xml","baseconfigbackup.xml");//备份文件
					}
					CheckUpdate(Integer.parseInt(sLatestVer), Integer.parseInt(sLowestVer), bManual);
					/*if(!bManual){
						String sAndroidMobilePhoneItemXmlVer = ItemXml.GetValueByName("ItemXMLVer");
						
						if(sAndroidMobilePhoneItemXmlVer == null){
							return false;
						}
						CheckAndroidMobilePhoneItemXmlUpdate(Integer.parseInt(sAndroidMobilePhoneItemXmlVer));
					}*/
					return true;
				}
			}else{
				//System.out.println("QueryNode 失败");
				return false;							
			}
		}catch(Exception e){
			//没有找到文件，或出现其他异常时
			//System.out.println("没有找到文件，或出现其他异常时");
			return false;
		}
    }//CheckConfigFile end
    
    /**
     * 
     * @param LatestVersion 配置文件中保存的服务器目前最新应用版本号
     * @param LowestVersion 配置文件中保存的服务器目前最低应用版本号
     * @param bManual 是否手动开启版本检查的标志位 true表示“是” flase表示“不是”
     */
    void CheckUpdate(int LatestVersion, int LowestVersion, boolean bManual){
    	if(bManual){
    		showUpdateDialog(LatestVersion,LowestVersion);
    	}else{
    		autoCompareVersion( LatestVersion, LowestVersion);
    	}
    	
    }
    
    /**
     * 手动触发更新版本提示框
     * @param LatestVersion 配置文件中保存的服务器目前最新应用版本号
     * @param LowestVersion 配置文件中保存的服务器目前最低应用版本号
     */
    private void showUpdateDialog(int LatestVersion, int LowestVersion){
    	try {
			if(LatestVersion <= current_version_code ){
				//当前版本是最新的
				Dialog undoDialog = new AlertDialog.Builder(cont)
				.setTitle(cont.getString(R.string.app_update_dialog_title))
				.setMessage(cont.getString(R.string.app_update_dialog_ok_message))
				.setPositiveButton(cont.getString(R.string.ok),new DialogInterface.OnClickListener() 
				{
					public void onClick(DialogInterface dialog, int whichButton)
					{
						dialog.dismiss();
					}
				})
				.create();
				undoDialog.show();
			}
			else{
				//有新版本存在，可以选择更新
				Dialog updateDialog = new AlertDialog.Builder(cont)
				.setTitle(cont.getString(R.string.app_update_dialog_title))
				.setMessage(R.string.app_update_dialog_new_found)	
				.setPositiveButton(cont.getString(R.string.app_update_dialog_update_button),new DialogInterface.OnClickListener() 
				{	
					//点击下载更新
					public void onClick(DialogInterface dialog, int whichButton)
					{	
						//判断是否有SD卡，有:下载APK  没有:建议去网站下载安装
						if (android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED)){
							
							Boolean result = isApkExist();
		            		if(result){
		            			SetupNewVersion(false);
		            			
		            		}else{
		            			showDownloadDialog();
		            		}
							
		            	}else{
		            		//无SD卡
		            		showWebUpdateDialog();
		            	}
					}
				})
				.setNegativeButton(cont.getString(R.string.cancel), new DialogInterface.OnClickListener(){

					@Override
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub
						//取消
						dialog.dismiss();
					}
					
				})
				.create();
				updateDialog.show();
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			
			Dialog errorDialog = new AlertDialog.Builder(cont)
			.setTitle(cont.getString(R.string.app_update_dialog_title))
			.setMessage(cont.getString(R.string.app_update_dialog_mistake_message) )
			.setPositiveButton(cont.getString(R.string.ok),new DialogInterface.OnClickListener() 
			{
				public void onClick(DialogInterface dialog, int whichButton){
					dialog.dismiss();
				}
			})
			.create();
			errorDialog.show();
		}
    }
    
    /**
     * 用户登录后，后台自动搜索新版本信息
     * @param LatestVersion 配置文件中保存的服务器目前最新应用版本号
     * @param LowestVersion 配置文件中保存的服务器目前最低应用版本号
     */
    private void autoCompareVersion(int LatestVersion, int LowestVersion){
    	if(LowestVersion > current_version_code){
    		//System.out.println("LowestVersion > current_version_code");
    		//当前版本过低
    		if (android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED)){
        		//有SD卡
//        		String sServerAPKMd5 = null;
//        		String sLocalAPKMd5 = null;
//        		boolean bEqual = false;
//        		try
//        		{
//        			sServerAPKMd5 = getConfig(cont, "APKMD5");
//        			sLocalAPKMd5 = NetUtil.getHash(android.os.Environment.getExternalStorageDirectory()+"/"+ApkName, "MD5");
//        			//System.out.println("sServerAPKMd5 = " + sServerAPKMd5 +"sLocalAPKMd5= " + sLocalAPKMd5);
//            		bEqual = sServerAPKMd5.toLowerCase().equals(sLocalAPKMd5.toLowerCase());
//        		}
//        		catch(Exception e)
//        		{
//        			e.printStackTrace();
//        		}
    			Boolean result = isApkExist();
        		if(result){	
        			SetupNewVersion(true);
        	    }else{
        			new AsyncUpdateAPK(cont).execute();
        		}
        	}else{
        		//无SD卡
        		showWebUpdateDialog();
        	}
    	}else if(LatestVersion > current_version_code){
    		//有新版本
    		//System.out.println("有新版本");
    		if (android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED)){
        		//有SD卡
//        		String sServerAPKMd5 = null;
//        		String sLocalAPKMd5 = null;
//        		boolean bEqual = false;
//        		try
//        		{
//        			sServerAPKMd5 = getConfig(cont, "APKMD5");
//        			sLocalAPKMd5 = NetUtil.getHash(android.os.Environment.getExternalStorageDirectory()+"/"+ApkName, "MD5");
//        			//System.out.println("sServerAPKMd5 = " + sServerAPKMd5 +"sLocalAPKMd5= " + sLocalAPKMd5);
//            		bEqual = sServerAPKMd5.toLowerCase().equals(sLocalAPKMd5.toLowerCase());
//        		}
//        		catch(Exception e)
//        		{
//        			e.printStackTrace();
//        		}
    			Boolean result = isApkExist();
        		if(result){	
        			SetupNewVersion(false);
        	    }else{
        			new AsyncUpdateAPK(cont).execute();
        		}
        	}else{
        		//无SD卡
        		showWebUpdateDialog();
        	}   		
    	}   	            	   	
    }
    
    /**
     * 网页更新APK提示框
     */
    private void showWebUpdateDialog(){
    	new AlertDialog.Builder(cont)
   		.setTitle(cont.getString(R.string.app_update_dialog_title))
   		.setMessage("新浪手机KTV有了新版本，请到官方网站下载新版本")
   		.setPositiveButton(cont.getString(R.string.ok), new DialogInterface.OnClickListener() 
   		{
   			public void onClick(DialogInterface dialog, int whichButton) 
   			{
   				dialog.dismiss();
   			}
   		})
   		.show();	
    }
    
    /**
     * 安装新版本程序
     * @param bForce 是否强制安装
     */
    public void SetupNewVersion(final boolean bForce) {   
    	
    	String sTitle = cont.getString(R.string.app_update_dialog_title);
    	String sMessage = "";
    	String sCancel = "";
    	//final boolean bFinalForce = bForce;
    	if(bForce){
    		sMessage = "您的新浪手机KTV版本已过时，需要更新版本后才能继续使用，是否现在更新！";
    		sCancel = "退出";
    		
    	}else{
    		sMessage = "新浪手机KTV新版本已下载完成，是否现在更新!";
    		sCancel = "取消";
    	}
    	
    	//String sMessage = "VGirl新版本已下载完成，是否现在更新";
		Dialog dialog = new AlertDialog.Builder(cont)
		.setTitle(sTitle)//设置标题
		.setMessage(sMessage)//设置内容
		.setPositiveButton("安装",new DialogInterface.OnClickListener(){
			
			public void onClick(DialogInterface dialog, int whichButton)
			{
				//安装
				Intent intent = new Intent();   
				intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);   
				intent.setAction(android.content.Intent.ACTION_VIEW);   
				try{
					intent.setDataAndType(Uri.fromFile(new File(android.os.Environment.getExternalStorageDirectory(),apkName)),apkMimeType);
				}catch(Exception e){
					e.printStackTrace();
				}
				cont.startActivity(intent);   
			}
		})
		.setNeutralButton(sCancel, new DialogInterface.OnClickListener(){
			
			public void onClick(DialogInterface dialog, int whichButton){
				if( bForce){
					ToolUtil.exitApp(cont);
					System.exit(0);
				}else{
					dialog.dismiss();
				}
				
			}
		}).create();//创建按钮
		dialog.show();
	}     
    
    /**
     * 下载APK提示框
     */
    void showDownloadDialog(){
    	Dialog dialog = new AlertDialog.Builder(cont)
		.setTitle(cont.getString(R.string.app_update_dialog_title))//设置标题
		.setMessage("正在为您下载最新安装包，您可以关闭本窗口，下载完会提示您安装！")//设置内容
		.setPositiveButton("确定",//设置确定按钮
		new DialogInterface.OnClickListener() 
		{
			public void onClick(DialogInterface dialog, int whichButton)
			{
				dialog.dismiss();
			}
		}).create();//创建按钮
		// 显示对话框
		dialog.show();
		new AsyncUpdateAPK(cont).execute();
    }
    
    /**
     * 解析baseconfig.xml文件
     * @param context
     * @param sTag
     * @return
     */
    private String getConfig(Context context, String sTag)
	{
		MyXml ItemXml = new MyXml();
		boolean bLoad = false;
		try{
			try
			{
				bLoad = ItemXml.loadIS(context.openFileInput("baseconfig.xml"),"UTF-8");
			}
			catch(Exception e)
			{
				e.printStackTrace();
			}
			if(!bLoad)
			{
				try{
					bLoad = ItemXml.loadIS(context.openFileInput("baseconfigbackup.xml"),"UTF-8");
				}
				catch(Exception e)
				{
					e.printStackTrace();
				}
			}
			if(!bLoad)
			{
				try{
					bLoad = ItemXml.loadIS(context.getResources().openRawResource(R.raw.baseconfig),"UTF-8");
				}
				catch(Exception e)
				{
					e.printStackTrace();
				}
			}
			if(!bLoad)
			{
				//System.out.println("LoadXml 失败");
			}
			if(!ItemXml.SelectNodeToList("//Config")){
				//System.out.println("SelectNodeToList 失败");
				return null;					
			}
			ItemXml.QueryNode(true);
			if(ItemXml.QueryNode(false) != null)
			{
				//System.out.println(ItemXml.GetValueByName(sTag));
				return (ItemXml.GetValueByName(sTag));				
			}
			else
			{
				//System.out.println("QueryNode 失败");
				return null;							
			}
		}
		catch(Exception e)
		{//没有找到文件，或出现其他异常时
			e.printStackTrace();
			//System.out.println("没有找到文件，或出现其他异常时");
			return null;
		}
	}
    
    
    /**
     * 备份文件
     * @param sSourceFile 需要备份的文件名
     * @param sDestFile	备份后的文件名
     * @return
     */
    private boolean CopyFile(String sSourceFile, String sDestFile)
	{
		try {
			FileInputStream fis = cont.openFileInput(sSourceFile);
			FileOutputStream fos = cont.openFileOutput(sDestFile, Context.MODE_WORLD_WRITEABLE);
	        int readLen = 0;   
	        byte[] buf = new byte[1024];   
	        while ((readLen = fis.read(buf)) != -1) {   
	            fos.write(buf, 0, readLen);   
	        }   
	        fos.flush();   
	        fos.close();   
	        fis.close(); 
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
    
    /**
     * 检查APK文件是否已存在
     * @return
     */
    private Boolean isApkExist(){
    	String sServerAPKMd5 = null;
		String sLocalAPKMd5 = null;
		boolean bEqual = false;
		try
		{
			sServerAPKMd5 = getConfig(cont, "APKMD5");
			sLocalAPKMd5 = ToolUtil.getHash(android.os.Environment.getExternalStorageDirectory()+"/"+apkName, "MD5");
			//System.out.println("sServerAPKMd5 = " + sServerAPKMd5 +"sLocalAPKMd5= " + sLocalAPKMd5);
    		bEqual = sServerAPKMd5.toLowerCase().equals(sLocalAPKMd5.toLowerCase());
		}catch(Exception e){
			e.printStackTrace();
		}	      
    	return bEqual;
    }
}
