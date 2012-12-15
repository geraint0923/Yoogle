package com.thu.yooglephone;

import org.json.JSONException;
import org.json.JSONObject;


public class PlaceUnit {
	public PlaceUnit(JSONObject obj) {
		try {
			id = obj.getInt("id");
			addr = obj.getString("addr");
			latCoord = obj.getDouble("lat_coord");
			lngCoord = obj.getDouble("lng_coord");
			name = obj.getString("name");
			postCode = obj.getInt("post_code");
			url = obj.getString("url");
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public String toString() {
		StringBuilder str = new StringBuilder("");
		str.append("id:"+id+"\n");
		str.append("addr:"+addr+"\n");
		str.append("name:"+name+"\n");
		return str.toString();
	}
	
	private int id;
	private String addr;
	private double latCoord;
	private double lngCoord;
	private String name;
	private int postCode;
	private String url;
	
	
	public int getId() {
		return id;
	}
	
	public String getAddr() {
		return addr;
	}
	
	public double getLatCoord() {
		return latCoord;
	}
	
	public double getLngCoord() {
		return lngCoord;
	}
	
	public String getName() {
		return name;
	}
	
	public int getPostCode() {
		return postCode;
	}
	
	public String getUrl() {
		return url;
	}
}