package assistant.entity;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.json.JSONArray;
import org.json.JSONObject;

import android.text.TextUtils;
import assistant.entity.ThirdAccount.AccountType;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

/**
 * @author 宋静
 * @version 创建时间：2013-12-4 下午2:56:18 类说明 用户账户信息
 */
public class UserInfo {
	public long userIdx;//uid
	public String xgId = "";//对应的星光ID
	public String xgPwd = "";//对应的星光密码
	/*
	 * xgDomain ：星光头像所在的域名，如果用户数据头像里面含有http则是第三方的头像（微博，腾讯）
	 * 则不需要使用这个域名，如果没有则使用 xgDomain+headurl 作为头像
	 */
	public String xgDomain = "";
	public String nickname;// 昵称
	public String headphoto;// 头像
	public int score; // 积分
	public int gold;//金币
	public int gender; // 0：女 1：男
	public int accountType;	// 1: 新浪，2：腾讯 ,3:手机注册 ，4:微信
	private Map<AccountType, ThirdAccount> accounts = new HashMap<AccountType, ThirdAccount>(); // 账号列表
	private int loginAccountNum = 0;
	public String token; // 账号accessToken
	public int canAutoLogin = 1;//是否能自动登陆,1能；0不能
	public int totalScore = 0;//歌曲演唱总分
	
	public UserInfo(long userIdx, String nickname, String headphoto, int point,
			int gender) {
		super();
		this.userIdx = userIdx;
		this.nickname = nickname;
		this.headphoto = headphoto;
		this.score = point;
		this.gender = gender;
	}

	/** 登录task使用 */
	public UserInfo(JSONObject jsonObject) {
		update(jsonObject);
	}
	
	@Override
	public String toString() {
		return "UserInfo [userIdx=" + userIdx + ", nickname=" + nickname
				+ ", headphoto=" + headphoto + ", score=" + score + ", gender="
				+ gender + "]";
	}

	public UserInfo() {
		this.userIdx = 0L;
		this.headphoto = "";
		this.nickname = "";
		this.score = 0;
		this.gender = 0;
		this.accountType = 0;
	}

	// ================== 第三方账户 ========================= //
	public Map<AccountType, ThirdAccount> getAccounts() {
		return accounts;
	}

	public void setAccounts(Map<AccountType, ThirdAccount> accounts) {
		if (accounts != null && accounts.size() > 0)
			this.accounts = accounts;
		// checkLoginAccounts();
	}

	/** 检查是否绑定了账号，isDeleted为true的话，在检查之后如果存在账号，删除之 */
	public boolean isBindAccount(AccountType type, boolean isDeleted) {
		if (type != null && accounts.size() > 0) {
			ThirdAccount a = accounts.get(type);
			if (a != null && a.type == type) {
				if (!TextUtils.isEmpty(a.token)) {
					if (isDeleted && canUnbindAccount(type))
						accounts.remove(type);
					return true;
				} else {
					accounts.remove(type);
				}
			}
		}

		return false;
	}

	public boolean bindAccount(ThirdAccount account) {
		if (account != null && account.type != null
				&& !TextUtils.isEmpty(account.token))
			accounts.put(account.type, account);

		return false;
	}

	public boolean unbindAccount(AccountType type) {
		if (type != null)
			return isBindAccount(type, true);
		else
			return false;
	}

	public ThirdAccount getThirdAccount(AccountType type) {
		if (type != null && accounts.size() > 0) {
			return accounts.get(type);
		}
		return null;
	}
	
	public ThirdAccount getThirdAccount(int iType) {
		AccountType type = AccountType.getTypeByInt(iType);
		return getThirdAccount(type);
	}

	public boolean canUnbindAccount(AccountType type) {
		checkLoginAccounts();
		if ((type == AccountType.SINA || type == AccountType.QQ)
				&& loginAccountNum <= 1)
			return false;
		return true;
	}

	public void checkLoginAccounts() {
		loginAccountNum = 0;
		if (accounts != null && accounts.size() > 0) {
			if (accounts.containsKey(AccountType.SINA)) {
				loginAccountNum++;
			}

			if (accounts.containsKey(AccountType.QQ)) {
				loginAccountNum++;
			}
		}
	}

	/** 本地读取 */
	public UserInfo(String str) {
		if (TextUtils.isEmpty(str))
			return;

		try {
			JSONObject json = new JSONObject(str);

			if (null != json) {
				userIdx = json.optLong("userIdx");
				xgId = json.optString("xgid");
				xgPwd = json.optString("pwd");
				xgDomain = json.optString("domin");
				nickname = json.optString("nickname");
				gender = json.optInt("gender");
				headphoto = json.optString("headphoto");
				//score = json.optInt("score");
				accountType = json.optInt("accountType");
				loginAccountNum = json.optInt("loginAccountNum");
				canAutoLogin = json.optInt("canAutoLogin",1);
				totalScore = json.optInt("TotalScore");

				JSONArray array = json.optJSONArray("accounts");
				int num = (array == null ? 0 : array.length());
				if (num <= 0)
					return;

				accounts = new HashMap<AccountType, ThirdAccount>();
				for (int i = 0; i < num; i++) {
					String item = (String)array.get(i);
					if (TextUtils.isEmpty(item))
						continue;

					ThirdAccount a = new ThirdAccount(item);
					accounts.put(a.type, a);
				}
			}

		} catch (Exception e) {
			ShowLog.showException(e);
		}
	}

	/** 本地保存 */
	public String write() {
		String result = null;

		try {
			JSONObject json = new JSONObject();

			json.put("userIdx", userIdx);
			json.put("xgid",xgId);
			json.put("pwd",xgPwd);
			json.put("domin",xgDomain);
			json.put("nickname", nickname);
			json.put("gender", gender);
			json.put("headphoto", headphoto);
			//json.put("score", score);
			json.put("accountType", accountType);
			json.put("loginAccountNum", loginAccountNum);
			json.put("canAutoLogin", canAutoLogin);
			json.put("totalScore", totalScore);

			JSONArray array = new JSONArray();

			Iterator iter = accounts.entrySet().iterator();
			while (iter.hasNext()) {
				Map.Entry entry = (Map.Entry) iter.next();
				ThirdAccount a = (ThirdAccount) entry.getValue();
				array.put(a.write());
			}

			json.put("accounts", array);

			result = json.toString();

		} catch (Exception e) {
			ShowLog.showException(e);
		}

		return result;
	}

	/**
	 * 返回100 * 100的头像地址
	 * 
	 * @return
	 */
	public String getHeadPhoto100() {
		return PCommonUtil.getUrlByScaleType(headphoto,
				PCommonUtil.IMAGE_SIZE_SMALL);
	}

	public void updateUserInfo(JSONObject jsonObject){
		try {
			if (null != jsonObject) {
				gold = jsonObject.optInt("gold"); 
				
				String str = jsonObject.optString("name");
				if(!TextUtils.isEmpty(str))
					nickname =PCommonUtil.decodeBase64(str);
				
				str = jsonObject.optString("headphoto");
				if(!TextUtils.isEmpty(str))
					headphoto =PCommonUtil.decodeBase64(str);
				
				totalScore = jsonObject.optInt("TotalScore");
			}
		} catch (Exception e) {
			ShowLog.showException(e);
		}
	}
	public void update(JSONObject jsonObject){
		try {
			if (null != jsonObject) {
				userIdx = jsonObject.optLong("uidx");
				xgId = jsonObject.optString("xgid");
				xgPwd = jsonObject.optString("pwd");
				xgDomain = jsonObject.optString("domin");
				gold = jsonObject.optInt("gold"); 
				
				String str = jsonObject.optString("name");
				if(!TextUtils.isEmpty(str))
					nickname =PCommonUtil.decodeBase64(str);
				
				str = jsonObject.optString("headphoto");
				if(!TextUtils.isEmpty(str))
					headphoto =PCommonUtil.decodeBase64(str);
				
				score = jsonObject.optInt("score");
				gender = jsonObject.optInt("gender");
				accountType = jsonObject.optInt("account_type_id");
				totalScore = jsonObject.optInt("TotalScore");
			}
		} catch (Exception e) {
			ShowLog.showException(e);
		}
	}

	public String getOpenId(){
		if(accountType > 0){
			ThirdAccount account = getThirdAccount(accountType);
			if(account != null && !TextUtils.isEmpty(account.openid))
				return account.openid;
		}
		
		return null;
	}

	
}

