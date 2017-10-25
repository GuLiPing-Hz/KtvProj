/*
 * Copyright (C) 2008 ZXing authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.zxing.view;

import java.util.Collection;
import java.util.HashSet;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.View;

import com.google.zxing.ResultPoint;
import com.zxing.camera.CameraManager;

/**
 * This view is overlaid on top of the camera preview. It adds the viewfinder
 * rectangle and partial transparency outside it, as well as the laser scanner
 * animation and result points. 
 */
public final class ViewfinderView extends View {

	private static final int[] SCANNER_ALPHA = { 0, 64, 128, 192, 255, 192,
			128, 64 };
	private static final long ANIMATION_DELAY = 100L;
	private static final int OPAQUE = 0xFF;

	private final Paint paint, laserPaint;
	private Bitmap resultBitmap;
	private final int maskColor;
	private final int resultColor;
	private final int frameColor;
	private final int laserColor, gradientColor;
	private final int resultPointColor;
	private int scannerAlpha;
	private Collection<ResultPoint> possibleResultPoints;
	private Collection<ResultPoint> lastPossibleResultPoints;

	private int paddingTop = 10;
	private int interval = 10;
	private int laserwidht = 6;

	// This constructor is used when the class is built from an XML resource.
	public ViewfinderView(Context context, AttributeSet attrs) {
		super(context, attrs);

		// Initialize these once for performance rather than calling them every
		// time in onDraw().
		paint = new Paint();
		laserPaint = new Paint();
		Resources resources = getResources();
		maskColor = resources.getColor(R.color.viewfinder_mask);
		resultColor = resources.getColor(R.color.result_view);
		frameColor = resources.getColor(R.color.home_itemtext);
		laserColor = resources.getColor(R.color.viewfinder_laser);
		gradientColor = resources.getColor(R.color.transparent);
		resultPointColor = resources.getColor(R.color.possible_result_points);
		scannerAlpha = 0;
		possibleResultPoints = new HashSet<ResultPoint>(5);
	}

	@Override
	public void onDraw(Canvas canvas) {
		CameraManager cameraMgr = CameraManager.get(getContext());
		if(cameraMgr== null)
			return;
		Rect frame = cameraMgr.getFramingRect();
		if (frame == null) {
			return;
		}
		int width = canvas.getWidth();
		int height = canvas.getHeight();

		// 绘制遮罩区域，
		paint.setColor(resultBitmap != null ? resultColor : maskColor);
		//中间扫描框的上边遮罩
		canvas.drawRect(0, 0, width, frame.top, paint);
		//中间扫描框的左边遮罩
		canvas.drawRect(0, frame.top, frame.left, frame.bottom + 1, paint);
		//中间扫面框的右边遮罩
		canvas.drawRect(frame.right + 1, frame.top, width, frame.bottom + 1,paint);
		//中间扫描框的下边遮罩
		canvas.drawRect(0, frame.bottom + 1, width, height, paint);

		if (resultBitmap != null) {
			// Draw the opaque result bitmap over the scanning rectangle
			paint.setAlpha(OPAQUE);
			canvas.drawBitmap(resultBitmap, frame.left, frame.top, paint);
		} else {

			// Draw a two pixel solid black border inside the framing rect
			/*
			 * paint.setColor(frameColor); canvas.drawRect(frame.left,
			 * frame.top, frame.right + 1, frame.top + 2, paint);
			 * canvas.drawRect(frame.left, frame.top + 2, frame.left + 2,
			 * frame.bottom - 1, paint); canvas.drawRect(frame.right - 1,
			 * frame.top, frame.right + 1, frame.bottom - 1, paint);
			 * canvas.drawRect(frame.left, frame.bottom - 1, frame.right + 1,
			 * frame.bottom + 1, paint);
			 */

			// Draw Four corners box inside the framing rect
			int linewidht = 5;
			int padding = 5;
			int linelength = 30;
			//画四个边角的横竖线 4*2=8需要画8个
			paint.setColor(frameColor);
			canvas.drawRect(padding + frame.left, padding + frame.top, padding
					+ (linewidht + frame.left), padding
					+ (linelength + frame.top), paint);
			canvas.drawRect(padding + frame.left, padding + frame.top, padding
					+ (linelength + frame.left), padding
					+ (linewidht + frame.top), paint);
			canvas.drawRect(-padding + ((0 - linewidht) + frame.right), padding
					+ frame.top, -padding + (/* 1 + */frame.right), padding
					+ (linelength + frame.top), paint);
			canvas.drawRect(-padding + (-linelength + frame.right), padding
					+ frame.top, -padding + frame.right, padding
					+ (linewidht + frame.top), paint);
			canvas.drawRect(padding + frame.left, -padding
					+ (-linelength + (1 + frame.bottom)), padding
					+ (linewidht + frame.left), -padding + (1 + frame.bottom),
					paint);
			canvas.drawRect(padding + frame.left, -padding
					+ ((0 - linewidht) + frame.bottom), padding
					+ (linelength + frame.left), -padding + (1 + frame.bottom),
					paint);
			canvas.drawRect(-padding + ((0 - linewidht) + frame.right),
					-padding + (-linelength + (1 + frame.bottom)), -padding
							+ (1 + frame.right), -padding + (1 + frame.bottom),
					paint);
			canvas.drawRect(-padding + (-linelength + frame.right), -padding
					+ ((0 - linewidht) + frame.bottom), -padding + frame.right,
					-padding + (linewidht - (linewidht - 1) + frame.bottom),
					paint);

			// Draw a red "laser scanner" line through the middle to show
			// decoding is active
			// paint.setColor(laserColor);
			// paint.setAlpha(SCANNER_ALPHA[scannerAlpha]);
			// scannerAlpha = (scannerAlpha + 1) % SCANNER_ALPHA.length;
			//int middle = frame.height() / 2 + frame.top;
			// canvas.drawRect(frame.left + 2, middle - 1, frame.right - 1,
			// middle + 2, paint);

			// Draw a "laser scanner" line
//			if((paddingTop + 10) > frame.height()){
//				paddingTop = 10;
//			}
//			int middle = paddingTop + frame.top;
//			if(paddingTop < 30 || frame.height() - paddingTop < 30){
//				paddingTop = paddingTop + interval / 2;
//			}else{
//				paddingTop = paddingTop + interval;
//			}
//			laserPaint.setColor(laserColor);
//			laserPaint.setAlpha(255);
//			int[] colors = new int[] { gradientColor, laserColor, laserColor,
//					gradientColor };
//
//			rect.set(frame.left + 2, middle - laserwidht/2, frame.right - 1, middle + laserwidht/2);
//			LinearGradient shader = new LinearGradient(rect.left, rect.top,
//					rect.right, rect.bottom, colors, null, TileMode.MIRROR);
//			laserPaint.setShader(shader);
//			canvas.drawRect(rect, laserPaint);

			//Draw Circle
			Collection<ResultPoint> currentPossible = possibleResultPoints;
			Collection<ResultPoint> currentLast = lastPossibleResultPoints;
			if (currentPossible.isEmpty()) {
				lastPossibleResultPoints = null;
			} else {
				possibleResultPoints = new HashSet<ResultPoint>(5);
				lastPossibleResultPoints = currentPossible;
				paint.setAlpha(OPAQUE);
				paint.setColor(resultPointColor);
				for (ResultPoint point : currentPossible) {
					canvas.drawCircle(frame.left + point.getX(), frame.top
							+ point.getY(), 6.0f, paint);
				}
			}
			if (currentLast != null) {
				paint.setAlpha(OPAQUE / 2);
				paint.setColor(resultPointColor);
				for (ResultPoint point : currentLast) {
					canvas.drawCircle(frame.left + point.getX(), frame.top
							+ point.getY(), 3.0f, paint);
				}
			}

			// Request another update at the animation interval, but only
			// repaint the laser line,
			// not the entire viewfinder mask.
			postInvalidateDelayed(ANIMATION_DELAY, frame.left, frame.top,
					frame.right, frame.bottom);
		}
	}

	public void drawViewfinder() {
		resultBitmap = null;
		invalidate();
	}

	/**
	 * Draw a bitmap with the result points highlighted instead of the live
	 * scanning display.
	 * 
	 * @param barcode
	 *            An image of the decoded barcode.
	 */
	public void drawResultBitmap(Bitmap barcode) {
		resultBitmap = barcode;
		invalidate();
	}

	public void addPossibleResultPoint(ResultPoint point) {
		possibleResultPoints.add(point);
	}

}
