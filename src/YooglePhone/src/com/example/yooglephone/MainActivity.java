package com.example.yooglephone;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapView;
import com.google.android.gms.maps.MapsInitializer;
import com.google.android.gms.maps.model.BitmapDescriptor;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.R.integer;
import android.app.Activity;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.Menu;
import android.view.View;
import android.view.View.OnLongClickListener;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SimpleAdapter;

public class MainActivity extends Activity {
    private MapView mapView;
    private GoogleMap googleMap;

    private double lat = 1.3;
    private double lng = 103.72;
    
    
    private ListView listView = null;
	private EditText editText = null;
	
	private List<PlaceUnit> unitList = null;
	
	private String queryString;
	
	private YooglePhone yooglePhone = null;
	
	private int[] pngList = new int[5];
	
	private BitmapDescriptor[] bdList = new BitmapDescriptor[5];
	
	private Marker[] markerList = new Marker[5];
	
	private Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			if(unitList != null) {
				setNewPlaceUnitList(unitList);
				for(int i = 0; i < 5; ++i) {
					markerList[i].setVisible(false);
				}
				for(int i = 0; i < unitList.size(); ++i) {
					PlaceUnit unit = unitList.get(i);
					markerList[i].setPosition(new LatLng(unit.getLatCoord(), unit.getLngCoord()));
					markerList[i].setVisible(true);
					markerList[i].setTitle(unit.getName());
					markerList[i].setDraggable(false);
					markerList[i].setSnippet(unit.getAddr());
				}
			}
		}
	};
	
	private List<Map<String, Object>> translatePlaceUnitList(List<PlaceUnit> unitList) {
		List<Map<String, Object>> retList = new ArrayList<Map<String, Object>>();
		for(int i = 0; i < unitList.size(); ++i) {
			Map<String, Object> mMap = new HashMap<String, Object>();
			mMap.put("name", unitList.get(i).getName());
			mMap.put("img", pngList[i]);
			//mMap.put("addr", unitList.get(i).getAddr());
			retList.add(mMap);
		}
		return retList;
	}
	
	private void setNewPlaceUnitList(List<PlaceUnit> unitList) {
		List<Map<String, Object>> mapList = translatePlaceUnitList(unitList);
		String[] keyName = 	new String[]{"name", "img"/*, "addr"*/};
		int[] keyId = new int[]{R.id.text_name, R.id.img/*, R.id.text_addr*/};
		SimpleAdapter adapter = new SimpleAdapter(this, mapList, R.layout.list_item, keyName, keyId);
		if(listView != null) {
			listView.setAdapter(adapter);
		}
	}

//    private RefreshTask refreshTask = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.activity_main);
	
		try {
		    MapsInitializer.initialize(this);
		} catch (GooglePlayServicesNotAvailableException impossible) {
		    impossible.printStackTrace();
		}
		mapView = (MapView) findViewById(R.id.map);
		mapView.onCreate(savedInstanceState);
	
		/*
		yooglePhone = new YooglePhone("192.168.23.142:3000");
		yooglePhone.setPosition(lat, lng);
		*/
		
		/*
		if (googleMap == null) {
			googleMap = ((MapView) findViewById(R.id.map)).getMap();
			if (googleMap != null) {
				googleMap.addMarker(new MarkerOptions().position(new LatLng(lat, lng)).title("I am here!"));
			
				googleMap.moveCamera(CameraUpdateFactory.newLatLngZoom(new LatLng(lat, lng), 10));
			}
		}
		*/
		
		pngList[0] = R.drawable.mark_0;
		pngList[1] = R.drawable.mark_1;
		pngList[2] = R.drawable.mark_2;
		pngList[3] = R.drawable.mark_3;
		pngList[4] = R.drawable.mark_4;
		
		if(mapView != null) {
			googleMap = mapView.getMap();
			if(googleMap != null) {
				googleMap.addMarker(new MarkerOptions().position(new LatLng(lat, lng)).title("I am here!")
						.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_AZURE)));
				googleMap.animateCamera(CameraUpdateFactory.newLatLngZoom(new LatLng(lat, lng), 10));
				for(int i = 0; i < 5; ++i) {
					markerList[i] = googleMap.addMarker(new MarkerOptions().position(new LatLng(lat, lng)).icon(BitmapDescriptorFactory.fromResource(pngList[i])));
					markerList[i].setVisible(false);
				}
			}
		}
		
		editText = (EditText) this.findViewById(R.id.edit_text);
		
		listView = (ListView) this.findViewById(R.id.res_list);
		
		editText.addTextChangedListener(new TextWatcher() {

			@Override
			public void afterTextChanged(Editable arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void beforeTextChanged(CharSequence arg0, int arg1,
					int arg2, int arg3) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onTextChanged(CharSequence arg0, int arg1, int arg2,
					int arg3) {
				// TODO Auto-generated method stub
				System.out.println("text changed:"+arg0);

				if(yooglePhone == null) {
					return;
				}
				
				queryString = arg0.toString();
				new Thread(new Runnable() {

					@Override
					public void run() {
						// TODO Auto-generated method stub
						if(queryString != null) {
							unitList = yooglePhone.prefixSearch((String) queryString);
							handler.sendEmptyMessage(0);
						}
					}
					
				}).start();
			}
			
		});
		
		editText.setLongClickable(true);
		editText.setOnLongClickListener(new OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				// TODO Auto-generated method stub
				String str = editText.getText().toString();
				editText.setText("");
				yooglePhone = new YooglePhone(str);
				
				return false;
			}
		});
		
		listView.setLongClickable(true);
		listView.setOnLongClickListener(new OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				// TODO Auto-generated method stub
				editText.setText("fuck");
				return false;
			}
		});
		
		listView.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
					long arg3) {
				// TODO Auto-generated method stub
				PlaceUnit unit = unitList.get(arg2);
				Marker marker = markerList[arg2];
				marker.showInfoWindow();
				googleMap.animateCamera(CameraUpdateFactory.newLatLngZoom(new LatLng(unit.getLatCoord(), unit.getLngCoord()), 11));
			}
		});
		
		listView.setOnItemLongClickListener(new OnItemLongClickListener() {

			@Override
			public boolean onItemLongClick(AdapterView<?> arg0, View arg1,
					int arg2, long arg3) {
				// TODO Auto-generated method stub
				googleMap.animateCamera(CameraUpdateFactory.newLatLngZoom(new LatLng(lat, lng), 10));
				return false;
			}
		});

		
    }
    

    @Override
    protected void onResume() {
    	super.onResume();
    	mapView.onResume();

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
	// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
    }


    @Override
    protected void onPause() {
		mapView.onPause();
		super.onPause();
    }

    @Override
    protected void onDestroy() {
		mapView.onDestroy();
		super.onDestroy();
    }

    

    @Override
    public void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
		mapView.onSaveInstanceState(outState);
    }

}
