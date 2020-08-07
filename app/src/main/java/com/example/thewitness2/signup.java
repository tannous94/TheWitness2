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
import com.google.firebase.firestore.DocumentReference;
import com.google.firebase.firestore.FirebaseFirestore;

import java.util.HashMap;
import java.util.Map;

public class signup extends AppCompatActivity {

    private FirebaseAuth firebaseAuth;
    private FirebaseFirestore db;
    private TextView go_home;

    EditText email, password, confirm_password, playername;
    Button register;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_signup);

        email = (EditText) findViewById(R.id.emailaddr);
        password = (EditText) findViewById(R.id.password);
        confirm_password = (EditText) findViewById(R.id.confirm);
        playername = (EditText) findViewById(R.id.playername);
        register = (Button) findViewById(R.id.btn_register);

        firebaseAuth = FirebaseAuth.getInstance();
        db = FirebaseFirestore.getInstance();

        register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String email_addr = email.getText().toString().trim();
                String passwd = password.getText().toString().trim();
                String confirm_pwd = confirm_password.getText().toString().trim();
                String pname = playername.getText().toString().trim();

                if (email_addr.isEmpty()) {
                    Toast.makeText(signup.this, "Please enter Email Address", Toast.LENGTH_LONG).show();
                    return;
                }
                if (passwd.isEmpty()) {
                    Toast.makeText(signup.this, "Please enter Password", Toast.LENGTH_LONG).show();
                    return;
                }
                if (confirm_pwd.isEmpty()) {
                    Toast.makeText(signup.this, "Please confirm your Password", Toast.LENGTH_LONG).show();
                    return;
                }
                if (pname.isEmpty()) {
                    Toast.makeText(signup.this, "Please choose Player Name", Toast.LENGTH_LONG).show();
                    return;
                }

                if (passwd.length() < 6) {
                    Toast.makeText(signup.this, "Password too short!", Toast.LENGTH_LONG).show();
                    return;
                }

                if (!passwd.equals(confirm_pwd)) {
                    Toast.makeText(signup.this, "Passwords do not match!", Toast.LENGTH_LONG).show();
                    return;
                }

                if (pname.length() < 3) {
                    Toast.makeText(signup.this, "Player Name too short!", Toast.LENGTH_LONG).show();
                    return;
                }

                if (pname.length() > 12) {
                    Toast.makeText(signup.this, "Player Name too long!", Toast.LENGTH_LONG).show();
                    return;
                }

                Map<String, Object> new_user = new HashMap<>();
                new_user.put("Level", 0);
                new_user.put("Player Name", pname);
                new_user.put("Points", 0);
                new_user.put("Rank", 0);
                new_user.put("Time Elapsed", "0");

                db.collection("Accounts").document(email_addr).set(new_user);

                firebaseAuth.createUserWithEmailAndPassword(email_addr, passwd)
                        .addOnCompleteListener(signup.this, new OnCompleteListener<AuthResult>() {
                            @Override
                            public void onComplete(@NonNull Task<AuthResult> task) {
                                if (task.isSuccessful()) {
                                    String email_addr = email.getText().toString().trim();
                                    Intent next = new Intent(signup.this, Profile.class);
                                    next.putExtra("EmailAddr", email_addr);
                                    startActivity(next);
                                    Toast.makeText(signup.this, "Registeration Completed!", Toast.LENGTH_LONG).show();
                                } else {
                                    Toast.makeText(signup.this, "Registeration Failed!", Toast.LENGTH_LONG).show();
                                }

                            }
                        });
            }
        });

        go_home = (TextView) findViewById(R.id.back);
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
