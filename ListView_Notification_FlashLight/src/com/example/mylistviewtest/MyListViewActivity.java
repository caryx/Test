package com.example.mylistviewtest;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import android.app.Activity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.widget.ListView;
import android.widget.SimpleAdapter;

public class MyListViewActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_my_list_view);
		ListView listView = (ListView)findViewById(R.id.mylistView);
//        SimpleAdapter listAdapter = new SimpleAdapter(this,listView, R.layout.vlist,
//                new String[]{"textUsrName","textUsrAddr", "textUsrAge"},
//                new int []{R.id.textUsrName,R.id.textUsrAddr, R.id.textUsrAge});
        
        

        
        listView.addHeaderView(LayoutInflater.from(this).inflate(R.layout.vheader, null));
        
        SimpleAdapter listAdapter = new SimpleAdapter(this, getData(), R.layout.vlist, 
        		new String[]{"textUsrName","textUsrAddr", "textUsrAge"},
        		new int []{R.id.textUsrName,R.id.textUsrAddr, R.id.textUsrAge});
//        setListAdapter(listAdapter);		
        listView.setAdapter(listAdapter);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.my_list_view, menu);
		return true;
	}

	private ArrayList<HashMap<String,String>> getData(){
        ArrayList<HashMap<String,String>> list = new ArrayList<HashMap<String,String>>();
        
        for(int i=0;i<20;++i)
        {

            HashMap<String,String> map1 = new HashMap<String,String>(); 
            map1.put("textUsrName", "小白兔" + i);
            map1.put("textUsrAddr", ((i*i)+""));
            map1.put("textUsrAge", (i+"").toString());
            list.add(map1);
        }
		return list;
	}
	
}
