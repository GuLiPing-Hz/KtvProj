/*
 * Copyright (C) 2010 The MobileSecurePay Project
 * All right reserved.
 * author: shiqun.shi@alipay.com
 * 
 *  提示：如何获取安全校验码和合作身份者id
 *  1.用您的签约支付宝账号登录支付宝网站(www.alipay.com)
 *  2.点击“商家服务”(https://b.alipay.com/order/myorder.htm)
 *  3.点击“查询合作者身份(pid)”、“查询安全校验码(key)”
 */

package com.alipay;

//
// 请参考 Android平台安全支付服务(msp)应用开发接口(4.2 RSA算法签名)部分，并使用压缩包中的openssl RSA密钥生成工具，生成一套RSA公私钥。
// 这里签名时，只需要使用生成的RSA私钥。
// Note: 为安全起见，使用RSA私钥进行签名的操作过程，应该尽量放到商家服务器端去进行。
public final class AlipayKeys {

	//合作身份者id，以2088开头的16位纯数字
	public static final String PARTNER = "2088511064760985";

	//收款支付宝账号
	public static final String SELLER = "xinlangzhushou@qq.com";//123456a，123456b

	//商户私钥，自助生成 : PKCS8加密之后的密钥
	public static final String PRIVATE = "MIICeAIBADANBgkqhkiG9w0BAQEFAASCAmIwggJeAgEAAoGBAJ0NFz+v6/2oc1NjWbTRUp9eiVl+rlssUrErfr4qSt6QYfLonUiuuKh1Z6o4uSUV2E7Sbs+kRyisZMNXXuCh0sE6gquwrt7ZCWWZgDXQ7gwSz2gAQb4lMln8wydOgyfD5iUguCg9NsKv+4boRISr/NEE+shYxjllOq/RGWHm0jrnAgMBAAECgYBP+gCi4BWPYR8Jkgh2ujoXCm9rccWcjF+39Wxfru3dlS0sJCUkhEVpFBCscKJoLk2vJh5kA0Zxggj2KSIZxo6qJ7lxb+SbsuTz6Ob15KlieNmy6C6YcCdUA2ALqY7neH7VJvbClBdfHlzXpiIn5K+jNlfoUg1jodHKZUc52ladYQJBAM0ubRD4iFbqYY1iZfAx1+rhH26o0VzwHd1zcQEKcH8ea9+1vGa5cZKmQ2rAmvRTeXk0R4jgBrf5Q9Gqs9hGJNkCQQDD8vTmFRa9BtTvc8dKJpJNuvvssp3oL4xCgPr5JskZj7sM6T/ssFE9Rh8nlxrNlnkVCmYKuEycD/mqlSWtGIW/AkEAppkEq9vw8AmdLexwcsg11hv2pqZiYIEC7tWWMs8kKsMrudhZYls+kWF4qgw7hRjmL/i4ALZOHkFBxNNC9d92eQJBAJELxzyJX8uMwxJ9IYhCBQqE3vbXQQweJjUURLMpgrLpMgzcm/UYjwpMXcNwluaX/AkXKXHF+u+K48ThooH/JQECQQCMXVoWEaPuYfdvp2uAMBwpfGwdLBt+Bzs2mUuSZnItlMNSZX+0Z9mueHg6cSQoioz8v15p/n9iSsMc/3+hHoXm";
	//支付宝公钥
	public static final String PUBLIC = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCnxj/9qwVfgoUh/y2W89L6BkRAFljhNhgPdyPuBV64bfQNN1PjbCzkIM6qRdKBoLPXmKKMiFYnkd6rAoprih3/PrQEB/VsW8OoM8fxn67UDYuyBTqA23MML9q1+ilIZwBC2AQ2UBVOrFXfFl75p6/B5KsiNG9zpgmLCUYuLkxpLQIDAQAB";

}
