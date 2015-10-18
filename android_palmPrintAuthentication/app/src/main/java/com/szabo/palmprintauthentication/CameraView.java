package com.szabo.palmprintauthentication;

import android.content.Context;
import android.hardware.Camera;
import android.util.AttributeSet;
import android.util.Log;

import org.opencv.android.JavaCameraView;

import java.io.FileOutputStream;

/**
 * Custom camera view that extends JavaCameraView
 * with capture functionality, by implementing PictureCallBack
 */
public class CameraView extends JavaCameraView implements Camera.PictureCallback {

    private String mPictureFileName;

    public CameraView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public void takePicture(final String fileName) {
        Log.i("OpenCv", "Taking picture");
        this.mPictureFileName = fileName;
        // Postview and jpeg are sent in the same buffers if the queue is not empty when performing a capture.
        // Clear up buffers to avoid mCamera.takePicture to be stuck because of a memory issue
        mCamera.setPreviewCallback(null);

        // PictureCallback is implemented by the current class
        mCamera.takePicture(null, null, this);
    }

    @Override
    public void onPictureTaken(byte[] data, Camera camera) {
        Log.i("OpenCv", "Saving a bitmap to file");
        // The camera preview was automatically stopped. Start it again.
        mCamera.startPreview();
        mCamera.setPreviewCallback(this);

        // Write the image in a file (in jpeg format)
        try {
            FileOutputStream fos = new FileOutputStream(mPictureFileName);

            fos.write(data);
            fos.close();

        } catch (java.io.IOException e) {
            e.printStackTrace();
            Log.e("OpenCv", "Exception in photoCallback");
        }
    }
}
