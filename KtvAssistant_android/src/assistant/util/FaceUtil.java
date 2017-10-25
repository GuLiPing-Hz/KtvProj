package assistant.util;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.drawable.Drawable;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.TextUtils;
import android.text.style.ImageSpan;

public class FaceUtil {

	public final static float faceScale = 0.6f;
	
	Context m_context;
	//键值与文件名称  举例 [a/1] —— q1 (q1.png) 以后希望能改的更好吧
	Map<String,String> m_mapSigFace = new HashMap<String,String>();
	@SuppressLint("UseSparseArrays")
	Map<Integer,String> m_mapFaceidSig = new HashMap<Integer,String>();
	
	public FaceUtil(Context context) {
		// TODO Auto-generated constructor stub
		m_context = context.getApplicationContext();
		
		try
		{
			InputStream in = context.getResources().getAssets().open("facemap");
			BufferedReader br = new BufferedReader(new InputStreamReader(in,"UTF-8"));
			String str = null;
			while ((str = br.readLine()) != null) {
				String[] strs = str.split(";");
				m_mapSigFace.put(strs[0], strs[1]);
				m_mapFaceidSig.put(m_context.getResources().getIdentifier(strs[1],"drawable",m_context.getPackageName())
						, strs[0]);
	 		}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}

	public boolean isFaceSig(String sig){
		return m_mapSigFace.containsKey(sig);
	}
	public String getFaceIdentify(int id){
		if(m_mapFaceidSig.containsKey(id))
			return m_mapFaceidSig.get(id);
		return "";
	}
	/*
	 * 解析字符串str，对其中所有的表情进行转义
	 * 返回一段富含表情文字的SpannableString
	 * 用于在TextView中显示
	 */
	public SpannableString getRichString(String str){
		SpannableString spannableString = new SpannableString(str);
		// 正则表达式比配字符串里是否含有表情，如： 我好[q1]啊
		// 代码原因需要转义\,实际书写的正则是 \[[^\]]+\] 意思是匹配"["，中间匹配(不是"]")一次或多次,后面匹配"]"
		String zhengze = "\\[[^\\]]+\\]";
		// 通过传入的正则表达式来生成一个pattern
		Pattern sinaPatten = Pattern.compile(zhengze, Pattern.CASE_INSENSITIVE);
		try {
			dealExpression(spannableString, sinaPatten, 0);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return spannableString;
	}
	/*
	 * 获取指定字符串包含表情元素的SpannableString，
	 * @param id[in]：指定该字符串指定的表情图片 R.drawable.id
	 */
	public SpannableString getFaceString(int id){
		String identify = getFaceIdentify(id);
		if(TextUtils.isEmpty(identify))
			return null;
		return getFaceString(id,identify);
	}
	/*
	 * 获取指定字符串包含表情元素的SpannableString，
	 * @param id[in]：指定该字符串指定的表情图片 R.drawable.id
	 * @param strIdendify[in] : 指定字符串 [xxx]
	 * 
	 * 注：默认strIdendify只有一个表情对应
	 */
	public SpannableString getFaceString(int id,String strIdendify) {
		if (TextUtils.isEmpty(strIdendify)) {
			return null;
		}
		
		Drawable drawable = m_context.getResources().getDrawable(id);
		drawable.setBounds(0,0,(int)(drawable.getIntrinsicWidth()*faceScale), (int)(drawable.getIntrinsicHeight()*faceScale));
		ImageSpan imageSpan = new ImageSpan(drawable);
		SpannableString spannable = new SpannableString(strIdendify);
		spannable.setSpan(imageSpan, 0, strIdendify.length(),Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
		return spannable;
	}
	
	private void dealExpression(SpannableString spannableString, Pattern patten, int start)
			throws Exception {
		Matcher matcher = patten.matcher(spannableString);
		while (matcher.find()) {
			String key = matcher.group();
			// 返回第一个字符的索引的文本匹配整个正则表达式,ture 则继续递归
			if (matcher.start() < start) {
				continue;
			}
			
			int startOffset = 0;
			String value = m_mapSigFace.get(key);
			if (TextUtils.isEmpty(value)) {
				
				startOffset = key.lastIndexOf("[");
				key = key.substring(startOffset);
				value = m_mapSigFace.get(key);
				if(TextUtils.isEmpty(value))
				{
					continue;
				}
			}
			int resId = m_context.getResources().getIdentifier(value, "drawable",m_context.getPackageName());
			if (resId != 0) {
				Drawable drawable = m_context.getResources().getDrawable(resId);
				drawable.setBounds(0, 0, (int)(drawable.getIntrinsicWidth()*faceScale), (int)(drawable.getIntrinsicHeight()*faceScale));
				ImageSpan imageSpan = new ImageSpan(drawable);
				// 计算该图片名字的长度，也就是要替换的字符串的长度
				int startNew = matcher.start()+startOffset;
				int end = startNew + key.length();
				// 将该图片替换字符串中规定的位置中
				spannableString.setSpan(imageSpan, startNew, end,Spannable.SPAN_INCLUSIVE_EXCLUSIVE);
				if (end < spannableString.length()) {
					// 如果整个字符串还未验证完，则继续。。
					dealExpression(spannableString, patten, end);
				}
				break;
			}
		}
	}
	
	public static boolean containsEmoji(String source) {
        if (source==null || TextUtils.isEmpty(source)) 
            return false;

        int len = source.length();

        for (int i = 0; i < len; i++) {
            char codePoint = source.charAt(i);

            if (!isNotEmojiCharacter(codePoint))
                return true;
        }

        return false;
    }

    private static boolean isNotEmojiCharacter(char codePoint) {
        return (codePoint == 0x0) ||
                (codePoint == 0x9) ||
                (codePoint == 0xA) ||
                (codePoint == 0xD) ||
                ((codePoint >= 0x20) && (codePoint <= 0xD7FF)) ||
                ((codePoint >= 0xE000) && (codePoint <= 0xFFFD)) ||
                ((codePoint >= 0x10000) && (codePoint <= 0x10FFFF));
    }
}
