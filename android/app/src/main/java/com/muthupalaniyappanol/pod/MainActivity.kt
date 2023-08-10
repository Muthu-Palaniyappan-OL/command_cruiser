package com.muthupalaniyappanol.pod

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.ImageButton
import android.widget.ImageView
import android.widget.Toast
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.coroutineScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import org.eclipse.paho.android.service.MqttAndroidClient
import org.eclipse.paho.client.mqttv3.IMqttActionListener
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken
import org.eclipse.paho.client.mqttv3.IMqttToken
import org.eclipse.paho.client.mqttv3.MqttAsyncClient
import org.eclipse.paho.client.mqttv3.MqttCallback
import org.eclipse.paho.client.mqttv3.MqttClient
import org.eclipse.paho.client.mqttv3.MqttConnectOptions
import org.eclipse.paho.client.mqttv3.MqttMessage
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence
import kotlin.coroutines.coroutineContext

class MainActivity : AppCompatActivity() {
    private lateinit var mqttAsyncClient: MqttAsyncClient

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        mqttAsyncClient = MqttAsyncClient("tcp://${Config.SERVER_IP}:1883", MqttClient.generateClientId(), MemoryPersistence());

        val options = MqttConnectOptions()
        options.isCleanSession = false
        options.userName = MqttClient.generateClientId()
        options.password = "Random".toCharArray()
        options.isAutomaticReconnect = true

        mqttAsyncClient.setCallback(object : MqttCallback{
            override fun connectionLost(cause: Throwable?) {

            }

            override fun messageArrived(topic: String?, message: MqttMessage?) {

            }

            override fun deliveryComplete(token: IMqttDeliveryToken?) {

            }
        })
        mqttAsyncClient.connect(options).waitForCompletion()

        findViewById<ImageView>(R.id.upButton).setOnClickListener { publishMessage("topic", "f"); }
        findViewById<ImageView>(R.id.downButton).setOnClickListener { publishMessage("topic", "b"); }
        findViewById<ImageView>(R.id.leftButton).setOnClickListener { publishMessage("topic", "l"); }
        findViewById<ImageView>(R.id.rightButton).setOnClickListener { publishMessage("topic", "r"); }
        findViewById<ImageView>(R.id.leftRotateButton).setOnClickListener { publishMessage("topic", "e"); }
        findViewById<ImageView>(R.id.rightRotateButton).setOnClickListener { publishMessage("topic", "w"); }
        findViewById<ImageView>(R.id.stopButton).setOnClickListener { publishMessage("topic", "s"); }
    }

    private fun publishMessage(topic: String, payload :String) {
        mqttAsyncClient.publish(topic, MqttMessage(payload.toByteArray()));
    }
}