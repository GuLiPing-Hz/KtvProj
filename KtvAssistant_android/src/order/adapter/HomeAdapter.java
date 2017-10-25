package order.adapter;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

/**
 * @author 宋静
 * @version 创建时间：2013-12-9 下午8:59:57 类说明 点歌首页菜单适配器（写死的）
 */
public class HomeAdapter extends BaseAdapter {
	private Context mContext;
	private List<ModelType> list = new ArrayList<ModelType>();

	public HomeAdapter(Context mContext, List<ModelType> types) {
		super();
		this.mContext = mContext;
		this.list = types;
		
	}

	@Override
	public int getCount() {
		return list.size();
	}

	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return list.get(arg0);
	}

	@Override
	public long getItemId(int position) {
		return list.get(position).getType();
	}

	@SuppressLint("ResourceAsColor")
	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// set view
		ViewHolder holder;
		if (convertView == null) {
			convertView = LayoutInflater.from(mContext).inflate(
					R.layout.item_order_home_menunew, null);
			holder = new ViewHolder();
			holder.image = (ImageView) convertView.findViewById(R.id.icon);
			holder.title1 = (TextView) convertView.findViewById(R.id.title1);
			holder.content1 = (TextView) convertView.findViewById(R.id.info1);
			holder.ktvroomname1 = (TextView) convertView.findViewById(R.id.ktvroomname);
			convertView.setTag(holder);
		} 
		else
			holder = (ViewHolder) convertView.getTag();

		// set data
		ModelType type = list.get(position);
		holder.title1.setText(type.getName());

		// holder.title.setTextColor(getTitleTextColorByPosition(position));
		holder.content1.setText(type.getContent());
		holder.image.setImageResource(getIconResourceByTypeId(type));

		return convertView;
	}

	static class ViewHolder {
		ImageView image;
		TextView title1;
		TextView content1;
		TextView ktvroomname1;
	}

	private int getIconResourceByTypeId(ModelType type) {
		int res = 0;
		switch (type) {
		case RANKLIST:
			res = R.drawable.index_pic_rank;
			break;
		case SINGERS:
			res = R.drawable.index_pic_singerclass;
			break;
		case TOPIC:
			res = R.drawable.pic_three;
			break;
		case HOTCHART:
			res = R.drawable.pic_four;
			break;

		default:
			res = R.drawable.ic_orderhome_list;
			break;
		}

		return res;
	}

	// 2.0模块id定义
	public enum ModelType {
		NONE(0, "无", "无"), 
		NEWSONG(1, "新曲", "新曲"), 
		PINYIN(2, "拼音", "拼音"), 
		SINGERS(3, "歌星", "人气歌手一网打尽！"), 
		RANKLIST(4, "排行榜","歌曲上榜即可领积分，兑换精美礼品！"), // 排行榜
		LANGUAGE(5, "语种", "语种"),
		SONGTYPE(6, "曲种", "曲种"), 
		TOPIC(7, "精选专题","邂逅最美声音，获取最新歌曲！"),
		HOTCHART(8, "热门排行", "小伙伴都在唱什么歌曲，你可别out喽！"), 
		THEME(9, "主题", "主题点歌");

		private int type;
		private String name;
		private String content;

		private ModelType(int type, String name, String content) {
			this.type = type;
			this.name = name;
			this.content = content;
		}

		public String getName() {
			return this.name;
		}

		public int getType() {
			return this.type;
		}

		public String getContent() {
			return this.content;
		}

		public static ModelType getTypeByName(String name) {
			for (ModelType type : values()) {
				if (type.name.equals(name)) {
					return type;
				}
			}
			return NONE; // 不应该执行到这里。。。。
		}

		public static ModelType getTypeByInt(int mIntType) {
			for (ModelType type : values()) {
				if (type.type == mIntType) {
					return type;
				}
			}
			return NONE; // 不应该执行到这里。。。。
		}

		public static String getName(int mIntType) {
			for (ModelType type : values()) {
				if (type.type == mIntType) {
					return type.name;
				}
			}
			return NONE.name; // 不应该执行到这里。。。。
		}

		public static int getInt(String name) {
			for (ModelType type : values()) {
				if (type.name.equals(name)) {
					return type.type;
				}
			}
			return NONE.type; // 不应该执行到这里。。。。
		}

	}

}
