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
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.firestore.FirebaseFirestore;

public class MainActivity extends AppCompatActivity {
    private TextView about_us, register, btn_reset;
    private FirebaseFirestore db;
    private FirebaseAuth firebaseAuth;

    EditText username, password1;
    Button login;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        username = (EditText) findViewById(R.id.username);
        password1 = (EditText) findViewById(R.id.password);
        login = (Button) findViewById(R.id.login);
        firebaseAuth = FirebaseAuth.getInstance();
        db = FirebaseFirestore.getInstance();

        login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String email = username.getText().toString().trim();
                String password = password1.getText().toString().trim();

                // Add email and password checks
                if (email.isEmpty()) {
                    Toast.makeText(MainActivity.this, "Please enter Email", Toast.LENGTH_LONG).show();
                    return;
                }

                if (password.isEmpty()) {
                    Toast.makeText(MainActivity.this, "Please enter Password", Toast.LENGTH_LONG).show();
                    return;
                }

                if (password.length() < 6) {
                    Toast.makeText(MainActivity.this, "Password too short!", Toast.LENGTH_LONG).show();
                    return;
                }


                firebaseAuth.signInWithEmailAndPassword(email, password)
                        .addOnCompleteListener(MainActivity.this, new OnCompleteListener<AuthResult>() {
                            @Override
                            public void onComplete(@NonNull Task<AuthResult> task) {
                                if (task.isSuccessful()) {

                                    String email = username.getText().toString().trim();
                                    Intent next = new Intent(MainActivity.this, Profile.class);
                                    next.putExtra("EmailAddr", email);
                                    // VVVV this line below: updates the Last Login email in the LoggedIn collection !
                                    db.collection("LoggedIn").document("CurrentLogin").update("Email", email);
                                    startActivity(next);

                                } else {
                                    Toast.makeText(MainActivity.this, "Login Failed !", Toast.LENGTH_LONG).show();
                                }
                            }
                        });
            }
        });

        about_us = (TextView) findViewById(R.id.textView3);
        about_us.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openAboutUs();
            }
        });

        register = (TextView) findViewById(R.id.sign_up);
        register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openSignUp();
            }
        });

        btn_reset = (TextView) findViewById(R.id.pwd_reset);
        btn_reset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openReset();
            }
        });
    }

    public void openAboutUs() {
        Intent intent = new Intent(this, AboutUs.class);
        startActivity(intent);
    }

    public void openSignUp() {
        Intent intent = new Intent(this, signup.class);
        startActivity(intent);
    }

    public void openReset() {
        Intent intent = new Intent(this, Reset.class);
        startActivity(intent);
    }
}
