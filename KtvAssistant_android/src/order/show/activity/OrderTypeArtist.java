package order.show.activity;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import order.adapter.HomeAdapter;
import order.adapter.SingerAdapter;
import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.TextView;
import assistant.entity.TopicInfo;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.global.RequestCodeDef;
import assistant.task.songapi.GetSongTopicsTask;
import assistant.util.ShowLog;
import assistant.view.LetterListView;
import assistant.view.LetterListView.OnTouchingLetterChangedListener;

/**
 * @author 宋静
 * @version 创建时间：2013-10-29 下午6:12:36 类说明 歌星列表
 */
public class OrderTypeArtist extends OrderActivity {

	private LetterListView letterListView;
	private TextView tv_overlay;
	private View vOverlay;
	private HashMap<String, Integer> alphaIndexer;// 存放存在的汉语拼音首字母和与之对应的列表位置
	private String[] sections;// 存放存在的汉语拼音首字母
	private Runnable overlayThread = new Runnable(){

		@Override
		public void run() {
			// TODO Auto-generated method stub
			if(vOverlay != null)
				vOverlay.setVisibility(View.GONE);
		}
		
	};
	//private ImageView iv_data_fail;
	
	/*搜索相关*/
	String searchText;

	SingerAdapter adapter;
	List<TopicInfo> singers = new ArrayList<TopicInfo>();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.ac_singers);
		init();
		onRegisterNet();
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		onUnregisterNet();
		super.onDestroy();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		
		updateSongNumber();
	}

	private void init() {
		initPreData();
		findViewList();
		setView();
		setListener();
		initData();
	}

	private void initPreData() {
		Bundle b = this.getIntent().getExtras();
		if (b != null) {
			type = b.getInt(OrderActivity.BUNDLE_MODELID);
			topic = (TopicInfo) b.getSerializable(OrderActivity.BUNDLE_CURTOPIC);
		}
	}

	@Override
	protected void findViewList() {
		// TODO Auto-generated method stub
		super.findViewList();

		letterListView = (LetterListView) findViewById(R.id.lv_letters);
		vOverlay = findViewById(R.id.ly_overlay);
		tv_overlay = (TextView)vOverlay.findViewById(R.id.tv_overlay);
	}

	@Override
	protected void setView() {
		super.setView();
		if (topic != null && !TextUtils.isEmpty(topic.topicTitle))
			tv_title.setText(topic.topicTitle);

		ly_search.setVisibility(View.GONE);
		letterListView.setVisibility(View.GONE);
	}

	@Override
	protected void setListener() {
		super.setListener();

		lv_menu.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
					long arg3) {
				entrySongList(arg2);
			}
		});

		letterListView.setOnTouchingLetterChangedListener(new LetterListViewListener());
		
	}

	protected void initData() {
		if(checkNetWork() && topic != null){
			mLoadingAnim.setVisibility(View.VISIBLE);
			
			//歌星一次性获取全，目前歌库上有1w6千多个歌星
			GetSongTopicsTask task = new GetSongTopicsTask(handler
					,HomeAdapter.ModelType.SINGERS.getType(),topic.topicId,1,20000);
			task.execute();
		}
		else
			showEmptyTips(true);
	}

	private void entrySongList(int position) {
		Intent i = new Intent();
		i.setClass(this, OrderTypeSongs.class);
		i.putExtra(OrderActivity.BUNDLE_MODELID, type);
		i.putExtra(OrderActivity.BUNDLE_CURTOPIC, singers.get(position));
		i.putExtra(OrderActivity.BUNDLE_PRETOPIC, topic);
		startActivityForResult(i,RequestCodeDef.REQUEST_ROOM_STATUS);
	}

	@SuppressLint("HandlerLeak")
	private Handler handler = new Handler() {

		@SuppressWarnings("unchecked")
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			try {
				switch (msg.what) {
				case MessageDef.WM_GET_SONG_TOPICS:
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						final List<TopicInfo> list = (List<TopicInfo>) msg.obj;
						new Thread(){

							@Override
							public void run() {
								// TODO Auto-generated method stub
								super.run();
								showSingers(list);
								
								handler.post(new Runnable(){

									@Override
									public void run() {
										// TODO Auto-generated method stub
										mLoadingAnim.setVisibility(View.GONE);
										if (adapter == null) 
										{
											adapter = new SingerAdapter(OrderTypeArtist.this, singers);
											lv_menu.setAdapter(adapter);
										}
										else
											adapter.notifyDataSetChanged();
										
										alphaIndexer = adapter.getAlphaIndexer();
										sections = adapter.getSections();
										letterListView.setVisibility(View.VISIBLE);
									}
									
								});
							}
							
						}.start();
						
					}
					else
					{
						mLoadingAnim.setVisibility(View.GONE);
						ShowLog.e(TAG, "handler error：" + msg.what + "," + msg.obj);
						showEmptyTips(true,(String)msg.obj);
					}
					break;
				}
			} catch (Exception e) {
				e.printStackTrace();
			}

		}
	};
	
	private void showSingers(List<TopicInfo> list) {
		if (!checkSingersList(list)) {
			return;
		}
		
		quickSort(list, 0 , list.size() - 1);
		
		singers.clear();
		singers.addAll(list);
		
	}

	@Override
	protected void updateSongListInSubClass() {
		// TODO Auto-generated method stub
		
	}

	protected boolean checkSingersList(List<TopicInfo> list) {
		if (list == null || list.size() <= 0) {
			showEmptyTips(true);
			return false;
		}

		// 排除错误单元
		for (int i = 0; i < list.size(); i++) {
			TopicInfo a = list.get(i);
			if (a == null || a.topicId <= 0 || TextUtils.isEmpty(a.topicTitle)) {
				list.remove(a);
				i--;
			}
		}

		if (list == null || list.size() <= 0) {
			showEmptyTips(true);
			return false;
		}

		showEmptyTips(false);
		return true;
	}

	/** 右边字母的点击监听类 */
	private class LetterListViewListener implements
			OnTouchingLetterChangedListener {

		@Override
		public void onTouchingLetterChanged(final String s) {
			if (alphaIndexer.get(s) != null) {
				int position = alphaIndexer.get(s);
				lv_menu.setSelection(position);
				vOverlay.setVisibility(View.VISIBLE);
				tv_overlay.setText(sections[position]);
				handler.removeCallbacks(overlayThread);
				// 延迟执行，让overlay为不可见
				handler.postDelayed(overlayThread, 1500);
			}
		}

	}

	/** 快速排序 */
    public void quickSort(List<TopicInfo> list,int low,int high) {
        // 枢纽元，一般以第一个元素为基准进行划分
        int i = low;
        int j = high;
        if (low < high) {
        	 // 从数组两端交替地向中间扫描
        	TopicInfo pivotKey = list.get(low);
            // 进行扫描的指针i,j;i从左边开始，j从右边开始
            while (i < j) {
                while (i < j && list.get(j).pinyin.compareToIgnoreCase(pivotKey.pinyin) > 0) {
                    j--;
                }// end while
                if (i < j) {
                    // 比枢纽元素小的移动到左边
                	list.set(i, list.get(j));
                    i++;
                }// end if
                while (i < j && list.get(i).pinyin.compareToIgnoreCase(pivotKey.pinyin) < 0) {
                    i++;
                }// end while
                if (i < j) {
                    // 比枢纽元素大的移动到右边
                	list.set(j, list.get(i));
                    j--;
                }// end if
            }// end while
            // 枢纽元素移动到正确位置
            list.set(i, pivotKey);
            // 前半个子表递归排序
            quickSort(list,low,i - 1);
            // 后半个子表递归排序
            quickSort(list,i + 1,high);
        }// end if
    }// end sort

	@Override
	public void onAnimaEnd() {
		// TODO Auto-generated method stub
	}
}
