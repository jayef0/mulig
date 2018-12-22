package com.androstock.notidryapp;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Typeface;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.Html;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import org.json.JSONException;
import org.json.JSONObject;
import java.text.DateFormat;
import java.util.Date;
import java.util.Locale;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.support.v4.app.NotificationCompat;

public class MainActivity extends AppCompatActivity {

    NotificationCompat.Builder notification;

    private static final int uniqueID1 = 45612;
    private static final int uniqueID2 = 45613;

    TextView selectCity, cityField, detailsField, currentTemperatureField, weatherIcon, updatedField;
    TextView status;

    Typeface weatherFont;
    String city = "Wernau, DE";
    /* Please Put your API KEY here */
    String OPEN_WEATHER_MAP_API = "5fb6bf2f59ffcb275fcf424f618812c3";
    /* Please Put your API KEY here */


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().hide();
        setContentView(R.layout.activity_main);

        notification = new NotificationCompat.Builder(MainActivity.this, "");
        notification.setAutoCancel(true);


        selectCity = (TextView) findViewById(R.id.selectCity);
        cityField = (TextView) findViewById(R.id.city_field);
        updatedField = (TextView) findViewById(R.id.updated_field);
        detailsField = (TextView) findViewById(R.id.details_field);
        currentTemperatureField = (TextView) findViewById(R.id.current_temperature_field);
        weatherIcon = (TextView) findViewById(R.id.weather_icon);
        weatherFont = Typeface.createFromAsset(getAssets(), "fonts/weathericons-regular-webfont.ttf");
        weatherIcon.setTypeface(weatherFont);
        status=(TextView) findViewById(R.id.drying_State);


        taskLoadUpStatus();
        WeatherLoadUp(city);
        selectCity.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AlertDialog.Builder alertDialog = new AlertDialog.Builder(MainActivity.this);
                alertDialog.setTitle("Change City");
                final EditText input = new EditText(MainActivity.this);
                input.setText(city);
                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                        LinearLayout.LayoutParams.MATCH_PARENT,
                        LinearLayout.LayoutParams.MATCH_PARENT);
                input.setLayoutParams(lp);
                alertDialog.setView(input);

                alertDialog.setPositiveButton("Change",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                city = input.getText().toString();
                                WeatherLoadUp(city);
                            }
                        });
                alertDialog.setNegativeButton("Cancel",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                dialog.cancel();
                            }
                        });
                alertDialog.show();
            }
        });

    }

//PUSH NOTIFICATION
    public void PushNotification_Badweather(){

        notification.setSmallIcon(R.drawable.rain);
        notification.setTicker("This is the ticker");
        notification.setWhen(System.currentTimeMillis());
        notification.setContentTitle("Warnung: Schlechtes Wetter voraus");
        notification.setContentText("Es könnte sein, dass ihre Wäsche wieder nass wird");

        Intent intent = new Intent(this, MainActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
        notification.setContentIntent(pendingIntent);

        //Builds notification and issues it
        NotificationManager nm = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        nm.notify(uniqueID1, notification.build());
    }

    public void Dry(View view){

        //Build the notification
        notification.setSmallIcon(R.drawable.logo);
        notification.setTicker("This is the ticker");
        notification.setWhen(System.currentTimeMillis( ));
        notification.setContentTitle("Deine Wäsche ist jetzt getrocknet.");
        notification.setContentText("Du kannst die Wäsche nun abhängen");

        Intent intent1 = new Intent(this, MainActivity.class);
        PendingIntent pendingIntent1 = PendingIntent.getActivity(this, 0, intent1, PendingIntent.FLAG_UPDATE_CURRENT);
        notification.setContentIntent(pendingIntent1);

        //Builds notification and issues it
        NotificationManager nm1 = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        nm1.notify(uniqueID2, notification.build( ));
    }



    public String setWeatherIcon(int actualId, long sunrise, long sunset){
        int id = actualId / 100;
        String icon = "";
        if(actualId == 800){
            long currentTime = new Date().getTime();
            if(currentTime>=sunrise && currentTime<sunset) {
                icon = "&#xf00d;";
            } else {
                icon = "&#xf02e;";
            }
        } else {
            switch(id) {
                case 2 : icon = "&#xf01e;"; //thunderstorm
                    PushNotification_Badweather();
                    break;
                case 3 : icon = "&#xf01c;"; //sprinkle
                    PushNotification_Badweather();
                    break;
                case 7 : icon = "&#xf014;";//fog
                    break;
                case 8 : icon = "&#xf013;";//cloudy
                    break;
                case 6 : icon = "&#xf01b;";//snow
                    PushNotification_Badweather();
                    break;
                case 5 : icon = "&#xf019;";//rain
                    PushNotification_Badweather();
                    break;
            }
        }
        return icon;
    }

    //GET DRYING STATUS
        public void taskLoadUpStatus() {
            if (Function.isNetworkAvailable(getApplicationContext())) {
                DownloadStatus task = new DownloadStatus();
                task.execute();
            } else {
                Toast.makeText(getApplicationContext(), "No Internet Connection", Toast.LENGTH_LONG).show();
            }
        }

         class DownloadStatus extends AsyncTask < String, Void, String > {
            @Override
            protected void onPreExecute() {
                super.onPreExecute();

            }
            protected String doInBackground(String...args) {
                String xml = Function.excuteGet("https://5c16a598713992000c76696c.onlosant.com/api/status");
                return xml;
            }
            @Override
            protected void onPostExecute(String xml) {

                try {
                    JSONObject json = new JSONObject(xml);
                    if (json != null) {

                        status.setText(json.getString("dryingState"));

                    }
                } catch (JSONException e) {
                    Toast.makeText(getApplicationContext(), "Error, Check City", Toast.LENGTH_SHORT).show();
            }
        }
    }




    //GET WEATHER DATA
        public void WeatherLoadUp(String query) {
            if (Function.isNetworkAvailable(getApplicationContext())) {
                DownloadWeather task = new DownloadWeather();
                task.execute(query);
            } else {
                Toast.makeText(getApplicationContext(), "No Internet Connection", Toast.LENGTH_LONG).show();
            }
        }

        class DownloadWeather extends AsyncTask < String, Void, String > {
        @Override
        protected void onPreExecute() {
            super.onPreExecute();

        }
        protected String doInBackground(String...args) {
            String xml = Function.excuteGet("http://api.openweathermap.org/data/2.5/weather?q=" + args[0] +
                    "&units=metric&lang=de&appid=" + OPEN_WEATHER_MAP_API);
            return xml;
        }
        @Override
        protected void onPostExecute(String xml) {

            try {
                JSONObject json = new JSONObject(xml);
                if (json != null) {
                    JSONObject details = json.getJSONArray("weather").getJSONObject(0);
                    JSONObject main = json.getJSONObject("main");
                    DateFormat df = DateFormat.getDateTimeInstance();

                    cityField.setText(json.getString("name").toUpperCase(Locale.US) + ", " + json.getJSONObject("sys").getString("country"));
                    detailsField.setText(details.getString("description").toUpperCase(Locale.US));
                    currentTemperatureField.setText(String.format("%.2f", main.getDouble("temp")) + "°");
                    updatedField.setText(df.format(new Date(json.getLong("dt") * 1000)));
                    weatherIcon.setText(Html.fromHtml(setWeatherIcon(details.getInt("id"),
                            json.getJSONObject("sys").getLong("sunrise") * 1000,
                            json.getJSONObject("sys").getLong("sunset") * 1000)));



                }
            } catch (JSONException e) {
                Toast.makeText(getApplicationContext(), "Error, Check City", Toast.LENGTH_SHORT).show();
            }
        }
    }
}