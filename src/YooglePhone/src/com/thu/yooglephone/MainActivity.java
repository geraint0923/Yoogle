package com.thu.yooglephone;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.Menu;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SimpleAdapter;

public class MainActivity extends Activity {

	private ListView listView = null;
	private EditText editText = null;
	
	private List<PlaceUnit> unitList = null;
	
	private String queryString;
	
	private Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			if(unitList != null) {
				setNewPlaceUnitList(unitList);
			}
		}
	};
	
	private List<Map<String, Object>> translatePlaceUnitList(List<PlaceUnit> unitList) {
		List<Map<String, Object>> retList = new ArrayList<Map<String, Object>>();
		for(int i = 0; i < unitList.size(); ++i) {
			Map<String, Object> mMap = new HashMap<String, Object>();
			mMap.put("name", unitList.get(i).getName());
			mMap.put("addr", unitList.get(i).getAddr());
			retList.add(mMap);
		}
		return retList;
	}
	
	private void setNewPlaceUnitList(List<PlaceUnit> unitList) {
		List<Map<String, Object>> mapList = translatePlaceUnitList(unitList);
		String[] keyName = 	new String[]{"name"/*, "addr"*/};
		int[] keyId = new int[]{R.id.text_name/*, R.id.text_addr*/};
		SimpleAdapter adapter = new SimpleAdapter(this, mapList, R.layout.list_item, keyName, keyId);
		if(listView != null) {
			listView.setAdapter(adapter);
		}
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		setContentView(R.layout.activity_main);
		
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
				queryString = arg0.toString();
				new Thread(new Runnable() {

					@Override
					public void run() {
						// TODO Auto-generated method stub
						if(queryString != null) {
							unitList = new YooglePhone("192.168.23.142:3000").prefixSearch((String) queryString);
							handler.sendEmptyMessage(0);
						}
					}
					
				}).start();
			}
			
		});
		
		/* 
		testButton = (Button) this.findViewById(R.id.test_button);
		 
		testButton.setOnClickListener(new Button.OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				new Thread(new Runnable() {

					@Override
					public void run() {
						// TODO Auto-generated method stub
						YooglePhone phone = new YooglePhone("192.168.23.142:3000");
						unitList = phone.prefixSearch("loop");
						if(unitList != null) {
							for(int i = 0; i < unitList.size(); ++i) 
								System.out.println(i+"***"+unitList.get(i)+"\n");
						}
						handler.sendEmptyMessage(0);
					}
					
				}).start();

			}
		});
		*/
		
		

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

}
