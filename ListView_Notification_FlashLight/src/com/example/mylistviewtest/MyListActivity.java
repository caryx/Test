package com.example.mylistviewtest;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import android.app.Activity;
import android.app.ListActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

public class MyListActivity extends ListActivity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_list);
		
//		listView = new ListView(this);
//		//listView = (ListView)findViewById(R.id.myListView);
//		//listView.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, getData()));
//		listView.setAdapter(new ArrayAdapter<String>(this, R.layout.vlist, getData()));
//		//listView.setAdapter(new ListAdapter<String>(this, android.R.layout.activity_list_item))
//		//myListView
//		setContentView(listView);

        
        getListView().addHeaderView(LayoutInflater.from(this).inflate(R.layout.vheader, null));

        SimpleAdapter listAdapter = new SimpleAdapter(this,getData(), R.layout.vlist,
                new String[]{"textUsrName","textUsrAddr", "textUsrAge"},
                new int []{R.id.textUsrName,R.id.textUsrAddr, R.id.textUsrAge});
        
        
        setListAdapter(listAdapter);
        //listView = (ListView) this.findViewById(R.layout.vlist);
        
        ///listView = (ListView) this.findViewById(R.id.li);
        //addHeaderView(LayoutInflater.from(this).inflate(R.layout.vheader, null));
        //addListHeaderView();
	}
	
    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        // TODO Auto-generated method stub

        super.onListItemClick(l, v, position, id);
        System.out.println("id--------------"+(id+1));
        System.out.println("position--------------"+position);
        Toast.makeText(getApplicationContext(), 
                "你点击了第"+(id+1)+"个单位",
                Toast.LENGTH_SHORT).show();
    }
    
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.list, menu);
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
