package com.zxing.activity;

import java.io.IOException;
import java.util.Vector;

import tiange.ktv.assistant.R;
import android.app.Activity;
import android.content.Intent;
import android.content.res.AssetFileDescriptor;
import android.graphics.Bitmap;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.os.Bundle;
import android.os.Handler;
import android.os.Vibrator;
import android.text.TextUtils;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;
import assistant.activity.BaseActivity;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;

import com.google.zxing.BarcodeFormat;
import com.google.zxing.Result;
import com.zxing.camera.CameraManager;
import com.zxing.decoding.CaptureActivityHandler;
import com.zxing.decoding.InactivityTimer;
import com.zxing.view.ViewfinderView;
/**
 * Initial the camera
 * @author Ryan.Tang
 */
public class CaptureActivity extends BaseActivity implements Callback {
	public static final String EXTRA_RESULT_DATA="com.zxing.activity.CaptureActivity.result";
	public static final String EXTRA_SACN_TYPE= "com.zxing.activity.CaptureActivity.ScanType";
	
	//扫描包厢密码
	public static final int SCANTYPE_ROOMPWD = 0;
	//扫描包厢金币
	public static final int SCANTYPE_GOLD = 1;
	//扫描通用二维码
	public static final int SCANTYPE_CODE = 2;
	
	int m_scanType;
	private CaptureActivityHandler handler;
	private ViewfinderView viewfinderView;
	private boolean hasSurface;
	private Vector<BarcodeFormat> decodeFormats;
	private String characterSet;
	private InactivityTimer inactivityTimer;
	private MediaPlayer mediaPlayer;
	private boolean playBeep;
	private static final float BEEP_VOLUME = 0.10f;
	private boolean vibrate;
	private View cancelScanButton;
	
	TextView m_textViewTitle;
	
	private Handler uihandler = new Handler();
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		m_scanType = this.getIntent().getIntExtra(EXTRA_SACN_TYPE, SCANTYPE_CODE);
		
		setContentView(R.layout.ac_camera);
		m_textViewTitle = (TextView) findViewById(R.id.tv_title);
		viewfinderView = (ViewfinderView) findViewById(R.id.viewfinder_view);
		cancelScanButton = findViewById(R.id.iv_back);
		hasSurface = false;
		inactivityTimer = new InactivityTimer(this);
		
		
		//如果超过10秒之后还是没有扫描出包厢密码，给予提示
		switch(m_scanType)
		{
			case SCANTYPE_ROOMPWD:
			{
				m_textViewTitle.setText(R.string.title_scan_enterroom);
				showUnsupportTips();
				break;
			}
			case SCANTYPE_GOLD:
			{
				m_textViewTitle.setText(R.string.title_scan_getgold);
				break;
			}
			
			default:
			{
				m_textViewTitle.setText(R.string.title_scan_code);
				break;
			}
		}
	}

	@Override
	protected void onResume() {
		super.onResume();
		SurfaceView surfaceView = (SurfaceView) findViewById(R.id.preview_view);
		SurfaceHolder surfaceHolder = surfaceView.getHolder();
		if (hasSurface) {
			initCamera(surfaceHolder);
		} else {
			surfaceHolder.addCallback(this);
			surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		}
		decodeFormats = null;
		characterSet = null;

		playBeep = true;
		AudioManager audioService = (AudioManager) getSystemService(AUDIO_SERVICE);
		if (audioService.getRingerMode() != AudioManager.RINGER_MODE_NORMAL) {
			playBeep = false;
		}
		initBeepSound();
		vibrate = true;
		
		//quit the scan view
		cancelScanButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				setResult(Activity.RESULT_CANCELED);
				CaptureActivity.this.finish();
			}
		});
	}

	@Override
	protected void onPause() {
		super.onPause();
		if (handler != null) {
			handler.quitSynchronously();
			handler = null;
		}
		CameraManager.get(this).closeDriver();
	}

	@Override
	protected void onDestroy() {
		inactivityTimer.shutdown();
		super.onDestroy();
	}
	
	/**
	 * Handler scan result
	 * @param result
	 * @param barcode
	 */
	public void handleDecode(Result result, Bitmap barcode) {
		inactivityTimer.onActivity();
		playBeepSoundAndVibrate();
		String resultString = result.getText();
		//FIXME
		if (TextUtils.isEmpty(resultString)) {
			ShowUtil.showLongToast(this, R.string.scan_error);
		}
		else 
		{
			if(m_scanType != SCANTYPE_CODE)
			{
				if(!ToolUtil.matcherLoginKey(resultString))
				{
					ShowUtil.showLongToast(this, R.string.scan_error);
					handler.sendEmptyMessageDelayed(R.id.restart_preview, 5*1000);
					return;
				}
			}
			
			Intent resultIntent = new Intent();
			Bundle bundle = new Bundle();
			bundle.putString(EXTRA_RESULT_DATA, resultString);
			resultIntent.putExtras(bundle);
			setResult(Activity.RESULT_OK, resultIntent);
		}
		CaptureActivity.this.finish();
	}
	
	private void initCamera(SurfaceHolder surfaceHolder) {
		try {
			CameraManager.get(this).openDriver(surfaceHolder);
		} catch (IOException ioe) {
			return;
		} catch (RuntimeException e) {
			return;
		}
		if (handler == null) {
			handler = new CaptureActivityHandler(this, decodeFormats,
					characterSet);
		}
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {

	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		if (!hasSurface) {
			hasSurface = true;
			initCamera(holder);
		}

	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		hasSurface = false;

	}

	public ViewfinderView getViewfinderView() {
		return viewfinderView;
	}

	public Handler getHandler() {
		return handler;
	}

	public void drawViewfinder() {
		viewfinderView.drawViewfinder();

	}

	private void initBeepSound() {
		if (playBeep && mediaPlayer == null) {
			// The volume on STREAM_SYSTEM is not adjustable, and users found it
			// too loud,
			// so we now play on the music stream.
			setVolumeControlStream(AudioManager.STREAM_MUSIC);
			mediaPlayer = new MediaPlayer();
			mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
			mediaPlayer.setOnCompletionListener(beepListener);

			AssetFileDescriptor file = getResources().openRawResourceFd(R.raw.beep);
			try {
				mediaPlayer.setDataSource(file.getFileDescriptor(),
						file.getStartOffset(), file.getLength());
				file.close();
				mediaPlayer.setVolume(BEEP_VOLUME, BEEP_VOLUME);
				mediaPlayer.prepare();
			} catch (IOException e) {
				mediaPlayer = null;
			}
		}
	}

	private static final long VIBRATE_DURATION = 200L;

	private void playBeepSoundAndVibrate() {
		if (playBeep && mediaPlayer != null) {
			mediaPlayer.start();
		}
		if (vibrate) {
			Vibrator vibrator = (Vibrator) getSystemService(VIBRATOR_SERVICE);
			vibrator.vibrate(VIBRATE_DURATION);
		}
	}

	/**
	 * When the beep has finished playing, rewind to queue up another one.
	 */
	private final OnCompletionListener beepListener = new OnCompletionListener() {
		public void onCompletion(MediaPlayer mediaPlayer) {
			mediaPlayer.seekTo(0);
		}
	};

	private void showUnsupportTips(){
		final TextView tips = (TextView)findViewById(R.id.camera_tips);
		uihandler.postDelayed(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				tips.setVisibility(View.VISIBLE);
			}
		}, 10000);
	}
}