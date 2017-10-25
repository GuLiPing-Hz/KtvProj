package assistant.view;

import tiange.ktv.assistant.R;
import android.R.integer;
import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

/**
 * @author 肖平平
 * @version 创建时间：2013-12-19 下午1:02:15
 * 类说明 用于显示抢榜后的消息提示
 */
public class RankMsgDialog extends Dialog{
	
	public ImageView btn_cancel;
	public TextView tv_tips;
	int typeId ;
	int layoutRes;
	Context context;
	public int flag = 0;
	
	public RankMsgDialog(Context context) {
		super(context);
		this.context = context;
	}
	
	public RankMsgDialog(Context context, int resLayout) {
		super(context);
		this.context = context;
		this.layoutRes = resLayout;
	}
	
	public RankMsgDialog(Context context, int theme, int resLayout, int typeId) {
		super(context, theme);
		this.context = context;
		this.layoutRes = resLayout;
		this.typeId = typeId;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		if(typeId == 1) {
			//显示成功的对话框
			this.setContentView(R.layout.dlg_rank_msg_detail_green);
			initView();
		} else  {
			//显示失败的对话框
			this.setContentView(R.layout.dlg_rank_msg_detail_red);
			initView();
		    if (typeId == 2 || typeId == 3) {
				tv_tips.setText("啊哦，被你好友抢先一步，这首歌已经被抢先上榜了");
		    } else if (typeId == 4) {
		    	tv_tips.setText("歌曲已被小伙伴超越，再来KTV挑战吧！");
		    }
		}
		setListener();
	}
	
	protected void initView() {
		btn_cancel = (ImageView) findViewById(R.id.btn_close);
		tv_tips = (TextView) findViewById(R.id.tv_tips);
	}
	
	protected void setListener() {
		btn_cancel.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				RankMsgDialog.this.dismiss();
			}
		});
	}
}
