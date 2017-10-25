package assistant.entity;

public class HomeMenuItem {
	int m_StringId;//标题
	int m_IconId;//图标
	int m_SelectColorId;//选中的颜色
	int m_DrawableId;//用于变化的颜色选择器
	
	public HomeMenuItem(int stringId, int iconId, int selectColorId,
			int drawableId) {
		super();
		m_StringId = stringId;
		m_IconId = iconId;
		m_SelectColorId = selectColorId;
		m_DrawableId = drawableId;
	}

	public int getStringId() {
		return m_StringId;
	}

	public void setStringId(int stringId) {
		m_StringId = stringId;
	}

	public int getIconId() {
		return m_IconId;
	}

	public void setIconId(int iconId) {
		m_IconId = iconId;
	}

	public int getSelectColorId() {
		return m_SelectColorId;
	}

	public void setSelectColorId(int selectColorId) {
		m_SelectColorId = selectColorId;
	}

	public int getDrawableId() {
		return m_DrawableId;
	}

	public void setDrawableId(int drawableId) {
		m_DrawableId = drawableId;
	}
}
