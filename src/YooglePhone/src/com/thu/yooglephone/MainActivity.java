package com.thu.yooglephone;

import java.util.List;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity {

	private Button testButton = null;
	private EditText editText = null;
	
	private List<PlaceUnit> unitList = null;
	
	private Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			if(unitList != null) {
				if(editText != null) {
					StringBuilder builder = new StringBuilder("");
					for(int i = 0; i < unitList.size(); ++i) {
						builder.append(unitList.get(i).getName());
					}
					editText.setText(builder.toString());
				}
			}
		}
	};
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		editText = (EditText) this.findViewById(R.id.edit_text);
		
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
		
		

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

}
