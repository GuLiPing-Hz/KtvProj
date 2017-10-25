package assistant.view;

import android.graphics.Camera;
import android.graphics.Matrix;
import android.view.animation.Animation;
import android.view.animation.Transformation;

/**
 * @author 肖平平
 * @version 创建时间：2013-12-25 上午9:40:28
 * 类说明  3d旋转
 */
public class Rotate3dAnimation extends Animation{
	private final float mFromDegrees;//开始的角度
	private final float mToDegrees;//结束的角度
	private final float mCenterX;//x轴
	private final float mCenterY;//y轴
	private float mDepthZ = 0.0f;//z轴
	private boolean mReverse = true;//是否翻转
	private Camera mCamera;
	
	public Rotate3dAnimation(float fromDegrees, float toDegrees,
			float centerX, float centerY) {
		mFromDegrees = fromDegrees;  
        mToDegrees = toDegrees;  
        mCenterX = centerX;  
        mCenterY = centerY;
	}
	/**
	 * 构造
	 * @param fromDegrees
	 * @param toDegrees
	 * @param centerX
	 * @param centerY
	 * @param depthZ
	 * @param reverse
	 */
	public Rotate3dAnimation(float fromDegrees, float toDegrees,
			float centerX, float centerY, float depthZ, boolean reverse) {
		mFromDegrees = fromDegrees;  
        mToDegrees = toDegrees;  
        mCenterX = centerX;  
        mCenterY = centerY;  
        mDepthZ = depthZ;  
        mReverse = reverse;
	}

	@Override
	protected void applyTransformation(float interpolatedTime, Transformation t) {
		//super.applyTransformation(interpolatedTime, t);
		float fromDegrees = mFromDegrees;  
        //旋转角度
        float degrees = fromDegrees + ((mToDegrees - fromDegrees) * interpolatedTime);  
        float centerX = mCenterX;  
        float centerY = mCenterY;  
        Camera camera = mCamera;  
        Matrix matrix = t.getMatrix(); 
        camera.save(); 
        
//        if(mDepthZ != 0.0f)
//        {
//        	 if (mReverse) {  
//                 // z的偏移会越来越大。这就会形成这样一个效果，view从近到远  
//                 camera.translate(0.0f, 0.0f, mDepthZ * interpolatedTime);  
//             } else {  
//                 // z的偏移会越来越小。这就会形成这样一个效果，我们的View从一个很远的地方向我们移过来，越来越近，最终移到了我们的窗口上面～  
//                 camera.translate(0.0f, 0.0f, mDepthZ * (1.0f - interpolatedTime));  
//             } 
//        }
        
        //在Y轴上面旋转
        camera.rotateY(degrees);  
        //取得变换后的矩阵  
        camera.getMatrix(matrix);  
        camera.restore(); 
        matrix.preTranslate(-centerX, -centerY);  
        matrix.postTranslate(centerX, centerY);  
	}

	@Override
	public void initialize(int width, int height, int parentWidth,
			int parentHeight) {
		// TODO Auto-generated method stub
		super.initialize(width, height, parentWidth, parentHeight);
		 mCamera = new Camera(); 
	}
}
