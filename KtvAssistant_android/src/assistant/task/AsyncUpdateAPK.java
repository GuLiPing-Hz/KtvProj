package assistant.task;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import android.content.Context;
import android.os.AsyncTask;
import assistant.global.MyXml;
import assistant.util.ApplicationUtil;

/**
 * 下载新版APK包
 * @author shou
 *
 */
public class AsyncUpdateAPK extends AsyncTask<Void, Void, Boolean> {
	private Context cont;
	
	public AsyncUpdateAPK(Context c){
		this.cont = c;
	}
	
	@Override
	protected void onPreExecute() {
		// TODO Auto-generated method stub
		super.onPreExecute();
	}

	@Override
	protected Boolean doInBackground(Void... params) {
		// TODO Auto-generated method stub
		String sValue = null;
		MyXml ItemXml = new MyXml();
		try{
			if(!ItemXml.loadIS(cont.openFileInput("baseconfig.xml"),"UTF-8")){
				//System.out.println("loadXML 失败");
				return false;
			}
			if(!ItemXml.SelectNodeToList("//Config")){
				//System.out.println("SelectNodeToList 失败");
				return false;					
			}
			if(ItemXml.QueryNode(false) != null){
				//System.out.println(ItemXml.GetValueByName("UpdateURL"));
				sValue = ItemXml.GetValueByName("UpdateURL");				
				if(sValue==null)
				{//读取失败
					//System.out.println("读取失败");
					return false;
				}
			}else{
				//System.out.println("QueryNode 失败");
				return false;							
			}
		}catch(Exception e){//没有找到文件，或出现其他异常时
			//System.out.println("没有找到文件，或出现其他异常时");
			return false;
		}
		
		final String sAPKUrl = sValue;
		
		HttpClient client = new DefaultHttpClient();     
        HttpGet get = new HttpGet(sAPKUrl);     
        HttpResponse response;     
        try {     
            response = client.execute(get);     
            HttpEntity entity = response.getEntity();     
            long length = entity.getContentLength();
            //System.out.println("Length = "+length);
            InputStream is = entity.getContent();     
            FileOutputStream fileOutputStream = null;     
            if (is != null) {     
            	File saveFile = new File(android.os.Environment.getExternalStorageDirectory(), ApplicationUtil.apkName);
            	fileOutputStream = new FileOutputStream(saveFile);
                byte[] buf = new byte[1024];     
                int ch = -1;     
                   
                while ((ch = is.read(buf)) != -1) {   
                	//System.out.println("apk下载中...");
                    fileOutputStream.write(buf, 0, ch);     
                    //Thread.sleep(1000);
                    if (length > 0) {     
                    }     
                }               
            }     
            fileOutputStream.flush();     
            if (fileOutputStream != null) {     
                fileOutputStream.close();     
            }     
            //System.out.println("apk下载完成");
            return true;
        } catch (ClientProtocolException e) {     
            e.printStackTrace();
            return false;
        } catch (Exception e) {     
            e.printStackTrace();
            return false;
        }     
	}

	@Override
	protected void onPostExecute(Boolean result) {
		// TODO Auto-generated method stub
		super.onPostExecute(result);
//		if(result){
//			ApplicationUtil.SetupNewVersion(false);
//		}
	}	
}