package assistant.util;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import android.text.TextUtils;
import assistant.entity.VersionUpdate;
import assistant.global.KtvAssistantAPIConfig;

import com.tiange.phttprequest.PHttpRequest;

public class VersionControl {
	
	public VersionControl() {
		// TODO Auto-generated constructor stub
	}

	public VersionUpdate checkApkVersion() {
		
		PHttpRequest request = PHttpRequest.requestWithURL(KtvAssistantAPIConfig.APK_CHECK_VERSION_URL);
		String result = request.startSyncRequestString();
		if(result == null)
			return null;
		
		try {
			InputStream is;
			is = new ByteArrayInputStream(result.getBytes("UTF-8"));
			return parserXmlByPull(is);
			
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	
	VersionUpdate parserXmlByPull(InputStream inputStream) throws Exception
    {
		VersionUpdate vp = new VersionUpdate();
		String curTag = null;
        
        //    创建XmlPullParserFactory解析工厂
        XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
        //    通过XmlPullParserFactory工厂类实例化一个XmlPullParser解析类
        XmlPullParser parser = factory.newPullParser();
        //    根据指定的编码来解析xml文档
        parser.setInput(inputStream, "utf-8");
        
        //    得到当前的事件类型
        int eventType = parser.getEventType();
        //    只要没有解析到xml的文档结束，就一直解析
        while(eventType != XmlPullParser.END_DOCUMENT)
        {
            switch (eventType)
            {
                //解析到文档开始的时候
                case XmlPullParser.START_DOCUMENT:
                	break;
                //解析到xml标签的时候
                case XmlPullParser.START_TAG:
                	curTag = parser.getName();
                     break;
                //解析字符串
                case XmlPullParser.TEXT:
                	if(TextUtils.equals(curTag,VersionUpdate.TAG_CODE))
                   	 	vp.versionCode = Integer.parseInt(parser.getText());
                	else if(TextUtils.equals(curTag, VersionUpdate.TAG_FORCE))
                    	vp.forceUpdate = Integer.parseInt(parser.getText());
                    else if(TextUtils.equals(curTag,VersionUpdate.TAG_NAME))
                    	vp.versionName = parser.getText();
                    else if(TextUtils.equals(curTag,VersionUpdate.TAG_URL))
                    	vp.apkUrl = parser.getText();
                    else if(TextUtils.equals(curTag, VersionUpdate.TAG_DETAIL))
                    {
                    	String detail = parser.getText();
                    	if(detail != null && !TextUtils.isEmpty(detail))
                    		vp.details.add(detail);
                    }
                    break;
                //解析到xml标签结束的时候
                case XmlPullParser.END_TAG:
                	curTag = "";
                	break;
                case XmlPullParser.END_DOCUMENT:
                	break;
            }
            //    通过next()方法触发下一个事件
            eventType = parser.next();
        }
        
        return vp;
    }
}
