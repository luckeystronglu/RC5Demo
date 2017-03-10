package com.yzh.myapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        byte[] macSource=new byte[]{(byte)0xDD,(byte)0xEE,(byte)0xFF};
        byte[] src= new byte[]{0x1F,0x01,0x02,0x53,0x4C,0x12,0x34,0x45,0x67,(byte)0x89,0x0A,0x65,0x67,0x34,0x65,(byte) 0xA7};
//        byte[] src= new byte[]{ (byte)0xaa,(byte)0xbb,(byte)0xcc,(byte)0xdd,(byte)0xee,(byte)0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
        byte[] dec=new byte[16];
        //使用 MAC 地址后 3 个字节与数据内容进行异或操作
        XOREncrypt(src,dec,16,macSource,macSource.length);
        Log.i("XOREncrypt:", bytes2HexString(dec));

//        byte[] enResult=new byte[16];
//        MD5Encode(dec,enResult);
//        Log.i("MD5Encode:", bytes2HexString(enResult));
//
//        byte[] deResult=new byte[16];
//        MD5Encode(enResult,deResult);
//        Log.i("MD5Decode:",bytes2HexString(deResult));
        // RC5 加密
        byte[] enResult=new byte[16];
        RC5Encode(dec,enResult);
        Log.i("RC5Encipher:",bytes2HexString(enResult));

        // RC5 解密
        byte[] deResult=new byte[16];
        RC5Decode(enResult,deResult);
        Log.i("RC5Decipher:",bytes2HexString(deResult));

        //异或操作得到 数据内容
        byte[] dec555=new byte[16];
        XOREncrypt(deResult,dec555,16,macSource,macSource.length);
        Log.i("RC5Decipher5555:",bytes2HexString(dec555));
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native  void XOREncrypt(byte[] src, byte[] desc,int len,byte[] macSource,int lenMacSource );
    public native  void RC5Encode(byte[] in,byte [] out );
    public native  void RC5Decode(byte[] in,byte [] out );
    public native  void MD5Encode(byte[] in,byte [] out );
    public native  void MD5Decode(byte[] in,byte [] out );



    public static String bytes2HexString(byte[] bytes) {

        StringBuilder builder = new StringBuilder();
        for (byte b : bytes) {
            builder.append(String.format("%1$02x", b & 0xff) + " ");
        }

        return builder.toString().toUpperCase();
    }
}
