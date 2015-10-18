package com.szabo.palmprintauthentication;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Toast;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.JavaCameraView;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Displaying camera view, allows to take pictures and save them.
 */
public class MainActivity extends Activity implements CameraBridgeViewBase.CvCameraViewListener2 {

    /**
     *  Custom camera view
     */
    private CameraView mOpenCvCameraView;

    /**
     * Basic implementation of LoaderCallbackInterface,
     * handles the connection status.
     */
    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {

        /**
         * Callback method that is called after OpenCV library initialization.
         * @param status
         */
        @Override
        public void onManagerConnected(int status) {
            switch (status){
                case BaseLoaderCallback.SUCCESS:
                {
                    Log.i("OPENCV", "Opencv loaded succesfully");
                    mOpenCvCameraView.enableView();
                    mOpenCvCameraView.setOnTouchListener(cameraViewTouchListener);
                    break;
                }
                default:{
                    super.onManagerConnected(status);
                }
            }
        }
    };

    /**
     * OnTouchListener on CameraView, captures and saves the current frame.
     */
    private SurfaceView.OnTouchListener cameraViewTouchListener = new SurfaceView.OnTouchListener(){

        @Override
        public boolean onTouch(View v, MotionEvent event) {
            Log.i("OpenCv","onTouch event");
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss");
            String currentDateandTime = sdf.format(new Date());
            String fileName = Environment.getExternalStorageDirectory().getPath() +
                    "/sample_picture_" + currentDateandTime + ".jpg";
            mOpenCvCameraView.takePicture(fileName);
            Toast.makeText(MainActivity.this, fileName + " saved", Toast.LENGTH_SHORT).show();
            return false;
        }
    };

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mOpenCvCameraView = (CameraView) findViewById(R.id.cameraView);
        mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);
        mOpenCvCameraView.setCvCameraViewListener(this);
    }


    @Override
    protected void onResume() {
        super.onResume();
        initOpenCvLibrary();
    }

    /**
     * Loads and initializes OpenCV library using OpenCV Manager.
     */
    private void initOpenCvLibrary(){
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, this, mLoaderCallback);
    }

    /**
     * This method is invoked when camera preview has started.
     * @param width -  the width of the frames that will be delivered
     * @param height - the height of the frames that will be delivered
     */
    @Override
    public void onCameraViewStarted(int width, int height) {

    }

    /**
     * This method is invoked when camera preview has been stopped for some reason
     */
    @Override
    public void onCameraViewStopped() {

    }

    /**
     * This method is invoked when delivery of the frame needs to be done.
     * @param inputFrame
     * @return is a modified frame which needs to be displayed on the screen.
     */
    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
        return inputFrame.rgba();
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(mOpenCvCameraView != null){
            mOpenCvCameraView.disableView();
        }
    }
}
