package com.test;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class ClientTest2 extends Activity implements OnClickListener
{
    Socket socket = null;  
    TextView textInfo = null;
    EditText editIP = null;
    EditText editPT = null;
    Button startButton=null;
    Button stopButton=null;
    Button nextButton=null;
    Button prevButton=null;
    
    private boolean test=false;
    Handler handler = new Handler();
    
    private static final String START_PAUSE = "start/pause";
    private static final String STOP = "stop";
    private static final String NEXT = "next";
    private static final String PREV = "prev";
    
    public static final String SETTING_INFOS = "SETTING_Infos";
	public static final String IPX = "IP";
	public static final String PORTX = "PORT";
    
    private String IP = "10.1.11.1";
    private int Port = 0;
    private String str;
    
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        textInfo = (TextView)findViewById(R.id.info);
        editIP = (EditText)findViewById(R.id.editIP);
        editPT = (EditText)findViewById(R.id.editPT);
        startButton = (Button)findViewById(R.id.start);
        stopButton = (Button)findViewById(R.id.stop);
        nextButton = (Button)findViewById(R.id.next);
        prevButton = (Button)findViewById(R.id.prev);
        
        SharedPreferences settings = getSharedPreferences(SETTING_INFOS,0);
        
        String ipx = settings.getString(IPX, "");
        String portx = settings.getString(PORTX, "");
        
        editIP.setText(ipx);
        editPT.setText(portx);
              
        startButton.setOnClickListener(this);
        stopButton.setOnClickListener(this);
        nextButton.setOnClickListener(this);
        prevButton.setOnClickListener(this);
        
    }
    
    public  class myThread implements Runnable
    {

		@Override
		public void run() 
		{
			// TODO Auto-generated method stub
			IP = editIP.getText().toString();
	        try
	        {
	            Port = Integer.parseInt((editPT.getText().toString()));
	        }
	        catch(Exception e)
	        {
	            e.printStackTrace();
	        }
	        finally
	        {
	            try 
			    {   
					socket = new Socket(IP, Port);   
	     
		             OutputStream netOut = null;
		             netOut = socket.getOutputStream();   
		             if(netOut == null)
		             {
		                	
		                 new Thread(new Runnable() 
		            	 {
		            		  public void run() 
		            		  {
		            		      showToast();
		            		  }
		            	  }).start();

		              }
		              else
		              {
		                  DataOutputStream doc = new DataOutputStream(netOut);   
		                  DataInputStream in = new DataInputStream(socket.getInputStream());   
	
		                  doc.writeChars(str);
//		                  doc.writeUTF(str);   
//		                  String res = in.readUTF();   
//		                  System.out.println(res);   
		                  
		                  test=true;
		                  
		                  doc.close();   
		                  in.close();   
		               }
		           } 
				   catch (Exception e) 
		           {   
		               e.printStackTrace();  
		                
		               new Thread(new Runnable() 
	            	    {
	            		    public void run() 
	            		    {
	            		        showToast();
	            		    }
	            		 }).start();		               
		           }
				   finally 
				   {   
		               if (socket != null) 
		               {   
		                   try 
		                   {   
		                       socket.close();   
		                   } 
		                   catch (IOException e) 
		                   {   
		                   }   
		                }   
		            }   
		       }
		}
    }
    
    public void showToast() 
    {
        handler.post(new Runnable() 
    	{
            @Override
    	    public void run() 
            {
    	        Toast.makeText(getApplicationContext(), "Please input the correct IP and Port£¡",
    	                       Toast.LENGTH_SHORT).show();
            }
    	 });
    }
    
    public void btnIdentify(String temp)
    {
    	myThread mT=new myThread();
		Thread tt=new Thread(mT);
		
		tt.start();
		try 
		{
			str = temp;
			Thread.sleep(500);
			if(test == false)
			    tt.stop();
		} 
		catch(Exception e) 
		{
			 e.printStackTrace();
		}
    }

	@Override
	public void onClick(View v) 
	{
		// TODO Auto-generated method stub
		switch (v.getId()) 
		{
		case R.id.start:
			btnIdentify(START_PAUSE);
		break;
		case R.id.stop:
			btnIdentify(STOP);
		break;
		case R.id.next:
			btnIdentify(NEXT);
		break;
		case R.id.prev:
			btnIdentify(PREV);
		break;
		}
	}

	@Override
	protected void onStop() 
	{
		// TODO Auto-generated method stub
		super.onStop();
		
        SharedPreferences settings = getSharedPreferences(SETTING_INFOS,0);
		
		settings.edit().putString(IPX, editIP.getText().toString())
		.putString(PORTX, editPT.getText().toString()).commit();
	}   
    
}

















