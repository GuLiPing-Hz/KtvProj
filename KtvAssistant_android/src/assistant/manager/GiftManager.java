package assistant.manager;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import assistant.global.AppStatus;

import com.jni.netutil.GiftInfo;

public class GiftManager {

	public final static int GIFTNUMBER_PER_PAGE = 8;
	
	boolean m_bInitGiftInfo = false;
	
	List<GiftInfo> m_giftInfoListNormal = new ArrayList<GiftInfo>();
	int m_giftNormalRemain = 0;
	public int m_giftNormalPageTotal = 0;
	List<GiftInfo> m_giftInfoListLuck = new ArrayList<GiftInfo>();
	int m_giftLuckRemain = 0;
	public int m_giftLuckPageTotal = 0;
	List<GiftInfo> m_giftInfoListLuxury = new ArrayList<GiftInfo>();
	int m_giftLuxuryRemain = 0;
	public int m_giftLuxuryPageTotal = 0;

	public int m_pageIndexNormal;
	public int m_pageIndexLuck;
	public int m_pageIndexLuxury;
	
	Comparator<GiftInfo> comparatorOrder = new Comparator <GiftInfo>(){

		@Override
		public int compare(GiftInfo arg0, GiftInfo arg1) {
			// TODO Auto-generated method stub
			return (arg0.order - arg1.order);
		}
		
	};
	public GiftManager() {
		// TODO Auto-generated constructor stub
	}
	//非强制初始化
	public void initGiftInfo(){
		initGiftInfo(false);
	}
	public void initGiftInfo(boolean isForce){
		//非强制更新，并且已经初始化完毕的话，
		if(!isForce && m_bInitGiftInfo)
			return ;
		
		if(m_bInitGiftInfo)
		{
			m_giftInfoListLuck.clear();
			m_giftInfoListNormal.clear();
			m_giftInfoListLuxury.clear();
		}
		
		int[] giftIds = AppStatus.s_NetUtilMgr.m_JniUtil.GetAllGiftId();
		for(int i=0;i<giftIds.length;i++)
		{
			int giftId = giftIds[i];
			GiftInfo info = AppStatus.s_NetUtilMgr.m_JniUtil.GetGiftInfoFromLocal(giftId);
			if(info == null)
				continue;
			switch(info.gift_type)
			{
			case 1:
				m_giftInfoListLuck.add(info);
				break;
			case 2:
				m_giftInfoListNormal.add(info);
				break;
			case 3:
				m_giftInfoListLuxury.add(info);
				break;
			}
		}
		
		m_giftLuckRemain = m_giftInfoListLuck.size()%GIFTNUMBER_PER_PAGE;
		m_giftLuckPageTotal = m_giftInfoListLuck.size()/GIFTNUMBER_PER_PAGE + (m_giftLuckRemain!=0?1:0);
		m_giftNormalRemain = m_giftInfoListNormal.size()%GIFTNUMBER_PER_PAGE;
		m_giftNormalPageTotal = m_giftInfoListNormal.size()/GIFTNUMBER_PER_PAGE+(m_giftNormalRemain!=0?1:0);
		m_giftLuxuryRemain = m_giftInfoListLuxury.size()%GIFTNUMBER_PER_PAGE;
		m_giftLuxuryPageTotal = m_giftInfoListLuxury.size()/GIFTNUMBER_PER_PAGE+(m_giftLuxuryRemain!=0?1:0);
		
		m_pageIndexNormal = m_giftNormalPageTotal;
		m_pageIndexLuck = m_pageIndexNormal+m_giftLuckPageTotal;
		m_pageIndexLuxury = m_pageIndexLuck+m_giftLuxuryPageTotal;
		
		Collections.sort(m_giftInfoListLuck,comparatorOrder);
		Collections.sort(m_giftInfoListNormal,comparatorOrder);
		Collections.sort(m_giftInfoListLuxury,comparatorOrder);
		m_bInitGiftInfo = true;
	}

	public boolean isInit(){
		return m_bInitGiftInfo;
	}

	public GiftInfo getGiftInfo(int page,int pos){
		if(0<=page && page<m_pageIndexNormal)
			return getGiftInfoNormal(page,pos);
		else if(page<m_pageIndexLuck)
			return getGiftInfoLuck(page-m_pageIndexNormal,pos);
		else if(page<m_pageIndexLuxury)
			return getGiftInfoLuxury(page-m_pageIndexLuck,pos);
		return null;
	}
	
	public int getGiftSize(int page){
		if(0<=page && page<m_pageIndexNormal)
			return getGiftNormalSize(page);
		else if(page<m_pageIndexLuck)
			return getGiftLuckSize(page-m_pageIndexNormal);
		else if(page<m_pageIndexLuxury)
			return getGiftLuxurySize(page-m_pageIndexLuck);
		return 0;
	}
	
	public int getGiftTotal(){
		return (m_giftLuckPageTotal+m_giftNormalPageTotal+m_giftLuxuryPageTotal);
	}
	/*
	 *page 从0开始 
	*/
	private int getGiftNormalSize(int page){
		if(page<0 || page>m_giftNormalPageTotal)
			return 0;
		if(m_giftNormalRemain == 0 || page < m_giftNormalPageTotal-1)
			return GIFTNUMBER_PER_PAGE;
		else
			return m_giftNormalRemain;
	}
	private GiftInfo getGiftInfoNormal(int page,int pos) {
		int index = page*GIFTNUMBER_PER_PAGE+pos;
		if(index >= m_giftInfoListNormal.size() || index <0)
			return null;
		return m_giftInfoListNormal.get(index);
	}

	private int getGiftLuckSize(int page){
		if(page<0 || page>m_giftLuckPageTotal)
			return 0;
		
		if(m_giftLuckRemain == 0 || page < m_giftLuckPageTotal-1)
			return GIFTNUMBER_PER_PAGE;
		else
			return m_giftLuckRemain;
	}
	private GiftInfo getGiftInfoLuck(int page,int pos) {
		int index = page*GIFTNUMBER_PER_PAGE+pos;
		if(index >= m_giftInfoListLuck.size() || index <0)
			return null;
		return m_giftInfoListLuck.get(index);
	}
	
	private int getGiftLuxurySize(int page){
		if(page<0 || page>m_giftLuxuryPageTotal)
			return 0;
		
		if(m_giftLuxuryRemain == 0 || page < m_giftLuxuryPageTotal-1)
			return GIFTNUMBER_PER_PAGE;
		else
			return m_giftLuxuryRemain;
	}
	private GiftInfo getGiftInfoLuxury(int page,int pos) {
		int index = page*GIFTNUMBER_PER_PAGE+pos;
		if(index >= m_giftInfoListLuxury.size() || index <0)
			return null;
		return m_giftInfoListLuxury.get(index);
	}
}
