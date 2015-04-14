package com.example.joshua.androidtoarduino;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;


public class BluetoothMain extends ActionBarActivity {
    Button btnOn, btnOff;
    TextView txtArduino;

    final int MESSAGE_READ = 1;

    private ConnectedThread mConnectedThread;
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private StringBuilder sb = new StringBuilder();

    private static final UUID SerialPortServiceClass_UUID =
            UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    // MAC-address of JY-MCU
    private static String address = "30:15:01:07:02:81";

    private final Handler mHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            switch(msg.what) {
                case MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    String strIncom = new String(readBuf, 0, msg.arg1);
                    sb.append(strIncom);
                    int endOfLineIndex = sb.indexOf("\r\n");
                    if(endOfLineIndex > 0) {
                        String sbprint = sb.substring(0, endOfLineIndex);
                        sb.delete(0, sb.length());
                        txtArduino.setText("Data from Arduino: " + sbprint);
                        btnOff.setEnabled(true);
                        btnOn.setEnabled(true);
                    }
                    break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth_main);

        btnOn = (Button) findViewById(R.id.btnOn);
        btnOff =(Button) findViewById(R.id.btnOff);
        txtArduino = (TextView) findViewById(R.id.txtArduino);

        btAdapter = BluetoothAdapter.getDefaultAdapter();
        checkBTState();

        BluetoothDevice device = btAdapter.getRemoteDevice(address);
        try {
            btSocket = device.createRfcommSocketToServiceRecord(SerialPortServiceClass_UUID);
            btAdapter.cancelDiscovery();
            btSocket.connect();
            txtArduino.append("Bluetooth device connection success");
            mConnectedThread = new ConnectedThread(btSocket);
            mConnectedThread.start();
        } catch(IOException e) {
            txtArduino.append("Bluetooth device connect failed" + e);
        }


        btnOn.setOnClickListener(new OnClickListener(){
            public void onClick(View v){
                btnOn.setEnabled(false);
                mConnectedThread.write("1");
            }
        });
        btnOff.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                btnOff.setEnabled(false);
                mConnectedThread.write("0");
            }
        });
    }


    private class ConnectedThread extends Thread {
        private final InputStream mmInstream;
        private final OutputStream mmOutputStream;

        public ConnectedThread(BluetoothSocket socket) {
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) {}
            mmInstream = tmpIn;
            mmOutputStream = tmpOut;
        }

        public void run(){
            byte[] buffer = new byte[1024];
            int bytes;
            while(true) try {
                bytes = mmInstream.read(buffer);
                mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer).sendToTarget();
            } catch (IOException e) {
                break;
            }
        }

        // Call this from the main Activity to send data to the remote device
        public void write(String message){
            byte[] msgBuffer = message.getBytes();
            try {
                mmOutputStream.write(msgBuffer);
            }catch (IOException e) {
                txtArduino.append("Error: data did not send" + e.getMessage() + "...");
            }
        }
    }

    private void errorExit(String title, String message){
        Toast.makeText(getBaseContext(), title + " - " + message,
                Toast.LENGTH_LONG).show();
        finish();
    }

    private void checkBTState(){
        if(btAdapter == null){
            errorExit("Fatal Error", "Bluetooth not supported");
        }
        else {
            if(btAdapter.isEnabled()){
                txtArduino.append("Bluetooth is Enabled \n");
            }
            else {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);
            }
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_bluetooth_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
