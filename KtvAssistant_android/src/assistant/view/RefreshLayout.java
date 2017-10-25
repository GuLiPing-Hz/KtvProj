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
import android.widget.AdapterView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.ScrollView;
import android.widget.TextView;
import assistant.util.ShowLog;

/**
 * @author song
 * @version 创建时间：2012-12-19 上午10:27:53 <br>
 * 类说明 上拉下拉都可以刷新的Layout，继承LinearLayout，里面可以加上ListView，Gridview或者ScrollView，加上的View高度必须是match_parent
 */
public class RefreshLayout extends LinearLayout{
	private final static int RELEASE_TO_REFRESH = 0;
	private final static int PULL_TO_REFRESH = 1;
	private final static int REFRESHING = 2;
	private final static int DONE = 3;
	private final static String TAG = "RefreshLayout";
	
	private Context myContext;
	private LayoutInflater inflater;
	private LinearLayout headView, footView;
	private TextView tipsViewHead,tipsViewFoot;
	private TextView lastUpdatedViewHead,lastUpdatedViewFoot;
	private ImageView arrowViewHead,arrowViewFoot;
	private ProgressBar progressBarHead,progressBarFoot;
	private AdapterView<?> mAdapterView;
	private ScrollView mScrollView;
	
	private int headContentHeight,middleContentHeight, footContentHeight;
	private int stateHead, stateFoot;
	private RotateAnimation animation, reverseAnimation,animation_root, reverseAnimation_root;
	private boolean isHeadBack,isFootBack;
	private boolean isHeaderScroll,isFooterScroll;
	private OnHeadRefreshListener headRefreshListener;
	private OnFootRefreshListener footRefreshListener;	
	private boolean isRecored;		// 用于保证startY的值在一个完整的touch事件中只被记录一次
	private int startY = 0;
//	private boolean isListNone = false;
	private boolean isRefreshable = true;
	private boolean isFootRefreshable = true;
		
	public RefreshLayout(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		myContext = context;
		init();
	}

	private void init(){		
		initAnimation();
		inflater = LayoutInflater.from(myContext);
		
		addHeadView();	
//    	isRefreshable = false;
	}
	
	private void addHeadView(){
		headView = (LinearLayout) inflater.inflate(R.layout.view_pullrefresh_header, null);
		arrowViewHead = (ImageView) headView.findViewById(R.id.pullrefresh_head_arrow);
		arrowViewHead.setMinimumWidth(30);
		arrowViewHead.setMinimumHeight(30);
		progressBarHead = (ProgressBar) headView.findViewById(R.id.pullrefresh_head_progressBar);
		tipsViewHead = (TextView) headView.findViewById(R.id.pullrefresh_head_tips);
		lastUpdatedViewHead = (TextView) headView.findViewById(R.id.pullrefresh_head_lastUpdated);
		lastUpdatedViewHead.setText(myContext.getString(R.string.view_refresh_time_text) + new Date().toLocaleString());
		
		measureView(headView);
		headContentHeight = headView.getMeasuredHeight();
		LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT,
				headContentHeight);
		
		// 设置topMargin的值为负的header View高度,即将其隐藏在最上方
		params.topMargin = -(headContentHeight);
		// mHeaderView.setLayoutParams(params1);
		addView(headView, params);

		ShowLog.v(TAG, "head height:"+ headContentHeight);
		
		stateHead = DONE;
		changeHeadViewByState(); 
	}
	
	private void addFootView(){
		footView = (LinearLayout) inflater.inflate(R.layout.view_pullrefresh_header, null);
		arrowViewFoot = (ImageView) footView.findViewById(R.id.pullrefresh_head_arrow);
		arrowViewFoot.setMinimumWidth(30);
		arrowViewFoot.setMinimumHeight(30);
//		arrowViewFoot.setImageResource(R.drawable.ic_refresh_arrow_up);
		progressBarFoot = (ProgressBar) footView.findViewById(R.id.pullrefresh_head_progressBar);
		tipsViewFoot = (TextView) footView.findViewById(R.id.pullrefresh_head_tips);
		lastUpdatedViewFoot = (TextView) footView.findViewById(R.id.pullrefresh_head_lastUpdated);
		lastUpdatedViewFoot.setText(myContext.getString(R.string.view_refresh_time_text) + new Date().toLocaleString());
		
		footView.setPadding(0, 10, 0, 0);
		
		measureView(footView);
		footContentHeight = footView.getMeasuredHeight();
		
		LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT,
				footContentHeight);

		// 由于是线性布局可以直接添加,只要AdapterView的高度是MATCH_PARENT,那么footer view就会被添加到最后,并隐藏
		addView(footView, params);

//		MyLog.v(TAG, "foot height:"+ footContentHeight);
		
		stateFoot = DONE;
		changeFootViewByState(); 
	}
	
	private void initAnimation(){
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
		
		animation_root = new RotateAnimation(0, 180,
				RotateAnimation.RELATIVE_TO_SELF, 0.5f,
				RotateAnimation.RELATIVE_TO_SELF, 0.5f);
		animation_root.setInterpolator(new LinearInterpolator());
		animation_root.setDuration(200);
		animation_root.setFillAfter(true);

		reverseAnimation_root = new RotateAnimation(180, 0,
				RotateAnimation.RELATIVE_TO_SELF, 0.5f,
				RotateAnimation.RELATIVE_TO_SELF, 0.5f);
		reverseAnimation_root.setInterpolator(new LinearInterpolator());
		reverseAnimation_root.setDuration(200);
		reverseAnimation_root.setFillAfter(true);
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

	private void initContentAdapterView() {
		int count = getChildCount();
		if (count < 2) {
			throw new IllegalArgumentException(
					"this layout must contain 2 child views, and AdapterView or ScrollView must in the second position!");
		}
		View view = null;
		for (int i = 0; i < count; i++) {
			view = getChildAt(i);
			if (view instanceof AdapterView<?>) {
				mAdapterView = (AdapterView<?>) view;
			}
			if (view instanceof ScrollView) {
				// finish later
				mScrollView = (ScrollView) view;
			}
		}
		if (mAdapterView == null && mScrollView == null) {
			throw new IllegalArgumentException(
					"must contain a AdapterView or ScrollView in this layout!");
		}
		
		
	}
	
	@Override
	protected void onFinishInflate() {
		super.onFinishInflate();
		initContentAdapterView();
		// footer view 在此添加保证添加到linearlayout中的最后
		addFootView();
		
	}
	
	// 当状态改变时候，调用该方法，以更新界面
	private void changeHeadViewByState(){
		switch (stateHead) {			
			case RELEASE_TO_REFRESH:				
				arrowViewHead.setVisibility(View.VISIBLE);
				progressBarHead.setVisibility(View.GONE);
				tipsViewHead.setVisibility(View.VISIBLE);

				arrowViewHead.clearAnimation();
				arrowViewHead.startAnimation(animation);

				tipsViewHead.setText(R.string.view_refresh_release_label);
				//MyLog.v(TAG, "Header 当前状态，松开刷新");					
				break;

			case PULL_TO_REFRESH:
				progressBarHead.setVisibility(View.GONE);
				tipsViewHead.setVisibility(View.VISIBLE);
				arrowViewHead.clearAnimation();
				arrowViewHead.setVisibility(View.VISIBLE);
				// 是由RELEASE_To_REFRESH状态转变来的
				if (isHeadBack) {
					isHeadBack = false;
					arrowViewHead.clearAnimation();
					arrowViewHead.startAnimation(reverseAnimation);

					tipsViewHead.setText(R.string.view_refresh_pull_label);
				} else {
					tipsViewHead.setText(R.string.view_refresh_pull_label);
				}
				//MyLog.v(TAG, "Header 当前状态，下拉刷新");
				break;

			case REFRESHING:
				setHeadTopMargin(20);				
//				headView.setPadding(10, 20, 10, 10);
//				headView.invalidate();
				
				arrowViewHead.clearAnimation();
				arrowViewHead.setVisibility(View.GONE);
				progressBarHead.setVisibility(View.VISIBLE);
				tipsViewHead.setText(R.string.view_refresh_refreshing_label);

				//MyLog.v(TAG, "Header 当前状态,正在刷新...");
				break;

			case DONE:
				setHeadTopMargin(-headContentHeight);
//				headView.setPadding(10, -1 * headContentHeight, 10, 0);
//				headView.invalidate();

				progressBarHead.setVisibility(View.GONE);
				arrowViewHead.clearAnimation();
				arrowViewHead.setImageResource(R.drawable.ic_refresh_arrow);
				tipsViewHead.setText(R.string.view_refresh_pull_label);

				//MyLog.v(TAG, "Header 当前状态，done");
				isRecored = false;
				isHeadBack = false;	
				break;
		  }
	 }
	
	// 当状态改变时候，调用该方法，以更新界面
	private void changeFootViewByState(){
		switch (stateFoot) {			
			case RELEASE_TO_REFRESH:				
				arrowViewFoot.setVisibility(View.VISIBLE);
				progressBarFoot.setVisibility(View.GONE);
				tipsViewFoot.setVisibility(View.VISIBLE);
				lastUpdatedViewFoot.setVisibility(View.VISIBLE);

				arrowViewFoot.clearAnimation();
				arrowViewFoot.startAnimation(animation_root);

				tipsViewFoot.setText(R.string.view_refresh_release_label);
				//MyLog.v(TAG, "Header 当前状态，松开刷新");					
				break;

			case PULL_TO_REFRESH:
				progressBarFoot.setVisibility(View.GONE);
				tipsViewFoot.setVisibility(View.VISIBLE);
				lastUpdatedViewFoot.setVisibility(View.VISIBLE);
//				arrowViewFoot.clearAnimation();
				arrowViewFoot.setVisibility(View.VISIBLE);
				// 是由RELEASE_To_REFRESH状态转变来的
				if (isFootBack) {
					isFootBack = false;
					arrowViewFoot.clearAnimation();
					arrowViewFoot.startAnimation(reverseAnimation_root);

					tipsViewFoot.setText(R.string.view_refresh_pull_up);
				} else {
					tipsViewFoot.setText(R.string.view_refresh_pull_up);
				}
				//MyLog.v(TAG, "Header 当前状态，下拉刷新");
				break;

			case REFRESHING:
				arrowViewFoot.clearAnimation();
				arrowViewFoot.setVisibility(View.GONE);
				progressBarFoot.setVisibility(View.VISIBLE);
				tipsViewFoot.setText(R.string.view_refresh_refreshing_label);
				lastUpdatedViewFoot.setVisibility(View.VISIBLE);
				
				setHeadTopMargin(-headContentHeight - footContentHeight -10);
				//MyLog.v(TAG, "Header 当前状态,正在刷新...");
				break;

			case DONE:
				progressBarFoot.setVisibility(View.GONE);
				arrowViewFoot.clearAnimation();
				arrowViewFoot.setImageResource(R.drawable.ic_refresh_arrow_up);
				tipsViewFoot.setText(R.string.view_refresh_pull_up);
				lastUpdatedViewFoot.setVisibility(View.VISIBLE);
				
				setHeadTopMargin(-headContentHeight);
				//MyLog.v(TAG, "Header 当前状态，done");
				break;
		  }
	 }

	/** 根据headView的topMargin改变headView或者footView的显示位置 */
	private void setHeadTopMargin(int topMargin) {
		LayoutParams params = (LayoutParams) headView.getLayoutParams();
		params.topMargin = topMargin;
		headView.setLayoutParams(params);
		invalidate();
	}
	
	public interface OnHeadRefreshListener {
		public void onHeadRefresh();
	}
	
	public interface OnFootRefreshListener {
		public void onFootRefresh();
	}
	
	public void setOnHeadRefreshListener(OnHeadRefreshListener refreshListener) {
		this.headRefreshListener = refreshListener;
	}
	
	private void onHeadRefresh() {
		if (headRefreshListener != null) {
			headRefreshListener.onHeadRefresh();
		}
	}
	
	public void onHeadRefreshComplete() {
		stateHead = DONE;
		lastUpdatedViewHead.setText(myContext.getString(R.string.view_refresh_time_text) + new Date().toLocaleString());
		changeHeadViewByState();
	}

		
	public void setOnFootRefreshListener(OnFootRefreshListener refreshListener) {
		this.footRefreshListener = refreshListener;
	}
	
	private void onFootRefresh() {
		if (footRefreshListener != null) {
			footRefreshListener.onFootRefresh();
		}
	}
	
	public void onFootRefreshComplete() {
		stateFoot = DONE;
		Date date = new Date();
		lastUpdatedViewFoot.setText(myContext.getString(R.string.view_refresh_time_text) 
				+ date.getHours()+":"+date.getMinutes());
		changeFootViewByState();
	}

	/** 截断Touch事件 */
	@Override
	public boolean onInterceptTouchEvent(MotionEvent e) {
		int y = (int) e.getRawY();
		switch (e.getAction()) {
			case MotionEvent.ACTION_DOWN:
				// 首先拦截down事件,记录y坐标
				startY = y;
				break;
			case MotionEvent.ACTION_MOVE:
				// deltaY > 0 是向下运动,< 0是向上运动
				int deltaY = y - startY;
				if (isRefreshable && isInterceptEvent(deltaY)) {
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
		if (stateHead == REFRESHING || stateFoot == REFRESHING) {
			return false;
		}
		//对于ListView和GridView
		if (mAdapterView != null) {
			// 子view(ListView or GridView)滑动到最顶端
			if (deltaY > 10) {
				View child = mAdapterView.getChildAt(0);
				if (child == null) {
					// 如果mAdapterView中没有数据,下拉刷新
					ShowLog.i(TAG, "isInterceptEvent child == null.  deltaY："+deltaY);
					isHeaderScroll = true;
					isFooterScroll = false;
					return true;
				}
				if (mAdapterView.getFirstVisiblePosition() == 0
						&& child.getTop() == 0) {
					ShowLog.i(TAG, "isInterceptEvent mAdapterView.getFirstVisiblePosition() == 0 && child.getTop() == 0.  deltaY："+deltaY);
					isHeaderScroll = true;
					isFooterScroll = false;
					return true;
				}
				int top = child.getTop();
				int padding = mAdapterView.getPaddingTop();
				if (mAdapterView.getFirstVisiblePosition() == 0
						&& Math.abs(top - padding) <= 8) {//这里之前用3可以判断,但现在不行,还没找到原因
					ShowLog.i(TAG, "isInterceptEvent mAdapterView.getFirstVisiblePosition() == 0 && Math.abs(top - padding) <= 8.  deltaY:"+deltaY);
					isHeaderScroll = true;
					isFooterScroll = false;
					return true;
				}
				isHeaderScroll = false;
				
			} else if (deltaY < -10) {
				if (mAdapterView.getChildCount() <= 0) {
					// 如果mAdapterView中没有数据,不拦截
					return false;
				}
				
				View lastChild = mAdapterView.getChildAt(mAdapterView
						.getChildCount() - 1);
				if (lastChild == null) {
					// 如果mAdapterView中没有数据,不拦截
					return false;
				}
//				View child = mAdapterView.getChildAt(0);
//				int top = child.getTop();
				// 最后一个子view的Bottom小于父View的高度说明mAdapterView的数据没有填满父view,
				// 等于父View的高度说明mAdapterView已经滑动到最后
				if (lastChild.getBottom() <= getHeight()
						&& mAdapterView.getLastVisiblePosition() == mAdapterView
								.getCount() - 1/* && child.getTop() != 0*/) {
					ShowLog.i(TAG, "isInterceptEvent 上拉.  deltaY:"+deltaY);
					isFooterScroll = true;
					isHeaderScroll = false;
					return true;
				}
			}
		}
		// 对于ScrollView
		if (mScrollView != null) {
			// 子scroll view滑动到最顶端
			View child = mScrollView.getChildAt(0);
			if (deltaY > 0 && mScrollView.getScrollY() == 0) {
				isHeaderScroll = true;
				return true;
			} else if (deltaY < 0
					&& child.getMeasuredHeight() <= getHeight()
							+ mScrollView.getScrollY()) {
				isFooterScroll = true;
				return true;
			}
		}
		return false;
	}
	
	/** 移动事件处理 */
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		switch(event.getAction())
		{
			case MotionEvent.ACTION_DOWN:
				// onInterceptTouchEvent已经记录					
				break;
				
			case MotionEvent.ACTION_MOVE:
				if(startY == 0)
					break;
				
				int dy  = (int) event.getRawY() - startY;
				
				if(isHeaderScroll)
					moveToChangeHeadState(dy);								
				else if(isFooterScroll && isFootRefreshable)
					moveToChangeFootState(dy);
				
				break;
				
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_CANCEL:
				if(isRecored)
				{
					if(isHeaderScroll)
						upToChangeHeadState();
					else if(isFooterScroll && isFootRefreshable)
						upToChangeFootState();
					
					isRecored = false;					
				}
				startY = 0;
				break;
			
		}		
				
		return super.onTouchEvent(event);
	}
	
	/** 修改Header view top margin的值 */
	private int changingHeadViewTopMargin(int deltaY) {
		LayoutParams params = (LayoutParams) headView.getLayoutParams();
		float newTopMargin = -headContentHeight + deltaY/* * 0.3f*/;
//		MyLog.i(TAG, "changingHeadViewTopMargin params.topMargin: "+params.topMargin+", deltaY: "+deltaY+", newTopMargin: "+newTopMargin);
		//这里对上拉做一下限制,因为当前上拉后然后不释放手指直接下拉,会把下拉刷新给触发了
		//表示如果是在上拉后一段距离,然后直接下拉
		if(deltaY>0 && isFooterScroll && Math.abs(params.topMargin) <= headContentHeight){
			return params.topMargin;
		}
		//同样地,对下拉做一下限制,避免出现跟上拉操作时一样的bug
		if(deltaY<0 && isHeaderScroll && Math.abs(params.topMargin) >= headContentHeight){
			return params.topMargin;
		}
		if(isHeaderScroll){
			if(newTopMargin >= 20)
				newTopMargin = 20;
			
			if(mAdapterView != null && middleContentHeight == 0){		//防止下拉时listview界面重绘
				if(mAdapterView != null){
					middleContentHeight = mAdapterView.getMeasuredHeight();
//					MyLog.i(TAG, "middleContentHeight:"+middleContentHeight);
					LayoutParams params1 = new LayoutParams(LayoutParams.MATCH_PARENT,
							middleContentHeight);
					mAdapterView.setLayoutParams(params1);
				}else if(mScrollView != null){
					middleContentHeight = mScrollView.getMeasuredHeight();
//					MyLog.i(TAG, "middleContentHeight:"+middleContentHeight);
					LayoutParams params1 = new LayoutParams(LayoutParams.MATCH_PARENT,
							middleContentHeight);
					mScrollView.setLayoutParams(params1);
				}
			}
		}else if(isFooterScroll){
			if(params.topMargin <= - headContentHeight - footContentHeight - 20)
				newTopMargin = - headContentHeight - footContentHeight - 20;
		}
	
		params.topMargin = (int) newTopMargin /*+ (isHeaderScroll?10:0)*/;
//		MyLog.i(TAG, "changingHeadViewTopMargin params.topMargin: "+params.topMargin);
		headView.setLayoutParams(params);
		headView.invalidate();
		return params.topMargin;
	}
	
	private void moveToChangeHeadState(int dy){
		if (!isRecored) {					
//			MyLog.v(TAG, "move，开始拉升:"+ dy);
			isRecored = true;
			isHeadBack = false;
			stateHead = PULL_TO_REFRESH;
			changeHeadViewByState();		
			changingHeadViewTopMargin(dy);
			return;
		 }
				
//		MyLog.v(TAG, "下拉 move，startY:"+ startY+", dy:"+ dy);
//		MyLog.v(TAG, "下拉 move，listview's Top and bottom:"+getChildAt(1).getTop()+", "+getChildAt(1).getBottom());
		
		int newTopMargin = changingHeadViewTopMargin(dy);	
//		MyLog.v(TAG, "下拉 move，newTopMargin:"+newTopMargin);

		if(newTopMargin <= -headContentHeight){
			stateHead = DONE;					
			changeHeadViewByState();

//			MyLog.v(TAG, "move，DONE.");
			return;
		}
		
		switch(stateHead)
		{
			case PULL_TO_REFRESH:	//原先是下拉
				if(newTopMargin > 0) {
					stateHead = RELEASE_TO_REFRESH;					
					changeHeadViewByState();

//					MyLog.v(TAG, "move，done or PULL--->RELEASE");
				}			
				break;
				
			case RELEASE_TO_REFRESH:					
				// 往上回推了，推到了屏幕足够掩盖head的程度，但是还没有推到全部掩盖的地步
				if (newTopMargin <= 0 &&  newTopMargin > -headContentHeight) {
					stateHead = PULL_TO_REFRESH;
					isHeadBack = true;
					changeHeadViewByState();
//					MyLog.v(TAG, "move，RELEASE--->PULL");
				}

				break;
				
			case DONE:
				isHeadBack = false;
				stateHead = PULL_TO_REFRESH;
				changeHeadViewByState();
//				MyLog.v(TAG, "move，DONE--->PULL_TO_REFRESH");
				break;
				
			default:
				break;
		}
	}

	private void upToChangeHeadState(){
		if (stateHead != REFRESHING) {
			if (stateHead == DONE) {
		     // 什么都不做
				changeHeadViewByState();
//				MyLog.v(TAG, "up，done!");
		    }else if(stateHead == PULL_TO_REFRESH) {
		    	stateHead = DONE;
		    	changeHeadViewByState();
		     
//		    	MyLog.v(TAG, "up，PULL--->done");
		    }else if(stateHead == RELEASE_TO_REFRESH) {
		    	stateHead = REFRESHING;
		    	changeHeadViewByState();
		    	onHeadRefresh();

//		    	MyLog.v(TAG, "up，RELEASE--->done");
		    }
		 }
		isHeadBack = false;		
	}
	
	private void moveToChangeFootState(int dy){
		if (!isRecored) {					
//			MyLog.v(TAG, "move，开始拉升:"+ dy);
			isRecored = true;
			isFootBack = false;
			stateFoot = PULL_TO_REFRESH;
			changeFootViewByState();
			changingHeadViewTopMargin(dy);
			return;
		 }
				
//		MyLog.v(TAG, "上拉 move，startY:"+ startY+", dy:"+ dy);
//		MyLog.v(TAG, "上拉 move，listview's Top and bottom:"+getChildAt(1).getTop()+", "+getChildAt(1).getBottom());
		
		int newTopMargin = changingHeadViewTopMargin(dy);	
//		MyLog.v(TAG, "上拉 move，newTopMargin:"+newTopMargin);

		switch(stateFoot)
		{
			case PULL_TO_REFRESH:	//原先是上拉
				if(Math.abs(newTopMargin) >= (headContentHeight + footContentHeight)) {
					stateFoot = RELEASE_TO_REFRESH;					
					changeFootViewByState();	
//					MyLog.v(TAG, "move，done or PULL--->RELEASE");
				}
				break;
				
			case RELEASE_TO_REFRESH:					
				// 往下回推了，推到了屏幕足够掩盖Foot的程度，但是还没有推到全部掩盖的地步
				if (Math.abs(newTopMargin) < (headContentHeight + footContentHeight)) {
					stateFoot = PULL_TO_REFRESH;
					isFootBack = true;
					changeFootViewByState();
//					MyLog.v(TAG, "move，RELEASE--->PULL");
				}
				break;
				
			case DONE:
				stateFoot = PULL_TO_REFRESH;
				changeFootViewByState();
//				MyLog.v(TAG, "move，DONE--->PULL_TO_REFRESH");
				break;
				
			default:
				break;
		}
	}

	private void upToChangeFootState(){
		if (stateFoot != REFRESHING) {
			if (stateFoot == DONE) {
		     // 什么都不做
				changeFootViewByState();
//				MyLog.v(TAG, "up，done!");
		    }else if(stateFoot == PULL_TO_REFRESH) {
		    	stateFoot = DONE;
		    	changeFootViewByState();
		     
//		    	MyLog.v(TAG, "up，PULL--->done");
		    }else if(stateFoot == RELEASE_TO_REFRESH) {
		    	stateFoot = REFRESHING;
		    	changeFootViewByState();
		    	onFootRefresh();

//		    	MyLog.v(TAG, "up，RELEASE--->done");
		    }
		 }
		isFootBack = false;		
	}

	public boolean isFootRefreshable() {
		return isFootRefreshable;
	}
	
	public void setFootRefreshable(boolean isFootRefreshable) {
		this.isFootRefreshable = isFootRefreshable;
	}
}
