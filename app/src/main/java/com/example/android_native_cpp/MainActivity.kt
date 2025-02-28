package com.example.android_native_cpp

import android.Manifest
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import android.os.Bundle
import android.util.Log;
import android.content.pm.PackageManager
import android.widget.TextView
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.widget.Toast
import com.example.android_native_cpp.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private val CAMERA_PERMISSION_CODE = 100

    private lateinit var binding: ActivityMainBinding

    companion object {
        init {
            try {
                System.loadLibrary("android_rust_lib")
                Log.d("RustLib", "Rust library loaded successfully!")
            } catch (e: UnsatisfiedLinkError) {
                Log.e("RustLib", "Failed to load Rust library!", e)
            }

            System.loadLibrary("android_native_cpp")
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == CAMERA_PERMISSION_CODE) {
            if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                openCameraJNI()
            } else {
                Toast.makeText(this, "Camera permission denied", Toast.LENGTH_SHORT).show()
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Log.d("Native", addNumbersJNI(5, 10).toString());

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) 
        != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, 
            arrayOf(Manifest.permission.CAMERA), CAMERA_PERMISSION_CODE)
        }

        val surfaceView = findViewById<SurfaceView>(R.id.camera_surface)
        surfaceView.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceCreated(holder: SurfaceHolder) {
                if (holder.surface == null) {
                    Log.e("Native", "ERROR: Surface is NULL!")
                    return
                }

                Log.d("Native", "Setting surface...")
                setSurfaceJNI(holder.surface)
                openCameraJNI()
            }

            override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {}
            override fun surfaceDestroyed(holder: SurfaceHolder) {}
        })
    }

    /**
     * A native method that is implemented by the 'android_native_cpp' native library,
     * which is packaged with this application.
     */
    external fun setSurfaceJNI(surface: Any)
    external fun openCameraJNI()
    // external fun addNumbersJNI(a: Int, b: Int): Int
}
