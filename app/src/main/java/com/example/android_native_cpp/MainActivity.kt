package com.example.android_native_cpp

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log;
import android.widget.TextView
import com.example.android_native_cpp.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = stringFromJNI()

        val dirPath = filesDir.absolutePath;
        // Log.d("Native", dirPath.toString());
        openCameraJNI();
        // val fileList = listFilesFromJNI(dirPath)
    }

    /**
     * A native method that is implemented by the 'android_native_cpp' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun openCameraJNI(): String;

    companion object {
        // Used to load the 'android_native_cpp' library on application startup.
        init {
            System.loadLibrary("android_native_cpp")
        }
    }
}
