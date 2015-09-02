package com.example.mylistviewtest;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.provider.ContactsContract.CommonDataKinds.Email;
import android.provider.ContactsContract.CommonDataKinds.Im;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.provider.ContactsContract.CommonDataKinds.Photo;
import android.provider.ContactsContract.CommonDataKinds.StructuredName;
import android.provider.ContactsContract.Data;
import android.provider.ContactsContract.RawContacts;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.ProgressBar;
import android.widget.Switch;
import android.widget.Toast;

public class MainActivity extends Activity {


	
	
	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		setIntent(intent);	//must store the new intent unless getIntent() will return the old one
		
		int showBar = intent.getIntExtra("showBar", 0);
		if (showBar > 0)
		{
			ProgressBar theBar = (ProgressBar)findViewById(R.id.hiddenBar);
			theBar.setVisibility(showBar);
		}
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		Intent intent = getIntent();
		
		Bundle bundle = intent.getExtras();
		int showBar = 0;
		if (bundle != null)
		{
			showBar = bundle.getInt("id");
		}
		
		
		
		showBar = intent.getIntExtra("showBar", 0);
		//int showBar = (int)(intent.getExtras().get("showBar"));
		//int showBar = intent.getIntExtra("showBar", 0);
		if (showBar > 0)
		{
			ProgressBar theBar = (ProgressBar)findViewById(R.id.hiddenBar);
			theBar.setVisibility(View.VISIBLE);
		}
		else
		{
			ProgressBar theBar = (ProgressBar)findViewById(R.id.hiddenBar);
			theBar.setVisibility(View.INVISIBLE);
		}
		
		initNotification();

		Switch mySwitch = (Switch)findViewById(R.id.light);
		mySwitch.setOnCheckedChangeListener(new OnCheckedChangeListener(){

			@Override
			public void onCheckedChanged(CompoundButton view, boolean checked) {
				// TODO Auto-generated method stub
				try
				{
					Switch lightSwitch = (Switch)view;
					
					int cameraCount = Camera.getNumberOfCameras();
					if (cameraCount > 0)
					{
						//boolean checked = lightSwitch.isChecked();
						if (checked)
						{
							if (camera != null)
							{
								camera.release();
								camera = null;
							}
							
							camera = Camera.open(0);
							Parameters mParameters = camera.getParameters();
							mParameters.setFlashMode(lightSwitch.isChecked() ? Camera.Parameters.FLASH_MODE_TORCH : Camera.Parameters.FLASH_MODE_OFF);//打开Camera.Parameters.FLASH_MODE_OFF则为关闭
							camera.setParameters(mParameters);
						}
						else
						{
							if (camera != null)
							{
								Parameters mParameters = camera.getParameters();
								mParameters.setFlashMode(lightSwitch.isChecked() ? Camera.Parameters.FLASH_MODE_TORCH : Camera.Parameters.FLASH_MODE_OFF);//打开Camera.Parameters.FLASH_MODE_OFF则为关闭
								camera.setParameters(mParameters);							
								camera.release();
								camera = null;
							}
						}
					}
				}
				catch(Exception e)
				{
					System.out.print(e.toString());
					Toast.makeText(getApplicationContext(), e.toString(), Toast.LENGTH_SHORT).show();
				}
			}
			
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	public void jumpToListClick(View view)
	{
		Toast.makeText(getApplicationContext(), "test",  Toast.LENGTH_LONG).show();
		//System.out.println("Clicked");
		Intent intent = new Intent(MainActivity.this, MyListActivity.class);
		startActivity(intent);
	}

	public void jumpToListViewClick(View view)
	{
		Toast.makeText(getApplicationContext(), "test",  Toast.LENGTH_LONG).show();
		//System.out.println("Clicked");
		Intent intent = new Intent(MainActivity.this, MyListViewActivity.class);
		startActivity(intent);
	}

	public void addContactsClick(View view)
	{
//		for(int i=0;i<20;++i)
//		{
//			insert((char)('A' + i) + "_name", "138164224"+i, "galaxy"+i+"@gmail.com","1234"+i);
//		}
		
		display();
		System.out.println("~~~~~~~~~~1~~~~~~~~~~");
		fetchContactInformation();
		System.out.println("~~~~~~~~~~2~~~~~~~~~~");
	}
	
	public void fetchContactInformation() {
        String id,name,phoneNumber,email;
        ContentResolver contentResolver = this.getContentResolver();
        Cursor cursor = contentResolver.query(android.provider.ContactsContract.Contacts.CONTENT_URI, null, null, null, null);
        while(cursor.moveToNext()) {
            id=cursor.getString(cursor.getColumnIndex(android.provider.ContactsContract.Contacts._ID));
            name=cursor.getString(cursor.getColumnIndex(android.provider.ContactsContract.Contacts.DISPLAY_NAME));
            
            //Fetch Phone Number
            Cursor phoneCursor = contentResolver.query(
                    android.provider.ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
                    null, android.provider.ContactsContract.CommonDataKinds.Phone.CONTACT_ID+"="+id, null, null);
            while(phoneCursor.moveToNext()) {
                phoneNumber = phoneCursor.getString(
                        phoneCursor.getColumnIndex(android.provider.ContactsContract.CommonDataKinds.Phone.NUMBER));
                System.out.println("ctest: id="+id+" name="+name+" phoneNumber="+phoneNumber);
            }
            phoneCursor.close();
            
            //Fetch email
            Cursor emailCursor = contentResolver.query(
                    android.provider.ContactsContract.CommonDataKinds.Email.CONTENT_URI, 
                    null, android.provider.ContactsContract.CommonDataKinds.Email.CONTACT_ID+"="+id, null, null);
            while(emailCursor.moveToNext()) {
                email = emailCursor.getString(
                        emailCursor.getColumnIndex(android.provider.ContactsContract.CommonDataKinds.Email.DATA));
                System.out.println("ctest: id="+id+" name="+name+" email="+email + " id=" + id);
            }
            emailCursor.close();
        }
        cursor.close();
    }
	
	public void display()
	{
		ContentResolver cr = getContentResolver();
		Cursor contactsCur = cr.query(ContactsContract.Contacts.CONTENT_URI, 
							//new String[] {ContactsContract.PhoneLookup.DISPLAY_NAME, ContactsContract.CommonDataKinds.Phone.NUMBER}, 
						  	null, null, null, null);
		
		while(contactsCur.moveToNext())
		{
		  //获取ID
		  String rawContactId = contactsCur.getString(contactsCur.getColumnIndex(ContactsContract.Contacts._ID));

		  int phoneCount = contactsCur.getInt(contactsCur.getColumnIndex(ContactsContract.Contacts.HAS_PHONE_NUMBER));
		  if (phoneCount >= 1)
		  {
	          Cursor phones = getContentResolver().query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI,null,ContactsContract.CommonDataKinds.Phone.CONTACT_ID + "=" + rawContactId, null, null);
              while(phones.moveToNext())
              {
                  //遍历所有的电话号码  
                  String phoneNumber= phones.getString(phones.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));  
                  System.out.println("ctest: number=" + phoneNumber);
              }
              
              phones.close();
		  }
		  
		  String displayName  = contactsCur.getString(contactsCur.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));
		  System.out.println("ctest: name=" + displayName  + " id=" + rawContactId);
		}
	}

	public boolean insert(String given_name, String mobile_number, String work_email, String im_qq) 
	{
		try 
		{
			ContentValues values = new ContentValues();

			// 下面的操作会根据RawContacts表中已有的rawContactId使用情况自动生成新联系人的rawContactId
			Uri rawContactUri = getContentResolver().insert(RawContacts.CONTENT_URI, values);
			long rawContactId = ContentUris.parseId(rawContactUri);

			// 向data表插入姓名数据
			if (given_name != "") {
				values.clear();
				values.put(Data.RAW_CONTACT_ID, rawContactId);
				values.put(Data.MIMETYPE, StructuredName.CONTENT_ITEM_TYPE);
				values.put(StructuredName.GIVEN_NAME, given_name);
				getContentResolver().insert(ContactsContract.Data.CONTENT_URI,
						values);
			}

			// 向data表插入电话数据
			if (mobile_number != "") {
				values.clear();
				values.put(Data.RAW_CONTACT_ID, rawContactId);
				values.put(Data.MIMETYPE, Phone.CONTENT_ITEM_TYPE);
				values.put(Phone.NUMBER, mobile_number);
				values.put(Phone.TYPE, Phone.TYPE_MOBILE);
				getContentResolver().insert(ContactsContract.Data.CONTENT_URI,
						values);
			}

			// 向data表插入Email数据
			if (work_email != "") {
				values.clear();
				values.put(Data.RAW_CONTACT_ID, rawContactId);
				values.put(Data.MIMETYPE, Email.CONTENT_ITEM_TYPE);
				values.put(Email.DATA, work_email);
				values.put(Email.TYPE, Email.TYPE_WORK);
				getContentResolver().insert(ContactsContract.Data.CONTENT_URI,
						values);
			}

			// 向data表插入QQ数据
			if (im_qq != "") {
				values.clear();
				values.put(Data.RAW_CONTACT_ID, rawContactId);
				values.put(Data.MIMETYPE, Im.CONTENT_ITEM_TYPE);
				values.put(Im.DATA, im_qq);
				values.put(Im.PROTOCOL, Im.PROTOCOL_QQ);
				getContentResolver().insert(ContactsContract.Data.CONTENT_URI,
						values);
			}
			// 向data表插入头像数据
			Bitmap sourceBitmap = BitmapFactory.decodeResource(getResources(),
					R.drawable.ic_launcher);
			final ByteArrayOutputStream os = new ByteArrayOutputStream();
			// 将Bitmap压缩成PNG编码，质量为100%存储
			sourceBitmap.compress(Bitmap.CompressFormat.PNG, 100, os);
			byte[] avatar = os.toByteArray();
			values.put(Data.RAW_CONTACT_ID, rawContactId);
			values.put(Data.MIMETYPE, Photo.CONTENT_ITEM_TYPE);
			values.put(Photo.PHOTO, avatar);
			getContentResolver().insert(ContactsContract.Data.CONTENT_URI, values);
		}

		catch (Exception e) {
			e.printStackTrace();
		}
		return true;
	}

	Camera camera = null;
	public void switchLightClick(View view)
	{
		if(view.getId() == R.id.light)
		{

		}
	}
	
	public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
    {
		
    }
	
	private Notification notification;
	void initNotification()
	{
		notification = new Notification();
		String tickerText = "测试Notifaction"; // 通知提示
		// 显示时间
		long when = System.currentTimeMillis();

		notification.icon = R.drawable.ic_launcher;// 设置通知的图标
		notification.tickerText = tickerText; // 显示在状态栏中的文字
		notification.when = when; // 设置来通知时的时间
		//notification.sound = Uri.parse("android.resource://com.sun.alex/raw/dida"); // 自定义声音
		notification.flags = Notification.FLAG_NO_CLEAR; // 点击清除按钮时就会清除消息通知,但是点击通知栏的通知时不会消失
		notification.flags = Notification.FLAG_ONGOING_EVENT; // 点击清除按钮不会清除消息通知,可以用来表示在正在运行
		notification.flags |= Notification.FLAG_AUTO_CANCEL; // 点击清除按钮或点击通知后会自动消失
		notification.flags |= Notification.FLAG_ONLY_ALERT_ONCE; // 一直进行，比如音乐一直播放，知道用户响应
		notification.defaults = Notification.DEFAULT_SOUND; // 调用系统自带声音
		notification.defaults = Notification.DEFAULT_SOUND;// 设置默认铃声
		notification.defaults = Notification.DEFAULT_VIBRATE;// 设置默认震动
		notification.defaults = Notification.DEFAULT_ALL; // 设置铃声震动
		notification.defaults = Notification.DEFAULT_ALL; // 把所有的属性设置成默认
	}
	
	public void startNotificationClick(View view)
	{
		// 单击通知后会跳转到NotificationResult类
		Intent intent = new Intent(MainActivity.this, MainActivity.class);

		Bundle bundle = new Bundle(); 
        bundle.putInt("id",123456); 
        intent.putExtras(bundle);
        intent.putExtra("showBar", 3);
        
		// 获取PendingIntent,点击时发送该Intent
		PendingIntent pIntent = PendingIntent.getActivity(MainActivity.this, (int)System.currentTimeMillis(),
				intent, 0);
		// 设置通知的标题和内容
		notification.setLatestEventInfo(MainActivity.this, "标题", "内容", pIntent);
		// 发出通知		
		NotificationManager nManager = (NotificationManager) this.getSystemService(NOTIFICATION_SERVICE);
		nManager.notify(1, notification);
	}
	
	public void cancelNotificationClick(View view)
	{
		NotificationManager nManager = (NotificationManager) this.getSystemService(NOTIFICATION_SERVICE);
		nManager.cancel(1);
	}
	
//	private void initCamera(SurfaceHolder surfaceHolder) {
//		if (surfaceHolder == null) {
//			throw new IllegalStateException("No SurfaceHolder provided");
//		}
//		if (cameraManager.isOpen()) {
//			ZXingApplication.print_i("CaptureActivity", "initCamera() while already open -- late SurfaceView callback?");
//			return;
//		}
//
//		try {
//			cameraManager.openDriver(surfaceHolder);
//		} catch (IOException ioe) {
//			return;
//		} catch (RuntimeException e) {
//			return;
//		}
//		if (handler == null) {
//			handler = new CaptureActivityHandler(this, decodeFormats,
//					decodeHints, characterSet, cameraManager);
//		}
//		ZXingApplication.print_i("CaptureActivity", "initCamera-----------finish");
//	}
}
