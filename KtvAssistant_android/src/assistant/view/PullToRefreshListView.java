package assistant.view;

import java.util.Date;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.LinearInterpolator;
import android.view.animation.RotateAnimation;
import android.widget.AbsListView;
import android.widget.AbsListView.OnScrollListener;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import assistant.util.ShowLog;

/**
 * @author 宋静
 * @version 创建时间：2013-11-1 下午2:13:30
 * 类说明
 */
public class PullToRefreshListView  extends ListView implements OnScrollListener {
	private final static int RELEASE_TO_REFRESH = 0;
	private final static int PULL_TO_REFRESH = 1;
	private final static int REFRESHING = 2;
	private final static int DONE = 3;
	
	private LayoutInflater inflater;

	private LinearLayout headView;
	private LinearLayout footer;
	
	private TextView tipsTextview;
	private TextView lastUpdatedTextView;
	private ImageView arrowImageView;
	private ProgressBar progressBar;
	private Context myContext;

	private RotateAnimation animation;
	private RotateAnimation reverseAnimation;

	// 用于保证startY的值在一个完整的touch事件中只被记录一次
	private boolean isRecored;

	private int headContentWidth;
	private int headContentHeight;

	private int startHeadY = 0;
	private int startFootY = 0;
	private int firstItemIndex;

	private int state;

	private boolean isBack;
	private boolean isListNone = false;
	private boolean listenFootRefresh = false;
	private boolean isLastRow = false;
	private boolean isHeadRefreshable = true;

	public OnRefreshListener refreshListener;

	private final static String TAG = "PULL_TO_FRESH";
	
	public PullToRefreshListView(Context context, AttributeSet attrs, boolean isManualRefreshFoot) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		myContext = context;
		listenFootRefresh = isManualRefreshFoot;
		init();		
		stopLoadMore();
	}
	
	public PullToRefreshListView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		myContext = context;
		init();
		stopLoadMore();
	}

	private void init(){
		inflater = LayoutInflater.from(myContext);
		
		headView = (LinearLayout) inflater.inflate(R.layout.view_pullrefresh_header, null);
		arrowImageView = (ImageView) headView.findViewById(R.id.pullrefresh_head_arrow);
		arrowImageView.setMinimumWidth(30);
		arrowImageView.setMinimumHeight(30);
		progressBar = (ProgressBar) headView.findViewById(R.id.pullrefresh_head_progressBar);
		tipsTextview = (TextView) headView.findViewById(R.id.pullrefresh_head_tips);
		lastUpdatedTextView = (TextView) headView.findViewById(R.id.pullrefresh_head_lastUpdated);
		
		measureView(headView);
		headContentHeight = headView.getMeasuredHeight();
		headContentWidth = headView.getMeasuredWidth();
		
		headView.setPadding(10, -1 * headContentHeight, 10, 5);
		headView.invalidate();

		//Log.v(TAG, "width:" + headContentWidth + " height:"+ headContentHeight);

		addHeaderView(headView);
		setOnScrollListener(this);

		animation = new RotateAnimation(0, -180,
				RotateAnimation.RELATIVE_TO_SELF, 0.5f,
				RotateAnimation.RELATIVE_TO_SELF, 0.5f);
		animation.setInterpolator(new LinearInterpolator());
		animation.setDuration(200);
		animation.setFillAfter(true);

		reverseAnimation = new RotateAnimation(-180, 0,
				RotateAnimation.RELATIVE_TO_SELF, 0.5f,
				RotateAnimation.RELATIVE_TO_SELF, 0.5f);
		reverseAnimation.setInterpolator(new LinearInterpolator());
		reverseAnimation.setDuration(200);
		reverseAnimation.setFillAfter(true);		

		state = DONE;
    	changeHeaderViewByState();
    	
    	footer = (LinearLayout) inflater.inflate(R.layout.view_load_more, null);
        addFooterView(footer);
	}
	
	@Override
	public void onScroll(AbsListView view, int firstVisibleItem,
			int visibleItemCount, int totalItemCount) {
		// TODO Auto-generated method stub
		firstItemIndex = firstVisibleItem;
		if (firstVisibleItem + visibleItemCount == totalItemCount && totalItemCount > 0) {    
            isLastRow = true;    
        }
	}

	@Override
	public void onScrollStateChanged(AbsListView view, int scrollState) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		switch(event.getAction())
		{
			case MotionEvent.ACTION_DOWN:
				if (firstItemIndex == 0 && !isRecored) {
					startHeadY = (int) event.getY();					
					ShowLog.v(TAG, "down，记录当前位置:"+startHeadY);
				}
				if(listenFootRefresh && isLastRow && !isRecored)
					startFootY = (int) event.getY();
				break;
			   
			case MotionEvent.ACTION_UP:
				if(isRecored)
				{
					if (state != REFRESHING) {
						if (state == DONE) {
					     // 什么都不做
							changeHeaderViewByState();
							ShowLog.v(TAG, "up，done!");
					    }
					    if (state == PULL_TO_REFRESH) {
					    	state = DONE;
					    	changeHeaderViewByState();
					     
					    	ShowLog.v(TAG, "up，PULL--->done");
					    }
					    if (state == RELEASE_TO_REFRESH) {
					    	state = REFRESHING;
					    	changeHeaderViewByState();
					    	onRefresh();

					    	ShowLog.v(TAG, "up，RELEASE--->done");
					    }
					 }

					isRecored = false;
					isBack = false;					
				}
				startHeadY = 0;
				break;
				 
			case MotionEvent.ACTION_MOVE:
				if(startHeadY == 0 && startFootY == 0)
					break;
				
				int dy = (int) event.getY() - startHeadY;
				int footy = (int) event.getY() - startFootY;
												
				if (!isRecored && !isLastRow && dy > 0) {					
					ShowLog.v(TAG, "move，开始拉升:"+ dy);
					isRecored = true;
					state = PULL_TO_REFRESH;
					changeHeaderViewByState();
					break;
				 }
				 
				if(!isRecored || state == REFRESHING)
				 	break;
				 	
				if(dy <= 0){
					state = DONE;
					break;
				}
				
				
				try{					
					if(!isListNone && getChildAt(1).getTop()<0){
						state = DONE;
						changeHeaderViewByState();
						isRecored = false;
						isBack = false;	
						startHeadY = 0;
						break;
					}
				}catch(Exception ex){
					ex.printStackTrace();
					isListNone = true;
					break;
				}
				
				ShowLog.v(TAG, "move，RELEASE_TO_REFRESH:"+ dy);
				ShowLog.v(TAG, "move，startY:"+ startHeadY);
				ShowLog.v(TAG, "move，firstItem's Top and bottom:"+getChildAt(1).getTop()+", "+getChildAt(1).getBottom());
				
				switch(state)
				{
					case PULL_TO_REFRESH:
						if(isListNone){
							if(dy >= headContentHeight+30){
								state = RELEASE_TO_REFRESH;
								isBack = true;
								changeHeaderViewByState();

								ShowLog.v(TAG, "move，done or PULL--->RELEASE");
							}		
						}
						else if(getChildAt(1).getTop() >= (headContentHeight+30)) {
							   state = RELEASE_TO_REFRESH;
							   isBack = true;
							   changeHeaderViewByState();

							   ShowLog.v(TAG, "move，done or PULL--->RELEASE");
						}
						
						headView.setPadding(10, -1 * headContentHeight + dy, 10, 10);
						headView.invalidate();
						break;
						
					case RELEASE_TO_REFRESH:
						if(isListNone){
							if(dy  < (headContentHeight+30)){
								state = PULL_TO_REFRESH;
								changeHeaderViewByState();

								ShowLog.v(TAG, "move，RELEASE--->PULL");
							}
							else if (dy >= (headContentHeight+60))
								break;
						}						
						// 往上推了，推到了屏幕足够掩盖head的程度，但是还没有推到全部掩盖的地步
						else if (getChildAt(1).getTop() < (headContentHeight+30)) {
							state = PULL_TO_REFRESH;
							changeHeaderViewByState();

							ShowLog.v(TAG, "move，RELEASE--->PULL");
						}
						else if (getChildAt(1).getTop() >= (headContentHeight+60))
							break;
						
						headView.setPadding(10, dy - headContentHeight, 10, 10);
						headView.invalidate();
						break;
						
					case DONE:
						state = PULL_TO_REFRESH;
						changeHeaderViewByState();
						ShowLog.v(TAG, "move，DONE--->PULL_TO_REFRESH");
						break;
						
					default:
						break;
				}
				break;
		}		
		return super.onTouchEvent(event);
	}

	@Override
	public boolean dispatchTouchEvent(MotionEvent ev) {
		// TODO Auto-generated method stub
//		switch(ev.getAction())
//		{
//			case MotionEvent.ACTION_DOWN:
//				
//			case MotionEvent.ACTION_UP:
//				
//			case MotionEvent.ACTION_MOVE:
//				break;
//		}
		return super.dispatchTouchEvent(ev);
	}
	
	/** 截断Touch事件 */
	@Override
	public boolean onInterceptTouchEvent(MotionEvent e) {
		int y = (int) e.getRawY();
		switch (e.getAction()) {
			case MotionEvent.ACTION_DOWN:
				// 首先拦截down事件,记录y坐标
				startHeadY = y;
//				MyLog.i(TAG, "onInterceptTouchEvent MotionEvent.ACTION_DOWN startY:"+startY);
				break;
			case MotionEvent.ACTION_MOVE:
				// deltaY > 0 是向下运动,< 0是向上运动
				int deltaY = y - startHeadY;
//				MyLog.i(TAG, "onInterceptTouchEvent MotionEvent.ACTION_DOWN y:"+y +", deltaY:" +deltaY);
				if (isHeadRefreshable && isInterceptEvent(deltaY)) {
					return true;
				}
				break;
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_CANCEL:
				break;
		}
		return false;
	}

	/** 是否截断Touch事件到本层做处理 */
	private boolean isInterceptEvent(int deltaY) {
		if (state == REFRESHING) {
			return false;
		}
		//对于ListView
		if (deltaY > 10) {
			if (this.getFirstVisiblePosition() == 0
					&& headView.getTop() == 0) {
//					MyLog.i(TAG, "isInterceptEvent mAdapterView.getFirstVisiblePosition() == 0 && child.getTop() == 0.  deltaY："+deltaY);
				return true;
			}			
		}
		return false;
	}

	// 当状态改变时候，调用该方法，以更新界面
	private void changeHeaderViewByState(){
		switch (state) {
			case RELEASE_TO_REFRESH:
				arrowImageView.setVisibility(View.VISIBLE);
				progressBar.setVisibility(View.GONE);
				tipsTextview.setVisibility(View.VISIBLE);

				arrowImageView.clearAnimation();
				arrowImageView.startAnimation(animation);

				tipsTextview.setText(R.string.view_refresh_release_label);

				//Log.v(TAG, "当前状态，松开刷新");
				break;
			case PULL_TO_REFRESH:
				progressBar.setVisibility(View.GONE);
				tipsTextview.setVisibility(View.VISIBLE);
				arrowImageView.clearAnimation();
				arrowImageView.setVisibility(View.VISIBLE);
				// 是由RELEASE_To_REFRESH状态转变来的
				if (isBack) {
					isBack = false;
					arrowImageView.clearAnimation();
					arrowImageView.startAnimation(reverseAnimation);

					tipsTextview.setText(R.string.view_refresh_pull_label);
				} else {
					tipsTextview.setText(R.string.view_refresh_pull_label);
				}
				//Log.v(TAG, "当前状态，下拉刷新");
				break;

			case REFRESHING:

				headView.setPadding(10, 20, 10, 10);
				headView.invalidate();
				
				arrowImageView.clearAnimation();
				arrowImageView.setVisibility(View.GONE);
				progressBar.setVisibility(View.VISIBLE);
				tipsTextview.setText(R.string.view_refresh_refreshing_label);

				//Log.v(TAG, "当前状态,正在刷新...");
				break;
			case DONE:
				headView.setPadding(10, -1 * headContentHeight, 10, 0);
				headView.invalidate();

				progressBar.setVisibility(View.GONE);
				arrowImageView.clearAnimation();
				arrowImageView.setImageResource(R.drawable.ic_refresh_arrow);
				tipsTextview.setText(R.string.view_refresh_pull_label);

				//Log.v(TAG, "当前状态，done");
				break;
		  }
	 }
    
	public void setOnRefreshListener(OnRefreshListener refreshListener) {
		this.refreshListener = refreshListener;
	}

	public interface OnRefreshListener {
		public void onRefresh();
	}

	public void onRefreshComplete() {
		state = DONE;
		lastUpdatedTextView.setText(myContext.getString(R.string.view_refresh_time_text) + new Date().toLocaleString());
		changeHeaderViewByState();
	}

	private void onRefresh() {
		if (refreshListener != null) {
			refreshListener.onRefresh();
		}
	}

	//定义宽度和高度的要求
	private void measureView(View child) {
		ViewGroup.LayoutParams p = child.getLayoutParams();
		if (p == null) {
			p = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
			ViewGroup.LayoutParams.WRAP_CONTENT);
		}
		int childWidthSpec = ViewGroup.getChildMeasureSpec(0, 0 + 0, p.width);
		int lpHeight = p.height;
		int childHeightSpec;
		if (lpHeight > 0) {
			childHeightSpec = MeasureSpec.makeMeasureSpec(lpHeight, MeasureSpec.EXACTLY);
		} else {
			childHeightSpec = MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);
		}
		child.measure(childWidthSpec, childHeightSpec);
	}
	
	public void startLoadMore(){
		if(footer !=null && footer.getVisibility() == View.GONE){
			footer.setVisibility(View.VISIBLE);
		}
	}
	
	public void stopLoadMore(){
		if(footer !=null && footer.getVisibility() == View.VISIBLE){
			footer.setVisibility(View.GONE);
		}
	}
	
	public void addFootView() {
        if (getFooterViewsCount() == 0) {
            addFooterView(footer);
        }
    }
	
	public void removeFootView(){
		removeFooterView(footer);
	}
	
	public boolean isListNone() {
		return isListNone;
	}
	
	public void setListNone(boolean isListNone) {
		this.isListNone = isListNone;
	}

}
