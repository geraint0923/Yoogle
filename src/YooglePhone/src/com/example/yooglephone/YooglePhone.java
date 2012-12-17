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

import com.google.android.gms.maps.model.RuntimeRemoteException;



public class YooglePhone implements Runnable {
	private String addr;
	
	private double lat_coord;
	private double lng_coord;
	
	public YooglePhone(String ip) {
		addr = ip;
	}
	
	public void setPosition(double lat, double lng) {
		lat_coord = lat;
		lng_coord = lng;
		/*
		if(sync) {
			run();
		} else {
			new Thread(this).start();
		}
		*/
	}
	
	public List<PlaceUnit> prefixSearch(String prefix) {
		JSONObject json = new JSONObject();
		DefaultHttpClient httpClient = new DefaultHttpClient();
		String finalAddr = "http://" + addr + "/list_search";
		
		//System.out.println(finalAddr+"8****************");
		HttpPost httpPost = new HttpPost(finalAddr);
		try {
			json.put("lat_coord", lat_coord);
			json.put("lng_coord", lng_coord);
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

	@Override
	public void run() {
		// TODO Auto-generated method stub
		JSONObject json = new JSONObject();
		DefaultHttpClient httpClient = new DefaultHttpClient();
		String finalAddr = "http://" + addr + "/set_position";
		
		//System.out.println(finalAddr+"8****************");
		HttpPost httpPost = new HttpPost(finalAddr);
		try {
			json.put("lat_coord", lat_coord);
			json.put("lng_coord", lng_coord);
			StringEntity se = new StringEntity(json.toString());
			httpPost.setEntity(se);
			
			httpPost.setHeader("Accept", "application/json");
			httpPost.setHeader("Content-type", "application/json");
			
			httpClient.execute(httpPost);
			
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
	}
}