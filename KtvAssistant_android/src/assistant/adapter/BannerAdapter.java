package assistant.adapter;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.graphics.Bitmap;
import android.support.v4.view.PagerAdapter;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import assistant.entity.Banner;
import assistant.util.ImageUtil;
import assistant.util.ShowLog;

/**
 * @author 宋静
 * @version 创建时间：2013-12-9 下午8:22:50 类说明
 */
public class BannerAdapter extends PagerAdapter {
	private List<View> mViews;
	private Context mContext;
	private List<Banner> bannerList;
	private boolean isDefault = false;

	public BannerAdapter(Context con, List<Banner> list,
			BannerClickMethod method, boolean isDefault) {
		super();
		mContext = con;
		bannerList = list;
		if (!bannerList.isEmpty()) {			
			LayoutInflater lf = LayoutInflater.from(mContext);
			mViews = new ArrayList<View>();// 将要分页显示的View装入数组中
			for (int i = 0; i < bannerList.size(); i++) {
				View view = lf.inflate(R.layout.view_image, null);
				mViews.add(view);
				
				ImageView img = (ImageView) view.findViewById(R.id.image);
				Banner banner = bannerList.get(i);
				if(banner != null){
					setImage(img, banner);
					method.click(img, banner);	
				}
			}
		}
		
		this.isDefault = isDefault;
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return bannerList.size();
	}

	@Override
	public boolean isViewFromObject(View arg0, Object arg1) {
		// TODO Auto-generated method stub
		return arg0 == arg1;
	}

	@Override
	public void destroyItem(ViewGroup container, int position, Object object) {
		// TODO Auto-generated method stub
		container.removeView(mViews.get(position));// 删除页卡
	}

	@Override
	public Object instantiateItem(ViewGroup container, int position) {
		// TODO Auto-generated method stub
		container.addView(mViews.get(position));// 添加页卡
//		ImageView img = (ImageView) mViews.get(position).findViewById(R.id.image);
//
//		Banner banner = bannerList.get(position);
//		if(banner != null){
//			setImage(img, banner);
//			method.click(img, banner);	
//		}
		
		return mViews.get(position);
	}

	public interface BannerClickMethod {
		public void click(ImageView img, Banner banner);
	}

	private void setImage(ImageView img, Banner banner) {
		try {
			/* 显示图片 */
			if (isDefault) {
				 img.setImageResource(R.drawable.ic_singer_second);
			} else {
				String pic = banner.photoUrl;

				if (!TextUtils.isEmpty(pic)) {
					Bitmap bitmap = ImageUtil.getBitmapBanner(pic);
					if (bitmap != null) 
						img.setImageBitmap(bitmap);
				}
			}
		} catch (Exception e) {
			ShowLog.e("BannerAdapter", "getView error--"
					+ ((banner == null) ? "banner == null" : banner.toString()));
		}
	}

	@Override
	public void notifyDataSetChanged() {
		// TODO Auto-generated method stub
		super.notifyDataSetChanged();

		if (mViews.isEmpty())
			return;

		for (int i = 0; i < mViews.size(); i++) {
			ImageView img = (ImageView) mViews.get(i).findViewById(R.id.image);
			Banner banner = bannerList.get(i);
			if(banner != null){
				setImage(img, banner);
			}
		}
	}

	public List<Banner> getBannerList() {
		return bannerList;
	}

}
