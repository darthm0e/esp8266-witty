package de.labs.ocknigma.immerlichtapp;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.Editable;
import android.text.InputFilter;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import java.text.MessageFormat;


public class MainActivity extends AppCompatActivity {
    private String ipAdd = "192.168.2.242";
    TextView saveButton;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    saveButton = (TextView) findViewById(R.id.save);
    saveButton.setOnClickListener(new View.OnClickListener() {

        @Override
        public void onClick(View v) {
            save();              }

    });
    }

    public void save(){

        EditText ipAddress = (EditText)findViewById(R.id.ip_address);
        InputFilter[] filters = new InputFilter[1];
        filters[0] = new InputFilter() {
            @Override
            public CharSequence filter(CharSequence source, int start, int end,android.text.Spanned dest, int dstart, int dend) {
                if (end > start) {
                    String destTxt = dest.toString();
                    String resultingTxt = destTxt.substring(0, dstart) + source.subSequence(start, end) + destTxt.substring(dend);
                    if (!resultingTxt.matches ("^\\d{1,3}(\\.(\\d{1,3}(\\.(\\d{1,3}(\\.(\\d{1,3})?)?)?)?)?)?")) {
                        return "";
                    } else {
                        String[] splits = resultingTxt.split("\\.");
                        for (int i=0; i<splits.length; i++) {
                            if (Integer.valueOf(splits[i]) > 255) {
                                return "";
                            }
                        }
                    }
                }
                return null;
            }
        };
        ipAddress.setFilters(filters);
        ipAdd = ipAddress.getText().toString();
    }
    public void callRed(View view){
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/red", ipAdd));
    }
    public void callBlue(View view) {
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/blue", ipAdd));
    }
    public void callGreen(View view) {
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/green", ipAdd));
    }
    public void callYellow(View view) {
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/yellow", ipAdd));
    }
    public void callCyan(View view) {
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/emerald", ipAdd));
    }
    public void callPink(View view) {
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/pink", ipAdd));
    }
    public void callWhite(View view) {
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/white", ipAdd));
    }
    public void callLoop(View view) {
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/looper", ipAdd));
    }
    public void callChaser(View view) {
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/chaser", ipAdd));
    }
    public void callRainbow(View view) {
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/rainbow", ipAdd));
    }
    public void callFader(View view) {
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/fader", ipAdd));
    }
    public void callOff(View view) {
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/off", ipAdd));
    }
    public void callShtdwn(View view) {
        new GetMethodDemo().execute(MessageFormat.format("http://{0}/black", ipAdd));
    }
}