package assistant.view;

import tiange.ktv.assistant.R;
import android.app.Activity;
import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AbsListView;
import android.widget.AbsListView.OnScrollListener;
import android.widget.LinearLayout;
import android.widget.ListAdapter;
import android.widget.ListView;

/**
 * @author shou_peng
 * @version 创建时间：2012-11-26 下午01:46:48 类说明:封装ListView拉到底自动加载下一页
 */
public class LoadMoreListView extends ListView implements OnScrollListener {
	//private static final String TAG = "LoadMoreListView";
	private AbsListView.OnScrollListener listener;
	private LinearLayout lLayout;
	// private ProgressBar bar;
	private LoadMoreCallBack callBack;
	private int count = 20;
	private int lastItem, totalCount;
	/**
	 * ListView中添加的不属于List的行数，在检查是否拉到最后一行有用。（宋）
	 * 例如：点歌台里的点歌列表在头部之下还添加了一行演唱本地歌曲，属于ListView的Content内容，但是不属于ListView里的List的Item。	
	 */
	private int exception = 0;
	
	public LoadMoreListView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		init(context);
	}

	public LoadMoreListView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		// TODO Auto-generated constructor stub
		init(context);
	}

	public LoadMoreListView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		init(context);
	}

	private void init(Context paramContext) {
		LayoutInflater inflater = ((LayoutInflater) paramContext
				.getSystemService(Activity.LAYOUT_INFLATER_SERVICE));
		this.lLayout = ((LinearLayout) inflater.inflate(
				R.layout.view_load_more, this, false));
		addFooterView(this.lLayout);
		this.lLayout.setVisibility(View.GONE);
		this.setFooterDividersEnabled(false);
		super.setOnScrollListener(this);
	}
	
	public void setExceptionItemsCount(int count){
		exception = count;
	}
	
	public int getExceptionItemsCount(){
		return exception;
	}

	@Override
	public void onScroll(AbsListView view, int firstVisibleItem,
			int visibleItemCount, int totalItemCount) {
		int i = getHeaderViewsCount() + getFooterViewsCount() + exception;
		lastItem = firstVisibleItem + visibleItemCount - i;
		
		if(totalCount != (totalItemCount-i)){
			totalCount = totalItemCount-i;
		}
		
		if( (lastItem != totalCount) && (lLayout.getVisibility() == View.VISIBLE)){
			lLayout.setVisibility(View.GONE);
		}

		if (listener != null) {
			listener.onScroll(view, firstVisibleItem, visibleItemCount,totalItemCount);
		}
	}

	@Override
	public void onScrollStateChanged(AbsListView view, int scrollState) {
		// TODO Auto-generated method stub
		//this.state = scrollState;
		if (this.listener != null) {
			this.listener.onScrollStateChanged(view, scrollState);
		}
		switch (scrollState) {
		case OnScrollListener.SCROLL_STATE_FLING:
			break;
		case OnScrollListener.SCROLL_STATE_IDLE:
			if(lastItem >0 && lastItem >= totalCount ){
				int i = lastItem % count;
				if(i == 0){
					lLayout.setVisibility(View.VISIBLE);
					if (callBack != null)
						callBack.loadMore();
				}
				else
				{
					if(getFooterViewsCount() != 0 ){
						//ShowLog.v(TAG, "删除页脚");
						removeFooterView(lLayout);
					}else{
						//ShowLog.v(TAG, "页脚已不存在");
					}
				}
			}
			break;
		case OnScrollListener.SCROLL_STATE_TOUCH_SCROLL:
			break;
		default:
			break;
		}
	}

	@Override
	public void setAdapter(ListAdapter adapter) {
		// TODO Auto-generated method stub
		super.setAdapter(adapter);
	}

	public void setOnScrollListener(AbsListView.OnScrollListener paramOnScrollListener) {
		this.listener = paramOnScrollListener;
	}

	public void setloadMoreListener(LoadMoreCallBack param) {
		this.callBack = param;
	}

	public void setStop() {
		this.lLayout.setVisibility(View.GONE);
	}
	
	public void removeFooter() {
		if(getFooterViewsCount() != 0 ){
			removeFooterView(lLayout);
		}
	}
	
	public void addFooter() {
		if(getFooterViewsCount() == 0 ){
			addFooterView(lLayout);
			this.lLayout.setVisibility(View.GONE);
			this.setFooterDividersEnabled(false);
		}
	}
	
	public void setCount(int parame) {
		this.count = parame;
	}

	public void isAddHead(boolean isAdd) {
	}
	
	public abstract interface LoadMoreCallBack {
		public abstract void loadMore();
	}

	public boolean isHasFoot(){
		if(getFooterViewsCount() != 0 ){
			return true;
		}else{
			return false;
		}
	}

}
