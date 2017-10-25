package assistant.entity;

import java.io.Serializable;

import org.json.JSONObject;

import android.text.TextUtils;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

/**
 * @author 宋静
 * @version 创建时间：2013-12-5 上午10:25:26
 * 类说明 第三方账户信息
 */
public class ThirdAccount implements Serializable {

	private static final long serialVersionUID = 1L;
	
	public AccountType type;
	public long idx; //9158的idx
	public String openid; // 账号userid(腾讯叫openid)
	public String token; // 账号accessToken
	public String freshtoken; // 账号freshToken
	public long expires = 0;		//过期时间点（时间戳，单位：秒）
	public String sessionKey; // 账号sessionKey
	public String sessionSecret; // 账号sessionSecret
	public String nickname; // 第三方账号平台的账号昵称

	public ThirdAccount(AccountType type, String openid, String token) {
		super();
		this.type = type;
		this.openid = openid;
		this.token = token;
	}
	
	public ThirdAccount(AccountType type, String openid, String token,
			long expires) {
		super();
		this.type = type;
		this.openid = openid;
		this.token = token;
		this.expires = expires;
	}

	public ThirdAccount(AccountType type, long idx, String openid,
			String token, long expires) {
		super();
		this.type = type;
		this.idx = idx;
		this.openid = openid;
		this.token = token;
		this.expires = expires;
	}
	
	/** 登录task使用 */
	public ThirdAccount(JSONObject jsonObject) {

		try {
			if (null != jsonObject) {
				this.openid = jsonObject.optString("accountid");
				this.token = jsonObject.optString("accesstoken");
				this.expires = jsonObject.optLong("expirationdate");
				this.nickname = PCommonUtil.decodeBase64(jsonObject
						.optString("nickname"));
			}

		} catch (Exception e) {
			if (ShowLog.isShow())
				e.printStackTrace();
		}

	}

	/** 本地读取用 */
	public ThirdAccount(String str) {
		if (TextUtils.isEmpty(str))
			return;

		try {
			JSONObject json = new JSONObject(str);

			if (null != json) {
				String name = json.optString("accountName");
				this.type = AccountType.getTypeByName(name);

				idx = json.optLong("idx");
				openid = json.optString("openid");
				nickname = json.optString("nickname");
				token = json.optString("token");
				freshtoken = json.optString("freshtoken");
				expires = json.optLong("expires");
				sessionKey = json.optString("sessionKey");
				sessionSecret = json.optString("sessionSecret");
			}
		} catch (Exception e) {
			ShowLog.showException(e);
		}
	}

	/** 本地记录用 */
	public String write() {
		String result = null;

		try {
			JSONObject json = new JSONObject();

			json.put("accountName", type.getAccountName());
			json.put("idx", idx);
			json.put("openid", openid);
			json.put("nickname", nickname);
			json.put("token", token);
			json.put("freshtoken", freshtoken);
			json.put("expires", expires);
			json.put("sessionKey", sessionKey);
			json.put("sessionSecret", sessionSecret);

			result = json.toString();

		} catch (Exception e) {
			ShowLog.showException(e);
		}

		return result;
	}

	public enum AccountType {
		NONE("无", 0), SINA("新浪微博", 1), QQ("QQ", 2), KTV("KTV",3),WEIXIN("微信",4);//5微信公众号注册账号

		private String accountName;
		private int type;

		AccountType(String accountName, int type) {
			this.accountName = accountName;
			this.type = type;
		}

		public String getAccountName() {
			return this.accountName;
		}

		public int getType() {
			return this.type;
		}

		public static AccountType getTypeByName(String name) {
			for (AccountType accountType : values()) {
				if (accountType.accountName.equals(name)) {
					return accountType;
				}
			}
			return NONE; // 不应该执行到这里。。。。
		}

		public static AccountType getTypeByInt(int mIntType) {
			for (AccountType accountType : values()) {
				if (accountType.type == mIntType) {
					return accountType;
				}
			}
			return NONE; // 不应该执行到这里。。。。
		}

	}

	@Override
	public String toString() {
		return "OtherUserAccount [type=" + type.accountName + ", idx=" + idx + ", openid="
				+ openid + ", token=" + token + ", freshtoken=" + freshtoken
				+ ", expires=" + expires + ", sessionKey=" + sessionKey
				+ ", sessionSecret=" + sessionSecret + ", nickname=" + nickname
				+ "]";
	}
}

