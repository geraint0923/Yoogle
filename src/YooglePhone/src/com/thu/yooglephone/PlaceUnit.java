package com.thu.yooglephone;


public class PlaceUnit {
	public PlaceUnit() {
		
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