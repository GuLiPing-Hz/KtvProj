package com.sliding;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup.LayoutParams;

import com.sliding.SlidingMenu.OnCloseListener;
import com.sliding.SlidingMenu.OnOpenListener;

public class SlidingActivity extends FragmentActivity implements SlidingActivityBase {

	private SlidingActivityHelper mHelper;

	/* (non-Javadoc)
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mHelper = new SlidingActivityHelper(this);
		mHelper.onCreate(savedInstanceState);
	}

	/* (non-Javadoc)
	 * @see android.app.Activity#onPostCreate(android.os.Bundle)
	 */
	@Override
	public void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);
		mHelper.onPostCreate(savedInstanceState);
	}

	/* (non-Javadoc)
	 * @see android.app.Activity#findViewById(int)
	 */
	@Override
	public View findViewById(int id) {
		View v = super.findViewById(id);
		if (v != null)
			return v;
		return mHelper.findViewById(id);
	}

	/* (non-Javadoc)
	 * @see android.app.Activity#onSaveInstanceState(android.os.Bundle)
	 */
	@Override
	protected void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
		mHelper.onSaveInstanceState(outState);
	}

	/* (non-Javadoc)
	 * @see android.app.Activity#setContentView(int)
	 */
	@Override
	public void setContentView(int id) {
		setContentView(getLayoutInflater().inflate(id, null));
	}

	/* (non-Javadoc)
	 * @see android.app.Activity#setContentView(android.view.View)
	 */
	@Override
	public void setContentView(View v) {
		setContentView(v, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
	}

	/* (non-Javadoc)
	 * @see android.app.Activity#setContentView(android.view.View, android.view.ViewGroup.LayoutParams)
	 */
	@Override
	public void setContentView(View v, LayoutParams params) {
		super.setContentView(v, params);
		mHelper.registerAboveContentView(v, params);
	}

	/* (non-Javadoc)
	 * @see com.slidingmenu.lib.app.SlidingActivityBase#setBehindContentView(int)
	 */
	public void setBehindContentView(int id) {
		setBehindContentView(getLayoutInflater().inflate(id, null));
	}

	/* (non-Javadoc)
	 * @see com.slidingmenu.lib.app.SlidingActivityBase#setBehindContentView(android.view.View)
	 */
	public void setBehindContentView(View v) {
		setBehindContentView(v, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
	}

	/* (non-Javadoc)
	 * @see com.slidingmenu.lib.app.SlidingActivityBase#setBehindContentView(android.view.View, android.view.ViewGroup.LayoutParams)
	 */
	public void setBehindContentView(View v, LayoutParams params) {
		mHelper.setBehindContentView(v, params);
	}
	
	public void setSecondaryContentView(int id) throws Exception {
		setSecondaryContentView(getLayoutInflater().inflate(id, null));
	}

	public void setSecondaryContentView(View v) throws Exception {
		mHelper.setSecondaryContentView(v);
	}

	/* (non-Javadoc)
	 * @see com.slidingmenu.lib.app.SlidingActivityBase#getSlidingMenu()
	 */
	public SlidingMenu getSlidingMenu() {
		return mHelper.getSlidingMenu();
	}

	/* (non-Javadoc)
	 * @see com.slidingmenu.lib.app.SlidingActivityBase#toggle()
	 */
	public void toggle() {
		mHelper.toggle();
	}

	/* (non-Javadoc)
	 * @see com.slidingmenu.lib.app.SlidingActivityBase#showAbove()
	 */
	@Override
	public void showContent(boolean animate) {
		mHelper.showContent(animate);
	}

	public void showContent(){
		mHelper.showContent(true);
	}
	/* (non-Javadoc)
	 * @see com.slidingmenu.lib.app.SlidingActivityBase#showBehind()
	 */
	public void showMenu() {
		mHelper.showMenu();
	}

	/* (non-Javadoc)
	 * @see com.slidingmenu.lib.app.SlidingActivityBase#showSecondaryMenu()
	 */
	public void showSecondaryMenu() {
		mHelper.showSecondaryMenu();
	}

	/* (non-Javadoc)
	 * @see com.slidingmenu.lib.app.SlidingActivityBase#setSlidingActionBarEnabled(boolean)
	 */
	public void setSlidingActionBarEnabled(boolean b) {
		mHelper.setSlidingActionBarEnabled(b);
	}

	/* (non-Javadoc)
	 * @see android.app.Activity#onKeyUp(int, android.view.KeyEvent)
	 */
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
//		boolean b = mHelper.onKeyUp(keyCode, event);
//		if (b) return b;
		return super.onKeyUp(keyCode, event);
	}
	
	public boolean isContentShowing(){
		return mHelper.isContentShowing();
	}

	public boolean isMenuShowing(){
		return mHelper.isMenuShowing();
	}

	/**
	 * Add a View ignored by the Touch Down event when mode is Fullscreen
	 *
	 * @param v a view to be ignored
	 */
	public void addIgnoredAboveView(View v) {
		mHelper.addIgnoredAboveView(v);
	}

	/**
	 * Remove a View ignored by the Touch Down event when mode is Fullscreen
	 *
	 * @param v a view not wanted to be ignored anymore
	 */
	public void removeIgnoredAboveView(View v) {
		mHelper.removeIgnoredAboveView(v);
	}

	/**
	 * Clear the list of Views ignored by the Touch Down event when mode is Fullscreen
	 */
	public void clearIgnoredAboveViews() {
		mHelper.clearIgnoredAboveViews();
	}

	/**
	 * Sets the OnOpenListener. {@link OnOpenListener#onOpen()
	 * OnOpenListener.onOpen()} will be called when the SlidingMenu is opened
	 * 
	 * @param listener
	 *            the new OnOpenListener
	 */
	public void setOnLeftOpenListener(OnOpenListener listener) {
		mHelper.setOnLeftOpenListener(listener);
	}

	/**
	 * Sets the OnCloseListener. {@link OnCloseListener#onClose()
	 * OnCloseListener.onClose()} will be called when the SlidingMenu is closed
	 * 
	 * @param listener
	 *            the new setOnCloseListener
	 */
	public void setOnCloseListener(OnCloseListener listener) {
		mHelper.setOnCloseListener(listener);
	}

}
