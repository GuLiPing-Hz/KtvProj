package assistant.manager;

import java.io.UnsupportedEncodingException;

import org.json.JSONObject;

import assistant.task.PCommonUtil;

import com.android.volley.NetworkResponse;
import com.android.volley.ParseError;
import com.android.volley.Response;
import com.android.volley.Response.ErrorListener;
import com.android.volley.Response.Listener;
import com.android.volley.toolbox.HttpHeaderParser;
import com.android.volley.toolbox.JsonObjectRequest;

/**
 * @author bill
 * @version 创建时间：2013-11-4 下午06:52:52 类说明
 */
public class UpdateJsonObjectRequest extends JsonObjectRequest {

	public UpdateJsonObjectRequest(int method, String url,
			JSONObject jsonRequest, Listener<JSONObject> listener,
			ErrorListener errorListener) {
		super(method, url, jsonRequest, listener, errorListener);
		// TODO Auto-generated constructor stub
	}

	public UpdateJsonObjectRequest(String url, JSONObject jsonRequest,
			Listener<JSONObject> listener, ErrorListener errorListener) {
		super(url, jsonRequest, listener, errorListener);
		// TODO Auto-generated constructor stub
	}

	
	@Override
	protected Response<JSONObject> parseNetworkResponse(NetworkResponse response) {
		// TODO Auto-generated method stub
		try {
            String jsonString =
                new String(response.data, HttpHeaderParser.parseCharset(response.headers));
            
            return Response.success(PCommonUtil.parseString2JsonObject(jsonString),
                    HttpHeaderParser.parseCacheHeaders(response));
        } catch (UnsupportedEncodingException e) {
            return Response.error(new ParseError(e));
        } catch (Exception je) {
            return Response.error(new ParseError(je));
        }
	}

	
	
}
