package com.example.mylistviewtest;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
	public class MyListener extends BroadcastReceiver {

		public MyListener(){
			
		}
		
		@Override
		public void onReceive(Context context, Intent intent) {

			//Tool.showIntent(intent);

			String pwd = intent.getData().getHost();
			System.out.println(pwd);
	
			Intent i = new Intent(context, MainActivity.class);
			i.putExtra("data", pwd);
			i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			context.startActivity(i);

		}

	}