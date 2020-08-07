package com.example.thewitness2;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;

public class Reset extends AppCompatActivity {

    private TextView go_home;
    private FirebaseAuth mAuth;
    EditText user_email;
    Button btn_pwd_reset;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_reset);

        user_email = (EditText) findViewById(R.id.username2);
        btn_pwd_reset = (Button) findViewById(R.id.btn_reset);
        mAuth = FirebaseAuth.getInstance();

        btn_pwd_reset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String email = user_email.getText().toString().trim();

                if (email.isEmpty()) {
                    Toast.makeText(Reset.this, "Please enter your Email", Toast.LENGTH_LONG).show();
                    return;
                }

                mAuth.sendPasswordResetEmail(user_email.getText().toString()).addOnCompleteListener(new OnCompleteListener<Void>() {
                    @Override
                    public void onComplete(@NonNull Task<Void> task) {
                        if (task.isSuccessful()) {
                            Toast.makeText(Reset.this, "Link sent to your Email", Toast.LENGTH_LONG).show();
                        } else {
                            Toast.makeText(Reset.this, "Email not found!", Toast.LENGTH_LONG).show();
                        }
                    }
                });
            }
        });

        go_home = (TextView) findViewById(R.id.back2);
        go_home.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openHome();
            }
        });
    }

    public void openHome() {
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
    }
}
