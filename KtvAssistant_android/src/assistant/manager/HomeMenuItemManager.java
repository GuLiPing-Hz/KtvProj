package assistant.manager;

import java.util.ArrayList;

import tiange.ktv.assistant.R;
import android.content.Context;
import assistant.entity.HomeMenuItem;

public class HomeMenuItemManager {
	
//	//首页
//	public final static int MENU_INDEX = 0;
//	//排行榜
//	public final static int MENU_RANK = 1;
//	//互动聊天
//	public final static int MENU_CHAT = 2;
//	//兑换礼物
//	public final static int MENU_EXANGEGIFT = 3;
//	//包厢预订
//	public final static int MENU_ORDERROOM = 4;
//	//购买金币
//	public final static int MENU_TOPUP = 5;
//	//个人中心
//	public final static int MENU_PERSON = 6;
//	//设置
//	public final static int MENU_SET = 7;
	
	//默认主页
	public final static int MENU_MAIN_INDEX = -1;
	//账户充值
	public final static int MENU_ACCOUNT_RECHARGE = 0;
	//礼物兑换
	public final static int MENU_GIFT_EXCHANGE = 1;
	//消费记录
	//public final static int MENU_CONSUME_RECORD = 2;
	//唱过的歌
	public final static int MENU_SONG_RECORD = 2;
	//歌曲收藏
	public final static int MENU_SONGS = 3;
	//设置
	public final static int MENU_SETTINGS = 4;
//	//互动聊天
//	public final static int MENU_CHAT = 5;
//	//排行榜
//	public final static int MENU_RANK = 6;
//	//包厢预订
//	public final static int MENU_ORDERROOM = 7;
//	//个人中心
//	public final static int MENU_PERSON = 8;
	
	private ArrayList<HomeMenuItem> m_HomeMenuItemList;
	
	private static HomeMenuItemManager s_HomeMenuItemLab;
	
	private HomeMenuItemManager(Context c){
		m_HomeMenuItemList = new ArrayList<HomeMenuItem>();
		//账户充值
		m_HomeMenuItemList.add(new HomeMenuItem(R.string.menuitem_buymoney,R.drawable.sidemenu_icon_charge
				,R.color.bg_item_selected,R.drawable.bg_item_cehua));
		//礼物兑换
		m_HomeMenuItemList.add(new HomeMenuItem(R.string.menuitem_giftexchange,R.drawable.sidemenu_icon_coin_exchange
				,R.color.bg_item_selected,R.drawable.bg_item_cehua));
		//消费记录 - 暂时不用，因为没有扫码消费
		//m_HomeMenuItemList.add(new HomeMenuItem(R.string.menuitem_consume,R.drawable.sidemenu_icon_consume_record
				//,R.color.bg_item_selected,R.drawable.bg_item_cehua));
		//唱过的歌
		m_HomeMenuItemList.add(new HomeMenuItem(R.string.menuitem_songrecord,R.drawable.sidemenu_icon_song_record
				,R.color.bg_item_selected,R.drawable.bg_item_cehua));
		//歌曲收藏
		m_HomeMenuItemList.add(new HomeMenuItem(R.string.menuitem_favourite,R.drawable.ic_collect
				,R.color.bg_item_selected,R.drawable.bg_item_cehua));
		//首页
		//m_HomeMenuItemList.add(new HomeMenuItem(R.string.menuitem_home,R.drawable.sidemenu_icon_index
				//,R.color.bg_item_selected,R.drawable.bg_item_cehua));
		//排行榜
		//m_HomeMenuItemList.add(new HomeMenuItem(R.string.menuitem_rank,R.drawable.sidemenu_icon_rank
				//,R.color.bg_item_selected,R.drawable.bg_item_cehua));
		//互动聊天
		//m_HomeMenuItemList.add(new HomeMenuItem(R.string.menuitem_chat,R.drawable.sidemenu_icon_chat
				//,R.color.bg_item_selected,R.drawable.bg_item_cehua));
		//包厢预订
		//m_HomeMenuItemList.add(new HomeMenuItem(R.string.menuitem_bookroom,R.drawable.sidemenu_icon_room
				//,R.color.bg_item_selected,R.drawable.bg_item_cehua));
		//个人中心
		//m_HomeMenuItemList.add(new HomeMenuItem(R.string.menuitem_my,R.drawable.sidemenu_icon_user
				//,R.color.bg_item_selected,R.drawable.bg_item_cehua));
		//设置
		m_HomeMenuItemList.add(new HomeMenuItem(R.string.menuitem_setting,R.drawable.sidemenu_icon_setting
				,R.color.bg_item_selected,R.drawable.bg_item_cehua));
				
	}
	
	public static HomeMenuItemManager getSingleInstance(Context c){
		if(s_HomeMenuItemLab == null)
		{
			s_HomeMenuItemLab = new HomeMenuItemManager(c.getApplicationContext());
		}
		return s_HomeMenuItemLab;
	}

	public ArrayList<HomeMenuItem> getHomeMenuItemList() {
		return m_HomeMenuItemList;
	}
}
