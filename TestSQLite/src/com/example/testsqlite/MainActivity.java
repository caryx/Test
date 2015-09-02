package com.example.testsqlite;

import java.util.ArrayList;
import java.util.HashMap;

import android.app.Activity;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.GridView;
import android.widget.SimpleAdapter;
import android.widget.TextView;

public class MainActivity extends Activity {
	private static final String TABLE_NAME = "stu"; 
	private static final String NAME = "cary_table";
	private static final String ID = "cary_id";
	private static final int PageSize = 10;
	private static int id = 0;
	SQLiteDatabase db;
	SimpleAdapter saPageID;
	ArrayList<HashMap<String, String> > listPageID;
	
	private TextView textView;

	void CreateDB() {
		if (db == null) {
			Log.e("Cary", "create DB failed.");
			return;
		}
		
		String amount = String.valueOf(databaseList().length);
		Log.e("Cary", "DB amount:" + amount);
		String sql = "CREATE TABLE " + TABLE_NAME + " (" + ID + " text not null, " + NAME + " text not null" + ");";
		
		try {
			db.execSQL("DROP TABLE IF EXISTS " + TABLE_NAME);
			db.execSQL(sql);
		}
		catch (SQLException e)
		{
			
		}
	}
	
	void InsertRecord(int n)
	{		
		int total = id + n;
		for(;id<total;id++)
		{
			String sql = "insert into " + TABLE_NAME + " (" + ID + ", " 
						+ NAME + ") values('" + String.valueOf(id) + "','test');";
			try{
				db.execSQL(sql);
			}
			catch(SQLException e){
			}
		}
		
		id = total;
	}
	
	void RefreshPage()
	{
		String sql = "select count(*) from " + TABLE_NAME;
		Cursor rec = db.rawQuery(sql, null);
		rec.moveToLast();
		
		long recSize = rec.getLong(0);// total count
		rec.close();
		
		int pageNum = (int)(recSize/PageSize) + 1;// page count
		
		listPageID.clear();
		for(int i=0;i<pageNum;i++) {
			HashMap<String, String> map = new HashMap<String, String>();
			map.put("ItemText", "No." + String.valueOf(i));
			
			listPageID.add(map);
		}
		
		saPageID.notifyDataSetChanged();
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		textView = (TextView)findViewById(R.id.textView1);
		GridView gridView = (GridView)findViewById(R.id.gridview);
		
		listPageID = new ArrayList<HashMap<String, String>>();
		saPageID = new SimpleAdapter(MainActivity.this, 
				listPageID,
				R.layout.pagebuttons,
				new String[] {"ItemText"},
				new int[] {R.id.ItemText});
		
		gridView.setAdapter(saPageID);

		db = SQLiteDatabase.create(null);
//		CreateDB();
//		LoadPage(1);
//		gridView.setOnItemClickListener(new OnItemListener(){
//			@Override
//			public void onItemClick(AdapterView<?> arg0, View agr1, int arg2, long arg3){
//				LoadPage(arg2);
//			}
//		});
		
//		gridView.setOnClickListener(new OnClickListener(){
//			@Override
//			public void onClick(View arg0) {
//				// TODO Auto-generated method stub
//				
//			}
//		});		
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	void LoadPage(int pageID)
	{
		String sql = "select * from " + TABLE_NAME +
				" Limit " + String.valueOf(PageSize) + " Offset " + String.valueOf(pageID*PageSize);
		Cursor rec = db.rawQuery(sql, null);
		
		setTitle("Current Page Number:" + String.valueOf(rec.getCount()));
		String title = "";
		int colCount = rec.getColumnCount();
		for(int i=0;i<colCount;i++)
		{
			title = title + rec.getColumnName(i) + " ";
		}
		
		String content = "";
		int recCount = rec.getCount();		
		for(int i=0;i<recCount;i++)
		{
			rec.moveToPosition(i);
			for(int k=0;k<colCount;k++)
			{
				content = content + rec.getString(k) + " ";
			}
			
			content = content + "\r\n";
		}
		
		textView.setText(content);
	}
	
    public void createClick(View v)
    {
    	CreateDB();
    }
    
    public void addDataClick(View v)
    {
    	InsertRecord(20);
    	RefreshPage();
    	LoadPage(0);
    }
    
    public void closeDBClick(View v)
    {
    }    
}
