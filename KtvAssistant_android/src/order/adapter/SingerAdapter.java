package order.adapter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.regex.Pattern;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import assistant.entity.TopicInfo;

/**
 * @author 宋静
 * @version 创建时间：2013-10-25 上午11:50:34
 * 类说明 歌星列表的适配器
 */
public class SingerAdapter extends BaseAdapter{
	private Context mContext;
	private List<TopicInfo> list = new ArrayList<TopicInfo>();
	private HashMap<String, Integer> alphaIndexer;// 存放存在的汉语拼音首字母和与之对应的列表位置
	private String[] sections;// 存放存在的汉语拼音首字母
	
	public SingerAdapter(Context con, List<TopicInfo> artists) {
		this.mContext = con;
		if (artists != null && artists.size() > 0)
			this.list = artists;
		
		checkAlpha();
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return list.size();
	}

	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return list.get(arg0);
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		//set view
		ViewHolder holder;
		if (convertView == null) {
			convertView = LayoutInflater.from(mContext).inflate(
					R.layout.item_singer, null);
			holder = new ViewHolder();
			holder.alpha = (TextView) convertView.findViewById(R.id.tv_letter);
			holder.text = (TextView) convertView.findViewById(R.id.tv_name);
			convertView.setTag(holder);
		} else {
			holder = (ViewHolder) convertView.getTag();
		}
		
		//set data
		TopicInfo singer = list.get(position);
		
		if (singer != null && !TextUtils.isEmpty(singer.topicTitle)) {
			holder.text.setText(singer.topicTitle);
		}

		if (singer != null && !TextUtils.isEmpty(singer.pinyin)) {
			String currentStr = getAlpha(singer.pinyin);
			String previewStr = (position - 1) >= 0 ? getAlpha(list
					.get(position - 1).pinyin) : " ";
			if (!previewStr.equals(currentStr)) {
				holder.alpha.setVisibility(View.VISIBLE);
				holder.alpha.setText(currentStr);
			} else {
				holder.alpha.setVisibility(View.GONE);
			}
		}

		
		return convertView;
	}
	
	private static class ViewHolder{
		TextView alpha;
		TextView text;
	}

	//获得汉语拼音首字母
    private String getAlpha(String str) {  
        if (TextUtils.isEmpty(str)) {  
            return "#";  
        }  
  
        str = str.trim();
        
        if (str.length() == 0) {  
            return "#";  
        }
  
        char c = str.substring(0, 1).charAt(0);  
        // 正则表达式，判断首字母是否是英文字母  
        Pattern pattern = Pattern.compile("^[A-Za-z]+$");  
        if (pattern.matcher(c + "").matches()) {  
            return (c + "").toUpperCase();  
        } else {  
            return "#";  
        }  
    } 

    private void checkAlpha(){
    	alphaIndexer = new HashMap<String, Integer>();
		sections = new String[list.size() > 0 ? list.size() : 1];

		for (int i = 0; i < list.size(); i++) {
			TopicInfo singer = list.get(i);
			if (singer == null){
				list.remove(i);
				i--;
				continue;
			}
			
			if(TextUtils.isEmpty(singer.pinyin))
				continue;
				
			// 当前汉语拼音首字母
			String currentStr = getAlpha(singer.pinyin);
			// 上一个汉语拼音首字母，如果不存在为“ ”
			String previewStr = (i - 1) >= 0 ? getAlpha(list.get(i - 1).pinyin)
					: " ";
			if (!previewStr.equals(currentStr)) {
				alphaIndexer.put(currentStr, i);
				sections[i] = currentStr;
			}
		}
    }

    public HashMap<String, Integer> getAlphaIndexer() {
		return alphaIndexer;
	}
    
	public String[] getSections() {
		return sections;
	}

	@Override
	public void notifyDataSetChanged() {
		// TODO Auto-generated method stub
		super.notifyDataSetChanged();
		
		checkAlpha();
	}
	
}
