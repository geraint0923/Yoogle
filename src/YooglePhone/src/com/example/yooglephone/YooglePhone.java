package com.example.yooglephone;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;



public class YooglePhone {
	private String addr;
	
	public YooglePhone(String ip) {
		addr = ip;
	}
	
	public List<PlaceUnit> prefixSearch(String prefix) {
		JSONObject json = new JSONObject();
		DefaultHttpClient httpClient = new DefaultHttpClient();
		String finalAddr = "http://" + addr + "/list_search";
		
		//System.out.println(finalAddr+"8****************");
		HttpPost httpPost = new HttpPost(finalAddr);
		try {
			json.put("query", prefix);
			StringEntity se = new StringEntity(json.toString());
			httpPost.setEntity(se);
			
			httpPost.setHeader("Accept", "application/json");
			httpPost.setHeader("Content-type", "application/json");
			
			HttpResponse httpResponse = httpClient.execute(httpPost);
			HttpEntity resEntity = httpResponse.getEntity();
			if(resEntity != null) {
				String resString = EntityUtils.toString(resEntity);
			//	System.out.println("result:"+resString);
				JSONTokener tokener = new JSONTokener(resString);
				JSONObject pack = (JSONObject) tokener.nextValue();
				JSONArray jsonList = pack.getJSONArray("result");
				ArrayList<PlaceUnit> unitList = new ArrayList<PlaceUnit>();
				for(int i = 0; i < jsonList.length(); ++i) {
					unitList.add(new PlaceUnit((JSONObject) jsonList.get(i)));
				}
				return unitList;
			}
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ClientProtocolException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return null;
	}
}