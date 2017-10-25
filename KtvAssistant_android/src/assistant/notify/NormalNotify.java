package assistant.notify;

import java.text.SimpleDateFormat;
import java.util.Date;

import tiange.ktv.assistant.R;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.util.Log;
import android.widget.RemoteViews;
import assistant.activity.CommonBrowserActivity;
import assistant.global.AppStatus;
import assistant.util.ToolUtil;

public class NormalNotify {

	Context m_context;
	
    PendingIntent mPendingIntent = null;
	NotificationManager m_notificationManager = null;
	
	int m_notificationId = R.id.normal_notify;
	public static final String Tag = "NormalNotify";
	
	public NormalNotify(Context context) {
		// TODO Auto-generated constructor stub
		m_context = context;
		m_notificationManager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
	}

	//msgs[0]是组织消息 msgs
	public void showNotify(int msgId,String msg){
		m_notificationId = msgId;
		String[] params = msg.split("\\|");
		if(params.length < 4)
		{
			Log.e(Tag, "notify message["+msg+"] is incorrect");
			return;
		}
		
		String title = params[0];
		String content = params[1];
		String url = params[2];
		//String area = params[3];
		 // Set the icon, scrolling text and timestamp 自定义RemoteView
		Notification notification = new Notification();
        notification.contentView = new RemoteViews(m_context.getPackageName()
        		,R.layout.notification_normal);

        // The PendingIntent to launch our activity if the user selects this notification
        Intent intent = new Intent(m_context, CommonBrowserActivity.class);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.putExtra(CommonBrowserActivity.EXTRA_TITLE, title);
        intent.putExtra(CommonBrowserActivity.EXTRA_URL, url);

        mPendingIntent = PendingIntent.getActivity(m_context, 1,intent, PendingIntent.FLAG_ONE_SHOT);

        notification.icon = R.drawable.icon_notify;
        notification.tickerText = title;
        notification.contentIntent = mPendingIntent;
        notification.contentView.setTextViewText(R.id.notify_title, title);
        //添加默认震动，默认闪光
        notification.ledARGB = Color.BLUE;
        notification.ledOffMS = 1000;
        notification.ledOnMS = 500;
        notification.defaults = Notification.DEFAULT_VIBRATE;
        notification.flags = Notification.FLAG_SHOW_LIGHTS|Notification.FLAG_AUTO_CANCEL;

		String timeDisplay = ToolUtil.getStringDate("HH:mm");
        notification.contentView.setTextViewText(R.id.notify_time, timeDisplay);
        notification.contentView.setTextViewText(R.id.notify_content, content);
        m_notificationManager.cancel(m_notificationId);//取消原来的通知
        
        // Set the info for the views that show in the notification panel.
        //mNotification.setLatestEventInfo(this, getText(R.string.local_service_label),text, contentIntent);
        m_notificationManager.notify(m_notificationId, notification);
	}
}
